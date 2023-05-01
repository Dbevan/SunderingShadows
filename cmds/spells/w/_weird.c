//Supposed to be AOE phantasmal killer. Noted for future changes.
#include <std.h>
#include <daemons.h>
#include <spell.h>
#include <magic.h>


inherit SPELL;

void create()
{
    ::create();
    set_spell_name("weird");
    set_spell_level(([ "mage" : 9, "cleric" : 9 ]));
    set_domains(({ "madness", "nightmare" }));
    set_spell_sphere("illusion");
    set_syntax("cast CLASS weird");
    set_damage_desc("mental damage, phantasmal killer, and cowering on failed save");
    set_description("With this spell, the caster projects fears and horrors from their own mind into the material plane, causing pain and suffering to everything in sight. The phantasmal killer spell is cast on all enemies and their minds assaulted with visions of horror. Only a strong will can withstand this power, and even then will only avoid some of the suffering.");
    set_verbal_comp();
    set_somatic_comp();
    splash_spell(3);
    set_save("will");
}

string query_cast_string()
{
    return "%^RESET%^%^C244%^" + caster->QCN + " %^RESET%^%^C244%^speaks words of %^RESET%^%^C052%^a%^C053%^r%^C054%^c%^C055%^a%^C053%^n%^C052%^e %^RESET%^%^C052%^h%^C053%^o%^C054%^r%^C055%^r%^C053%^o%^C052%^r%^RESET%^%^C244%^.%^CRST%^";
}

int preSpell()
{
    object nspell, *enemies;
    
    enemies = target_selector();
    
    if(!sizeof(enemies))
    {
        tell_object(caster, "%^RESET%^%^C244%^You aren't fighting anyone.%^CRST%^");
        return 0;
    }
    
    foreach(object ob in enemies)
    {
        nspell = new("/cmds/spells/p/_phantasmal_killer");
        nspell->set_silent_casting(1);
        nspell->set_spell_source(spell_type);
        nspell->use_spell(caster, ob, clevel, 100, "mage");
    }
    
    return 1;
}

void spell_effect(int prof)
{
    object* attackers;
    int i, admg;
    attackers = target_selector();

    tell_room(place, "%^RESET%^%^C244%^" + caster->QCN + " %^RESET%^%^C244%^raises " + caster->QP + " %^RESET%^%^C244%^hands and releases waves of %^RESET%^%^C052%^h%^C053%^o%^C054%^r%^C055%^r%^C056%^i%^C054%^f%^C053%^i%^C052%^c %^RESET%^%^C052%^i%^C053%^m%^C054%^a%^C055%^g%^C053%^e%^C052%^s%^RESET%^%^C244%^!%^RESET%^");
    if (!sizeof(attackers)) {
        tell_object(caster, "%^RESET%^%^C244%^The area is filled with images of unspeakable %^RESET%^%^C052%^h%^C053%^o%^C054%^r%^C055%^r%^C053%^o%^C052%^r%^RESET%^%^C244%^, but nothing happens.%^CRST%^");
        dest_effect();
        return;
    }

    for (i = 0; i < sizeof(attackers); i++) {
        if (!objectp(attackers[i])) {
            continue;
        }
        admg = sdamage;
        if (do_save(attackers[i])) {
            admg = admg / 2;
        }else {
            "/std/effect/status/cowering"->apply_effect(attackers[i], clevel / 18 + 1);
        }
        tell_object(attackers[i], "%^RESET%^%^C244%^Your %^RESET%^%^C087%^m%^C123%^i%^C195%^n%^C087%^d %^RESET%^%^C244%^is filled with images of %^RESET%^%^C088%^s%^C124%^u%^C160%^f%^C196%^f%^C160%^e%^C124%^r%^C088%^i%^C124%^n%^C160%^g %^RESET%^%^C244%^and %^RESET%^%^C052%^h%^C053%^o%^C054%^r%^C055%^r%^C053%^o%^C052%^r%^RESET%^%^C244%^!%^CRST%^");
        damage_targ(attackers[i], attackers[i]->return_target_limb(), admg, "mental");
    }
    spell_successful();
    dest_effect();
}

void dest_effect()
{
    ::dest_effect();
    if (objectp(TO)) {
        TO->remove();
    }
}
