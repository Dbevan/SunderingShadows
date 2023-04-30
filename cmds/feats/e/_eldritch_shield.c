#include <std.h>
#include <daemons.h>
#include <magic.h>

#define DELAY 120

inherit FEAT;

int mod, duration;

void create()
{
    ::create();
    feat_type("instant");
    feat_category("PactBond");
    feat_name("eldritch shield");
    feat_prereq("Warlock L21");
    feat_syntax("eldritch_shield");
    feat_desc("You have proven yourself a useful pawn and your patron is forced to recognize your dedication. With this feat, you demand your patron make good on their promises. You expend all of your soul burn, gaining a damage absorption shield that will block physical damage equal to your warlock prestige level times the level of soul burn you currently have. Your soul burn will be fully expended.");
}

int allow_shifted() { return 1; }

int prerequisites(object ob)
{
    if(!objectp(ob)) { return 0; }

    if(ob->query_class_level("warlock") < 21)
    {
        dest_effect();
        return 0;
    }

    return ::prerequisites(ob);
}

int cmd_eldritch_shield(string str)
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
    int amount;
    
    ::execute_feat();

    if((int)caster->query_property("using instant feat"))
    {
        tell_object(caster,"You are already in the middle of using a feat!");
        dest_effect();
        return;
    }

    if(caster->query_property("eldritch shield"))
    {
        tell_object(caster,"You are already using eldritch shield!");
        dest_effect();
        return;
    }

    if(caster->cooldown("eldritch shield"))
    {
        tell_object(caster, "You aren't ready to use Eldritch Shield yet.");
        dest_effect();
        return;
    }
    
    if(caster->query("available burn") < 1)
    {
        tell_object(caster, "You don't have any Soul Burn to use for your shield.");
        dest_effect();
        return;
    }
    
    amount = caster->query("available burn");
    amount = caster->query_prestige_level("warlock") * amount;

    caster->set_property("using instant feat", 1);
    caster->set_property("eldritch shield", amount);
    caster->set("available burn", 0);
    caster->recalculate_max_hp_from_soulburn();

    tell_object(caster, "%^C244%^Without a word, you reach out to your %^C093%^p%^C099%^a%^C105%^t%^C111%^r%^C105%^o%^C093%^n%^C244%^ and demand protection for your dedication.%^CRST%^");
    tell_object(caster, "%^C244%^Your demands are answered and a %^C093%^d%^C099%^a%^C105%^r%^C093%^k s%^C099%^h%^C105%^i%^C099%^e%^C093%^ld%^C244%^ protects you from harm.%^CRST%^");
}

void execute_attack()
{

    if(!objectp(caster))
    {
        dest_effect();
        return;
    }

    caster->remove_property("using instant feat");
    ::execute_attack();
    
    dest_effect();
}

void dest_effect()
{
    ::dest_effect();
    remove_feat(this_object());
    return;
}
