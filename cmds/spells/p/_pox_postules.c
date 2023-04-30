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
    set_spell_name("pox postules");
    set_spell_level(([ "druid" : 2, "cleric" : 2 ]));
    set_domains("plague");
    set_circle("pestilence");
    set_spell_sphere("necromancy");
    set_syntax("cast CLASS pox postules on TARGET");
    set_damage_desc("-4 to dex, sickened");
    set_description("You curse someone with a powerful itching curse.");
    set_save("fort");
    set_verbal_comp();
    set_somatic_comp();
    set_target_required(1);
}

int preSpell()
{
    if(target->query_property("itching_curse"))
    {
        tell_object(caster,"The target is already itchy.");
        return 0;
    }
    return 1;
}

void spell_effect()
{
    int i;

    tell_object(caster,"%^GREEN%^You extend your hand and proclaim an itching curse on "+target->QCN);
    tell_room(place,"%^GREEN%^"+caster->QCN+" points with their hand at "+target->QCN+"%^RESET%^",({target,caster}));
    tell_object(target,"%^GREEN%^You feel very itchy after "+caster->QCN+" proclaims a curse.%^RESET%^");

    if(do_save(target,0))
    {
        tell_object(target,"%^GREEN%^You concentrate and ignore effects of the itchy curse.%^RESET%^");
        tell_object(caster,"%^GREEN%^"+target->QCN+" shakes the curse off.%^RESET%^");
        ::dest_effect();
        TO->remove();
        return;
    }

    spell_kill(target, caster);

    spell_duration = (clevel / 5+ roll_dice(1, 6)) * ROUND_LENGTH;
    set_end_time();
    call_out("dest_effect",spell_duration);
    call_out("itch_itch",ROUND_LENGTH);

    bonus = clevel/18+1;

    target->add_stat_bonus("dexterity", -4);

    target->set_property("spelled", ({TO}) ); //Makes the curse dispellable
    addSpellToCaster();
    spell_successful();

}

void itch_itch()
{
    if (!objectp(target)) {
        dest_effect();
        return;
    }
    if (!do_save(target, 2)) {
        tell_object(target, "%^GREEN%^Unbearable itching sickens you.%^RESET%^");
        "/std/effect/status/sickened"->apply_effect(target,1);
    }
    call_out("itch_itch", ROUND_LENGTH * 3);
}

void dest_effect()
{
    int i;

    remove_call_out("itch_itch");
    if(objectp(target))
    {
        tell_object(target, "%^GREEN%^Annoying itch finally fades.%^RESET%^");
        target->remove_property("itching_curse");
        target->add_stat_bonus("dexterity", -4);
        target->remove_property_value("spelled", ({TO}) );

    }
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}
