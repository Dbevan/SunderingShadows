

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
    set_name("Serakii Milita");
    set_id(({ "militia", "serakii militia", "human", "guard" }));
    set_short("A tall human");
    set_gender(random(2) ? "male" : "female");
    hairColor();
    eyeColor();
    doHeight();
    doWeight();
    setupName("watch");
    set_race("human");
    set_hd(20 + random(20), 1);
    set_class("fighter");
    set_mlevel("fighter", query_hd());

    set_guild_level("fighter", query_hd());
    add_search_path("/cmds/fighter");
    set_fighter_style("soldier");
    set_property("full attacks", 1);
    set_mob_magic_resistance("average");
    set_property("add kits", 20);
    set_skill("rope use", 45);
    set_monster_feats(({
        "parry",
        "powerattack",
        "shatter",
        "sunder",
        "rush",
        "damage resistance",
        "improved damage resistance",
        "improved toughness",
        "regeneration",
        "toughness",
        "expertise",
        "knockdown"
    }));

    set_funcs(({
        "rushit",
        "flashit",
        "shatterit",
        "sunderit",
        "rushit",
        "kdit",
        "kdit"
    }));
    set_func_chance(80);
    set_ac(5 - random(30));
    set_detecting_invis(random(2));
    set_exp(1);
    set_long("Dressed in a matching set of %^C074%^arandur banded%^CRST%^ and draped in a %^C244%^fine gray robe%^CRST%^, this %^C248%^guardsman%^CRST%^ stands out, %^C079%^parade ready%^CRST%^. The armor %^C044%^gleams%^CRST%^ with a %^C041%^green light %^CRST%^as the guard moves and the contrast of that to the %^C245%^fine wool robe%^CRST%^ catches your eye. They wear a matching %^C042%^coif%^CRST%^ that covers their head but leaves a %^C221%^friendly%^CRST%^, assured face to look out on the city. On one arm they wear a %^C095%^shield%^CRST%^ and they rest a %^C241%^cold iron sword%^CRST%^ casually on their shoulder. The shield bears the insignia of %^C252%^three towers%^CRST%^ behind an %^C244%^anchor%^CRST%^. This guardsman is ready to defend their home, the %^C109%^Port of Serith%^CRST%^ from any threats.");

    if(!clonep())
        return;
    
//equipment
            ob = new("/d/common/obj/weapon/longsword");
            ob->set_property("enchantment",3);
            ob->set_short("%^C245%^cold iron %^CRST%^%^C220%^longsword%^CRST%^");
			ob->set_long("%^C220%^A straight double edged longsword %^CRST%^%^C242%^made from the best cold iron. %^CRST%^%^C245%^The blade looks strong %^CRST%^%^C221%^and will not break easily %^CRST%^%^C160%^and it's edges honed keen and sharp. %^CRST%^%^C130%^Leather stripping wraps around the pommel to ensure a solid grip at all times.%^CRST%^");
            ob->set_property("monsterweapon",1);
            ob->move(TO);
    
            ob = new("/d/common/obj/armour/banded");
            ob->set_property("enchantment",3);
            ob->set_short("%^C048%^arandur%^C051%^ banded mail%^CRST%^");
            ob->set_long("%^C159%^A banded mail made of arandur, an %^C024%^i%^C045%^r%^C087%^i%^C015%^d%^C191%^e%^C118%^s%^C191%^c%^C015%^e%^C045%^n%^C024%^t%^CRST%^ %^C067%^s%^C110%^i%^C116%^l%^C123%^v%^C159%^e%^C195%^r%^C251%^-%^C015%^b%^C251%^l%^C195%^u%^C159%^e%^RESET%^%^C159%^ metal that gleams with a %^C123%^g%^C121%^r%^C119%^e%^C192%^e%^C154%^n s%^C193%^h%^C119%^e%^C121%^e%^C123%^n%^RESET%^%^C159%^ in the right light. The design is simple, with a solid breastplate over the chest and segmented horizontal bands overlapping to protect the ribs and stomach for full articulation and ease of movement. The pauldrons, tasses and couters that protect the neck and shoulders, hips, and elbows respectively all have a nautical design to them; they are a combination overlapping leaves of foliose coral and scallop shells, all rendered in the colour-shifting arandur. Beneath the heavier armor riveted chainmaille is worn to protect vulnerable points where the pieces join.%^CRST%^");
            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear banded");

            ob = new("/d/common/obj/clothing/robe");
            ob->set_short("%^C246%^A simple gray robe%^CRST%^");
            ob->set_long("%^C246%^Though simple in appearance, this robe seems to fulfill its duty quite well. Aside from the distinctive diagonal grain of the silky black thread, the only stand-out detail is a small representation of the %^C255%^island of Serakii%^C246%^ embroidered on the left sleeve.%^CRST%^");

            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear robe");

            ob = new("/d/common/obj/armour/shield");
            ob->set_property("enchantment",3);
            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear shield");
            command("wield sword");
            ob = new("/d/common/obj/armour/coif");
            ob->set_short("%^C048%^arandur%^CRST%^ %^C248%^chain%^C051%^ coif%^CRST%^");
            ob->set_property("enchantment",3);
            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wear coif");
    
}



void setupName(string type)
{
    string name;
    name = do_name();
    set_short(capitalize(name) + ", " + query_gender() + " %^C060%^guardsman%^CRST%^ of %^C042%^Port Serith%^CRST%^");
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
        halves = (height % 12 < 4) ? "" : ((height % 12 < 9) ? " and a half" : "");
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
  ob=(present("hound soldier",ETO));
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
