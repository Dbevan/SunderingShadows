/*
  _massacre.c
  
  Necromancy Splash Spell.
  
  -- Tlaloc --
*/

#include <std.h>
#include <spell.h>
#include <magic.h>
inherit SPELL;

int current;
string target_limb;
object *targets;

void create()
{
    ::create();
    set_author("nienne");
    set_spell_name("massacre");
    set_spell_level(([ "mage" : 9, "cleric" : 9 ]));
    set_domains(({ "evil" }));
    set_spell_sphere("necromancy");
    set_syntax("cast CLASS massacre on TARGET");
    set_damage_desc("Death or Divine Damage to target with splash");
    set_save("fortitude");
    set_description("You unleash a wave of necromantic energy that snuffs out the life force of those in its path. This wave pulses out from you, visibly ripping the souls from the bodies of those it passes through. All other must make a combat death save or die. Those who make the save will take full normalized divine damage, and have their healing diminished for a short time. This spell hits fewer targets than spells like Wail of the Banshee.");
    evil_spell(1);
    set_verbal_comp();
    set_target_required(1);
    splash_spell(2);
}

string query_cast_string()
{
    return "%^BOLD%^%^BLACK%^" + caster->QCN + " begins to utter words of slaughter.%^RESET%^";
}

void spell_effect(int prof)
{
    int difflevel, myoutput, mydmg;

    if (!target || !present(target,environment(caster)))
    {
        tell_object(caster,"%^BOLD%^Your target is not in this area.\n");
        dest_effect();
        return;
    }
    
    tell_object(caster,"%^BOLD%^%^GREEN%^You motion and a wave of slaughter ripples away from you, killing those in its path!");
    tell_room(place,"%^BOLD%^%^GREEN%^"+caster->QCN+" motions and a wave of slaughter ripples outward, killing all those in its path!",({caster}));
    
    spell_successful();
    spell_kill(target, caster);

    targets = target_selector();
    
    foreach(object ob in targets)
    {
        spell_kill(ob, caster);
        
        if((ob->query_character_level() < caster->query_character_level() - 5) ||
        !combat_death_save(ob, 0))
        {
            tell_object(target,"%^BOLD%^The wave of death rips your soul from your body, leaving a pale after-image!");
            tell_room(place,"%^BOLD%^"+target->QCN+"'s soul is ripped from their body, leaving a pale after-image!",({target}));
            target->set_hp(-100);
            continue;
        }
        else
        {
            tell_object(target,"%^BOLD%^The wave of death passes through you, and you barely retain your soul!");
            tell_room(place,"%^BOLD%^The wave of death passes through "+target->QCN+", and they barely retain their soul!",({target}));
            target && target->cause_typed_damage(target, target->return_target_limb(), sdamage,"divine");
            target && target->set_property("fester", sdamage / 2);
            continue;
        }
    }
 
    call_out("dest_effect", 1 + (6 * clevel) / 10); 
}

void dest_effect()
{
    foreach(object ob in targets)
    {
        if(!objectp(ob))
            continue;
        
        tell_object(ob, "You feel your wounds stop festering.");
        ob->remove_property("fester");
    }
    
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}
