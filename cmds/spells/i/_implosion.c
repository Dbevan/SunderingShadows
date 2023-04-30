#include <std.h>
#include <spell.h>
#include <magic.h>
#include <priest.h>
inherit SPELL;

void create()
{
    ::create();
    set_spell_name("implosion");
    set_spell_level(([ "cleric" : 9 ]));
    set_spell_sphere("invocation_evocation");
    set_syntax("cast CLASS implosion on TARGET");
    set_damage_desc("Untyped damage over several rounds. Successful save per round reduces damage by half.");
    set_description("Calling on your power, you extend a hand and clench it into a fist, inflicting massive crushing force on your target. The target takes untyped damage, and continues to take a lesser amount of crushing damage each round as they are mercilessly squeezed into nothingness.");
    set_save("fortitude");
    set_target_required(1);
    set_verbal_comp();
    set_somatic_comp();
}

string query_cast_string()
{
    return "%^CYAN%^"+caster->QCN+" extends "+caster->QP+" hand and recites a prayer.";
}

void spell_effect(int prof)
{
    
    if(!objectp(caster))
        return;
    
    if(!objectp(target) || environment(target) != place)
    {
        tell_object(caster, "Your target is no longer here.");
        dest_effect(target);
        return;
    }
    
    if(target->query_property("implosion"))
    {
        tell_object(caster, "Your target is already affected by implosion.");
        dest_effect();
        return;
    }
    
    spell_successful();
    spell_kill(caster, target);
    
    tell_room(place, "%^C111%^" + caster->query_cap_name() + "%^C248%^ finishes a prayer and clenches a fist, crushing " + target->query_cap_name() + " with unreleneting force!%^CRST%^", ({ target, caster }));
    tell_object(caster, "%^C248%^You finish your prayer and close your hand into a clenched fist, crushing " + target->query_cap_name() + " with unrelenting force!%^CRST%^");
    tell_object(target, "%^C248%^" + caster->query_cap_name() + " finishes a prayer and crushes you with unrelenting force!%^CRST%^");
    
    if(do_save(target, 0))
        damage_targ(target, "torso", sdamage / 2, "untyped");
    else
        damage_targ(target, "torso", sdamage, "untyped");
        
    target->set_property("implosion", 1);
    
    call_out("keep_crushing", ROUND_LENGTH, 4);
}

void keep_crushing(int dur)
{
    if(!objectp(caster) || !objectp(target))
    {
        dest_effect();
        return;
    }
    
    spell_kill(caster, target);
    define_base_damage(0);
    tell_object(target, "%^C248%^The crushing force squeezes you mercilessly!%^CRST%^");
    tell_room(place, "%^C248%^The crushing force squeezes " + target->query_cap_name() + " mercilessly!%^CRST%^", target);
    
    if(do_save(target, 0))
        damage_targ(target, "torso", sdamage / 8, "untyped");
    else
        damage_targ(target, "torso", sdamage / 5, "untyped");
    
    dur--;
    if(dur <= 0)
    {
        dest_effect();
        return;
    }
    
    call_out("keep_crushing", ROUND_LENGTH, dur);
}

void dest_effect()
{
    if (find_call_out("keep_crushing") != -1)
        remove_call_out("keep_crushing");
    
    if(target)
    {
        target->remove_property("implosion");
        tell_object(target, "%^BOLD%^The crushing force finally relents.%^RESET%^");
        objectp(caster) && tell_object(target, "%^BOLD%^Your crushing force relents from " + target->query_cap_name() + ".%^RESET%^");
    }
    
    tell_room(place, "");
    
    ::dest_effect();
}
