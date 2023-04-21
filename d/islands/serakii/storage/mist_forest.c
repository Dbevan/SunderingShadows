
//Forest of Tears

#include <std.h>
#include "../serakii.h"

inherit CROOM;



void create() {
    ::create();
	pick_critters();
    set_repop(60);
    set_terrain(GRASSLANDS);
    set_travel(RUTTED_TRACK);
    set_property("indoors",0);
    set_property("no teleport",1);
    set_property("no scry",1);
    set_name("mist covered forest");
    set_short("%^C194%^mi%^C245%^s%^C194%^t c%^C245%^o%^C194%^v%^C248%^"+
        "e%^C194%^r%^C244%^e%^C194%^d f%^C246%^o%^C194%^"+
        "r%^C248%^e%^C243%^s%^C194%^t%^CRST%^");
    set_long("The %^C071%^forest %^CRST%^here is "+
        "filled with %^C071%^trees %^CRST%^that seem like they "+
        "are %^C059%^thousands%^CRST%^ of years old, each "+
        "%^C058%^trunk %^CRST%^measuring at least five feet "+
        "across and towering so high above you the "+
        "%^C071%^canopy %^CRST%^is unseen. What is odd here "+
        "as well is that while you are outside, the "+
        "%^C076%^canopy %^CRST%^seems to block all %^C245%^weather "+
        "%^CRST%^and instead you are left with a layer of "+
        "%^C254%^mist %^CRST%^across the %^C059%^soft ground%^CRST%^, "+
        "obscuring it from view completely. You find you cannot "+
        "look too deep into the forest without your vision swimming.\n");
    set_items(([
        ({"trees","trunks"}) : "The %^C071%^trees %^CRST%^here are massive "+
        "and obviously extremely old. The %^C058%^trunks %^CRST%^are at "+
        "least five feet across and sturdy.",
        ({"sky","canopy"}) : "The %^C076%^canopy %^CRST%^of %^C022%^"+
        "leaves %^CRST%^from the %^C076%^trees %^CRST%^is so far above "+
        "you that you cannot discern it however it seems to block "+
        "out all of the %^C244%^weather %^CRST%^so it must be solid.",
        ({"ground","mist"}) : "A thin layer of %^C251%^mist "+
        "%^CRST%^crawls on the ground, obscuring it.",
    ]));

set_smell("default","You smell %^C058%^moist earth%^CRST%^.");
set_listen("default","You hear %^C056%^strange whispers%^CRST%^ all around you.");


}

query_weather() { return "\n%^C252%^The only weather here is a fine mist "+
"that covers the ground.%^CRST%^"; }

void init(){
   ::init();
   add_action("peer_fun","peer");
}

void peer_fun(string str){

    string player_name;
    int damroll;
    string* my_visions = ({ "%^C060%^A ravaged %^C058%^corpse%^CRST%^", "%^C059%^A pile of %^C066%^dead%^C059%^ children%^CRST%^", 
        "%^C220%^A %^C228%^man%^C220%^ trapped in amber%^CRST%^","%^C065%^An %^C067%^ogre-mage%^C065%^ dripping in miasma%^CRST%^", 
        "%^C055%^A mangled %^C060%^demon%^CRST%^", "%^C240%^A black %^C245%^gate%^CRST%^" });
    string my_vision;

    if(!random(3)){return 0;}
    damroll = roll_dice(5, 10) + 10;
    this_player()->add_hp(-damroll);
    my_vision = my_visions[random(sizeof(my_visions))];
    player_name = this_player()->query_cap_name();
    tell_object(this_player(), "%^C088%^You feel a sharp pain in your head and suddenly your "+
        "vision is filled with a vivid image of a " + my_vision + "%^C088%^! The pain hits "+
        "the back of your head as the vision swims, and then it is gone again.\n%^CRST%^");
    tell_room(this_object(), "%^C088%^With a frown " + player_name + " %^C088%^squeezes "+
        "their eyes shut!%^CRST%^\n",this_player());
    return 1;

}

void pick_critters(){
   switch(random(5)){
         case 0:   set_monsters(
({MOBS"hound_mounted",MOBS"minor_abom"}),({random(2),random(3)}) );
                   break;
         case 1:   set_monsters(
({MOBS"minor_abom"}),({random(5)}) );
                   break;
         case 2:   set_monsters(
({MOBS"hound_mounted"}),({random(2)+2}) );
                   break;
         case 3..4: break;
   }
}

