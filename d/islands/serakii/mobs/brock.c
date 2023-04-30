//Commander Brock - LoKi 2021

#include <std.h>
#include <daemons.h>
#include "../serakii.h"

inherit MONSTER;

void create()
{
    object obj,ob;
    ::create();
    set_name("Commander Arnuf Brock");
    set_id(({ "arnuf", "brock", "arnuf brock", "commander", "legion commander" }));
    set_short("%^C111%^Commander %^C244%^Ar%^C111%^n%^C244%^uf %^C111%^B%^C244%^r%^C111%^o%^C245%^c%^C111%^k%^CRST%^");
    set_long("%^C111%^Dressed in the %^C246%^armor %^C111%^of the "+
	"%^C231%^Pearl Legion%^C111%^, this man is a giant and %^C094%^barrel "+
	"chested%^C111%^. He wields a %^C242%^bardiche %^C111%^like it weighs "+
	"nothing and his armor is pristine and gleaming. %^C228%^Long flowing "+
	"golden%^C111%^ hair falls down below his shoulders and %^C027%^gleaming "+
	"blue eyes %^C111%^look back at you. He must be in his early %^C094%^"+
	"forties%^C111%^, though %^C242%^age %^C111%^has not caught up with "+
	"him yet.%^CRST%^\n");
    set_race("human");
    set_gender("male");
    set_class("fighter");
    set_guild_level("fighter",40);
    set_property("full attacks",1);
    set_hd(30, 6);
    set_size(2);
    set_overall_ac(-20);
    set_alignment(2);
    set_property("magic", 1);
    set_max_hp(2500);
    set_hp(2500);
    set_property("swarm", 1);
    set_property("no death", 1);
    set_property("no blind", 1);
    set_property("no hold", 1);
    set_property("no paralyze", 1);
    set_property("no knockdown", 1);
    set_property("magic", 1);
    set_stats("strength", 25);
    set_stats("dexterity", 25);
    set_stats("constitution", 25);
    set("aggressive", 5);
    set_mob_magic_resistance("very high");
    set_property("damage resistance", 20);
    set_property("spell damage resistance", 40);
    set_new_exp(30, "boss");

    set_funcs(({ 

      "rushit",
	  "flashit",
	  "kdit"

	}));
	
    set_func_chance(101);
    set_attacks_num(10);
    set_property("function and attack", 1);
    add_money("gold", 8000 + random(6000));
    set_monster_feats(({
         "precise strikes",
         "dodge",
         "mobility",
         "scramble",
         "spring attack",
         "defensive roll",
         "resistance",
         "increased resistance",
         "damage resistance",
         "improved damage resistance",
         "toughness",
		 "rush",
		 "knockdown",
		 "improved rush",
    }));
    set_skill("athletics", 20); 
    set_attacks_num(10);
    set_property("function and attack", 1);
    
    if(!clonep())
        return;
	
	ob = new(OBJ"legion_cassock.c");
    ob->move(TO);
    command("wear cassock");
    ob = new(OBJ"legion_sash.c");
    ob->move(TO);
    command("wear sash");
    ob = new(OBJ"legion_bracers.c");
    ob->move(TO);
    command("wear bracers");
	
    ob = new(OBJ"flying_wind.c");
	ob->move(TO);
        command("wield bardiche");
	
	ob = new("/d/common/obj/armour/studded");
    ob->move(TO);
    command("wear studded");
                 new(OBJ"legion_seal")->move(TO);
}

void set_paralyzed(int time,string message){return 1;}

void kdit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("knockdown "+targ->query_name());
}

void flashit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("flash "+targ->query_name());
}

void rushit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("rush "+targ->query_name());
}

void init(){
   ::init();
   add_action("xstab","stab");
   add_action("xcrit","crit");
   add_action("xrush","rush");

}

int xstab(string str){

   if(str=="arnuf" || str=="attacker"){

      tell_object(TP,"%^BOLD%^%^RED%^As you try to stab arnuf he grabs you and throws you over his shoulder to the ground!%^RESET%^");
      tell_room(ETO,"As "+TPQCN+" tries to stab arnuf, he grabs "+TP->QO+" and flips them over!",TP);
	  TP->do_damage("torso",random(100)+80);

      return 1;
}
}

int xrush(string str){

   if(str=="arnuf" || str=="attacker"){
	   
      tell_object(TP,"arnuf watches as you rush.");
      tell_object(TP,"%^BOLD%^%^RED%^At the last second, he steps aside and punches you in the face!%^RESET%^");
      tell_room(ETO,"As "+TPQCN+" tries to rush arnuf, he sidesteps and punches them in the face!",TP);
	  TP->do_damage("torso",random(100)+80);
	  
      return 1;
   }
}


int xcrit(string str){
   if(str=="arnuf" || str=="attacker"){
      tell_object(TP,"You slide up to the commander and try to sneak in a deadly blow.");
      tell_object(TP,"%^BOLD%^%^RED%^arnuf grabs you and knees you hard in the belly with an armored knee!%^RESET%^");
      tell_room(ETO,"As "+TPQCN+" tries to attack they get kneed in the belly and double over!",TP);
	  TP->do_damage("torso",random(100)+80);
	  
      return 1;

   }
}
