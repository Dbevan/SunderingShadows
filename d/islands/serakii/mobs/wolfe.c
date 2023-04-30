// Legion Wolfe - LoKi - 2021


#include <std.h>
#include <daemons.h>
#include "../serakii.h"
inherit NPC;


void create(){
   object ob;
   ::create();
   set_name("lieutenant Travis Wolfe");
   set_id(({ "lieutenant","travis","wolfe","travis wolfe","officer","legion officer"}));
   set_short("%^C111%^Li%^C246%^e%^C111%^utena%^C246%^n%^C111%^t Trav%^C246%^i%^C111%^s W%^C246%^o%^C111%^lfe%^CRST%^");
set_long("%^C248%^Dressed in the %^C243%^"+
"armor%^C248%^ and %^C015%^cassock%^C248%^ "+
"of a %^C111%^Legion of Pearl%^C248%^ "+
"soldier, this young man strikes a %^C036%^"+
"handsome%^C248%^ picture. Unlike the %^C206%^"+
"officers%^C248%^ and %^C094%^soldiers%^C248%^ "+
"on duty though, his %^C231%^cassock%^C248%^ is "+
"undone and his %^C111%^sash%^C248%^ hangs over "+
"his shoulder. His %^C094%^brown hair%^C248%^ "+
"has been shorn close to his head on both sides "+
"and the middle hangs down on either side framing "+
"his young face. He is %^C180%^clean shaven%^C248%^ "+
"and his expression is one of %^C036%^relaxed%^C248%^ "+
"readiness.\n%^CRST%^");
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
"%^C060%^You hear %^C242%^soldiers %^C060%^march in the %^C250%^hallway %^C060%^outside%^CRST%^.",
"%^C060%^The %^C244%^soldier %^C060%^absently flips through %^C255%^papers%^C060%^.%^CRST%^",
"%^C062%^The %^C094%^torch %^C226%^flickers %^C062%^and %^C088%^sputters%^C062%^.%^CRST%^",
}), 0);
   set_property("no bows",1);
   set_spoken("wizish");
   command("speech %^C060%^speak in a %^C044%^soft %^C060%^voice%^CRST%^");
   
   if(!clonep())
       return;

   ob = new(OBJ"legion_cassock.c");
   ob->set_property("monsterweapon",1);
   ob->move(TO);
   command("wear cassock");
   ob = new(OBJ"legion_sash.c");
   ob->set_property("monsterweapon",1);
   ob->move(TO);
   command("wear sash");
   ob->set_property("monsterweapon",1);
   ob = new(OBJ"legion_bracers.c");
   ob->move(TO);
   command("wear bracers");
   command ("pose %^C092%^sitting with his feet up on the desk%^CRST%^");
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
    tell_room(ETO,"%^C108%^The soldier looks up from the %^C094%^desk%^C108%^.%^CRST%^\n");
    force_me("say Hello there. Nice day, %^C111%^who are you%^CRST%^?");
    return 1;
}

void reply_introductions(string msg, object who){
        tell_room(ETO,"%^C108%^The soldier starts to fix his uniform then shrugs.%^CRST%^\n");
    force_me("say I am lieutenant Travis Wolfe, ambassador to the Pearl %^C111%^Legion%^CRST%^.");

    return 1;
}

void reply_hounds(string msg, object who){
        tell_room(ETO,"%^C108%^The soldier frowns.%^CRST%^\n");
    force_me("say The Hounds?");
        tell_room(ETO,"%^C108%^His jaw clenches%^CRST%^\n");
    force_me("say They are scum. They were responsible for the destruction of so many lives.");
    return 1;
}
void reply_legion(string msg, object who){
        tell_room(ETO,"%^C108%^The soldier waves around him.%^CRST%^\n");
    force_me("say The legion?");
        tell_room(ETO,"%^C108%^We are the Legion of %^C111%^Pearl%^CRST%^. A grand company formed from those who couldn't survive after the betrayal by the %^C111%^Hounds%^CRST%^. You should look to %^C111%^join us%^CRST%^. We always need more recruits.%^CRST%^\n");
    return 1;
}

void reply_join(string msg, object who){
        tell_room(ETO,"%^C108%^The soldier sits up a little straighter.%^CRST%^\n");
    force_me("say You want to join?");
        tell_room(ETO,"%^C108%^He nods.%^CRST%^\n");
        force_me("say Go see the quartermaster. She's on the third floor in an office.");
    return 1;
}

void reply_commander(string msg, object who){
        tell_room(ETO,"%^C108%^ .%^CRST%^\n");
    force_me("say The Commander?");
        tell_room(ETO,"%^C108%^The soldier smiles.%^CRST%^\n");
        force_me("say Commander Brock pulled us together when all hope was lost. He is the reason we stay.");
        tell_room(ETO,"%^C108%^The soldier nods and smiles again.%^RST%^\n");
    force_me("say You'll find him in the fourth floor office.");
    return 1;
}
