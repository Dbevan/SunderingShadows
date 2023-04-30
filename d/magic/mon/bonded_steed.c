/*
  bonded_steed.c
  
  Animal companion code for bonded steed for paladin.
  
  -- Tlaloc --
*/

inherit "/d/magic/mon/mounts/bonded_steed";

#include <daemons.h>
#include "/d/magic/mon/summoned_monster.h"

#define SAVEDIR "/d/save/summons/" + owner->query_name() + "/steed/"

object owner;

string saved_short,
       saved_long;

int setup,
    bonus;

int set_owner(object ob) { owner = ob; return 1; }
object query_owner()     { return owner; }

void create(){
    ::create();
    set_name("steed");
    set_id(({"steed", "bonded steed", "greater summon"}));
    set_short("A generic bonded steed");
    set_hd(4,1);
    set_max_hp(600);
    set_hp(600);
    set_exp(1);
    set_damage(2, 6);
    set_size(3);
    set_stats("strength", 30);
    set_stats("constitution", 30);
    set_stats("dexterity", 10);
    set_stats("wisdom", 10);
    set_stats("charisma", 18);
    set_stats("intelligence", 5);
    set_gender("neuter");
    set_funcs( ({ "special_attack" }) );
    set_func_chance(35);
}

void init()
{
    ::init();
    
    if(this_player() != owner)
        return;
    
    saved_short = read_file(SAVEDIR + "short");
    saved_long = read_file(SAVEDIR + "long");
    
    //Used read_file here - don't want to save whole object for 2 variables
    if(!strlen(saved_short) && !strlen(saved_long))
    {
        mkdir("/d/save/summons/" + this_player()->query_name());
        mkdir(SAVEDIR);
    }
    else
    {
        set_short(saved_short);
        set_long(saved_long);
        set_vehicle_short(saved_short);
    }
    
    add_action("animal_command", "steed");
}

int animal_command(string str)
{
    string *input, compacted;
    
    if(this_player() != owner)
        return 0;
    
    input = explode(str, " ");
    
    if(sizeof(input) < 2)
    {
        tell_object(this_player(), "Syntax : steed [long/short] [input].");
        return 1;
    }
    
    switch(input[0])
    {
        case "short":
        compacted = implode(input[1..], " ");
        this_object()->set_short(compacted);
        tell_object(this_player(), "Your Bonded Steed will now be seen as: \n" + query_short());
        rm(SAVEDIR + "short");
        write_file(SAVEDIR + "short", compacted);
        break;
        case "long":
        compacted = implode(input[1..], " ");
        this_object()->set_long(compacted);
        tell_object(this_player(), "Your Bonded Steed will now be described as: " + query_long());
        rm(SAVEDIR + "long");
        write_file(SAVEDIR + "long", compacted);
        break;
        case "command":
        command(implode(input[1..], " "));
        break;
        case "follow":
        tell_object(this_player(), "Your Bonded Steed is now following you.");
        this_player()->add_follower(this_object());
        default:
        tell_object(this_player(), "Please select 'long', 'short', 'follow' or 'command' as options.");
        return 1;
        break;
    }
    
    return 1;
}
    

void heart_beat()
{
    object *attackers, room;
           
    ::heart_beat();

    if (!objectp(this_object()))
        return;
    
    room = environment(this_object());
    
    if(!objectp(owner) || owner->query_property("animal_companion") != this_object())
    {
        this_object()->remove();
        return;
    }
    
    //Faithful companion finds his master
    if(objectp(owner) && room != environment(owner))
        this_object()->move(environment(owner));
    
    attackers = owner->query_attackers();
    
    if(sizeof(attackers))
    {
        foreach(object ob in attackers)
            this_object()->kill_ob(ob);
    }
    else
    {
        add_hp(query_max_hp() / 20);
        bonus = 0;
    }
    
    if(query_hp() < query_max_hp() / 2 && present("vial", this_object()))
        command("drink vial");
}
    
void die(object ob)
{
    //"/daemon/yuck_d"->save_inventory(this_object(), SAVEDIR + "acompanion");
    owner && tell_object(owner, "%^RED%^Your bonded steed screams in agony as it passes from this world!%^RESET%^");
    owner && owner->remove_property("animal_companion");
    remove();
}

int remove()
{
    //"/daemon/yuck_d"->save_inventory(this_object(), SAVEDIR + "acompanion");
    //all_inventory(this_object())->remove();
    if(present("barding", this_object()))
        present("barding", this_object())->remove();
    owner && owner->remove_property("animal_companion");
    ::remove();
}
