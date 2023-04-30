#include <std.h>
#include <spell.h>
#include <magic.h>
#include <priest.h>
inherit SPELL;

void create()
{
    ::create();
    set_spell_name("creeping doom");
    set_spell_level(([ "oracle" : 7, "druid" : 7 ]));
    set_mystery("nature");
    set_circle("pestilence");
    set_spell_sphere("conjuration_summoning");
    set_syntax("cast CLASS creeping doom [on TARGET]");
    set_damage_desc("Piercing damage over several rounds. Successful save per round reduces damage by half.");
    set_description("This spell will call a swarm of biting and stinging centipedes to torment the target for several rounds. Each round, a fortitude save will reduce the damage by half.");
    set_save("fortitude");
    set_target_required(1);
    set_verbal_comp();
    set_somatic_comp();
}

string query_cast_string()
{
    return "%^CYAN%^"+caster->QCN+" lifts "+caster->QP+" head and lets out a chittering voice.";
}

int color(string str)
{
    return CRAYON_D->color_string(str, "dark green");
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
    
    if(target->query_property("creeping doom"))
    {
        tell_object(caster, "Your target is already affected by creeping doom.");
        dest_effect();
        return;
    }
    
    spell_successful();
    
    tell_room(place, color("A mass of creeping centipedes erupt from the ground, crawling up " + target->query_cap_name() + "'s legs and biting viciously!"), target);
    tell_object(target, color("A mass of creeping centipedes erupt from the ground, crawling up your legs and biting you viciously!"));
    
    if(do_save(target, 0))
        damage_targ(target, "torso", sdamage / 2, "piercing");
    else
        damage_targ(target, "torso", sdamage, "piercing");
    
    target->set_property("creeping doom", 1);
    
    call_out("keep_biting", ROUND_LENGTH, 4);
}

void keep_biting(int dur)
{
    if(!objectp(caster) || !objectp(target))
    {
        dest_effect();
        return;
    }
    
    tell_object(target, "%^GREEN%^BOLD%^The centipedes bite viciously into your flesh!");
    tell_room(place, "%^GREEN%^BOLD%^The centipedes bite viciously into " + target->query_cap_name() + "'s flesh!", target);
    
    if(do_save(target, 0))
        damage_targ(target, "torso", sdamage / 8, "piercing");
    else
        damage_targ(target, "torso", sdamage / 4, "piercing");
    
    dur--;
    if(dur <= 0)
    {
        dest_effect();
        return;
    }
    
    call_out("keep_biting", ROUND_LENGTH, dur);
}

void dest_effect()
{
    if (find_call_out("keep_biting") != -1)
        remove_call_out("keep_biting");
    
    if(target)
        target->remove_property("creeping doom");
    
    tell_room(place, "%^GREEN%^The centipedes fall to the ground, dead.");
    
    ::dest_effect();
}
