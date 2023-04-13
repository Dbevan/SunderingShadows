// New psychic warrior class (psywarrior to avoid recoding a ton of stuff to take two-word class names)
// ~Circe~ 9/1/15
#include <std.h>

inherit DAEMON;

void create() { ::create(); }

string *search_paths() { return ({ "/cmds/psionics", "/cmds/psywarrior" }); } // temporary

int caster_class() { return 1; }

string *restricted_races() { return ({"beastman", "bugbear", "centaur", "drow", "dwarf", "elf", "firbolg", "gnoll", "gnome", "goblin", "half-ogre", "halfling", "kobold", "minotaur", "ogre", "ogre-mage", "orc", "voadkyn", "wemic", }); }

string *restricted_classes() { return ({ "psion" }); }

int *restricted_alignments() { return ({}); }

string *restricted_gods() { return ({}); }

mapping stat_requirements() {
    return ([ "intelligence" : 14 ]); // making them intelligence-based like psions
}

// strong & weak saving throws. Fort, Ref, Will
int *saving_throws() { return ({ 1,-1,-1 }); }

string *combat_styles() {
    return ({});
}

string *class_feats(string myspec) {
    return ({ "light armor proficiency", "medium armor proficiency", "heavy armor proficiency", "shield proficiency", "simple weapon proficiency", "martial weapon proficiency", "armored manifester", "combat manifester" });
}

mapping class_featmap(string myspec) {
    return ([ 1 : ({ "light armor proficiency", "medium armor proficiency", "heavy armor proficiency", "shield proficiency", "simple weapon proficiency", "martial weapon proficiency", "armored manifester", "combat manifester" }), 5 : ({ "trance" }), 7 : ({ "warrior path" }), 11 : ({ "martial power" }), 21 : ({ "deep focus" }), 31 : ({ "eternal warrior" }) ]);
}

string *class_skills()
{
    return ({ "academics","athletics","endurance","perception" });
//climb, jump, and ride are class skills, so giving them athletics
//giving them endurance in place of autohypnosis class skill
}

int skill_points() { return 4; }

string old_save_type() { return "fighter"; }

string new_save_type() { return "fighter"; }

void advanced_func(object player) {
    player->set_guild_level("psywarrior",(int)player->query_class_level("psywarrior"));
    return;
}

int hit_dice() { return 8; }  // hit dice rolled for hitpoints each level

int default_hitpoints() { return 4; } // hitpoints per level above level 20

string armor_allowed() { return "fighter"; }

string weapons_allowed() { return "fighter"; }

int max_stance_offensive() { return 4; }

int max_stance_defensive() { return 4; }

int attack_bonus(object player)
{
    int bonus;
    float penalty, full_level, class_level;
    
    full_level = to_float(player->query_base_character_level());
    class_level = to_float(player->query_prestige_level("psywarrior"));
    
    if(full_level < 20.00)
    {
        bonus = (to_int(class_level) * 3 / 4);
        return bonus;
    }
    
    // Above 20
    // 3/4 BAB gets half penalty to BAB
    // Weighted average of class level compared to total level
    penalty = (10.00 * (class_level / full_level)) / 2.00;
    bonus = to_int(class_level - penalty);
    
    return bonus;
}

int number_of_attacks(object player){
    int num;
    if(!objectp(player)) { return 0; }
    //num = "/daemon/bonus_d.c"->attack_bonus("psywarrior",player->query_class_level("psywarrior"),player);
    num = attack_bonus(player);
    num = num / 7;
    return num;
}

string query_casting_stat(){
   return "intelligence";
}

// ADD THESE
// NO
mapping query_class_spells()
{
    return(([ ]));
}

void newbie_func(object who)
{
    object ob;
    if(!objectp(who)) return;
}

int caster_level_calcs(object player, string the_class)
{
    int level;
    if(!objectp(player)) { return 0; }
    switch(the_class)
    {
        case "psywarrior":
        default:
            return player->query_class_level(the_class);
    }
    return 0;
}
