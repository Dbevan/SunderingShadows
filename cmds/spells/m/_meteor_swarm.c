//Meteor Swarm coded by Bane//
// Extensive damage formula change to hopefully put on par with other spells. Nienne, 03/09.
#include <std.h>
#include <spell.h>
#include <magic.h>
#include <daemons.h>

inherit SPELL;


void create()
{
    ::create();
    set_spell_name("meteor swarm");
    set_spell_level(([ "mage" : 9, "oracle":9 ]));
    set_mystery(({"heavens"}));
    set_spell_sphere("invocation_evocation");
    set_syntax("cast CLASS meteor swarm on TARGET");
    set_damage_desc("bludgeoning damage on target on ranged touch attack, fire damage to others.");
    set_description("You call upon a swarm of meteors to slam down on your target, causing bludgeoning damage to them on a successful ranged touch attack. The swarm of meteors explodes on contact, causing fire damage to everyone present, reduced by half on a successful reflex save. This spell is affected by the master of elements feat.");
    set_verbal_comp();
    set_somatic_comp();
    splash_spell(1);
    set_save("reflex");
    set_target_required(1);
    set_immunities(({ "fire" }));
}


void spell_effect(int prof)
{
    object *foes, mycolor, element, mess;
    int i, roll;
    string my_name, your_name, my_poss;

    if(!objectp(caster) || !objectp(place))
    {
        dest_effect();
        return;
    }

    if(!present(target,place))
    {
        tell_object(caster,"%^BOLD%^Your target is not in this area.\n");
        dest_effect();
        return;
    }

    element = (string)caster->query("elementalist");
    if(element){
        set_immunities(({ element }));
        define_clevel();
        define_base_damage(0);
    }
    switch(element)
    {
    case "acid":        mycolor = "%^RESET%^%^CRST%^%^C065%^"; break;
    case "electricity": mycolor = "%^RESET%^%^CRST%^%^C214%^"; break;
    case "cold":        mycolor = "%^RESET%^%^CRST%^%^C039%^"; break;
    case "fire":        mycolor = "%^RESET%^%^CRST%^%^C124%^"; break;
    case "sonic":       mycolor = "%^RESET%^%^CRST%^%^C212%^"; break;
    default:            mycolor = "%^RESET%^%^CRST%^%^C124%^"; element = "fire"; break;
    }
    
    my_name = caster->query_cap_name();
    your_name = target->query_cap_name();
    my_poss = caster->query_possessive();
    
    roll = (int)BONUS_D->process_hit(caster, target, 1, 0, 0, 1);

    switch(element)
    {
    case "acid":
        tell_object(caster, ""+mycolor+"You finish your chant and the area %^C036%^hisses "+mycolor+"under an explosion of %^C030%^s%^C036%^ea%^C042%^r%^C030%^i%^C036%^ng %^C042%^a%^C030%^c%^C036%^id%^"+mycolor+"!%^CRST%^");
        tell_room(place, ""+mycolor+my_name+mycolor+" finishes "+my_poss+" chant and the area %^C036%^hisses "+mycolor+"under an explosion of %^C030%^s%^C036%^ea%^C042%^r%^C030%^i%^C036%^ng %^C042%^a%^C030%^c%^C036%^id%^"+mycolor+"!%^CRST%^", caster);
        roll && tell_object(target, ""+mycolor+"A swarm of %^C036%^hissing %^C030%^m%^C036%^e%^C042%^teo%^C036%^r%^C030%^s "+mycolor+"slams into you with tremendous force!%^CRST%^");
        roll && tell_room(place, ""+mycolor+"A swarm of %^C036%^hissing %^C030%^m%^C036%^e%^C042%^teo%^C036%^r%^C030%^s "+mycolor+"slams into "+your_name+mycolor+" with tremendous force!%^CRST%^", target);
        break;
    case "cold":
        tell_object(caster, ""+mycolor+"You finish your chant and the area crackles under an explosion of %^C045%^c%^C051%^hi%^C123%^ll%^C051%^in%^C045%^g %^C255%^ice!%^CRST%^");
        tell_room(place, ""+mycolor+my_name+mycolor+" finishes "+my_poss+" chant and the area crackles under an explosion of %^C045%^ch%^C051%^illi%^C045%^ng %^C255%^ice!%^CRST%^", caster);
        roll && tell_object(target, ""+mycolor+"A swarm of %^C045%^frozen m%^C051%^e%^C123%^t%^C253%^e%^C123%^o%^C051%^r%^C045%^s "+mycolor+"slams into you with tremendous force!%^CRST%^");
        roll && tell_room(place, ""+mycolor+"A swarm of %^C045%^frozen m%^C051%^e%^C123%^t%^C253%^e%^C123%^o%^C051%^r%^C045%^s "+mycolor+"slams into "+your_name+mycolor+" with tremendous force!%^CRST%^", target);
        break;
    case "electricity":
        tell_object(caster, ""+mycolor+"You finish your chant and the area %^C226%^cr%^C230%^a%^C220%^c%^C226%^kl%^C230%^e%^C220%^s "+mycolor+"with an explosion of %^C230%^sparks "+mycolor+"and %^C230%^static!%^CRST%^");
        tell_room(place, ""+mycolor+my_name+mycolor+" finishes "+my_poss+" chant and the area %^C226%^cr%^C230%^a%^C220%^c%^C226%^kl%^C230%^e%^C220%^s "+mycolor+"with an explosion of %^C230%^sparks "+mycolor+"and %^C230%^static!%^CRST%^", caster);
        roll && tell_object(target, ""+mycolor+"A swarm of %^C220%^c%^C226%^ra%^C230%^c%^C220%^k%^C226%^li%^C230%^n%^C220%^g m%^C226%^e%^C230%^teo%^C226%^r%^C220%^s "+mycolor+"slams into you with tremendous force!%^CRST%^");
        roll && tell_room(place, ""+mycolor+"A swarm of %^C220%^c%^C226%^ra%^C230%^c%^C220%^k%^C226%^li%^C230%^n%^C220%^g m%^C226%^e%^C230%^teo%^C226%^r%^C220%^s "+mycolor+"slams into "+your_name+mycolor+" with tremendous force!%^CRST%^", target);
        break;
    case "sonic":
        tell_object(caster, ""+mycolor+"You finish your chant and the area %^C198%^p%^C200%^u%^C206%^l%^C198%^s%^C200%^e%^C206%^s "+mycolor+"under an explosion of %^C200%^sound!%^CRST%^");
        tell_room(place, ""+mycolor+my_name+mycolor+" finishes "+my_poss+" chant and the area %^C198%^p%^C200%^u%^C206%^l%^C198%^s%^C200%^e%^C206%^s "+mycolor+"under an explosion of %^C200%^sound!%^CRST%^", caster);
        roll && tell_object(target, ""+mycolor+"A swarm of %^C198%^p%^C200%^u%^C206%^l%^C198%^s%^C200%^i%^C206%^n%^C198%^g %^C206%^m%^C200%^e%^C198%^teo%^C200%^r%^C206%^s "+mycolor+"slams into you with tremendous force!%^CRST%^");
        roll && tell_room(place, ""+mycolor+"A swarm of %^C198%^p%^C200%^u%^C206%^l%^C198%^s%^C200%^i%^C206%^n%^C198%^g %^C206%^m%^C200%^e%^C198%^teo%^C200%^r%^C206%^s "+mycolor+"slams into "+your_name+mycolor+" with tremendous force!%^CRST%^", target);
        break;
    default:
        element = "fire";
        tell_object(caster, ""+mycolor+"You finish your chant and the area %^C196%^bleeds red "+mycolor+"with an explosion of %^C226%^sp%^C230%^ark%^C226%^s "+mycolor+"and %^C208%^f%^C220%^i%^C208%^r%^C196%^e%^C230%^!%^CRST%^");
        tell_room(place, ""+mycolor+my_name+mycolor+" finishes "+my_poss+" chant and the area %^C196%^bleeds red "+mycolor+"with an explosion of %^C226%^sp%^C230%^ark%^C226%^s "+mycolor+"and %^C208%^f%^C220%^i%^C208%^r%^C196%^e%^C230%^!%^CRST%^", caster);
        roll && tell_object(target, ""+mycolor+"A swarm of %^C208%^f%^C220%^i%^C208%^e%^C196%^r%^C208%^y m%^C214%^e%^C220%^teo%^C214%^r%^C208%^s "+mycolor+"slams into you with tremendous force!%^CRST%^");
        roll && tell_room(place, ""+mycolor+"A swarm of %^C208%^f%^C220%^i%^C208%^e%^C196%^r%^C208%^y m%^C214%^e%^C220%^teo%^C214%^r%^C208%^s "+mycolor+"slams into "+your_name+mycolor+" with tremendous force!%^CRST%^", target);
        break;
    }
    if(!roll)
    {
        tell_room(place, "%^RESET%^%^CRST%^%^C243%^"+your_name+"%^RESET%^%^CRST%^%^C243%^ leaps to the side as a plummeting %^C058%^m%^C100%^e%^C101%^te%^C100%^o%^C058%^r %^RESET%^%^C243%^slams into the ground, avoiding some of the damage!%^CRST%^", ({ target }));
        tell_object(target, "%^RESET%^%^CRST%^%^C243%^You leap aside as a plummeting %^C058%^m%^C100%^e%^C101%^te%^C100%^o%^C058%^r %^RESET%^%^C243%^slams into the ground, avoiding some of the damage!%^CRST%^");
    }
    else target->cause_typed_damage(target, target->return_target_limb(), sdamage, "bludgeoning");

    spell_successful();
    spell_kill(target, caster);

    foes = target_selector();
    foes -= ({ target });
    foes -= ({ caster });

    if(!sizeof(foes))
    {
        dest_effect();
        return;
    }

    foreach(object ob in foes)
    {
        if(!objectp(ob))
            continue;

        if(!do_save(foes[i], 0)){
            tell_room(place, ""+mycolor+ob->query_cap_name()+mycolor+" is caught in the blast of the impact!%^CRST%^", ({ ob }));
            tell_object(ob, ""+mycolor+"You are caught in the blast of the impact!%^CRST%^");
            ob->cause_typed_damage(ob, ob->return_target_limb(), sdamage, element);
        }
        else
        {
            tell_room(place, ""+mycolor+ob->query_cap_name()+mycolor+" jumps for cover, avoiding some of the damage!%^CRST%^", ({ ob }));
            tell_object(ob, ""+mycolor+"You jump for cover, avoiding some of the damage!%^CRST%^");
            ob->cause_typed_damage(ob, ob->return_target_limb(), sdamage/2, element);
        }
    }

    dest_effect();
    return;
}

void dest_effect()
{
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}

