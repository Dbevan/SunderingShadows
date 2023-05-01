#include <std.h>
#include <daemons.h>
#include <magic.h>

inherit WEAPONLESS;

object *combatants = ({  });

void create()
{
    ::create();
    set_property("no death", 1);
    set_property("no fear", 1);
    set_property("no bows", 1);
    set_property("no knockdown", 1);
    set_property("no trip", 1);
    set_property("no tripped", 1);
    set_property("no steal", 1);
    set_property("no dominate", 1);
    set_property("no hold", 1);
    set_property("confusion_immunity", 1);
    set_property("hidden inventory", 1);
    set_property("cast and attack", 1);
    set_property("function and attack", 1);
    set_property("full attacks", 1);
}

void init()
{
    ::init();
    
    if(userp(this_player()) && !avatarp(this_player()) && this_player()->query_base_character_level() >= 40)
        combatants += ({ this_player() });
}

void boss_death_event() { return; }

void die()
{
    string *files, *selected;
    object *attackers;
    
    files = get_dir("/d/common/bosses/loot/*.c");
    files = filter_array(files, (: !catch(load_object("/d/common/bosses/loot/" + $1)) :));
    
    selected = ({  });
    
    while(sizeof(selected) < 5)
    {
        int temp;
        
        temp = random(sizeof(files));
        selected += ({ files[temp] });
        files -= ({ files[temp] });        
        selected = distinct_array(selected);
    }
    
    foreach(string file in selected)
        new("/d/common/bosses/loot/" + file)->move(this_object());
        
    catch(this_object()->boss_death_event());
    environment(this_object())->set_had_players(288);
    
    /*
    attackers = all_living(environment(this_object()));
    attackers -= ({ this_object() });
    attacker = filter_array(attackers, (: (userp($1) && !avatarp($1)) :));
    */
       
    //foreach(object ob in attackers)
    foreach(object ob in combatants)
    {
        if(!objectp(ob))
            continue;
        
        if(ob->query_base_character_level() >= 40)
            ob->set("boss avatar", 1);
    }
        
    ::die();
        
    foreach(object ob in combatants)
        ob->cease_all_attacks();
}

