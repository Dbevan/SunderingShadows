
#include <std.h>
#include "../../serakii.h"

inherit STORAGE"mist_forest.c";



void create(){
   ::create();


set_exits(([

    "east" : FOR"43",
    "north" : FOR"45",

]));

}

