#include <std.h>
#include <daemons.h>
#include <dirs.h>

inherit FEAT;

void create() 
{
    ::create();
    feat_type("permanent");
    feat_category("WeaponAndShield");
    feat_name("shield focus");
    feat_desc("This feat represents skill at deflecting blows with a shield. This feat adds +1 AC and an additional +1 AC per 10 character levels, for a max of +6 AC at max level. Please note that bucklers do not count as shields for the purpose of this feat.");
    permanent(1);
    set_target_required(0);
    set_required_for(({}));
}

int allow_shifted() { return 0; }

int prerequisites(object ob)
{
    if(!objectp(ob)) { return 0; }

    return ::prerequisites(ob);
}

void execute_feat()
{
    ::execute_feat();
    dest_effect();
    return;
}

void dest_effect()
{
    ::dest_effect();
    remove_feat(TO);
    return;
}
