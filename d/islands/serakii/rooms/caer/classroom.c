
//Instruction Room

#include <std.h>
#include "../../serakii.h"

inherit ROOM;



void create() {
    ::create();

    set_terrain(CITY);
    set_travel(PAVED_ROAD);
    set_property("light",3);
    set_property("indoors",1);
    set_property("no teleport",1);

	
    set_name("library classroom");
    set_short("%^C136%^l%^C094%^i%^C136%^br%^C094%^"+
	"a%^C136%^ry cla%^C094%^s%^C136%^sro%^C094%^o%^C136%^m%^CRST%^");
    set_long("This smaller room off from the library "+
	"has been set up as a classroom where researchers "+
	"can teach the younger students. A %^C058%^lectern%^CRST%^ "+
	"sits at the front of the room and a series of "+
	"%^C094%^benches %^CRST%^are arranged where the "+
	"students can %^C109%^>sit<%^CRST%^ to listen. "+
	"The walls are %^C094%^wood panels%^CRST%^ and "+
	"the ceiling a %^C138%^tall undecorated arch%^CRST%^. "+
	"%^C061%^Students%^CRST%^ mill about the back "+
	"of the room and sit on the farthest benches.\n");
    set_items(([
        ({"lectern"}) : "The %^C059%^lectern %^CRST%^is a "+
		"stand where the teacher can speak from. It is made "+
		"from %^C094%^wood %^CRST%^and has a place where "+
		"the teacher can place their %^C252%^notes%^CRST%^.",
        ({"benches","bench"}) : "%^C094%^Benches %^CRST%^face "+
		"the %^C060%^lectern %^CRST%^and could facilitate "+
		"twenty or so %^C062%^students%^CRST%^.",
    ]));

set_smell("default","%^C058%^You smell %^C094%^wood %^C058%^polish.%^CRST%^");
set_listen("default","%^C068%^Students %^C062%^talk all around you.%^CRST%^");

set_exits(([
    "east" : CAER"library",
]));

}



void init(){
   ::init();
   add_action("sit_fun","sit");
}

int sit_fun(string str){
   if(!str){
      tell_object(TP,"You could >sit on the benches<.");
      return 1;
   }
   if(str == "on bench" || str == "on the benches"){
      tell_object(TP," You sit on the benches to watch the lectern.");
      tell_room(ETP,""+TPQCN+" sits down on the benches.",TP);
      TP->force_me("pose sitting on the benches facing the lectern.");
      return 1;
   }
   return 0;
}

void reset(){
   ::reset();
   if(!present("professor")){
      new(MOBS"professor.c")->move(TO);
   }
}


