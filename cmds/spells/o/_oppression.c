/*
  _oppression.c

  Rewrite of previous version using standard mechanics.

  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>
#include <spell.h>
#include <magic.h>

inherit SPELL;

object *saved = ({  });
int counter, flag;
string my_name, my_poss, my_pron;

void create()
{
    ::create();
    set_author("tlaloc");
    set_spell_name("oppression");
    set_spell_sphere("enchantment_charm");
    set_spell_level( ([ "cleric" : 4, "paladin" : 3, "inquisitor" : 4 ]) );
    set_syntax("cast CLASS oppression");
    set_damage_desc("Shaken on attackers on failed save.");
    set_description("This spell surrounds the caster in a dark aura of fear, causing enemies caught in the aura to be shaken unless they make a will saving throw. This aura will attempt to apply the shaken effect every round until the will save is made, then will no longer affect that target for the duration of combat.");
    traveling_spell(1);
    set_save("will");
    evil_spell(1);
}

string query_cast_string()
{
    my_name = caster->query_cap_name();
    my_poss = caster->query_possessive();
    my_pron = caster->query_subjective();

    tell_object(caster, "%^BOLD%^%^BLACK%^Clenching your fists, you begin to growl the words of a prayer in a fearsome tone.");
    tell_room(place, "%^BOLD%^%^BLACK%^" + my_name + " clenches " + my_poss + " fist, growing the words of a prayer in a fearsome tone.", caster);
    return "display";
}

int preSpell()
{
    if(caster->query_property("oppression"))
    {
        tell_object(caster, "You already have an aura of oppression up.");
        return 0;
    }

    return 1;
}

void spell_effect()
{
    int duration;

    ::spell_effect();

    my_name = caster->query_cap_name();
    my_poss = caster->query_possessive();
    my_pron = caster->query_subjective();

    tell_object(caster, "%^C063%^You begin to %^C081%^glow%^C063%^ with d%^C069%^a%^C075%^r%^C063%^k e%^C069%^n%^C075%^e%^C081%^r%^C075%^g%^C063%^y as you complete your chant.%^CRST%^");
    tell_room(place, "%^C063%^" + my_name + " begins to %^C081%^glow%^C063%^ with d%^C069%^a%^C075%^r%^C063%^k e%^C069%^n%^C075%^e%^C081%^r%^C075%^g%^C063%^y as " + my_pron + " completes " + my_poss + " chant.%^CRST%^", caster);

    caster->set_property("spelled", ({ this_object() }));
    caster->set_property("added short",({" %^C063%^(o%^C069%^p%^C075%^p%^C081%^r%^C075%^e%^C069%^s%^C063%^sive a%^C069%^u%^C075%^r%^C063%^a)%^CRST%^"}));
    caster->set_property("oppression", 1);
    addSpellToCaster();
    counter = 8 * clevel;
    spell_duration = counter * ROUND_LENGTH;
    set_end_time();
    execute_attack();
    call_out("room_check",ROUND_LENGTH);
}

void room_check()
{
    if(!objectp(caster) || !objectp(environment(caster)))
    {
        dest_effect();
        return;
    }

    prepend_to_combat_cycle(environment(caster));

    call_out("room_check",ROUND_LENGTH * 2);
    return;
}

void execute_attack()
{
    object *foes, targ;
    int i, dam, enchant;

    if(!flag)
    {
        flag = 1;
        ::execute_attack();
        return;
    }

    if(!objectp(caster))
    {
        dest_effect();
        return;
    }

    if(!objectp(environment(caster)))
    {
        dest_effect();
        return;
    }

    place = environment(caster);

    if(!objectp(place) || counter < 0)
    {
        dest_effect();
        return;
    }

    foes = caster->query_attackers();

    if(!sizeof(foes))
        saved = ({  });

    foes -= saved;

    if(sizeof(foes))
    {
        foreach(object ob in foes)
        {
            if(!objectp(ob))
                continue;

            if(!do_save(ob, 0) && !PLAYER_D->immunity_check("fear"))
            {
                tell_object(ob, "%^C081%^You cower in %^C069%^fear%^C081%^ from %^C069%^" + caster->query_cap_name() + "'s%^C081%^ oppressive aura!%^CRST%^");
                tell_room(place, "%^C081%^" + ob->query_cap_name() + " cowers in %^C069%^fear%^C081%^ from %^C069%^" + caster->query_cap_name() + "'s%^C081%^ oppressive aura!%^CRST%^", ob);
                "/std/effect/status/shaken"->apply_effect(ob, 1, caster);
            }
            else
            {
                tell_object(ob, "%^C063%^You manage to shrug off the oppressive aura.%^CRST%^");
                tell_room(place, "%^C063%^" + ob->query_cap_name() + " %^C063%^manages to shrug off the oppressive aura.%^CRST%^", ob);
                saved += ({ ob });
            }
        }

    }

    prepend_to_combat_cycle(place);
    counter--;
}

void dest_effect()
{
    remove_call_out("room_check");
    if (objectp(caster))
    {
        tell_room(environment(caster), "%^C081%^" + caster->query_cap_name() + "'s oppressive aura dissipates.%^CRST%^", caster);
        tell_object(caster, "%^C081%^Your oppressive aura dissipates.%^CRST%^");
        caster->remove_property_value("added short",({" %^C063%^(o%^C069%^p%^C075%^p%^C081%^r%^C075%^e%^C069%^s%^C063%^sive a%^C069%^u%^C075%^r%^C063%^a)%^CRST%^"}));
        caster->remove_property("oppression");
    }
    ::dest_effect();

    if (objectp(this_object()))
        this_object()->remove();
}
