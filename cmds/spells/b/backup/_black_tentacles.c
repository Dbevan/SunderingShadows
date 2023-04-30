// lowlevel conjuration school specialty spell. Nienne, 11/09.
#include <std.h>
#include <magic.h>
#include <daemons.h>
#include <priest.h>
inherit SPELL;

object* inven, * caught;
int worked, duration, mydam, first_execute;
void do_tentacles();

void create()
{
    ::create();
    set_author("nienne");
    set_spell_name("black tentacles");
    set_spell_level(([ "mage" : 4, "warlock" : 3, "magus" : 4, "cleric" : 4 ]));
    set_domains("nightmare");
    set_spell_sphere("conjuration_summoning");
    set_syntax("cast CLASS black tentacles");
    set_description("This spell summons up a mass of writhing black tentacles from the earth at your feet. These will ensnare anything nearby to slow their escape, and attempt to knock them from their feet.");
    set_verbal_comp();
    set_somatic_comp();
    splash_spell(3);
    set_feats_required(([ "warlock" : "tome of ancient secrets"]));
}

int preSpell()
{
    if (!objectp(CASTER))
        return 0;

    if ((int)CASTER->query_property("black tentacles"))
    {
        tell_object(CASTER, "%^RESET%^%^C241%^You are already maintaining such a spell!%^CRST%^");
        return 0;
    }

    return 1;
}

string query_cast_string()
{
    if (!objectp(caster))
        return "";

    if (!objectp(place))
        return "";

    tell_object(caster, "%^RESET%^%^C241%^You kneel down, %^RESET%^%^C039%^w%^C045%^h%^C051%^i%^C087%^s%^C123%^p%^C159%^e%^C087%^r%^C051%^i%^C045%^n%^C039%^g %^RESET%^%^C241%^your summons through the %^RESET%^%^C094%^e%^C130%^a%^C136%^r%^C130%^t%^C094%^h%^RESET%^%^C241%^.%^CRST%^");
    tell_room(place, "%^RESET%^%^C241%^Kneeling, " + caster->query_cap_name() + " %^RESET%^%^C051%^w%^C087%^h%^C123%^i%^C159%^s%^C195%^p%^C123%^e%^C087%^r%^C051%^s %^RESET%^%^C241%^a summoning through the %^RESET%^%^C094%^e%^C130%^a%^C136%^r%^C130%^t%^C094%^h%^RESET%^%^C241%^.%^RESET%^", caster);
    return "display";
}

void spell_effect(int prof)
{
    int i;

    caught = ({});
    inven = target_selector();
    inven -= ({ caster });

    if (!objectp(caster))
    {
        dest_effect();
        return;
    }

    if (!objectp(place))
    {
        dest_effect();
        return;
    }

    if (!sizeof(inven))
    {
        tell_object(caster, "%^RESET%^%^C241%^From just beneath the %^RESET%^%^C094%^e%^C130%^a%^C136%^r%^C130%^t%^C094%^h %^RESET%^%^C241%^at your feet, you feel the %^RESET%^%^C051%^s%^C087%^h%^C123%^i%^C159%^v%^C087%^e%^C051%^r %^RESET%^%^C241%^of the %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C241%^e%^C240%^s %^RESET%^%^C241%^as they find no target to ensnare, and simply %^RESET%^%^C240%^f%^C244%^a%^C248%^d%^C252%^e %^RESET%^%^C241%^away.%^CRST%^");
        dest_effect();
        return;
    }

    worked = 1;
    CASTER->set_property("black tentacles", 1);
    mydam = 4 + (clevel / 2);

    tell_room(place, "%^RESET%^%^C240%^R%^C241%^u%^C242%^b%^C241%^b%^C240%^e%^C241%^r%^C242%^y %^RESET%^%^C241%^b%^C240%^l%^C241%^a%^C242%^c%^C241%^k %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C241%^e%^C240%^s %^RESET%^%^C241%^writhe up out of the %^RESET%^%^C094%^e%^C130%^a%^C136%^r%^C130%^t%^C094%^h%^RESET%^%^C241%^, coiling and grasping at anything within reach!%^CRST%^");

    for (i = 0; i < sizeof(inven); ++i)
    {
        if (!objectp(inven[i]))
            continue;

        if (random((int)inven[i]->query_stats("strength") + roll_dice(1, 20)) < random(clevel + 8))
        {
            tell_object(inven[i], "%^RESET%^%^C241%^One manages to wrap about your leg and %^RESET%^%^C240%^e%^C241%^n%^C242%^s%^C243%^n%^C242%^a%^C241%^r%^C240%^e %^RESET%^%^C241%^you!%^CRST%^");
            tell_room(place, "%^RESET%^%^C241%^One manages to wrap about " + inven[i]->QCN + "%^RESET%^%^C241%^'s leg and %^RESET%^%^C240%^e%^C241%^n%^C242%^s%^C243%^n%^C242%^a%^C241%^r%^C240%^e%^RESET%^%^C241%^ " + inven[i]->QO + "%^RESET%^%^C241%^!%^CRST%^", inven[i]);
            inven[i]->set_property("added short", ({ "%^RESET%^%^C240%^ (%^RESET%^%^C241%^e%^C242%^n%^C243%^t%^C242%^a%^C241%^n%^C242%^g%^C243%^l%^C242%^e%^C241%^d%^RESET%^%^C240%^)%^CRST%^" }));
            caught += ({ inven[i] });
            continue;
        }

        if (random((int)inven[i]->query_stats("dexterity") + 1) < roll_dice(1, 25) && random(2)) {
            tell_object(inven[i], "%^RESET%^%^C241%^You %^RESET%^%^C063%^s%^C075%^t%^C081%^u%^C087%^m%^C075%^b%^C069%^l%^C063%^e %^RESET%^%^C241%^as one of the %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C241%^e%^C240%^s %^RESET%^%^C241%^almost knocks you from your feet!%^CRST%^");
            tell_room(place, "%^RESET%^%^C241%^" + inven[i]->QCN + " %^RESET%^%^C063%^s%^C069%^t%^C075%^u%^C081%^m%^C087%^b%^C075%^l%^C069%^e%^C063%^s %^RESET%^%^C241%^as a %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C241%^e %^RESET%^%^C240%^knocks " + inven[i]->QO + " %^RESET%^%^C241%^from " + inven[i]->QP + " %^RESET%^%^C241%^feet!%^CRST%^", inven[i]);
            inven[i]->set_tripped(1, "%^RESET%^%^C241%^You're regaining your %^RESET%^%^C049%^b%^C050%^a%^C051%^l%^C123%^a%^C051%^n%^C050%^c%^C049%^e%^RESET%^%^C241%^!%^CRST%^", 1);
            continue;
        }
        
    }

    addSpellToCaster();
    spell_successful();
    duration = (clevel / 10 + 3) * ROUND_LENGTH;
    spell_duration = duration;
    set_end_time();
    call_out("dest_effect", duration);
    call_out("do_tentacles", ROUND_LENGTH);
}

//void do_tentacles()
void execute_attack()
{
    object* removing;
    int i;

    if (!objectp(caster))
    {
        dest_effect();
        return;
    }

    if (!objectp(PLACE))
    {
        dest_effect();
        return;
    }

    //tell_object(caster, "PLACE = "+identify(PLACE));
    if (!present(caster, PLACE))
    {
        dest_effect();
        return;
    }

    if (caster->query_unconscious())
    {
        dest_effect();
        return;
    }

    if (!sizeof(inven))
    {
        dest_effect();
        return;
    }

    if(!first_execute)
    {
        ++first_execute;
        ::execute_attack();
        return;
    }
    /*
    if (worked >= duration) {
        dest_effect();
        return;
    }
    */
    //worked++;
    removing = ({});
    for (i = 0; i < sizeof(inven); ++i)
    {
        if (!objectp(inven[i]))
            continue;

        if (!present(inven[i], PLACE))
        {
            inven[i]->remove_property_value("added short", ({ "%^RESET%^%^C240%^ (%^RESET%^%^C241%^e%^C242%^n%^C243%^t%^C242%^a%^C241%^n%^C242%^g%^C243%^l%^C242%^e%^C241%^d%^RESET%^%^C240%^)%^CRST%^" }));
            removing += ({ inven[i] });
        }
    }

    inven -= removing;
    caught -= removing;

    if (!sizeof(inven))
    {
        dest_effect();
        return;
    }

    removing = ({});

    if (sizeof(caught))
    {
        for (i = 0; sizeof(caught) > 0, i < sizeof(caught); ++i)
        {
            if (!objectp(caught[i]))
                continue;

            if (random(caught[i]->query_stats("strength") + roll_dice(1, 20)) < random(clevel + 8))
            {
                tell_object(caught[i], "%^RESET%^%^C241%^The %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C240%^e %^RESET%^%^C241%^s%^C242%^q%^C243%^u%^C242%^e%^C241%^e%^C242%^z%^C243%^e%^C242%^s %^RESET%^%^C241%^the %^RESET%^%^C036%^l%^C037%^i%^C038%^f%^C039%^e %^RESET%^%^C241%^from you!%^CRST%^");
                tell_room(place, "%^RESET%^%^C241%^The %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C240%^e squeezes around " + caught[i]->QCN + "!", caught[i]);
                damage_targ(caught[i], caught[i]->return_target_limb(), mydam, "bludgeoning");
            }
            else
            {
                tell_object(caught[i], "%^RESET%^%^C241%^You %^RESET%^%^C088%^r%^C124%^i%^C088%^p %^RESET%^%^C241%^free of the %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C240%^e%^RESET%^%^C241%^!%^CRST%^");
                tell_room(place, "%^RESET%^%^C241%^" + caught[i]->QCN + " %^RESET%^%^C088%^r%^C124%^i%^C160%^p%^RESET%^%^C088%^s %^RESET%^%^C241%^free of the %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C240%^e%^RESET%^%^C241%^!%^CRST%^", caught[i]);
                caught[i]->remove_property_value("added short", ({ "%^RESET%^%^C240%^ (%^RESET%^%^C241%^e%^C242%^n%^C243%^t%^C242%^a%^C241%^n%^C242%^g%^C243%^l%^C242%^e%^C241%^d%^RESET%^%^C240%^)%^CRST%^" }));
                removing += ({ caught[i] });
            }
        }
    }

    caught -= removing;

    for (i = 0; i < sizeof(inven); ++i)
    {
        if (!objectp(inven[i]))
            continue;

        if (inven[i]->query_tripped())
            continue;

        if (random((int)inven[i]->query_stats("dexterity") + 1) < roll_dice(1, 25) && random(2))
        {
            tell_object(inven[i], "%^RESET%^%^C241%^You %^RESET%^%^C063%^s%^C069%^t%^C075%^u%^C081%^m%^C075%^b%^C069%^l%^C063%^e%^RESET%^%^C241%^ as one of the " + "%^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C241%^e%^RESET%^%^C240%^s %^RESET%^%^C241%^almost knocks you from your feet!%^CRST%^");
            tell_room(place, "%^RESET%^%^C241%^" + inven[i]->QCN + " %^RESET%^%^C063%^s%^C069%^t%^C075%^u%^C081%^m%^C087%^b%^C075%^l%^C069%^e%^C063%^s %^RESET%^%^C241%^as a %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C240%^e %^RESET%^%^C241%^knocks " + inven[i]->QO + " %^RESET%^%^C241%^ " + inven[i]->QP + " %^RESET%^%^C241%^feet!%^CRST%^", inven[i]);
            inven[i]->set_tripped(1, "%^RESET%^%^C241%^You're regaining your " + "%^RESET%^%^C049%^b%^C050%^a%^C051%^l%^C123%^a%^C051%^n%^C050%^c%^C049%^e%^RESET%^%^C241%^!%^CRST%^", 1);
        }
    }
    
    if(present(caster, place) && !caster->query_unconscious())
    {
        place->addObjectToCombatCycle(this_object(), 1);
        return;
    }

    else
    {
        dest_effect();
        return;
    }
    //call_out("do_tentacles", ROUND_LENGTH);
}

void dest_effect()
{
    int i;
    if (worked)
    {
        tell_room(place, "%^RESET%^%^C241%^The %^RESET%^%^C240%^w%^C241%^r%^C242%^i%^C241%^t%^C240%^h%^C241%^i%^C242%^n%^RESET%^%^C240%^g %^RESET%^%^C240%^t%^C241%^e%^C242%^n%^C241%^t%^C240%^a%^C241%^c%^C242%^l%^C241%^e%^RESET%^%^C240%^s %^RESET%^%^C051%^s%^C087%^h%^C123%^i%^C159%^v%^C087%^e%^C051%^r " + "%^RESET%^%^C241%^and crumble into %^RESET%^%^C250%^dust%^RESET%^%^C241%^, %^RESET%^%^C240%^f%^C244%^a%^C248%^d%^C250%^i%^C252%^n%^C254%^g %^RESET%^%^C241%^before your eyes.%^RESET%^");
        for (i = 0; i < sizeof(inven); ++i)
        {
            if (!objectp(inven[i]))
                continue;

            inven[i]->remove_property_value("added short", ({ "%^RESET%^%^C240%^ (%^RESET%^%^C241%^e%^C242%^n%^C243%^t%^C242%^a%^C241%^n%^C242%^g%^C243%^l%^C242%^e%^C241%^d%^RESET%^%^C240%^)%^CRST%^" }));
        }

        if (objectp(CASTER))
            CASTER->remove_property("black tentacles");
    }

    ::dest_effect();

    if (objectp(this_object()))
        this_object()->remove();
}

