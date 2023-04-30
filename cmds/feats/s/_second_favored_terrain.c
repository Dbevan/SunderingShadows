/*
  _second_favored_terrain.c
  
  Allows the ranger to set their second favored terrain
  
  -- Tlaloc -- 7.10.20
*/

#include <std.h>
#include <daemons.h>
#include <dirs.h>
#include <favored_types.h>

inherit FEAT;

string *valid_choices;

void create()
{
    ::create();
    
    valid_choices = keys(VALID_TERRAIN);
    
    set_author("tlaloc");
    feat_type("instant");
    feat_category("WildernessLore");
    feat_name("second favored terrain");
    feat_prereq("Ranger L13");
    feat_syntax("second_favored_terrain [TYPE]");
    feat_desc("This feat allows a ranger to focus their knowledge and training for operating on a second favored terrain of their choice. While within their favored terrain, the ranger gains an additional +2 to stealth, perception and survival skills. You can select from the following favored terrain: \n\n"+"%^GREEN%^"+implode(valid_choices, "\n")+"%^RESET%^\n\n\To start selection process type <second_favored_terrain> [TYPE].");
    allow_tripped(1);
    set_required_for(({ "third favored terrain" }));
}

int allow_shifted() { return 1; }

int cmd_second_favored_terrain(string str)
{
    object feat;
    
    if(!objectp(this_player()))
        return 0;
    
    if(this_player()->query_class_level("ranger") < 13)
    {
        dest_effect();
        return 0;
    }
    
    feat = new(base_name(this_object()));
    feat->setup_feat(this_player(), str);
    
    return 1;
}

int execute_feat()
{
    ::execute_feat();
    
    if(!arg)
    {
        write("Your second favored terrain is: " + this_player()->query_favored_terrain(2));
        return 1;
    }
    
    if(this_player()->query_property("second_favored_terrain_change") > time() - 259200)
    {
        write("You may only change your second favored terrain once every three weeks.");
        return 1;
    }
    
    if(member_array(arg, valid_choices) < 0)
    {
        write("Valid choices are : " + implode(valid_choices, ", "));
        return 1;
    }
    
    write("You have selected " + arg + " as a favored terrain. Type <yes> to confirm. Anything else will abort.");
    input_to("confirm_selection", 0);
    
    return 1;
}

void confirm_selection(string str)
{
    if (str != "yes") {
        tell_object(this_player(), "Aborting...");
        return;
    }

    write("You declare your second favored terrain as : " + arg + ".");
    this_player()->set_favored_terrain(2, arg);
    this_player()->set_property("second_favored_terrain_change", time());
    dest_effect();
    return;
}

int prerequisites(object ob)
{
    if (!objectp(ob)) {
        return 0;
    }
    if(ob->query_class_level("ranger") < 13)
    {
        dest_effect();
        return 0;
    }
    return ::prerequisites(ob);
}

void dest_effect()
{
    ::dest_effect();
    remove_feat(TO);
    return;
}