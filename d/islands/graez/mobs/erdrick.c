// Tobacco Merchant for Graez

#include <std.h>
#include "../graez.h"
#include <daemons.h>
inherit "/std/vendor.c";

/* Edrick is a vet of the conflict on Graez who can no longer man the battlements but 
serves his brothers by lifting their spirits with tobacco and pipes. Fiercely loyal to 
the cause he will not leave until he's the last one, or until they carry him out */


void create() {
   object ob;
   ::create();
   set_name("Erdrick Knoll"); 
   set_short("%^C064%^Erdrick Knoll%^C101%^, one armed veteran%^CRST%^"); 
   set_id(({ "erdrick","Erdrick","Erdrick Knoll","erdrick knoll",
      "veteran","one armed veteran", "erdrick knoll, one armed veteran"})); 
   set_long("This %^C244%^soldier%^CRST%^ has obviously seen %^C060%^countless "+
      "battles%^CRST%^ in %^C144%^Graez%^CRST%^ and has come out the other "+
      "end a little worse for wear. His %^C247%^battered armor%^CRST%^ "+
      "is %^C088%^scarred%^CRST%^ with a thousand %^C059%^knicks%^CRST%^ "+
      "and %^C059%^scratches%^CRST%^ and his face is likewise %^C061%^marred%^CRST%^. "+
      "What strikes you most though is that he is completely missing his left "+
      "arm at the shoulder. At his side is a %^C136%^large haversack%^CRST%^ "+
      "that is filled with %^C144%^tobacco%^CRST%^ and instruments for the "+
      "smoking of %^C144%^tobacco%^CRST%^ to sell to the %^C246%^soldiers%^CRST%^ "+
      "who come into the %^C058%^barracks%^CRST%^.\n"); 
   set_race("human");
   set_stats("intelligence",14);
   set_stats("wisdom",18);
   set_stats("strength",20);
   set_stats("charisma",16);
   set_stats("dexterity",12);
   set_stats("constitution",8); // loss of arm

   set_class("fighter");  
   set_mlevel("fighter",30);
   set_guild_level("fighter",30);
   
   set_alignment(1);
   set_gender("male");
   set_size(2);
   set_exp(0); 
   set_overall_ac(-10); 
   set_hd(30,10);
   set_max_hp(400);
   set_hp(query_max_hp());
   set("aggressive",0); 

   set_emotes(2, ({ 
      "%^C143%^Erdrick%^C060%^ calls over to a fellow %^C247%^soldier%^C060%^ returning back from patrol%^CRST%^",
      "%^C101%^With deftness, %^C143%^Erdrick%^C101%^ manages to roll a %^C137%^cigarette%^C101%^ with one hand%^CRST%^.",
      "%^C143%^Erdrick%^C151%^ rummages through his %^C136%^haversack%^C151%^ and pulls out a %^C253%^new pipe%^CRST%^",
      "%^C059%^The whole room %^C243%^echos%^C059%^ with the cheers of %^C242%^soldiers%^C059%^ celebrating%^CRST%^",
      "%^C146%^A %^C160%^bloodied patrol%^C146%^ arrives back, obviously %^C244%^missing%^C146%^ most of the member%^CRST%^s",
}), 0);
   set_property("no bows",1); 
   set_spoken("wizish"); 
   command("speech speak in a gruff voice");

   set_property("no_random_treasure",1);
   set_items_allowed("tobacco");
   set_storage_room(STORAGE"erdrick_storage");
   
   if(!clonep())
       return;
   
   ob = new("/d/common/obj/armour/banded");
   ob->move(TO); 
   ob->set_short("%^C059%^dent%^C060%^e%^C059%^d and sca%^C060%^rr%^C059%^ed banded%^CRST%^"); 
   ob->set_long("%^C059%^dent%^C060%^e%^C059%^d and sca%^C060%^rr%^C059%^ed banded%^CRST%^"); 
   ob->set_property("enchantment",3); 
   ob->set_property("monsterweapon",1); 
   command("wear banded"); 

}
