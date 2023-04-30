#include <std.h>
#include <spell.h>
#include <magic.h>
#include <skills.h>

inherit SPELL;
int bonus;

int is_curse()
{
    return 1;
}

void create()
{
    ::create();
    set_spell_name("itching curse");
    set_spell_level(([ "paladin" : 1, "bard" : 1, "druid" : 1, "inquisitor" : 1, "mage" : 1 ]));
    set_spell_sphere("necromancy");
    set_syntax("cast CLASS itching curse on TARGET");
    set_damage_desc("reduced skills, attack bonus, saves by clevel/18+1 points");
    set_description("You curse someone with itching curse. Annoying! This spell must be evil.");
    set_save("will");
    set_verbal_comp();
    set_somatic_comp();
    set_target_required(1);
}

int preSpell()
{
    if (!objectp(caster))
        return 0;

    if(target->query_property("itching_curse"))
    {
        tell_object(caster, "The target is already itchy.");
        return 0;
    }
    return 1;
}

void spell_effect()
{
    object me;
    int i;

    if (!objectp(me = this_object()))
    {
        ::dest_effect();
        return;
    }

    if (!objectp(caster) || !objectp(target) || !objectp(place))
    {
        ::dest_effect();
        return;
    }

    tell_object(caster, "%^GREEN%^You extend your hand and proclaim an itching curse on "+target->query_cap_name());
    tell_room(place, "%^GREEN%^"+caster->query_cap_name()+" points with their hand at "+target->query_cap_name()+"%^RESET%^",({target,caster}));
    tell_object(target, "%^GREEN%^You feel very itchy after "+caster->query_cap_name()+" proclaims a curse.%^RESET%^");

    if(do_save(target, 0))
    {
        tell_object(target, "%^GREEN%^You concentrate and ignore effects of the itchy curse.%^RESET%^");
        tell_object(caster, "%^GREEN%^"+target->query_cap_name()+" shakes the curse off.%^RESET%^");
        ::dest_effect();
        return;
    }

    spell_duration = (clevel + roll_dice(1, 6)) * ROUND_LENGTH;
    set_end_time();
    call_out("dest_effect", spell_duration);
    call_out("itch_itch", ROUND_LENGTH);

    bonus = clevel / 18 + 1;

    for (i = 0; i < sizeof(CORE_SKILLS); ++i)
        target->add_skill_bonus(CORE_SKILLS[i], -bonus);

    target->add_attack_bonus(-bonus);
    target->add_saving_bonus("all", -bonus);

    target->set_property("spelled", ({me}) ); //Makes the curse dispellable
    addSpellToCaster();
    spell_successful();
}

void itch_itch()
{
    if(!objectp(target))
    {
        dest_effect();
        return;
    }

    tell_object(target, "%^GREEN%^Unbearable itching distracts you.%^RESET%^");
    call_out("itch_itch", ROUND_LENGTH*3);
}

int dest_effect()
{
    object me;
    int i;

    remove_call_out("itch_itch");
    if(objectp(target))
    {
        tell_object(target, "%^GREEN%^Annoying itch finally fades.%^RESET%^");
        target->remove_property("itching_curse");
        target->remove_property_value("spelled", ({me}) );

        for(i = 0; i < sizeof(CORE_SKILLS); ++i)
            target->add_skill_bonus(CORE_SKILLS[i],bonus);

        target->add_attack_bonus(bonus);
        target->add_saving_bonus("all", bonus);
    }

    ::dest_effect();

    if(objectp(me = this_object()))
        me->remove();

    return 1;
}
