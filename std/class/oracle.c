#include <std.h>
#include <mysteries.h>
#include <daemons.h>

inherit DAEMON;

void create() { ::create(); }

string *search_paths() { return ({ "/cmds/cleric" }); } // temporary

int caster_class() { return 1; }

string *restricted_races() {
    return ({ "beastman","ogre","ogre-mage","voadkyn","wemic" });
}

string *restricted_classes() { return ({  }); }

int *restricted_alignments() { return ({}); }

string *restricted_gods() { return ({}); }

mapping stat_requirements() {
    return ([ "charisma" : 16 ]);
}

// strong & weak saving throws. Fort, Ref, Will
int *saving_throws() { return ({ -1,-1,1 }); }

string *combat_styles() {
    return ({});
}

string *class_feats(string myspec)
{
    return ({ "light armor proficiency",
              "simple weapon proficiency" });
}

mapping class_featmap(string myspec, object player) {
    
    if(player->query_mystery() == "battle")
    {
        return ([
                 1: ({ "light armor proficiency", "medium armor proficiency", "heavy armor proficiency", "simple weapon proficiency", "martial weapon proficiency", "shield proficiency", "spell focus" }),
                 5 : ({ "indomitable" }),
                 10: ({ "force of personality" }),
                 21: ({ "weapon focus", "weapon specialization" }),
               ]);
    }
    
    if(player->query_mystery() == "dragon")
    {
        return ([
                 1: ({ "light armor proficiency", "medium armor proficiency", "simple weapon proficiency", "shield proficiency", "spell focus", "dragon affinity", "dragon aspect" }),
                 5 : ({ "indomitable" }),
                 10: ({ "force of personality" }),
                 15: ({ "blindfight" }),
               ]);
    }
    
    if(player->query_mystery() == "bones")
    {
        return ([
                 1: ({ "light armor proficiency", "medium armor proficiency", "simple weapon proficiency", "shield proficiency", "spell focus" }),
                 5 : ({ "indomitable", "raise the dead" }),
                 10: ({ "force of personality" }),
               ]);
    }
    
    if(player->query_mystery() == "life")
    {
        return ([
                 1: ({ "light armor proficiency", "medium armor proficiency", "simple weapon proficiency", "shield proficiency", "spell focus", "blindfight" }),
                 5 : ({ "indomitable" }),
                 10: ({ "force of personality" }),
                 31: ({ "death ward" }),
               ]);
    }
    
    if(player->query_mystery() == "spellscar")
    {
        return ([
                 1: ({ "light armor proficiency", "medium armor proficiency", "simple weapon proficiency", "shield proficiency", "spell focus" }),
                 5 : ({ "indomitable" }),
                 10: ({ "force of personality" }),
                 31: ({ "primal scar" }),
               ]);
    }
    if(player->query_mystery() == "lunar")
    {
        return ([
                 1: ({ "light armor proficiency", "medium armor proficiency", "simple weapon proficiency", "shield proficiency", "spell focus", "claw and tooth" }),
                 5 : ({ "indomitable", "primal companion" }),
                 10: ({ "force of personality" }),
               ]);
    }
    
    return ([
        1: ({ "light armor proficiency", "medium armor proficiency", "simple weapon proficiency", "shield proficiency", "spell focus" }),
        5 : ({ "indomitable" }),
        10: ({ "force of personality" }),
    ]);
}

mapping query_cantrip_spells(object ob)
{
    return ([ "create water" : 1, "detect magic" : 1, "detect poison" : 1, "guidance" : 1, "resistance" : 1, "light" : 1, "mending" : 1 ]);
}

mapping query_innate_spells(object player)
{   
    mapping innate_spells;
    int olevel;
    
    if(!objectp(player))
        return ([  ]);
    
    innate_spells = ([  ]);
    olevel = player->query_class_level("oracle");
    
    switch(player->query_mystery())
    {
        case "shadow":
        innate_spells += ([ "cloak of darkness" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 5)
            innate_spells += ([ "wings of darkness" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;
        
        case "heavens":
        innate_spells += ([ "coat of many stars" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 5)
            innate_spells += ([ "lure of the cosmos" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 15)
            innate_spells += ([ "interstellar void" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;
        case "nature":
        innate_spells += ([ "erosion touch" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 15)
            innate_spells += ([ "natural divination" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 31)
            innate_spells += ([ "cocoon" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;
        
        case "battle":
        if(olevel >= 5)
            innate_spells += ([ "battlecry" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;
        
        case "life":
        if(olevel >= 5)
            innate_spells += ([ "life link" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 21)
            innate_spells += ([ "energy body" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;

        case "bones":
        if(olevel >= 1)
            innate_spells += ([ "armor of bones" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;
        
        case "spellscar":
        if(olevel >= 1)
            innate_spells += ([ "mystic bolt" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        if(olevel >= 10)
            innate_spells += ([ "conjure elemental" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        break;
        
        case "lunar":
        if(olevel >= 21)
        {
            innate_spells += ([ "werewolf shape" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
            innate_spells += ([ "weretiger shape" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
            innate_spells += ([ "wererat shape" : ([ "type" : "spell", "daily uses" : -1, "level required" : 0 ]), ]);
        }
        break;
    }
    
    return innate_spells;
}

string *class_skills()
{
    return ({"spellcraft" });
}

string *mystery_skills(string mymystery)
{
    string *mySkills;
    mySkills = class_skills();

    if (member_array(mymystery, keys(MYSTERY_SKILLS)) != -1) {
        mySkills += MYSTERY_SKILLS[mymystery];
    }

    return mySkills;
}

int skill_points() { return 4; }

string old_save_type() { return "cleric"; }

string new_save_type() { return "cleric"; }

void advanced_func(object player)
{
    player->set_guild_level("oracle",(int)player->query_class_level("oracle"));
    return;
}

int hit_dice() { return 8; }  // hit dice rolled for hitpoints each level

int default_hitpoints() { return 3; } // hitpoints per level above level 20

string armor_allowed() { return "fighter"; }

string weapons_allowed() { return "fighter"; }

int max_stance_offensive() { return 3; }

int max_stance_defensive() { return 6; }

int attack_bonus(object player)
{
    int bonus;
    float penalty, full_level, class_level;
    
    full_level = to_float(player->query_base_character_level());
    class_level = to_float(player->query_prestige_level("oracle"));
    
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

int number_of_attacks(object player)
{
    int num;
    if(!objectp(player)) { return 0; }
    //num = "/daemon/bonus_d.c"->attack_bonus("oracle",player->query_class_level("oracle"),player);
    num = attack_bonus(player);
    num = num / 7;
    return num;
}

string query_casting_stat(){
   return "charisma";
}

mapping query_class_spells()
{
    return(([ ]));
}

int caster_level_calcs(object player, string the_class)
{
    int level;
    if(!objectp(player)) { return 0; }
    switch(the_class)
    {
        case "radiant_servant":
        case "radiant servant":
        case "oracle":
            level = player->query_class_level("oracle");
            level += player->query_class_level("radiant_servant");
            return level;

        default:
            return player->query_class_level(the_class);
    }
    return 0;
}

string newbie_choice() { return "Mystery"; }
string *query_newbie_stuff() { return sort_array(keys(MYSTERY_SKILLS),1); }
void process_newbie_choice(object who, string str)
{
    if(!objectp(who) || !stringp(str))
        return;
    who->set_mystery(str);
}
