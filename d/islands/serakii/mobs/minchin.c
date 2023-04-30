#include <std.h>
#include <daemons.h>
#include "../serakii.h"

inherit MONSTER;


void create()
{
    object obj,ob;
    ::create();
    set_name("Force Commander Minchin");
    set_id(({ "minchin", "Minchin", "Force Commander Minchin", "commander", "force commander" }));
    set_short("%^C245%^Fo%^C090%^r%^C245%^ce Comman%^C092%^d%^C245%^er M%^C093%^i%^C245%^"+
	"nchin%^CRST%^");
    set_long("%^C245%^Dressed in %^C094%^leather "+
"armor%^C245%^ and a %^C242%^long black "+
"cloak%^C245%^, this human cuts a %^C088%^"+
"dangerous%^C245%^ figure. He is %^C095%^middle "+
"aged%^C245%^, with deep %^C248%^circles%^C245%^ under "+
"his eyes and a thick amount of white in his hair, but "+
"he still bears a great amount of %^C088%^strength "+
"%^C245%^in his form. His armor is %^C242%^"+
"black%^C245%^, with a mark of a %^C252%^white "+
"canine print%^C245%^ on it, marking him as "+
"a %^C094%^Hound%^C245%^.%^CRST%^ ");
    set_race("human");
    set_gender("male");
    set_class("fighter");
    set_hd(60, 6);
    set_size(3);
    set_overall_ac(-20);
    set_property("magic", 1);
    set_max_hp(14000);
    set_hp(14000);
    set_property("swarm", 1);
    set_property("no death", 1);
    set_property("no blind", 1);
    set_property("no hold", 1);
    set_property("no paralyze", 1);
    set_property("no knockdown", 1);
    set_property("magic", 1);
    set_stats("strength", 30);
    set_stats("dexterity", 30);
    set_stats("constitution", 30);
    set("aggressive", "aggfunc");
    set_mob_magic_resistance("very high");
    set_property("damage resistance", 20);
    set_property("spell damage resistance", 40);
    set_new_exp(60, "boss");

    set_funcs(({ 

      "rushit",
	  "flashit",
	  "kdit"

	}));
	
    set_func_chance(101);
    set_attacks_num(10);
    set_property("function and attack", 1);
    add_money("gold", 80000 + random(60000));
    set_monster_feats(({
         "unarmed parry",
         "rapid strikes",
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
    
	ob = new(OBJ"hound_robe.c");
    ob->move(TO);
    command("wear robe");
    ob = new(OBJ"hound_belt.c");
    ob->move(TO);
    command("wear belt");
    ob = new(OBJ"hound_bracers.c");
    ob->move(TO);
    command("wear bracers");
	
	ob = new("/d/islands/common/obj/shefflix.c");
	ob->set_property("monsterweapon", 1);
	ob->move(TO);
	command("wield sword");
	
	ob = new("/d/common/obj/armour/studded");
    ob->move(TO);
    ob->set_property("monsterweapon",1);
    command("wear studded");
}

string *query_mini_quests()
{
    return ({ "%^BOLD%^%^RED%^Defeated the great red wyrm Klauth!%^RESET%^" });
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
//   add_action("xroosh","rush");

}

int xstab(string str){

   if(str=="minchin" || str=="attacker"){

      tell_object(TP,"%^BOLD%^%^RED%^As you try to stab Minchin he grabs you and throws you over his shoulder to the ground!%^RESET%^");
      tell_room(ETO,"As "+TPQCN+" tries to stab Minchin, he grabs "+TP->QO+" and flips them over!",TP);
	  TP->do_damage("torso",random(100)+80);

      return 1;
}

/*int xroosh(string str){

   if(str=="minchin" || str=="attacker"){
      tell_object(TP,"Minchin watches as you rush.");
      tell_object(TP,"%^BOLD%^%^RED%^At the last second, he steps aside and punches you in the face!%^RESET%^");
      tell_room(ETO,"As "+TPQCN+" tries to rush Minchin, he sidesteps and punches them in the face!",TP);
	  TP->do_damage("torso",random(100)+80);
      return 1;
   }
*/}


int xcrit(string str){
   if(str=="minchin" || str=="attacker"){
      tell_object(TP,"You slide up to the commander and try to sneak in a deadly blow.");
      tell_object(TP,"%^BOLD%^%^RED%^Minchin grabs you and knees you hard in the belly with an armored knee!%^RESET%^");
      tell_room(ETO,"As "+TPQCN+" tries to attack they get kneed in the belly and double over!",TP);
	  TP->do_damage("torso",random(100)+80);

   }
}

void aggfunc()
{
    command("sic "+TP->query_true_name());
    command("kill "+TP->query_true_name());
    set("aggressive",100);
}
