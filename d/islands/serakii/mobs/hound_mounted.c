
// Corrupted Hound Mounted - 2022

#include <std.h>
#include "../serakii.h"

inherit MONSTER;
int lev;

void charge();

void create() 
{
    string me;
    object ob;
    ::create();
    set_name("corrupted mounted infantry");
    set_id(({"mounted infantry", "infantry", "hound", "mounted",
	"corrupted mounted infantry", "hound soldier"}));
    
    set_short("%^C070%^co%^C065%^r%^C070%^rup%^C065%^"+
	"t%^C070%^ed m%^C058%^o%^C070%^un%^C058%^t%^C070%^ed i%^C243%^"+
	"n%^C070%^fan%^C243%^t%^C070%^ry%^CRST%^");
    set_long("Wearing %^C244%^heavy "+
	"plate%^CRST%^ and a %^C238%^long black "+
	"cloak%^CRST%^, this human %^C243%^soldier %^CRST%^"+
	"is a figure of %^C124%^immense strength%^CRST%^, "+
	"though hunches over as if he is in great "+
	"%^C124%^pain%^CRST%^. Their %^C243%^armor "+
	"%^CRST%^itself is in amazing condition and "+
	"seemingly new, but some %^C076%^fell miasma%^CRST%^ "+
	"seems to leak out from under it to stain the "+
	"ground around him. He moves in quick, ragged "+
	"motions like a %^C058%^puppet %^CRST%^on strings.\n");
        
    set_race("human");
    set_body_type("human");
    set_gender("male");
    set_size(2);
    set_overall_ac(-30);
    set_class("paladin");
    
    lev = 40 + random(9);
    set_guild_level("paladin",lev);
    set_mlevel("paladin", lev);
    
    set_max_hp(lev*40 + random(1000));
    set_hp(query_max_hp());
    set_speed(30);
    	
    //set_nogo(({ }));
    
    set_property("full attacks",1);
	set_property("cast and attack",1);
    set_property("no dominate",1);
    set_property("no tripped", 1);
    set_property("no paralyze",1);
    set_property("no death",1);
    
    set_stats("strength",19);
    set_stats("dexterity",15);
    set_stats("constitution",15);
    set_stats("intelligence", 10);
    set_stats("wisdom", 20);
    set_stats("charisma", 12);
    
    set("aggressive",55);
    me = TO->query_short();
    
        
    set_new_exp(lev, "high");
    
    set_alignment(9);
    set_attack_bonus(10);
    
    set_spells(({"call lightning","dark bolt","sticks into snakes", "unholy orb"}));
 
    set_spell_chance(40);
    
    set_funcs(({"wrack", "charge"}));
    
    set_func_chance(45);
	
    set_skill("athletics", 40);

	set_monster_feats(({
	    "charge",
		"mounted combat",
		"ride-by attack",
		"trample",
		"blade block",
		"impale",
		"light weapon",
		"strength of arm",
		"sweepingblow"
    }));
    
    if(!clonep())
        return;
    
    if(!present("horse")) { call_out("do_horse",1,TO); }   
    add_money("platinum",random(500));
    ob = new("/d/common/obj/weapon/longsword");
    ob->set_property("enchantment", 7);
    ob->move(TO);
    ob = new("/d/common/obj/armour/shield");
    ob->set_property("enchantment", 7);
    ob->move(TO);
    command("wear shield");
    ob = new("/d/common/obj/armour/fullplate");
    ob->set_property("enchantment", 7);
    ob->move(TO);
    command("wear plate");
    ob = new("/d/common/obj/weapon/lance");
    if(random(4)) ob->set_property("monsterweapon", 1);
    ob->move(TO);
    
	add_money("gold", 500 + random(10000));
	
}



void wrack(object targ) 
{
  command("cause wracking pains "+targ->query_name()+"");
  return;
}

void charge()
{    
    object targ;
    targ = TO->query_current_attacker();
    if(!objectp(targ)) return;
    if(!TO->query_in_vehicle()) 
    { 
        switch (random(3))
        {
        case 0:
            command("sweepingblow "+targ->query_name());
            break;
        case 1:
            command("charge "+targ->query_name());
            break;
        case 2:
            command("impale "+targ->query_name());
            break;
        }
    }
    command("charge "+targ->query_name());
    return;
}
    
void do_horse()
{
    object horse;
    if(!objectp(ETO)) return;
    if(!objectp(TO)) return;
    horse = new("/d/common/mounts/war_horse.c");
    
    horse->set_owner(TO);
    horse->move(ETO);
    TO->force_me("mount horse");
    return;
}

void heart_beat()
{
    ::heart_beat();
    if(!objectp(TO)) return;
    if(!objectp(ETO)) return;

    if(TO->query_in_vehicle())
    {
        command("unwield sword");
        command("wield lance");
    }

    if(!sizeof(query_wielded())) { command("wield sword"); }
    return;
}

void init()
{
    if(!objectp(TO)) { return; }
    if(!objectp(TP)) { return; }
    ::init();

    if(TP->query_true_invis()) return;
    if(!userp(TP) || random(3)) return;
    command("charge "+TPQN); 
}
