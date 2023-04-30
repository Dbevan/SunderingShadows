//hound soldier - LoKi

#include <std.h>
#include "../serakii.h"

inherit MONSTER;

void make_me();

void create(){
   ::create();
   set_name("hound officer");
   set_id(({"hound","hound officer","officer","hound militia", "hound soldier"}));
   set_short("officer");
set_long("%^C246%^Dressed in "+
"%^C251%^dark clothing and "+
"armour%^C246%^, this %^C088%^"+
"soldier %^C246%^looks %^C058%^"+
"menacing %^C246%^and %^C058%^"+
"rough%^C246%^. They walk with "+
"the gait of one in a state of "+
"constant %^C146%^training "+
"%^C246%^and %^C147%^readiness "+
"%^C246%^and bearing the "+
"%^C252%^weapons %^C246%^and "+
"%^C058%^armour %^C246%^to "+
"leap into action. Across most "+
"of their gear you see a small "+
"%^C058%^canine paw print%^C246%^ "+
"motif, indicating this is a "+
"soldier of the mercenary group "+
"from %^C129%^Serakii%^C246%^, "+
"the Hounds.\n%^CRST%^");
   set_hd(25,12);

   set_exp(1400);
   set_stats("strength",18);
   set_stats("dexterity",18);
   set_stats("wisdom",18);
   set_stats("intelligence",18);
   set_alignment(2);
   set_size(2);
   set_body_type("human");
   add_limb("neck","neck",0,0,0);
   set_wielding_limbs(({"right hand","left hand"}));
   set_gender("male");
   set("aggressive",6);
   set_property("swarm",1);
   set_mob_magic_resistance("average");
   make_me();
   add_money("gold",3000+random(5));
   add_money("silver",5000+random(5));
   add_money("electrum",3000+random(5));
   set_property("no random treasure", 1);
   set_property("not random monster", 1);
   set_new_exp(35, "normal");
   set_speed(20);

}

void make_me(){
        object ob;
        int i,j,k;
        string theclass,race;
        i = random(5);
        switch(i){
        case 0: race = "%^C125%^half-drow%^CRST%^";
                break;
        case 1: race = "%^C125%^human%^CRST%^";
                break;
        case 2: race = "%^C125%^half orc%^CRST%^";
                break;
        case 3: race = "%^C125%^half-elf%^CRST%^";
                break;
        case 4: race = "%^C125%^human%^CRST%^";
                break;
        }

        set_race(race);

    set_short("%^C137%^A " +query_race()+ "%^C137%^ O%^C246%^"+
    "f%^C137%^ficer of t%^C246%^h%^C137%^e H%^C246%^o%^C137%^u%^C246%^n%^C137%^ds%^CRST%^");
        i = random(4);
        switch(i){
        case 0..1: theclass = "duelist";
                break;
        case 2..3: theclass = "warlock";
        }
        set_class(theclass);
       set_guild_level(theclass,30);
       set_mlevel(theclass,30);
       
       if(!clonep())
           return;

        switch(theclass){
        case "duelist":
            set_hp(1000);
            ob = new("/d/common/obj/weapon/longsword");
            ob->set_property("enchantment",5);
            ob->set_property("monsterweapon",1);
            ob->move(TO);
            command("wield sword in right hand");
            ob = new(OBJ"hound_robe.c");
            ob->move(TO);
            if(random(6)){
                ob->set_property("monsterweapon",1);
            }
        command("wear robe");


        ob = new(OBJ"hound_belt.c");
            if(random(6)){
            ob->set_property("monsterweapon",1);
        }

        ob->move(TO);
        command("wear belt");
            if(random(6)){
            ob->set_property("monsterweapon",1);
        }

        ob = new(OBJ"hound_bracers.c");
            ob->move(TO);
            if(random(6)){
                ob->set_property("monsterweapon",1);
            }
        command("wear bracers");
                set_monster_feats(({
                    "weapon focus",
                    "expertise",
                    "knockdown",
                    "powerattack",
                    "shatter",
                    "sunder",
                    "rush",
					"Canny defense",
					"scramble",
					"mobility",
					"spring attack",
                }));

                set_funcs(({
                    "rushit",
                    "flashit",
                    "kdit",
                }));

                break;
        case "warlock":
            set_hp(1000);
            ob = new(OBJ"hound_robe.c");
			ob->move(TO);
			command("wear robe");
            if(random(6)){
				ob->set_property("monsterweapon",1);
                        }
        ob = new(OBJ"hound_belt.c");
            if(random(6)){
            ob->set_property("monsterweapon",1);
        }

        ob->move(TO);
        command("wear belt");
            if(random(6)){
            ob->set_property("monsterweapon",1);
        }

        ob = new(OBJ"hound_bracers.c");
            ob->move(TO);
            if(random(6)){
                ob->set_property("monsterweapon",1);
            }
        command("wear bracers");
            set_monster_feats(({
                "perfect caster",
                "spell penetration",
                "spell power",
                }));
	            set_spells(({
					"eldritch burst",
                    "eldritch blast",
                    "nightmares made real",
                    "eldritch chain"
				}));
                set_spell_chance(90);
                break;
        }
}
void rushit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("rush "+targ->query_name());
}


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


void init()
{
    ::init();
    if (!interactive(TP)) {
        return;
    }
    if ((mixed*)TO->query_attackers() != ({ })) {
        return;
    }
	else 
        call_out("friend_or_foe", 1, TP);
        return;
    }
	
int friend_or_foe()
{
	int i;
	if(member_array("Enemy of the Hounds of Serakii",TP->query_mini_quests())== 1 ){
        if(random(6)){
			force_me("yell Enemies! Call reinforcements!");
		}	
	 force_me("kill " + TP->query_name() + "");

	 return 1;
	}
	else if(member_array("Serakii_Hounds",TP->query_mini_quests())== 1 ){
             i = random(5);
        switch(i){
        case 0: force_me("say Welcome home.");
                break;
        case 1: force_me("nod " + TP->query_name() + "");
                break;
        case 2..4: break;
        }
	 return 1;
	}
   else{
  if(!random(5)) {force_me("say What do you want?"); }
    return 1; 
 }
}	

void die(object ob) {
    int i;
    object *att;

    ETO->lockdown();
    att = all_living(ETO);
    for(i=0;i<sizeof(att);i++){
      if(!interactive(att[i])) continue;
      att[i]->set_mini_quest("Enemy of the Hounds of Serakii", 0, "Enemy of the Hounds of Serakii");
   }
    return ::die(ob);
}


