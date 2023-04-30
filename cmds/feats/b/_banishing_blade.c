#include <std.h>
#include <daemons.h>

#define DELAY 300

inherit FEAT;

void reverse_shape();

void create()
{
    ::create();
    feat_type("instant");
    feat_category("CrimsonTemplar");
    feat_name("banishing blade");
    feat_prereq("Crimson Templar L7");
    feat_syntax("banishing_blade");
    feat_desc("With this feat, the Crimson Templar instills their weapon with holy power, granting it the ability to banish unworthy souls with a mere touch. When using this active feat, your normal weapon strikes will automatically destroy any minion belonging to an evil or chaotic caster/summoner. This feat has a cooldown and limited duration. This feat requires a weapon to use.");
    set_target_required(0);
    allow_blind(1);
    set_required_for(({}));
}

int allow_shifted() { return 0; }

int prerequisites(object ob)
{
    if(!objectp(ob)) { return 0; }

    if((int)ob->query_class_level("crimson_templar") < 7)
    {
        dest_effect();
        return 0;
    }
    return ::prerequisites(ob);
}

int cmd_banishing_blade(string str)
{
    object feat;
    if(!objectp(this_player())) { return 0; }
    feat = new(base_name(this_object()));
    feat->setup_feat(this_player(),str);
    return 1;
}

void execute_feat()
{
    if (FEATS_D->is_active(caster, "banishing blade"))
    {
        tell_object(caster, "%^RESET%^%^C039%^You r%^C045%^e%^C051%^l%^C087%^e%^C123%^a%^C159%^s%^RESET%^%^C195%^e the %^RESET%^%^C033%^b%^C039%^a%^C045%^n%^C051%^i%^C087%^s%^C051%^h%^C045%^i%^C039%^n%^RESET%^%^C033%^g p%^RESET%^%^C039%^o%^C045%^w%^C039%^e%^RESET%^%^C033%^r %^RESET%^%^C039%^from your weapons.%^CRST%^");
        dest_effect();
        return;
    }    
    if(caster->query_casting())
    {
        tell_object(caster,"%^BOLD%^You can't use banishing blade while casting a spell.");
        dest_effect();
        return;
    }   
    if(caster->query_property("using instant feat"))
    {
        tell_object(caster,"You are already in the middle of using a feat!");
        dest_effect();
        return;
    }    
    if(caster->cooldown("banishing blade"))
    {
        tell_object(caster, "You aren't ready to use banishing blade yet.");
        dest_effect();
        return;
    }
    if(!sizeof(caster->query_wielded()))
    {
        tell_object(caster, "You need to be wielding a weapon to use banishing blade.");
        dest_effect();
        return;
    }
    
    ::execute_feat();

    tell_object(caster,"%^C039%^You %^RESET%^%^C015%^channel %^RESET%^%^C039%^your given %^RESET%^%^C033%^p%^C045%^o%^C051%^w%^C045%^e%^C033%^r %^RESET%^%^C039%^and %^RESET%^%^C087%^instill%^RESET%^%^C039%^ your weapon with %^RESET%^%^C220%^h%^C226%^o%^C227%^l%^C220%^y e%^RESET%^%^C226%^n%^C227%^e%^C228%^r%^C226%^g%^RESET%^%^C220%^y%^RESET%^%^C039%^.%^CRST%^");
    caster->set_property("using instant feat",1);
    caster->set_property("active_feats", ({ this_object() }));
}

void execute_attack()
{
    object shape;
    string pact, file;

    if(!objectp(caster)) { return; }

    caster->remove_property("using instant feat");
    ::execute_attack();
    
    if(!sizeof(caster->query_wielded()))
    {
        tell_object(caster, "You need to be wielding a weapon to use banishing blade");
        dest_effect();
        return;
    }
        
    
    tell_object(caster, "%^C039%^You feel %^RESET%^%^C015%^w%^C230%^h%^C229%^i%^C228%^t%^C227%^e %^RESET%^%^C229%^h%^C230%^o%^RESET%^%^C015%^t %^RESET%^%^C220%^p%^C226%^o%^C227%^w%^C226%^e%^C220%^r %^RESET%^%^C039%^course through your weapon as you instill it with %^RESET%^%^C033%^b%^C039%^a%^C045%^n%^C051%^i%^C087%^s%^C051%^h%^C045%^i%^C039%^n%^RESET%^%^C033%^g %^RESET%^%^C033%^e%^RESET%^%^C039%^n%^C045%^e%^C051%^r%^C039%^g%^RESET%^%^C033%^y%^RESET%^%^C039%^.%^CRST%^");
    tell_room(place, "%^RESET%^%^C039%^" + caster->query_cap_name() + "%^RESET%^%^C039%^ %^RESET%^%^C087%^instills%^RESET%^%^C039%^ " + caster->query_possessive() + " %^RESET%^%^C039%^weapon with %^RESET%^%^C033%^b%^C039%^a%^C045%^n%^C051%^i%^C087%^s%^C051%^h%^C045%^i%^C039%^n%^RESET%^%^C033%^g p%^RESET%^%^C039%^o%^C045%^w%^C039%^e%^RESET%^%^C033%^r%^RESET%^%^C039%^!%^CRST%^", caster);
    caster->set_property("banishing blade", 1);
    
    caster->add_cooldown("banishing blade", DELAY);
    call_out("dest_effect", 60);
    
    return;
}

void dest_effect()
{   
    if(objectp(caster))
    {
        caster->remove_property("banishing blade");
        tell_object(caster, "%^RESET%^%^C039%^The %^RESET%^%^C033%^b%^C039%^a%^C045%^n%^C051%^i%^C087%^s%^C051%^h%^C045%^i%^C039%^n%^RESET%^%^C033%^g e%^RESET%^%^C039%^n%^C045%^e%^C051%^r%^C039%^g%^RESET%^%^C033%^y %^RESET%^%^C051%^f%^C087%^a%^C123%^d%^C159%^e%^C195%^s %^RESET%^%^C039%^from your weapon.%^CRST%^");
    }
    
    ::dest_effect();
    remove_feat(this_object());
    return;
}
