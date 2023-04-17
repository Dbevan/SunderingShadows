//human commander

#include <std.h>
#include "../graez.h"


inherit SHELL;

int i;
object ob;

void create(){
   ::create();

   set_name("Commander Ashton");
   set_short("%^C069%^Ashton%^C067%^, %^C059%^Commander%^C067%^ of the city guard%^CRST%^");
   set_long("Ashton stands before you the very example of a %^C059%^seasoned military "+
   "commander%^CRST%^. His figure is %^C074%^fine%^CRST%^ and %^C067%^handsome%^CRST%^ "+
   "but marred with enough scars to show that he has been in enough %^C124%^battles%^CRST%^ "+
   "to have earned his commission. His %^C238%^jet black hair%^CRST%^ is short cropped "+
   "and %^C081%^piercing blue eyes%^CRST%^ survey his surroundings with a keen eye. "+
   "He wears %^C246%^full plate%^CRST%^ with ease and rests a %^C255%^two handed "+
   "sword%^CRST%^ on his shoulder.");
   set_id(({"commander","human commander","Commander Ashton of the City Guard",
   "commander ashton of the city guard","ashton","commander ashton","Commander Ashton"}));
   set_race("human");
  set_property("swarm",0);   
   rank = 4;
   faction = 1;
   
   set_moving(0);
   set_speed(0);
   set_class("fighter");
   set_guild_level("fighter", 30);
   set_mlevel("fighter",30);
   set_class("paladin");
   set_guild_level("paladin", 30);
   set_mlevel("paladin",30);
   set_property("swarm",0);
   set_alignment(1);
   set_monster_feats(({
      "two weapon fighting",
      "improved two weapon fighting",
      "greater two weapon fighting",
      "weapon focus",
      "expertise",
      "knockdown",
      "powerattack",
      "shatter",
      "sunder",
      "rush",
      "blindfight",
      "improved rush",
      "blade block",
      "strength of arm",
      "titan grip",
      "Sweepingblow",
      "light weapon",
      "impale",
      "cleave"
   }));

   set_funcs(({
      "rushit",
      "rushit",
      "sweepit",
      "impaleit",
      "sweepit",
      "impaleit"
   }));

   set_func_chance(80);

   set_max_hp(1000);
   set_hp(query_max_hp());
   set_overall_ac(-20);
   add_attack_bonus(roll_dice(1,10)+3);
   add_damage_bonus(roll_dice(1,6)+3);
   
   set_stats("strength", 25);
   set_stats("dexterity",25);
   set_stats("wisdom",20);
   set_stats("constitution",20);
   set_new_exp(30,"normal");
   set_sight_bonus(10);
   set_property("full attacks",1);
   set_property("no dominate",1);
   set_property("no death",1);

   set_emotes(3, ({
"%^C241%^Commander Ashton%^C073%^ flips "+
"through %^C143%^maps %^C073%^in front of him.%^CRST%^\n",
"%^C065%^A %^C245%^messenger %^C065%^runs in, "+
"handing %^C240%^Ashton %^C065%^new "+
"%^C143%^reports %^C065%^from the field.%^CRST%^\n",
"%^C060%^Ashton speaks in a %^C246%^terse%^C060%^ "+
"%^C124%^commanding%^C060%^ tone%^CRST%^ Bring "+
"me the death knight's axe! Only when its "+
"destroyed can we have rest.%^RESET%^\n",
}), 0);

   if(!clonep())
       return;
   
   ::rally(); //free rally for commander
   recruited = 0;

   broadcast_area("/d/islands/graez/rooms/", "\n%^C186%^Graez's defenders "+
   "rally as %^C220%^Commander Ashton%^C186%^ takes the %^C247%^field%^C186%^!%^CRST%^\n");
   
   ob = new("/d/common/obj/armour/fullplate");
   ob->move(TO);
   ob->set_property("enchantment",5);
   ob->set_property("monsterweapon",1);
   command("wear plate");

   ob = new(OBJ"undead_bane");
   ob->move(TO);
   command("wield sword");
   command("offer all");
   command("offer all");
   command("speech %^C060%^speak in a "+
   "%^C246%^terse%^C060%^ %^C124%^commanding%^C060%^ "+
   "tone");

}

int rally(object ob)
{
   object ashton,room;
   
   ashton = this_object();
   room = environment(ashton);
   if(recruited == 1){return 1;}
   
   force_me("say Take reinforcements to the front.");
   tell_room(room,"\n%^C143%^A group of %^C241%^soldiers %^C143%^form "+
   "rank and receive orders%^CRST%^.\n");
   ::rally();
   return 1;
}

void rushit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("rush "+targ->query_name());
}


void impaleit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("impale "+targ->query_name());
}

void sweepit(object targ){
   if(!objectp(targ)) return;
   if(!objectp(TO)) return;
   TO->force_me("sweep "+targ->query_name());
}

int move_me(){
return;
}

void die(object ob){

   broadcast_area("/d/islands/graez/rooms/", "%^C244%^The "+
   "defense of %^C060%^Graez %^C244%^collapses as %^C214%^Commander "+
   "Ashton%^C244%^ succumbs to his %^C088%^wounds%^CRST%^!");
   ::die(ob);
}

void receive_given_item(object obj){
   string name;
   name = TPQN;
   if(!objectp(obj)) return;
   if((string)obj->query_name() != "wretched wrath") { 
      force_me("say This isnt quite what I'm looking for. Bring me the axe of the death knight!");
      force_me("give "+obj->query_name()+" to "+name+""); 
      return;
   }

   tell_room(ETO,"Ashton hefts the axe and frowns.\n");
   force_me("say This axe has killed countless of my boys.");
   
   new(OBJ"undead_bane.c")->move(TO); 
   force_me("say It is good to see the death knight destroyed and "+
   "his axe destroyed. Here, take this sword as an exchange");
   force_me("give undead bane to "+name+"");
   force_me("offer axe");    


   TP->set_mini_quest("Earned Ashton's Undead Bane", 500000, "Earned Ashton's Undead Bane");
   return;
}