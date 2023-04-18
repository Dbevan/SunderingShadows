// Hounds Johann - LoKi - 2021


#include <std.h>
#include <daemons.h>
#include "../serakii.h"
inherit NPC;


void create(){
   object ob;
   ::create();
   set_name("johann");
   set_short("%^C138%^Johann Rall, %^C247%^Hound %^C138%^Corporal%^CRST%^");
set_long("Dressed in %^C246%^black clothing%^CRST%^ and %^C058%^armour%^CRST%^, this %^C088%^soldier %^CRST%^looks menacing and %^C058%^rough%^CRST%^. Their equipment is very well kept and across their %^C058%^bracers %^CRST%^you see a %^C058%^canine paw%^CRST%^ and %^C146%^two chevrons%^CRST%^ indicating this man is a %^C138%^corporal %^CRST%^in the Hounds of %^C098%^Serakii%^CRST%^. His %^C246%^black hair%^CRST%^ has been slicked back and %^C081%^pale blue eyes%^CRST%^ peer over his domain with %^C253%^cold %^C058%^indifference. %^CRST%^\n");
   set_race("human");
   set_stats("intelligence",20);
   set_stats("wisdom",18);
   set_stats("strength",16);
   set_stats("charisma",16);
   set_stats("dexterity",12);
   set_stats("constitution",14);
   set_class("fighter");
   set_mlevel("fighter",30);
   set_guild_level("fighter",30);
   set_alignment(4);
   set_gender("male");
   set_size(2);
   set_exp(0);
   set_overall_ac(-10);
   set_hd(35,15);
   set_max_hp(400);
   set_hp(query_max_hp());
   set("aggressive",0);
   set_emotes(1, ({
"%^C247%^The %^C088%^soldier %^C247%^"+
"looks over %^C058%^crates%^C247%^.%^CRST%^",
"%^C162%^the %^C245%^lantern %^C162%^above "+
"you %^C160%^flickers%^C162%^.%^CRST%^",
"%^C035%^A %^C245%^scuffle %^C035%^breaks "+
"out outside the %^C058%^tent%^C035%^, its "+
"cut short with a %^C242%^growl %^C035%^of "+
"a %^C058%^dog %^C035%^and a curt %^C160%^"+
"scream%^C035%^.%^CRST%^",
}), 0);
   set_property("no bows",1);
   set_spoken("wizish");
   command("speech %^C144%^speak in an "+
   "%^C107%^assured %^C144%^voice%^CRST%^");

   if(!clonep())
       return;
   
   ob = new(OBJ"hound_robe.c");
   ob->set_property("monsterweapon",1);
   ob->move(TO);
   command("wear robe");
   ob = new(OBJ"hound_belt.c");
   ob->set_property("monsterweapon",1);
   ob->move(TO);
   command("wear belt");
   ob->set_property("monsterweapon",1);
   ob = new(OBJ"hound_bracers.c");
   ob->move(TO);
   command("wear bracers");

}

void catch_say(string message){
    if(strsrch(message,"hello") != -1) {
      call_out("reply_hello",1,TP,message);
    }
    if(strsrch(message,"who are you") != -1) {
      call_out("reply_introductions",1,TP,message);
    }
    if(strsrch(message,"serakii") != -1) {
      call_out("reply_serakii",1,TP,message);
    }
    if(strsrch(message,"hounds") != -1) {
      call_out("reply_hounds",1,TP,message);
    }
	    if(strsrch(message,"legion") != -1) {
      call_out("reply_legion",1,TP,message);
    }
	    if(strsrch(message,"join") != -1) {
      call_out("reply_join",1,TP,message);
    }
	    if(strsrch(message,"commander") != -1) {
      call_out("reply_commander",1,TP,message);
    }
}

void reply_hello(string msg, object who){
    tell_room(ETO,"%^C103%^The soldier looks over from the crates.%^CRST%^\n");
    force_me("say Hey!! %^C205%^Who are "+
	"you%^CRST%^? and what are you doing here?");
    return 1;
}

void reply_introductions(string msg, object who){
	tell_room(ETO,"\n");
    force_me("say I am Corporal Johann Hess of the %^C205%^Hounds%^CRST%^ of %^C205%^Serakii%^CRST%^!");
    tell_room(ETO,"%^C103%^The soldier stands a little taller.%^CRST%^\n");

    return 1;
}

void reply_hounds(string msg, object who){
	tell_room(ETO,"\n");
    force_me("say We hounds are the protectors of the %^C205%^Serakii%^CRST%^ port, we're the true holders of the contract those bastards in the %^C205%^legion%^CRST%^ sit on and we're the most powerful mercenary company in the world.");
	tell_room(ETO,"");
    force_me("say You should %^C205%^join%^CRST%^ us.");
    return 1;
}
void reply_legion(string msg, object who){
	tell_room(ETO,"\n");
    force_me("say The legion?");
	tell_room(ETO,"");	
	force_me("say They're nothing but the garbage the other companies didn't want. Not worth their equipment. Go see for yourself! Over at the White Keep. Bet the %^C205%^commander%^CRST%^ would love for you to do some damage to them."); 
    return 1;
}

void reply_join(string msg, object who){
	tell_room(ETO,"\n");
    force_me("say You want to join?");
	tell_room(ETO,"");	
	force_me("say Go see the quartermaster. He's in the weapon's yard, southwest corner of the camp."); 
    return 1;
}

void reply_commander(string msg, object who){
	tell_room(ETO,"\n");
    force_me("say The Commander?");
	tell_room(ETO,"");	
	force_me("say After successfull end to our contract in Tharis Commander Kava came back with the regiment to take over from old.. leadership."); 
	tell_room(ETO,"");
    force_me("say You'll find her at the northeast of the camp.");
    return 1;
}

