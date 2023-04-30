#include <std.h>
#include <daemons.h>
#include <magic.h>

#define DELAY 18

inherit FEAT;

int mod, duration;

void create()
{
    ::create();
    feat_type("instant");
    feat_category("PactBond");
    feat_name("reclaim soul");
    feat_prereq("Warlock L17");
    feat_syntax("reclaim_soul");
    feat_desc("With this feat, you relinquish some of your vast and dark power to reclaim and mend the shattered and burnt pieces of your broken soul. This feat removes one soul burn. This feat has a small cooldown.");
}

int allow_shifted() { return 1; }

int prerequisites(object ob)
{
    if(!objectp(ob)) { return 0; }

    if(ob->query_class_level("warlock") < 17)
    {
        dest_effect();
        return 0;
    }

    return ::prerequisites(ob);
}

int cmd_reclaim_soul(string str)
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
    ::execute_feat();

    /*
    if((int)caster->query_property("using instant feat"))
    {
        tell_object(caster,"You are already in the middle of using a feat!");
        dest_effect();
        return;
    }
    */

    if(caster->cooldown("reclaim soul"))
    {
        tell_object(caster,"You are not ready to use Reclaim Soul yet!");
        dest_effect();
        return;
    }

    if(!USER_D->spend_pool(caster, -1, "burn"))
    {
        tell_object(caster, "You have no soul burn at this time.");
        dest_effect();
        return;
    }

    //caster->set_property("using instant feat", 1);
    caster->add_cooldown("reclaim soul", DELAY - (FEATS_D->has_feat(caster, "resilient soul") * 6));

    tell_object(caster, "%^C066%^You give up some of your power to mend your soul burn.%^CRST%^");
    tell_object(caster, "%^C072%^You feel a little bit more whole.%^CRST%^");
}

/*
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
*/

void dest_effect()
{
    ::dest_effect();
    remove_feat(this_object());
    return;
}
