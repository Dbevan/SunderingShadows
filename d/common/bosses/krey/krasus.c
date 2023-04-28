/*
  krasus.c
  
  Great golden dragon. Avatar boss of Krey.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>
#include <magic.h>

#define COREPARTY 4

inherit "/d/common/bosses/avatar_boss.c";

int buffed,
    coreparty,
    ticker;

mapping checkpoints = ([
                        "shield"  : 0;
                        "spear"   : 0;
                        "barrage" : 0;
                        "dragon"  : 0;
                        "waves"   : 0;
                      ]);
                      
object barrage_room;

void create()
{
    set_name("krasus");
    set_id( ({ "dragonkin", "defender", "warrior", "golden defender" }) );
    set_short("Krasus, the Great Golden Defender");
    set_long("");
    set_race("human");
    set_gender("male");
    set_hd(75, 10);
    set_class("paladin");
    set_mlevel("paladin", 75);
    set_class("immortal_defender");
    set_mlevel("immortal_defender", 10);
    set_class("cleric");
    set_mlevel("cleric", 75);
    set("base_class", "paladin");
    set_alignment(1);
    set_damage(20, 10);
    set_attacks_num(10);
    
    set_stats("strength", 30);
    set_stats("constitution", 30);
    set_stats("charisma", 30);
    set_stats("wisdom", 10);
    set_stats("dexterity", 14);
    set_stats("intelligence", 10);    
    set_skill("perception", 50);
    set_mob_magic_resistance("high");
    set_overall_ac(-80);
    set_sight_bonus(10);
    set_max_hp(125000);
    set_hp(125000);
    
    set_monster_feats( ({ "damage resistance", "improved damage resistance", "weapon focus", "rush", "shield focus", "shieldbash", "resistance", "improved resistance", "increased resistance", "expertise", "parry", "shieldwall", "counter", "weapon bond", "armor bond", "penetrating strike", "layonhands", "smite", "dreadful carnage", "cornugon smash", "shatter defenses", "intimidating prowess", "dazzling display" }) );
    
    set_spells( ({ "shield of law", "stone body", "angelic aspect", "archon aura" }) );
    
    if(!clonep())
        return;
    
    obj = new("/d/magic/obj/shields/kreysneothosies");
    obj->set_property("enchantment", 7);
    obj->move(this_object());
    obj->set_property("monster weapon", 1);
    force_me("wear shield");

    obj = new("/d/magic/obj/weapons/kreysneothosies");
    obj->set_property("enchantment", 7);
    obj->move(this_object());
    obj->set_property("monster weapon", 1);
    force_me("wield weapon");
}

void init()
{
    object player, room;
    
    ::init();
    
    player = this_player();
    player && room = environment(this_object());
    
    if(!player || !room)
        return;
    
    if (wizardp(player) || player->query_true_invis()) {
        return;
    }
    
    if(!buffed)
    {
        command("shieldwall max");
        command("enhance add axiomatic");
        command("enhance add holy");
        command("enhance weapon");
        command("enhance add light fortification")
        command("enhance armor");
        new("/cmds/spells/2/_shield_of_law.c")->use_spell(this_object(), 0, 70, 100, "cleric");
        new("/cmds/spells/s/_stone_body.c")->use_spell(this_object(), 0, 70, 100, "cleric");
        new("/cmds/spells/a/_angelic_aspect.c")->use_spell(this_object(), 0, 70, 100, "cleric");
        new("/cmds/spells/a/_archon_aura.c")->use_spell(this_object(), 0, 70, 100, "cleric");
        buffed = 1;
    }        
}
    

