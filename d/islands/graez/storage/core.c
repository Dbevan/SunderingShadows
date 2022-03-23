
#include <std.h>
#include "../graez.h"

inherit CROOM;

void pick_critters();
void capture(string faction);

int faction_control,room_id;
string faction_name;

void create() {
    ::create();
   faction_control=0;
   // set_repop(60);

    switch(faction_control){
        case 0: faction_name = "no one";
                break;
        case 1: faction_name = "mortals";
                break;
       case 2: faction_name = "undead";
                break;
        }

    set_name("core room");
    set_short("core room");
    set_long("This shouldnt look like this. the room is borked with the room belonging to "+faction_control+" \n");

}


int check_faction(){
    return faction_control;
}

int set_faction(int i){
    faction_control = i;
    set_long("This room is controlled by the "+faction_control+" who are the "+faction_name+"  isnt that grand!\n");
    return 1;
}


void pick_critters(int rank, int faction){


   switch(faction){

      case 0 : break;
      case 1 : 

      set_monsters( ({MOBS"human_grunt"}),({random(9) }) );

      break;


      case 2:  

      set_monsters( ({MOBS"undead_grunt"}),({random(9) }) );

      break;

}
}
