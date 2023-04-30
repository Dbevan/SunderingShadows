/*
  _warlocks_curse.c
  
  Softens the target to the warlock's attacks and spells.
  
  -- Tlaloc -- 8.20.22
*/

#include <std.h>
#include <daemons.h>
#include <magic.h>

inherit FEAT;

#define FEATTIMER 60
#define ROUNDS 5

void create()
{
    ::create();
    feat_type("insant");
    feat_name("warlocks curse");
    feat_category("PactBond");
    feat_syntax("warlocks_curse [TARGET]");
    feat_prereq("Warlock L5");
    feat_classes("warlock");
    feat_desc("Apply a nefarious curse to your opponents, making it plain for all to see the sheer foolishness of their endeavor. You may apply one curse to a given opponent (they do not stack) which will make them particularly weak to YOUR attacks and spells. The opponent will take an additional 1d6 (plus 1d6 more per 10 warlock levels) damage from any attack or spell you land upon them. This feat has a cooldown.");
}

int allow_shifted() { return 1; }

int prerequisites(object ob)
{
    if(ob->query_class_level("warlock") < 5)
    {
        dest_effect();
        return 0;
    }
    return ::prerequisites(ob);
}

int cmd_warlocks_curse(string str)
{
    object feat;
    
    if(!objectp(this_player()))
        return 0;
    
    feat = new(base_name(this_object()));
    feat->setup_feat(this_player(),str);
    return 1;
}

void execute_feat()
{
    int delay;
    string my_name, your_name, my_poss;
    
    if(!objectp(caster))
    {
        dest_effect();
        return;
    }
    if(!sizeof(caster->query_attackers()))
    {
        tell_object(caster,"You're not under attack!");
        dest_effect();
        return;
    }
    if((int)caster->query_property("using instant feat"))
    {
        tell_object(caster,"You are already in the middle of using a feat!");
        dest_effect();
        return;
    }
    if(caster->cooldown("warlocks curse"))
    {
        tell_object(caster, "You can't use warlocks curse yet.");
        dest_effect();
        return;
    }
    
    if(!objectp(target))
        target = caster->query_current_attacker();
    
    ::execute_feat();
    
    if(!objectp(target) || !objectp(caster))
    {
        dest_effect();
        return;
    }
    
    my_name = caster->query_cap_name();
    your_name = target->query_cap_name();
    my_poss = caster->query_possessive();
  
    tell_object(caster, "%^C242%^You point your finger and utter an i%^C244%^n%^C246%^s%^C143%^i%^C149%^d%^C155%^i%^C149%^o%^C143%^u%^C242%^s curse of v%^C244%^u%^C246%^l%^C143%^n%^C149%^e%^C155%^r%^C149%^a%^C143%^b%^C246%^i%^C244%^l%^C242%^ity upon " + your_name + "!%^CRST%^");
    tell_object(target,"%^C242%^" + my_name + " points " + my_poss + " finger and utters an i%^C244%^n%^C246%^s%^C143%^i%^C149%^d%^C155%^i%^C149%^o%^C143%^u%^C242%^s curse of v%^C244%^u%^C246%^l%^C143%^n%^C149%^e%^C155%^r%^C149%^a%^C143%^b%^C246%^i%^C244%^l%^C242%^ity upon you!%^CRST%^");
    tell_room(place, "%^C242%^" + my_name + " points " + my_poss + " finger and utter an i%^C244%^n%^C246%^s%^C143%^i%^C149%^d%^C155%^i%^C149%^o%^C143%^u%^C242%^s curse of v%^C244%^u%^C246%^l%^C143%^n%^C149%^e%^C155%^r%^C149%^a%^C143%^b%^C246%^i%^C244%^l%^C242%^ity upon " + your_name + "!%^CRST%^", ({ caster, target }));
    caster->set_property("using instant feat",1);
}

void execute_attack()
{
    int dam, mod, glvl, opposed;
    object target, attackers;
    
    caster->remove_property("using instant feat");
    
    if(!caster || caster->query_unconscious())
    {
        dest_effect();
        return;
    }
    
    ::execute_attack();
    
    if(!target)
        target = caster->query_current_attacker();
    
    if(!target)
        return;
    
    tell_object(caster, "%^C143%^You feel a twinge of satisfaction as your curse takes hold!%^CRST%^");
    tell_object(target, "%^C143%^You feel the curse sink deep into your soul!%^CRST%^");
    tell_room(place, "%^C143%^" + caster->query_cap_name() + " smiles in grim satisfaction as " + caster->query_possessive() + " curse takes hold.%^CRST%^", ({ caster, target }));
    target->remove_property("warlocks curse");
    target->set_property("warlocks curse", caster);
    caster->add_cooldown("warlocks curse", FEATTIMER);
    call_out("finish_curse", ROUNDS * ROUND_LENGTH, target, caster);
}

void finish_curse(object you, object me)
{
    
    if(you)
    {
        if(you->query_property("warlocks curse"))
        {
            you->remove_property("warlocks curse");
            tell_object(you, "%^BOLD%^You feel the warlock's curse fade from you.%^RESET%^");
            me && tell_object(me, "%^BOLD%^Your warlock curse fades from your foe.%^RESET%^");
        }
    }
    dest_effect();
}

void dest_effect()
{
    ::dest_effect();
    remove_feat(this_object());
}