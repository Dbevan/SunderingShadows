#include <std.h>
#include <spell.h>
#include <magic.h>
#include <rooms.h>
inherit SPELL;
int num_mon;

void fail();

#define UNDEADDIR "/d/magic/mon/create_undead/"
#define MAX_POOL 10

void create()
{
    ::create();
    set_spell_name("reanimate");
    set_spell_level(([ "warlock" : 3 ]));
    set_spell_sphere("necromancy");
    set_heritage("gloom");
    set_syntax("cast CLASS reanimate");
    set_description("Animating dead is a pathetic craft for the weak. A true necromancer's art is to change what was into something more potent and powerful that can serve his fell desires and schemes. This spell uses a fallen corpse to make an undead creature from rotting flesh. Such a creature is more potent than lesser skeletons and zombies, and will serve the warlock until discorporated. This spell, without a doubt, is evil." + ("/cmds/spells/a/_animate_dead"->extra_help()));
    evil_spell(1);
    set_property("undead spell", 1);
    set_helpful_spell(1);
    set_arg_needed();
}

string undead_to_raise()
{
    return "graveknight";
}

string query_cast_string()
{
    tell_object(caster, "%^C244%^D%^C075%^a%^C069%^r%^C244%^k e%^C081%^n%^C075%^e%^C069%^r%^C063%^g%^C244%^y begins to form in the area as %^C081%^you%^C244%^ reach out with palms open.%^CRST%^");
    return "%^C244%^D%^C075%^a%^C069%^r%^C244%^k e%^C081%^n%^C075%^e%^C069%^r%^C063%^g%^C244%^y begins to form in the area as %^C081%^" + caster->query_cap_name() + "%^C244%^ reaches out with palms open.%^CRST%^";
}

void spell_effect(int prof)
{
    object controller, undead, corpse, * corpses = ({}), * undeadl;
    int lvl;

    if ((int)caster->query_property("raised") >= MAX_POOL) {
        tell_object(caster, "%^BOLD%^%^BLACK%^You can't raise any more undead.%^RESET%^");
        TO->remove();
        return;
    }

    corpses = all_inventory(ENV(caster));
    corpses = filter_array(corpses, (: (int)$1->is_corpse() == 1 :));
    if (!sizeof(corpses)) {
        tell_object(caster, "%^RESET%^%^BOLD%^%^BLACK%^There is no corpse here to reanimate.%^RESET%^%^RESET%^");
        TO->remove();
        return;
    }

    tell_room(place, "%^BOLD%^%^GREEN%^The corpses %^GREEN%^t%^BLACK%^w%^GREEN%^i%^BLACK%^st%^GREEN%^ and %^BLACK%^c%^GREEN%^h%^BLACK%^an%^GREEN%^g%^BLACK%^e%^GREEN%^s%^GREEN%^ under %^GREEN%^t%^GREEN%^h%^BLACK%^e %^BLACK%^f%^GREEN%^e%^BLACK%^ll %^BLACK%^ma%^GREEN%^g%^BLACK%^i%^GREEN%^c%^BLACK%^,%^GREEN%^ and then finally %^BLACK%^o%^GREEN%^b%^BLACK%^edien%^GREEN%^t%^BLACK%^l%^GREEN%^y%^GREEN%^ stands as %^BLACK%^" + undead_to_raise() + "%^RESET%^", caster);
    corpse = corpses[0];
    undead = new(UNDEADDIR + undead_to_raise());
    if (!(undead)) {
        tell_object(caster, "Something went wrong.");
        TO->remove();
        return;
    }
    corpse->remove();


    tell_object(caster, "%^BOLD%^%^BLACK%^THE %^WHITE%^" + capitalize(replace_string(undead_to_raise(), "_", " ")) + "%^BLACK%^ RISES%^RESET%^");

    if (present("undead_controller", caster)) {
        controller = present("undead_controller", caster);
    }else {
        controller = new("/d/magic/obj/undead_controller");
        controller->set_caster(caster);
        controller->move(caster);
    }

    undead->set_property("raised", 1);
    undead->set_property("minion", caster);

    undead->move(environment(caster));

    setup_undead_scaling(undead);

    undead->add_id("summoned monster");
    undead->set_property("spell", TO);
    undead->set_property("spell_creature", TO);
    undead->set_property("minion", caster);

    undead->control(caster);
    caster->add_follower(undead);
    controller->add_monster(undead);
    
    undead->set_owner(caster);
    undead->setup_minion(clevel, spell_level, "greater");

    caster->set_property("raised", 1);

    dest_effect();
    spell_successful();
    return;
}

void setup_undead_scaling(object undead)
{
    undead->set_guild_level("fighter", clevel);
    undead->set_weap_enchant(clevel / 10);
    undead->set_skill("athletics", clevel);
    undead->set_skill("endurance", clevel);
    undead->set_skill("perception", clevel - clevel / 10);
    undead->set_level(clevel);
    undead->set_hd(clevel, 6);
    undead->set_max_hp(clevel * 14 + 100);
    undead->set_hp(undead->query_max_hp());
    undead->set_overall_ac(10 - clevel);
}

void dest_effect()
{
    ::dest_effect();
    if (objectp(TO)) {
        TO->remove();
    }
    ;
}
