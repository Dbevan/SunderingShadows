#include <std.h>
#include "../../serakii.h"

inherit VAULT;

void init() {
  ::init();
  add_action("read", "read");
}


void create() {
    ::create();
    set_terrain(GRASSLANDS);
    set_travel(PAVED_ROAD);
    set_property("indoors",1);
    set_property("light",2);
    set_name("Rook's Rest Tavern");
    set_short("%^C061%^R%^C245%^o%^C061%^"+
	"o%^C245%^k%^C061%^'s Re%^C094%^s%^C061%^"+
	"t Tave%^C094%^r%^C061%^n%^CRST%^");
	
    set_long("%^C094%^The inside of this "+
	"tavern is %^C247%^expansive%^C094%^ "+
	"and %^C088%^comfortable%^C094%^. The "+
	"giant %^C245%^stone walls%^C094%^ of "+
	"its walls are lit by %^C242%^iron "+
	"sconces%^C094%^ with %^C200%^en%^C207%^"+
	"c%^C200%^han%^C207%^t%^C200%^ed "+
	"cryst%^C207%^a%^C200%^ls%^C094%^ "+
	"lighting the room with a %^C207%^s%^C164%^"+
	"o%^C207%^ft lig%^C163%^h%^C207%^t%^C094%^. "+
	"The center of the room is a large "+
	"%^C058%^wooden bar%^CRST%^ %^C094%^with a "+
	"giant tattered flag hanging over it "+
	"with obvious reverence. On the %^C094%^north "+
	"wall is a %^C244%^stone fireplace%^C094%^ "+
	"with a %^C124%^ra%^C009%^g%^C124%^ing "+
	"fi%^C009%^r%^C124%^e%^C094%^. Tables "+
	"dot the room, %^C094%^each with enough chairs "+
        "%^C094%^for the patrons. Near the %^C244%^fireplace "+
		"%^C094%^a %^C101%^fine hammock%^C094%^ is hung, "+
        "%^C094%^and above it is mounted a fishing rod. Overall "+
		"the mood of "+
	"the room is %^C094%^one of %^C059%^calm "+
	"comfort%^C094%^.%^CRST%^\n");

   set_items(([
       ({"flag","tattered flag"}) : "%^C248%^Above "+
	   "the %^C094%^bar%^C248%^ hangs a "+
	   "large %^C252%^tattered%^C248%^ "+
	   "%^C252%^white flag%^C248%^. It has "+
	   "obviously been through several "+
	   "pitched %^C088%^battles%^C248%^ and "+
	   "bears the %^C245%^scars%^C248%^. It "+
	   "originally was %^C255%^pure white%^C248%^ "+
	   "with a %^C242%^black Rook chess piece%^C248%^.\n%^CRST%^",
       ({"walls","wall"}) : "%^C246%^The walls "+
	   "are formed from %^C243%^giant stone "+
	   "blocks%^C246%^. They are thick and strong "+
	   "enough to protect you from anything "+
	   "happening outside.%^CRST%^\n",
       ({"sconces","crystals"}) : "%^C242%^Iron "+
	   "sconces%^C246%^ line the walls at regular "+
	   "intervals, filled with %^C092%^sma%^C165%^"+
	   "l%^C092%^l glow%^C165%^i%^C092%^ng cryst%^C165%^"+
	   "a%^C092%^ls%^C056%^ %^C246%^that fill the "+
	   "room with %^C165%^g%^C093%^e%^C165%^ntle lig%^C093%^h%^C165%^t%^C246%^.%^CRST%^\n%^CRST%^",
       ({"bar","wooden bar"}) : "%^C094%^The massive bar "+
	   "takes up the entirety of the center of the "+
	   "tavern with room for dozens to sit around "+
	   "it.%^CRST%^\n%^CRST%^",
	   ({"fireplace","fire"}) : "%^C244%^A giant "+
	   "%^C248%^stone fireplace%^C244%^ is set on "+
	   "the northern wall. A %^C160%^r%^C088%^"+
	   "o%^C160%^aring fi%^C088%^r%^C160%^"+
	   "e%^C244%^ is always blazing. Embedded "+
	   "the stout oak mantle is a %^C204%^small "+
	   "bronze plaque%^C244%^.\n%^CRST%^",
	   ({"tables","chairs"}) : "%^C094%^Tables dot "+
	   "the room and are surrounded by chairs "+
	   "for patrons%^CRST%^\n%^CRST%^",
	   ({"plaque","bronze plaque"}) : "%^C058%^A small bronze "+
	   "plaque is set on the mantle of the fireplace"+
	   ".\n\n%^C228%^This inn will always stand as a "+
	   "memorial to the Last Rook, Marcello, sent by "+
	   "the Rooks of Shadow and savior to the "+
	   "survivors of the Night of the Betrayal. "+
	   "Death to the Hounds and Glory to the "+
	   "Rooks of Shadow and their last "+
	   "Commander Iskall.\n\n%^C058%^Underneath "+
	   "is a small insignia of a Rook chess "+
	   "piece. It is worn down though as "+
	   "if thousands of hands have brushed "+
           "over it.\n",
       ({"hammock","fishing rod"}) : "%^C058%^Strung up near the fireplace is a hammock large enough for two people to comfortably lay in. Above it is a %^C241%^glossy black %^C058%^fishing rod with a %^C028%^bright green cording%^C058%^ for a handle.\n",
    ]));

    set_exits(([
                   "out" : INN"1",
				   "east" : INN"darts",
        ]));


    set_door("door",INN"1","out",0);


set_smell("default","%^C058%^You smell %^C125%^"+
"rich food%^C058%^ and %^C094%^wood fire%^CRST%^");
set_listen("default","%^C061%^You hear %^C063%^constant "+
"conversation%^C061%^ around you%^CRST%^");



}

void reset(){
    ::reset();
    if(!present("waiter")){new(MOBS"christof.c")->move(TO);}
}

int read(string str)
{
        if(str == "menu") {
        tell_room(ETP,""+TPQCN+" reads over the menu.",TP);
		tell_object(TP,"You read over the menu.\n");
	    tell_object(TP,"%^C125%^**********************************\n%^CRST%^");
		tell_object(TP,"%^C063%^Drinks%^CRST%^");
        tell_object(TP,"Water\t\t\t1 Gold");
		tell_object(TP,"Coffee\t\t\t10 Gold");
		tell_object(TP,"Earl Graez tea\t\t10 Gold");
        tell_object(TP,"Red Wine\t\t25 Gold");
        tell_object(TP,"Dark Ale\t\t10 Gold");
		tell_object(TP,"Whiskey\t\t\t30 Gold\n");
		tell_object(TP,"%^C179%^Food%^CRST%^");
		tell_object(TP,"Meat Platter\t\t20 Gold");
		tell_object(TP,"Adventuring Feast\t50 Gold");
		                tell_object(TP,"Bread Platter\t\t20 Gold");
						tell_object(TP,"Fried Fish\t\t25 Gold\n");
		tell_object(TP,"%^C125%^**********************************\n%^CRST%^");
		tell_object(TP,"<buy> any dish to get it\n");
                return 1;
        }
}

