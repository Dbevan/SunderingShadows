#include <std.h>
#include <dirs.h>
#include <daemons.h>

inherit DAEMON;


void create()
{
    ::create();
}

int is_locked_class() { return 0; }

object base_class_ob(object ob)
{
    object class_ob;
    if(!objectp(ob) || !ob->query("base_class")) { class_ob = find_object_or_load(DIR_CLASSES+"/fighter.c"); }
    else { class_ob = find_object_or_load(DIR_CLASSES+"/"+ob->query("base_class")+".c"); }
    if(!objectp(class_ob)) { class_ob = find_object_or_load(DIR_CLASSES+"/fighter.c"); }
    return class_ob;
}

int set_base_class(object obj, string choice)
{
    object *classes;

    if (!objectp(obj)) {
        return 0;
    }
    if (choice == 0) {
        obj->delete("base_class");
        return 1;
    }
    classes = obj->query_classes();
    if (!sizeof(classes)) {
        return 0;
    }
    if (member_array(choice, classes) == -1) {
        return 0;
    }

    obj->set("base_class", choice);
    return 1;
}

string *query_base_classes(object obj)
{
    string base;
    if(!objectp(obj)) { return ({}); }
    base = obj->query("base_class");
    if(!base) { return ({}); }
    return ({ base });
}

void remove_base_class(object obj)
{
    if(!objectp(obj)) { return; }
    obj->delete("base_class");
    return;
}


int has_base_class_set(object obj)
{
    if(!objectp(obj)) { return 0; }
    if(obj->query("base_class")) { return 1; }
    return 0;
}

int is_prestige_class()
{
    return 1;
}

int requires_base_class_set()
{
    return 1;
}                               // for prestige classes that allow many different base classes

string *search_paths(object ob)
{
    return base_class_ob(ob)->search_paths();
}

int caster_class(object ob)
{
    return 1;
    //return base_class_ob(ob)->caster_class();
}

string *restricted_races(object ob)
{
    return base_class_ob(ob)->restricted_races();
}

string *restricted_classes(object ob)
{
    return base_class_ob(ob)->restricted_classes();
}

int *restricted_alignments(object ob)
{
    return base_class_ob(ob)->restricted_alignments();
}

string *restricted_gods(object ob)
{
    return base_class_ob(ob)->restricted_gods();
}

string requirements() // string version, maybe we'll need this, maybe not, can remove later if not
{
    string str;
    str = "Prerequisites:\n"
        "    20 Levels"
        "    Lawful Good\n"
        "    Kreysneothosies, Edea or Jarmila follower\n"
        "    10 Ranks spent in Stealth Skill\n";

    return str;
}

int prerequisites(object player)
{
    mapping skills;
    object race_ob;
    string race, base;
    int adj;

    if (!objectp(player)) {
        return 0;
    }

    race = player->query("subrace");
    if (!race) {
        race = player->query_race();
    }
    race_ob = find_object_or_load(DIR_RACES + "/" + player->query_race() + ".c");
    if (!objectp(race_ob)) {
        return 0;
    }
    skills = player->query_skills();

    //if(player->query("subrace") == "tiefling")
    //    return 0;

    base = player->query("base_class");
    if (!base) {
        return 0;
    }
    if (!player->is_class(base))
    {
        return 0;
    }

    if ((player->query_class_level(base)) < 20)
    {
        write("fail level");
        return 0;
    }

    if(player->query_true_align() != 1)
    {
        write("fail alignment");
        return 0;
    }

    if(player->query_diety() != "kreysneothosies" && player->query_diety() != "jarmila" && player->query_diety() != "edea")
    {
        write("fail deity");
        return 0;
    }

    if (!skills["stealth"] || skills["stealth"] < 10)
    {
        write("fail stealth");
        return 0;
    }
    return 1;
}

mapping stat_requirements(object ob)
{
    return ([  ]);
}

int *saving_throws(object ob) { return base_class_ob(ob)->saving_throws(); }

string *combat_styles(object ob) { return base_class_ob(ob)->combat_styles(); }

string *class_feats(string myspec) { return base_class_ob(0)->class_feats(myspec); }

int caster_level_calcs(object player, string the_class)
{
    int level;
    string base;

    if (!objectp(player)) {
        return 0;
    }
    base = player->query("base_class");

    //level = player->query_class_level(base);
    level = player->query_class_level(the_class);

    if(base == the_class)
        level += player->query_class_level("crimson_templar");

    return level;
}

mapping class_featmap(string myspec) {
    return ([ 1 : ({ "fiendish studies" }), 4 : ({ "heavenly fire" }), 7 : ({ "banishing blade" }), ]);
}

string *class_skills(object ob) { return base_class_ob(ob)->class_skills(ob); }

int skill_points(object ob) { return base_class_ob(ob)->skill_points(); }

string old_save_type(object ob) { return base_class_ob(ob)->old_save_type(); }

string new_save_type(object ob) { return base_class_ob(ob)->new_save_type(); }

// unsure on this one, will have to investigate
void advanced_func(object player) { return base_class_ob(player)->advance_func(player); }

int hit_dice(object ob) { return base_class_ob(ob)->hit_dice(ob); }  // hit dice rolled for hitpoints each level

int default_hitpoints(object ob) { return base_class_ob(ob)->default_hitpoints(); } // hitpoints per level above level 20

string armor_allowed(object ob) { return base_class_ob(ob)->armor_allowed(); }

string weapons_allowed(object ob) { return base_class_ob(ob)->weapons_allowed(); }

int max_stance_offensive(object ob) { return base_class_ob(ob)->max_stance_offensive(); }

int max_stance_defensive(object ob) { return base_class_ob(ob)->max_stance_defensive(); }

int attack_bonus(object player) { return 0; }

int number_of_attacks(object player) { return base_class_ob(player)->number_of_attacks(player); }

string newbie_choice(object ob) { return base_class_ob(ob)->newbie_choice(); }

string *query_newbie_stuff(object ob) { return base_class_ob(ob)->query_newbie_stuff(); }

void process_newbie_choice(object who, string str) { return base_class_ob(who)->process_newbie_choice(who,str); }

string query_casting_stat(object ob) { return base_class_ob(ob)->query_casting_stat(); }

mapping query_class_spells(object ob) { return base_class_ob(ob)->query_class_spells(); }
