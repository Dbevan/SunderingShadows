#include <std.h>
#include "../jarmila.h"

inherit STORAGE"hill.c";

void turning(object tp, int num);

void create(){
   ::create();
   set_exits(([
      "west" : ROOMS"35",
   ]));
}

void reset(){
   ::reset();
   //if(!present("avatar")){
   //   new("/d/common/bosses/jarmila/jarmila.c")->move(TO);
   //}
}

void pick_critters(){
               return;
}

int func_bork(){

tell_room(TO,"%^C216%^The mist fills the area, and you feel "+
"magic flee the area%^CRST%^.");
set_property("no magic",1);
return;
}

int func_unbork(){
tell_room(TO,"%^C216%^The mist clears the area, and you feel "+
"magic return the area%^CRST%^.");
remove_property("no magic");
return;
}


