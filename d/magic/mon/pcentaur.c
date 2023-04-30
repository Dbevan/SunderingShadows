//pbat.c

#include <std.h>

inherit MINION;

void create(){
   ::create();

   set_name("phantom centaur");
   set_short("%^RESET%^%^RED%^Shimmering phantom centaur");
   set_id(({"centaur","phantom","phantom centaur","construct"}));
   //set("not living",1);
   set_resistance_percent("negative energy", 100);
   set_resistance_percent("positive energy", 100);



   set_long(
@OLI
   %^CYAN%^This shimmering form is a centaur. Translucent in some form, it seems very real
in another.
OLI
   );
   set_race("centaur");
    set_property("no animate",1);
   set_gender("male");
   set_body_type("centaur");
   set_hd(17,1);
   set_stats("strength",17);
   set_hp(205);
   set_attack_bonus(3);
   set_weight(10000);
   new("/d/shadow/room/city/cguild/paladin/lhlance")->move(TO);
   command("wield lance");
   set_damage_bonus(3);
   set_size(1);
   set_overall_ac(0);
   set("magic",1);
   set_property("no random treasure", 1);
   set_heart_beat(1);
}

void heart_beat()
{
    object owner;
    
    ::heart_beat();
    
    if(!objectp(this_object()) || !environment(this_object()))
        return;
    
    owner = this_object()->query_property("minion");
    
    if(!objectp(owner) || environment(this_object()) != environment(owner))
    {
        die();
        return;
    }
}

void die(object ob){
   tell_room(ETO,"%^RED%^The phantom centaur shimmers and is dispelled.");
   remove();
   return;
}
