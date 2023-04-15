#include <spell.h>
#include <magic.h>
inherit SPELL;

int power;

void create()
{
    ::create();
    set_spell_name("pull of the grave");
    set_spell_level(([ "warlock" : 4 ]));
    set_spell_sphere("necromancy");
    set_heritage("gloom");
    set_syntax("cast CLASS pull of the grave on TARGET");
    set_damage_desc("Death or negative energy and negative levels");
    set_description("You condemn your target to the cold, unrelenting oblivion of the grave. Shards of necrotic steel swirl around them, forming a dark and forbidding coffin from whence few will ever return. On a failed combat death save, the target will obey the call and die instantly. On a successful save, the target gets 1d4 negative levels due to their close brush with death and take normalized negative energy damage as they wallow in their blackened steel tomb.");
    set_verbal_comp();
    set_save("fort");
    set_somatic_comp();
    set_target_required(1);
}

string query_cast_string()
{
    return "%^RESET%^%^CRST%^%^C066%^"+caster->query_cap_name()+"%^RESET%^%^CRST%^%^C066%^ raises their hands and wisps of %^C118%^f%^C112%^el%^C106%^l %^C118%^e%^C112%^ne%^C106%^r%^C118%^g%^C112%^y %^RESET%^%^C066%^materialize in the air.%^CRST%^";
}

void spell_effect()
{   
    int dur;
    string my_name, your_name;
    
    spell_successful();
    
    my_name = caster->query_cap_name();
    your_name = target->query_cap_name();
    
    tell_object(target, "%^RESET%^%^CRST%^%^C066%^Wisps of %^C118%^f%^C112%^el%^C106%^l %^C118%^e%^C112%^ne%^C106%^r%^C118%^g%^C112%^y %^RESET%^%^C066%^coalesce around you, encasing them and solidifying into a %^C065%^ne%^C064%^c%^C065%^roti%^C064%^c %^C065%^to%^C064%^m%^C065%^b%^RESET%^%^C066%^...%^CRST%^");
    tell_room(place, "%^RESET%^%^CRST%^%^C066%^Wisps of %^C118%^f%^C112%^el%^C106%^l %^C118%^e%^C112%^ne%^C106%^r%^C118%^g%^C112%^y %^RESET%^%^C066%^coalesce around "+your_name+"%^RESET%^%^CRST%^%^C066%^, encasing them and solidifying into a %^C065%^ne%^C064%^c%^C065%^roti%^C064%^c %^C065%^to%^C064%^m%^C065%^b%^RESET%^%^C066%^...%^CRST%^", target);
    
    if(combat_death_save(target, 0))
    {
        tell_object(target, "%^RESET%^%^CRST%^%^C066%^You break free of the %^C065%^fo%^C064%^u%^C065%^l pr%^C064%^i%^C065%^s%^C064%^o%^C065%^n%^RESET%^%^C066%^, %^C059%^gasping %^C066%^for %^C253%^breath!%^CRST%^");
        tell_room(place, "%^RESET%^%^CRST%^%^C066%^"+your_name+"%^RESET%^%^CRST%^%^C066%^ breaks free of the %^C065%^fo%^C064%^u%^C065%^l pr%^C064%^i%^C065%^s%^C064%^o%^C065%^n%^RESET%^%^C066%^, %^C059%^gasping %^C066%^for %^C253%^breath!%^CRST%^", target);
        damage_targ(target, target->return_target_limb(), sdamage, "negative energy");
        
        if(!target || !objectp(target))
            return;
        
        dur = 6 * (clevel / 10 + roll_dice(1, 4));        
        //target->set_paralyzed(dur, "You are weighed down by the pull of the grave");
        //"/std/effect/status/shaken"->apply_effect(target, dur, caster);
        "/std/effect/status/negative_level"->apply_effect(target, roll_dice(1, 20), roll_dice(1, 4));
    }
    else
    {
        tell_object(target, "%^RESET%^%^CRST%^%^C066%^The %^C065%^pr%^C064%^i%^C065%^so%^C064%^n %^C066%^splinters and deteriorates, and your %^C100%^l%^C058%^i%^C100%^f%^C058%^el%^C100%^es%^C058%^s hu%^C100%^s%^C058%^k %^RESET%^%^C066%^tumbles to the ground.%^CRST%^");
        tell_room(place, "%^RESET%^%^CRST%^%^C066%^The %^C065%^pr%^C064%^i%^C065%^so%^C064%^n %^C066%^splinters and deteriorates, and "+your_name+"%^RESET%^%^CRST%^%^C066%^'s %^C100%^l%^C058%^i%^C100%^f%^C058%^el%^C100%^es%^C058%^s hu%^C100%^s%^C058%^k %^RESET%^%^C066%^tumbles to the ground.", target);
        target->set_hp(-100);
    }
    
    dest_effect();
}

void dest_effect()
{
    ::dest_effect();
    if(objectp(this_object()))
        this_object()->remove();
}
