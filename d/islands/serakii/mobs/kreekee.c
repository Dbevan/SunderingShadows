// Incense woman - LoKi - 2021


#include <std.h>
#include <daemons.h>
inherit NPC;

void create(){
   object ob;
   ::create();
   set_name("kreekee");
   set_short("%^C154%^Kreekee%^C106%^, "+
   "the %^C130%^incense %^C106%^woman%^CRST%^");
   set_id(({ "kreekee","woman","incense woman"}));
   set_long("%^C138%^This woman is %^C130%^elderly%^C138%^, "+
   "perhaps one of the oldest humans you have encountered. "+
   "She has a small %^C247%^wiry frame%^C138%^ and spindly "+
   "limbs that seem to flail about on their own volition. "+
   "Her %^C094%^frizzy brown hair%^C138%^ is likewise a "+
   "%^C097%^chaotic %^C138%^mess with flakes of %^C215%^"+
   "herbs %^C138%^and %^C094%^twigs %^C138%^in it. "+
   "%^C034%^Deep green eyes %^C138%^stare back at "+
   "you with a hidden %^C082%^fire %^C138%^though "+
   "and while she looks absolutely %^C205%^crazy%^C138%^, "+
   "there is %^C092%^power %^C138%^behind her gaze. "+
   "She wears homespun clothing covered in %^C247%^"+
   "incense%^C138%^.\n%^CRST%^");
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
   set_gender("female");
   set_size(2);
   set_exp(0);
   set_overall_ac(-10);
   set_hd(35,15);
   set_max_hp(400);
   set_hp(query_max_hp());
   set("aggressive",0);
   set_emotes(1, ({
"%^C182%^A spark of %^C200%^power %^C182%^flicks from "+
"the woman's hands as she chants over some %^C094%^"+
"incense%^C182%^.%^CRST%^",
"%^C152%^The woman adds more %^C070%^herbs %^C152%^to "+
"a %^C247%^mortar and pestle%^C152%^ and grinds it to "+
"a %^C094%^powder%^C152%^.%^CRST%^",
"%^C094%^A%^C249%^ %^C009%^fire %^C094%^in the "+
"room%^C249%^ %^C197%^sputters %^C095%^and%^C249%^ "+
"%^C247%^spits%^C254%^.%^CRST%^",
}), 0);
   set_property("no bows",1);
   set_spoken("wizish");
   command("speech %^C142%^spe%^C251%^a%^C142%^"+
   "k in a p%^C251%^a%^C142%^per th%^C251%^"+
   "i%^C142%^n voi%^C251%^c%^C142%^e%^CRST%^");

    ob = new("/d/common/obj/armour/cloak_hooded");
       ob->set_short("A simple cloak");
       ob->move(TO);
       ob->set_property("enchantment",3);
       ob->set_property("monsterweapon",1);
       command("wear cloak");

    ob = new("/d/common/obj/weapon/sstaff");
       ob->move(TO);
       ob->set_short("A crooked walking staff");
       ob->set_property("enchantment",3);
       ob->set_property("monsterweapon",1);
       command("wield staff");
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
    if(strsrch(message,"incense") != -1) {
      call_out("reply_incense",1,TP,message);
    }
    if(strsrch(message,"visions") != -1) {
      call_out("reply_visions",1,TP,message);
    }
    if(strsrch(message,"hounds") != -1) {
      call_out("reply_hounds",1,TP,message);
    }
	    if(strsrch(message,"legion") != -1) {
      call_out("reply_legion",1,TP,message);
    }
}

void reply_hello(string msg, object who){
    tell_room(ETO,"%^C224%^The woman turns from a bowl "+
	"of incense and squeels!%^CRST%^\n");
    force_me("say Hello Hello! Welcome! %^C205%^Who are "+
	"you%^CRST%^?");
    return 1;
}

void reply_introductions(string msg, object who){
	tell_room(ETO,"\n");
    force_me("say Me!? I am Kreekee! I am the keeper "+
	"of %^C205%^incense%^CRST%^ here "+
	"on %^C205%^Serakii%^CRST%^!");
    tell_room(ETO,"%^C224%^She does a little "+
	"twirl, then turns serious again.%^CRST%^\n");

    return 1;
}

void reply_incense(string msg, object who){
	tell_room(ETO,"\n");
   force_me("say The I make the incense... I leave "+
	"the incense where memories are strongest "+
	"on %^C205%^Serakii%^CRST%^.");
    tell_room(ETO,"%^C224%^The woman gestures to the "+
	"piles of incense around her.%^CRST%^\n");	
 	force_me("say The incense is special. Memories "+
	"trapped in the herbs, fire brings them out.. "+
	"Inhale them and see %^C205%^visions%^CRST%^ of our past. ");

    return 1;
}
void reply_visions(string msg, object who){
	tell_room(ETO,"\n");
	force_me("say Inhaling the incense will bring "+
	"visions of the past the island has seen.");
	tell_room(ETO,"\n");
	force_me("say Remember too, who you are, changes "+
	"what you see. %^C205%^Incens%^CRST%^e is a mix "+
	"of all %^C205%^Serakii%^CRST%^, so who you "+
	"align with here, will change what the incense "+
	"shows you! %^C205%^hounds%^CRST%^, %^C205%^"+
	"legion%^CRST%^, who will you fight for?");
    return 1;
}

void reply_hounds(string msg, object who){
    force_me("say The hounds have a camp to the "+
	"northeast. Used to be a forest glade used "+
	"by the village here. They are cutthroat, "+
	"powerful though. Used to have the contract "+
	"for the white keep. The %^C205%^"+
	"Legion%^CRST%^ holds that now");
    return 1;
}
void reply_legion(string msg, object who){
    force_me("say The Legion of Pearl formed "+
	"after the Night of Betrayal from remenants "+
	"of other companies that couldn't stand alone."+
       "They hold the contract for the white keep. They "+
"have locked the gates north, so you can't go any "+
"further north. They tussle with the %^C205%^"+
"hounds%^CRST%^ constantly.");
    return 1;
}

void reply_serakii(string msg, object who){
    force_me("say This is Serakii! I suppose this town had a name, but after the betrayal we stopped calling it anything.");
    return 1;
}
