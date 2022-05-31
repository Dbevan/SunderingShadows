#include <std.h>
#include "../lizard.h"

inherit MONSTER;
int FLAG;

void create() {
	object obj; 
    	::create();
    	set_name("young lizardman");
    	set_id(({"lizard","lizardman","young lizardman"}));
    	set_race("lizardman");
    	set_gender("male");
    	set("short","%^BOLD%^%^GREEN%^Young Lizardman%^RESET%^");
    	set("long","Standing over 4' tall this reptile walks on 2 legs like a man.  "+
      	"His hide is green with mottled brown markings.  He looks like he "+
      	"might be a tough customer in a fight, even despite his youthful apperance.");
  	set_property("water breather",1);
    	set_speed(25);
    	set_overall_ac(0);
    	set_body_type("human");
    	set_class("fighter");
    	set_mlevel("fighter", 18);
    	set_property("full attacks",1);
    	set("aggressive",18);
    	set_alignment(9);
    	set_size(1);
    	set_hd(18,1);
    	set_stats("strength",random(4)+15);
    	set_stats("charisma", 6);
    	set_stats("dexterity",random(8)+10);
    	set_stats("constitution",10);
    	set_stats("wisdom",9);
    	set_stats("intelligence",9);
    	set_max_hp(74+random(20));
        set_hp(query_max_hp());
    	set_emotes(1,({
		"The lizardman hisses loudly for reinforcements.",
		"The lizardman licks the blood from his blade!",
		"The lizardman swing his tail around to hit you.",
		"The lizardman claws at your face.",
		"The lizardman hisses.",
		"%^BOLD%^%^GREEN%^Lizardman hisses: %^RESET%^Leave usssss!",
		"The lizardman tests the air with his forked tongue.",
		"The lizardman twitches his tail angrily.",
		"%^BOLD%^%^GREEN%^Lizardman hisses: %^RESET%^Sssssoon....ssssoon.",
		"%^BOLD%^%^GREEN%^Lizardman hisses: %^RESET%^Sssssomeone is "+
			"close...i can tassste them.",
		"The lizardman flickers his forked tongue in and out.",
		"The lizardman narrows his reptilian eyes at you.",
    	}),0);
    	set_exp(1500);
    	set_wielding_limbs(({"right hand","left hand"}));
    	new(SHWEAP+"shortsword")->move(this_object());
   		command("wield sword");
  	new(SHWEAP+"shortsword")->move(this_object());
  		command("wield sword");
	obj=new("/d/islands/pirates/armor/woolcloak.c");
		if(random(3)){ 
		obj->set_property("monsterweapon",1);
				}
			obj->move(TO);
	switch(random(2)){
		case 0:
			obj=new("/d/common/obj/rand/r_helm.c");
				if(random(5)){
				obj->set_property("monsterweapon",1);
				}
			obj->move(TO);
			obj=new("/d/common/obj/rand/rand_chain.c");
			obj->set_property("enchantment",1);
				if(random(4)){
				obj->set_property("monsterweapon",1);
				}
	obj->move(TO);
	command("wearall");
	break;
	case 1:
		obj=new("/d/common/obj/rand/rand_leath_head.c");
			if(random(5)){
				obj->set_property("monsterweapon",1);
			}
			obj->move(TO);
		obj=new("/d/shadow/room/farm/obj/leatherbp.c");
			obj->set_property("enchantment",1);
			if(random(4)){
				obj->set_property("monsterweapon",1);
			}
	obj->move(TO);
	set_class("thief");
	set_mlevel("thief",19);
	set_guild_level("thief",19);
	add_search_path("/cmds/thief");
	set_thief_skill("hide in shadows",95);
     	set_thief_skill("move silently",95);
	set_scrambling(1);
	command("wearall");
	break;
	}
   	add_money("silver",random(200));
    	add_money("gold",random(50));
      add_search_path("/cmds/fighter");
    	FLAG = 0;
    	set_funcs(({"flag_set"}));
    	set_func_chance(25);
}
void init(){
	string mrace=TP->query_race();
    	::init();
    	if(avatarp(TP) || TP->query_invis() || mrace == "lizardman") return;
    	call_out("coverass", 2, TP);
}
void coverass(object targ){
      if(!objectp(targ)) return;  //added by Circe 12/5/03
	force_me("kill "+targ->query_name()+"");
	if(!random(25))force_me("rush "+targ->query_name()+"");
	if (FLAG == 1) return;
	force_me("parry");
	FLAG = 1;
}
void flag_set(){
   	if (FLAG == 0)return;
   	if(query_attackers() != ({})){
		FLAG = 0;
		return;
	}
}