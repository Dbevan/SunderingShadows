#include <std.h>
#include <daemons.h>
inherit FEAT;

void create() {
    ::create();
    feat_type("permanent");
    feat_category("CryptStalker");
    feat_name("gird the soul");
    feat_prereq("Crypt Stalker L1");
    feat_desc(""
"Crypt stalkers rely on their faith and their unwavering hatred of undead to inure them to the effects of undead attacks. They have studied relentless in their pursuits of the undead and have reaped the benefits. They have gained extensive knowledge of the crypts where the undead often reside. The Crypt Stalker gains an immunity to death effects along with 20% negative energy resistance. Additionally, they gain a bonus when studying undead creatures.");
    permanent(1);
    set_required_for(({}));
}

int allow_shifted() { return 1; }

int prerequisites(object ob)
{
    if (!objectp(ob))
    {
        return 0;
    }

    if ((int)ob->query_class_level("crypt_stalker") < 1)
    {
        dest_effect();
        return 0;
    }
    return ::prerequisites(ob);
}

void execute_feat()
{
    ::execute_feat();
    dest_effect();
}

void permanent_effects(object ob)
{
    ::permanent_effects(ob);
    dest_effect();
    return;
}

void reverse_permanent_effects(object ob)
{
    ::reverse_permanent_effects(ob);
    dest_effect();
    return;
}

void dest_effect()
{
    ::dest_effect();
    remove_feat(TO);
    return;
}
