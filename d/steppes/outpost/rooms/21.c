

#include <std.h>
#include "../outpost.h"

inherit STORAGE"roads.c";


void create() {
    ::create();

set_exits(([
"south" : ROOMS"20",
"north" : ROOMS"32",

]));

}

