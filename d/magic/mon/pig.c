#include <std.h>
inherit MINION;

// LoKi - Edited so pigs wander off

void go_home(object to);

void create()
{
    ::create();
    set_name("pig");
    set_id(({ "pig", "swine" }));
    set_short("%^BOLD%^%^MAGENTA%^a pig%^RESET%^");
    set_long("%^MAGENTA%^This fat pig is a mottled pink in appearance, " +
             "covered with short, coarse hair. Dark black spots " +
             "are scattered along its girth, making it only slightly " +
             "distinct from others of its kind. The pig seems " +
             "unusually fat, perhaps from a rich diet. It doesn't " +
             "seem all that aware or interested in anything other " +
             "than eating.");
    set_gender(random(2)?"male":"female");
    set_size(1);
    set_race("pig");
    set_body_type("quadruped");
    set_stats("intelligence", 6);
    set_stats("wisdom", 6);
    set_stats("strength", 12);
    set_stats("charisma", 3);
    set_stats("dexterity", 10);
    set_stats("constitution", 13);
    set("aggressive", 8);
    set_hd(4, 2);
    set_attack_limbs(({ "right forepaw", "left forepaw", "head" }));
    set_property("no_random_treasure", 1);
    call_out("go_home", 600);
}



void go_home(object to)
{
    if (!objectp(to)) {
        return;
    }
    tell_room(environment(to), "The pig wanders off...");
    to->move("/d/shadowgate/void");
    to->remove();
}

void reset()
{
    ::reset();
    go_home(TO);
}