
#include <std.h>
#include "../../graez.h"

inherit STORAGE"heavy_forest.c";

void create() {
    ::create();

set_exits(([

"west" : FOREST"20",
"east" : FOREST"16",
"north" : FOREST"18",

]));

}

