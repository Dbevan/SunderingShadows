//Undead Commander

#include <std.h>
#include "../graez.h"


inherit SHELL;

int i;
object ob;

void create(){
   ::create();
   set_name("Ser Garoteth");
   set_id(({"commander","undead commander", "ser garoteth", "ser garoteth the ravenous", "death knight", "knight", "Garoteth"}));
   set_short("%^C059%^Ser %^C060%^Garoteth%^C059%^, the %^C160%^Ravenous%^CRST%^");
   set_long("Dressed in %^C059%^gothic armor%^CRST%^ a century out of fashion and draped in a %^C160%^blood red%^CRST%^ %^C124%^heavy cloak%^CRST%^, this %^C238%^death knight%^CRST%^ before you is a terrifying sight. bright angry %^C160%^red eyes%^CRST%^ flare from behind a %^C243%^sugarloaf helm%^CRST%^. It wields a %^C239%^great battle axe%^CRST%^ in both hands and watches over the %^C058%^blasted battlefield%^CRST%^ without emotion or reaction.");
   set_race("undead");
   set_property("swarm",0);
   rank = 4;
   faction = 2;

   set_moving(0);
   set_speed(0);
   set_class("fighter");
   set_guild_level("fighter", 30);
   set_mlevel("fighter",30);
   set_class("paladin");
   set_guild_level("paladin", 30);
   set_mlevel("paladin",30);
   set_alignment(9);

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
   
   if(!clonep())
       return;
   
   ob = new("/d/common/obj/armour/fullplate");
   ob->move(TO);
   ob->set_property("enchantment",5);
   ob->set_property("monsterweapon",1);
   command("wear plate");
 
   ob = new(OBJ"wretched_wrath");
   ob->move(TO);
   command("wield axe");
   command("offer all");
   command("offer all");
   command("speech %^C231%^utter in a %^C244%^"+
   "deathless %^C247%^hollow %^C231%^voice%^CRST%^");

   set_emotes(3, ({
"%^C243%^The %^C238%^deathknight %^C243%^rests "+
"the %^C247%^axe %^C243%^on its shoulder%^CRST%^\n",
"%^C243%^The %^C238%^deathknight %^C231%^utters in a %^C244%^"+
   "deathless %^C247%^hollow %^C231%^voice%^CRST%^ "+
   "Bring me the sword of the wretch Ashton!^\n",
"%^C244%^Masses of %^C060%^undead %^C244%^can be "+
"seen just under the surface of the %^C239%^monolith%^CRST%^\n",
}), 0);

   ::rally(); //free rally for commander
   recruited = 0;
   
   broadcast_area("/d/islands/graez/rooms/", "\%^C190%^Lightning %^C066%^strikes the %^C238%^monolith %^C066%^as %^C067%^Ser Garoteth%^C066%^ the %^C241%^Death Knight%^C066%^ emerges to lead the %^C061%^undead%^C066%^!%^CRST%^\n");   
   
}

int rally(object ob)
{
   object garoteth,room;
   
   garoteth = this_object();
   room = environment(garoteth);
   if(recruited == 1){return 1;}
   
   force_me("say Kill! Kill! Kill!.");
   tell_room(room,"\n%^C240%^The monolith%^C072%^ surges "+
   "and %^C061%^undead %^C072%^pour from its %^C242%^surface "+
   "%^C072%^like emerging from water!%^CRST%^\n");
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

   broadcast_area("/d/islands/graez/rooms/", "%^C037%^A cheer goes up from the %^C060%^battlements %^C037%^of %^C060%^Graez %^C037%^as word reaches them %^C240%^Ser Garoteth %^C037%^has fallen!%^CRST%^");
   ::die(ob);
}

void receive_given_item(object obj){
   string name;
   name = TPQN;
   if(!objectp(obj)) return;
   if((string)obj->query_name() != "undead bane") { 
      force_me("say This is garbage! Bring me the rat Ashton's sword!");
      force_me("give "+obj->query_name()+" to "+name+""); 
      return;
   }

   tell_room(ETO,"%^C241%^The deathknight seems to chuckle dryly.%^RESET%^\n");
   force_me("say Now, they will all die.");
   
   new(OBJ"wretched_wrath.c")->move(TO); 
   force_me("say Take this axe, and reap the entire city in my name.");
   force_me("give axe to "+name+"");
   force_me("offer axe");    


   TP->set_mini_quest("Became the holder of Wrath", 500000, "Became the holder of Wrath");
   return;
}