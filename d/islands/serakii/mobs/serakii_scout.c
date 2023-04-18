#include <std.h>
#include <daemons.h>
#include "/d/common/common.h"

#define GUARDNAMES "/d/common/defines/guards.lst"

inherit "/std/guardsman.c";

void set_guard_stuff();
void setupName(string type);
string do_name();
void hairColor();
void eyeColor();
void doHeight();
void doWeight();

object ob;

void create()
{
    string rank;
    ::create();
    will_open_doors(1);
    set_nogo(({ "/d/islands/serakii/road/11" }));
    set_name("Serakii Scout");
    set_id(({ "militia", "serakii scout", "human", "scout" }));
    set_short("A tall human");
    set_gender(random(2) ? "male" : "female");
    hairColor();
    eyeColor();
    doHeight();
    doWeight();
    setupName("watch");
    set_race("human");
    set_hd(20 + random(20), 1);
    set_class("thief");
    set_mlevel("thief", query_hd());

    set_guild_level("thief", query_hd());
    add_search_path("/cmds/thief");
    set_fighter_style("soldier");
    set_property("full attacks", 1);
    set_mob_magic_resistance("average");
    set_property("add kits", 20);
    set_skill("rope use", 45);
    set_monster_feats(({
        "dodge",
        "mobility",
        "danger sense",
        "spring attack",
        "defensive roll",
        "evasion",
       "scramble",
        "blindfight",
        "death ward",
        "toughness",
        "expertise",
        "knockdown",
		"sunder"
    }));

    set_funcs(({
        "sunderit",
        "kdit",
        "kdit"
    }));
    set_func_chance(80);
    set_ac(5 - random(30));
    set_detecting_invis(random(2));
    set_exp(1);
    set_long(" Dressed in a %^C058%^heavy cloak%^CRST%^ and %^C244%^cowled robe%^CRST%^, this scout looks comfortable and confident as they stand before you. They are dressed in %^C095%^light armor%^CRST%^, with a series of small sheaths strapped across their belt and bandoleers. Their skin, where visible their skin has been marked with some %^C094%^smudge %^CRST%^and %^C241%^dark khol%^CRST%^ to obscure them even more. They are geared for either %^C022%^wilderness %^CRST%^or %^C059%^city%^CRST%^, and by how they move.\n");
    
    if(!clonep());

//equipment
            ob = new("/d/common/obj/weapon/dagger");
            ob->set_property("enchantment",3);
            ob->set_short("cold iron dagger");
            ob->set_property("monsterweapon",1);
            ob->move(TO);

            ob = new("/d/common/obj/armour/studded");
            ob->set_property("enchantment",3);
            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear banded");

            ob = new("/d/common/obj/clothing/robe");
            ob->set_short("A simple gray robe");

            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear robe");
            ob = new("/d/common/obj/armour/coif_leather.c");
            ob->set_property("enchantment",3);
            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear coif");

}




void setupName(string type)
{
    string name;
    name = do_name();
    set_short(capitalize(name) + "%^C070%^,%^CRST%^ " + query_gender() + " %^C059%^scout %^C106%^of%^CRST%^ %^C034%^Serakii%^CRST%^");
    add_id(capitalize(name));
    add_id(lower_case(name));
}


string do_name()
{
    string names, name, * names2 = ({});
    int i;

    if (!names2 || names2 == ({})) {
        names = read_file(GUARDNAMES);

        names2 = explode(names, ", ");
    }
    name = names2[random(sizeof(names2))];
    names2 -= ({ name });
    return name;
}

void hairColor()
{
    switch (random(6)) {
    case 0:        set("hair", "blonde");        break;

    case 1:        set("hair", "red");           break;

    case 2:        set("hair", "auburn");        break;

    case 3:        set("hair", "black");         break;

    case 4:        set("hair", "gray");          break;

    case 5:        set("hair", "brown");         break;
    }
}


void eyeColor()
{
    switch (random(5)) {
    case 0:        set("eye", "blue");         break;

    case 1:        set("eye", "gray");         break;

    case 2:        set("eye", "green");        break;

    case 3:        set("eye", "brown");        break;

    case 4:        set("eye", "azure");        break;
    }
}

void doHeight()
{
    int feet, halves;
    int height = 63 + random(20);
    feet = height / 12;
    halves = "";
    if (height % 12) {
        halves = (height % 12 < 4) ? "" : ((height % 12 < 9) ? " and a
half" : "");
        if (height % 12 > 8) {
            height++;
        }
    }
    set("height", "" + feet + halves);
}

void doWeight()
{
    set("weight", (120 + random(100)));
}


init()

{
    ::init();
  ob=(present("hound",ETO));
  if(ob){ call_out("kill_hound",1,ob); }
}

void kdit(object targ)
{
    if (!objectp(targ)) {
        return;
    }
    if (!objectp(TO)) {
        return;
    }
    TO->force_me("knockdown " + targ->query_name());
}

void flashit(object targ)
{
    if (!objectp(targ)) {
        return;
    }
    if (!objectp(TO)) {
        return;
    }
    TO->force_me("flash " + targ->query_name());
}

void rushit(object targ)
{
    if (!objectp(targ)) {
        return;
    }
    if (!objectp(TO)) {
        return;
    }
    TO->force_me("rush " + targ->query_name());
}

void sunderit(object targ)
{
    if (!objectp(targ)) {
        return;
    }
    if (!objectp(TO)) {
        return;
    }
    TO->force_me("sunder " + targ->query_name());
}

void shatterit(object targ)
{
    if (!objectp(targ)) {
        return;
    }
    if (!objectp(TO)) {
        return;
    }
    TO->force_me("shatter " + targ->query_name());
}

void kill_hound(object obj)
{
  command("yell Hounds attacking!");
  command("kill "+obj->query_name());
  //if(objectp(obj)) obj->die();
  return;
}

