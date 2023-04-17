#include <std.h>
#include <objects.h>

inherit ROOM;

void create() {
    room::create();
   set_terrain(HEAVY_FOREST);
   set_travel(FOOT_PATH);
    set_property("light",1);
    set_property("indoors",0);
    set_short("Kobold Forest");
    set_long(
@KAYLA
You are among the confusing trees of the Kobold Forest.  All
around you, fog blankets the forest like a flowing dark cloak.
The path bends here towards the west, to the south the
dense forest continues.
KAYLA
    );
      set_listen("default","Peculiar, the forest is deadly quiet.");
    set_exits(([
       "south" : "/d/deku/new/new12",
       "west" : "/d/deku/new/new16",
       "north" : "/d/deku/new/new15"
    ] ));
    set_items(([
       "forest" : ".",
       "trees" : "."
    ] ));
}

void init() {
  ::init();
  
  if(!userp(this_player()))
      return;
  
  do_random_encounters(({"/d/deku/monster/goblin"}),90,6);
}
