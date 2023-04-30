#include <std.h>
#include <daemons.h>

inherit FEAT;

void create()
{
    ::create();
    feat_type("permanent");
    feat_category("WeaponMastery");
    feat_name("weapon specialization");
    feat_prereq("Weapon Focus, Fighter L4");
    feat_desc("You gain a +2 bonus on all damage rolls you make.");
    permanent(1);
    set_required_for( ({ "greater weapon specialization" }) );
}

int allow_shifted() { return 1; }

int prerequisites(object ob) {
    int magus = 0;
    int oracle = 0;
    if (!objectp(ob)) {
        return 0;
    }

    if (ob->is_class("magus") && file_exists("/std/class/magus.c")) {
        magus = (int)"/std/class/magus.c"->fighter_training(ob);
    }
    if(ob->query_class_level("oracle") > 20 && ob->query_mystery() == "battle")
        oracle = 1;
        
    if(ob->query_class_level("fighter") + magus < 4 && !oracle) {
        dest_effect();
        return 0;
    }
    return ::prerequisites(ob);
}

void execute_feat(){
    ::execute_feat();
    dest_effect();
}

void permanent_effects(object ob){
    ::permanent_effects(ob);
    dest_effect();
    return;
}

void reverse_permanent_effects(object ob){
    ::reverse_permanent_effects(ob);
    dest_effect();
    return;
}

void dest_effect(){
    ::dest_effect();
    remove_feat(TO);
    return;
}
