// Branwen (6/4/22)
// Yniam Waystation - Shrine to Cacia

#include <std.h>
inherit "/std/temple";

int clutter;

void create(){
    ::create();
    set_terrain(BEACH);
    set_travel(DIRT_ROAD);
    set_property("light", 2);
    set_property("indoors", 0);
    set_temple("cacia");
    set_name("cacia's shrine");
    set_short("%^RESET%^%^CRST%^%^C060%^Cacia's Shrine%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C101%^A %^C243%^stone path%^C101%^ leads to a circle of pavement that gives way to a raised %^C243%^m%^C245%^a%^C243%^r%^C245%^b%^C243%^l%^C245%^e %^C243%^pl%^C245%^at%^C243%^fo%^C245%^rm%^RESET%^%^C101%^ dominated by a %^C060%^beguiling sculpture%^RESET%^%^C101%^. Offerings are lain out at its feet or placed reverently in its upturned palms. An %^C102%^inscription%^RESET%^%^C101%^ wraps around the periphery of the dais, her charge to the faithful.\n\n%^RESET%^%^C059%^Little else flourishes here, and even the wind seems hesitant to expel its breath, the world stilled as if waiting for a ravenous predator to pass.\n\n%^C058%^Piles of lumber and stone ring the area, and scaffolding has already been installed as workers move about the area.\n%^CRST%^");
    set_smell("default","%^RESET%^%^CRST%^%^C156%^The scents of the plains are carried on the breeze.%^CRST%^");
    set_listen("default","%^RESET%^%^CRST%^%^C247%^The %^C244%^silence%^C247%^ is palpable.%^CRST%^");
    set_items(([
        "offerings" : "%^RESET%^%^CRST%^%^C109%^Placed with intention are traces of %^C245%^worldly vices%^C109%^: Bottles of spirits, cigarettes, pouches of pungent herbs. Splashes of blood, wine, and other curious substances are all %^C114%^reverently%^C109%^ left here by those seeking her favor.%^CRST%^",
        ({ "statue", "platform", "dais", "sculpture" }) : "%^RESET%^%^CRST%^%^C060%^A sinuous sculpture lurks before you, its %^C054%^alluring%^C060%^ feminine form encapsulated in solid %^C061%^darksteel%^RESET%^%^C060%^, the edge of each detail picked out in a sinister %^C053%^viol%^C052%^e%^C053%^t tarni%^C052%^s%^C053%^h%^RESET%^%^C060%^. A smile tugs at the corner of her %^C059%^swollen lips%^RESET%^%^C060%^, her comely gaze cast downward to take in your measure. Six supple arms grace her slender body, the two at her shoulders reaching high and low along her curves to seek out the %^C059%^tantalizing%^C060%^ swell of a breast and the %^C059%^shaded secrets%^C060%^ between clenched thighs. The other four slink outwards from the serpentine curve of her back, her delicate hands palm upwards, awaiting the %^C066%^offerings%^C060%^ of her supplicants.%^CRST%^",
        "inscription" : ({"%^RESET%^%^CRST%^%^C102%^Words have been carved into the curve of the stone base.%^CRST%^", "%^RESET%^%^CRST%^%^C059%^\"She Who Drinks of the World.\"%^CRST%^", "common"}),
        ]));
    set_exits(([
        "north" : "/d/dagger/yniam/room/outpost_hub",
        ]));
        
    clutter = random(7) + 3;
    place_long();
    place_offerings();
}

void init(){
    ::init();
    add_action("read_charge","read");
}

int read_charge(string str){
    object player;
    
    if(!str) return 0;
    if(str != "charge" && str != "inscription") return 0;
    
    player = this_player();
    tell_object(player, "%^RESET%^%^CRST%^%^C025%^The void will consume all in the end, thus the wise know to take all they want while they can. Indulge in the experiences of the world, challenge the taboo, and disregard the morals of civil society for they are only intended to oppress and suppress. Entice others to satiate their own vices often, laying the world open to Cacia.%^RESET%^%^CRST%^");
    if(!player->query_invis() || !player->query_hidden()) tell_room(environment(player), "%^RESET%^%^CRST%^"+player->query_cap_name()+"%^RESET%^%^CRST%^ looks over the charge.%^CRST%^", player);
   return 1;
}

void place_long(){
    string long_desc;
    
    long_desc = "%^RESET%^%^CRST%^%^C101%^A %^C243%^stone path%^C101%^ leads to a circle of pavement that gives way to a raised %^C243%^m%^C245%^a%^C243%^r%^C245%^b%^C243%^l%^C245%^e %^C243%^pl%^C245%^at%^C243%^fo%^C245%^rm%^RESET%^%^C101%^ dominated by a %^C060%^beguiling sculpture%^RESET%^%^C101%^.";
    switch(clutter){
        case 3..4:
            long_desc += " A %^C144%^few %^C101%^offerings";
            break;
        case 5..7:
            long_desc += " A %^C144%^gathering %^C101%^of various offerings";
            break;
        case 8..10:
            long_desc += " A %^C144%^multitude %^C101%^of offerings";
            break;
    }
    long_desc += " are lain out at its feet or placed reverently in its upturned palms. An %^C102%^inscription%^RESET%^%^C101%^ wraps around the periphery of the dais, her charge to the faithful.\n\n%^RESET%^%^C059%^Little else flourishes here, and even the wind seems hesitant to expel its breath, the world stilled as if waiting for a ravenous predator to pass.\n\n%^C058%^Piles of lumber and stone ring the area, and scaffolding has already been installed as workers move about the area.\n%^CRST%^";
    set_long(long_desc);
    return;
}

void place_offerings(){
    string offerings_desc, *offerings;
    int i, selection;
    
    offerings = ({ "%^CRST%^%^RESET%^%^C245%^a s%^C248%^m%^C245%^all fla%^C248%^s%^C245%^k%^CRST%^", "%^CRST%^%^RESET%^%^C059%^sil%^C243%^k%^C059%^y un%^C243%^de%^C059%^rgarm%^C243%^en%^C059%^ts%^CRST%^", "%^CRST%^%^RESET%^%^C058%^a half-smoked %^C130%^c%^C136%^i%^C130%^g%^C136%^a%^C130%^r%^CRST%^", "%^CRST%^%^RESET%^%^C250%^a %^C243%^s%^C245%^i%^C250%^lv%^C245%^e%^C243%^r %^C250%^ci%^C101%^g%^C250%^ar%^C245%^e%^C250%^tte ca%^C245%^s%^C250%^e%^CRST%^", "%^CRST%^%^RESET%^%^C248%^an unopened %^C101%^bottle%^C248%^ of %^C255%^v%^C250%^o%^C255%^dka%^CRST%^", "%^CRST%^%^RESET%^%^C109%^a ri%^C103%^bb%^C109%^ed gla%^C103%^s%^C109%^s dil%^C103%^d%^C109%^o%^CRST%^", "%^CRST%^%^RESET%^%^C021%^a de%^C026%^e%^C021%^p bl%^C026%^u%^C021%^e m%^C026%^u%^C021%^shro%^C026%^o%^C021%^m%^CRST%^", "%^CRST%^%^RESET%^%^C088%^a r%^C089%^e%^C088%^d-cap%^C089%^p%^C088%^ed m%^C089%^u%^C088%^shro%^C089%^o%^C088%^m%^CRST%^", "%^CRST%^%^RESET%^%^C101%^an empty %^C100%^whiskey %^C101%^bottle%^CRST%^", "%^CRST%^%^RESET%^%^C090%^a plate of p%^C096%^a%^C089%^s%^C090%^tr%^C096%^ie%^C090%^s%^CRST%^", "%^CRST%^%^RESET%^%^C104%^a b%^C122%^o%^C104%^wl of gr%^C122%^a%^C104%^pes%^CRST%^", "%^CRST%^%^RESET%^%^C243%^a set of shackles%^CRST%^", "%^CRST%^%^RESET%^%^C052%^a le%^C053%^a%^C052%^ther whip%^CRST%^", "%^CRST%^%^RESET%^%^C059%^an obsidian ring%^CRST%^", "%^CRST%^%^RESET%^%^C184%^a go%^C190%^ld%^C184%^en ri%^C190%^n%^C184%^g%^CRST%^", "%^CRST%^%^RESET%^%^C255%^a d%^C159%^i%^C250%^a%^C255%^mo%^C159%^n%^C255%^d ri%^C159%^n%^C255%^g%^CRST%^", "%^CRST%^%^RESET%^%^C136%^a p%^C142%^i%^C148%^l%^C226%^e o%^C190%^f c%^C226%^o%^C148%^i%^C142%^n%^C136%^s%^CRST%^", "%^CRST%^%^RESET%^%^C161%^c%^C160%^o%^C088%^l%^C089%^o%^C090%^r%^C091%^f%^C092%^u%^C093%^l %^C021%^g%^C020%^e%^C025%^m %^C030%^s%^C035%^h%^C040%^a%^C047%^r%^C084%^d%^C085%^s%^CRST%^" });
    offerings += ({ "%^CRST%^%^RESET%^%^C188%^a small pouch of %^C195%^white powder%^CRST%^", "%^CRST%^%^RESET%^%^C124%^a sprinkling of %^C196%^r%^C124%^e%^C196%^d du%^C124%^s%^C196%^t%^CRST%^", "%^CRST%^%^RESET%^%^C101%^a %^C102%^pipe%^C101%^ packed full of %^C082%^h%^C076%^e%^C082%^rbs%^CRST%^", "%^CRST%^%^RESET%^%^C094%^c%^C095%^ho%^C094%^col%^C095%^at%^C094%^e %^C124%^st%^C088%^r%^C125%^a%^C124%^wberr%^C125%^i%^C088%^e%^C124%^s%^CRST%^", "%^CRST%^%^RESET%^%^C243%^a sp%^C060%^ik%^C243%^ed c%^C060%^o%^C243%^ll%^C060%^a%^C243%^r%^CRST%^", "%^CRST%^%^RESET%^%^C189%^a half-empty %^C183%^wine%^C189%^ glass%^CRST%^", "%^CRST%^%^RESET%^%^C101%^half of a %^C102%^bottle%^C101%^ of %^C100%^rum%^CRST%^", "%^CRST%^%^RESET%^%^C150%^a gl%^C144%^a%^C150%^ss e%^C144%^y%^C150%^e%^CRST%^", "%^CRST%^%^RESET%^%^C100%^a w%^C101%^o%^C100%^rn w%^C101%^o%^C100%^od%^C101%^e%^C100%^n dil%^C101%^d%^C100%^o%^CRST%^", "%^CRST%^%^RESET%^%^C243%^a ball gag%^CRST%^", "%^CRST%^%^RESET%^%^C159%^a p%^C158%^a%^C157%^i%^C156%^r %^C157%^o%^C158%^f %^C157%^d%^C158%^i%^C159%^r%^C158%^t%^C157%^y %^C156%^p%^C157%^a%^C158%^n%^C159%^t%^C158%^i%^C157%^e%^C156%^s%^CRST%^", "%^CRST%^%^RESET%^%^C143%^a small dish of %^C136%^h%^C142%^o%^C136%^ney%^CRST%^", "%^CRST%^%^RESET%^%^C102%^an empty %^C101%^beer stein%^CRST%^", "%^CRST%^%^RESET%^%^C191%^a sheaf of %^C184%^wheat%^CRST%^", "%^CRST%^%^RESET%^%^C083%^a bouquet of %^C127%^w%^C126%^i%^C125%^l%^C124%^d %^C196%^f%^C197%^l%^C198%^o%^C199%^w%^C200%^e%^C201%^r%^C207%^s%^CRST%^", "%^CRST%^%^RESET%^%^C255%^milky drippings%^CRST%^", "%^CRST%^%^RESET%^%^C053%^a %^C052%^scourge%^C053%^ whip%^CRST%^", "%^CRST%^%^RESET%^%^C250%^a %^C253%^shiny %^C243%^d%^C245%^a%^C247%^gg%^C245%^e%^C243%^r%^CRST%^", "%^CRST%^%^RESET%^%^C243%^a razor-edged poker%^CRST%^", "%^CRST%^%^RESET%^%^C231%^a jug of %^C195%^m%^C231%^o%^C195%^onshi%^C231%^n%^C195%^e%^CRST%^", "%^CRST%^%^RESET%^%^C059%^a t%^C058%^u%^C059%^ft of d%^C058%^a%^C059%^rk f%^C058%^u%^C059%^r%^CRST%^", "%^CRST%^%^RESET%^%^C176%^f%^C177%^u%^C176%^zzy h%^C177%^a%^C176%^n%^C177%^dc%^C176%^uf%^C177%^f%^C176%^s%^CRST%^", "%^CRST%^%^RESET%^%^C089%^a %^C124%^blo%^C088%^o%^C124%^dy%^C089%^ finger%^CRST%^", "%^CRST%^%^RESET%^%^C245%^a %^C243%^charred%^C245%^ portrait%^CRST%^", "%^CRST%^%^RESET%^%^C189%^a %^C195%^crusty drawing%^C189%^ of a %^C183%^woman%^CRST%^", "%^CRST%^%^RESET%^%^C101%^an old l%^C100%^u%^C101%^te%^CRST%^", "%^CRST%^%^RESET%^%^C124%^a single %^C088%^red%^C124%^ rose%^CRST%^", "%^CRST%^%^RESET%^%^C255%^a jar of milk%^CRST%^", "%^CRST%^%^RESET%^%^C040%^a %^C035%^l%^C030%^o%^C025%^n%^C020%^g %^C021%^p%^C026%^e%^C031%^a%^C036%^c%^C041%^o%^C046%^c%^C082%^k %^C047%^f%^C042%^e%^C037%^a%^C032%^t%^C027%^h%^C033%^e%^C038%^r%^CRST%^", "%^CRST%^%^RESET%^%^C076%^a small bag of %^C082%^c%^C083%^r%^C084%^y%^C085%^s%^C086%^t%^C158%^a%^C157%^l%^C156%^l%^C157%^i%^C158%^z%^C086%^e%^C085%^d %^C084%^h%^C083%^e%^C082%^r%^C083%^b%^C084%^s%^CRST%^", "%^CRST%^%^RESET%^%^C250%^a s%^C245%^c%^C101%^a%^C250%^tter%^C245%^i%^C101%^n%^C250%^g of %^C255%^ci%^C250%^ga%^C245%^r%^C101%^e%^C250%^tt%^C255%^es%^CRST%^", "%^CRST%^%^RESET%^%^C070%^a cup of %^C065%^green tea%^CRST%^", "%^CRST%^%^RESET%^%^C101%^a %^C243%^wafting %^C101%^stick of %^C103%^incense%^CRST%^", "%^CRST%^%^RESET%^%^C125%^a sensual %^C124%^r%^C088%^e%^C124%^d c%^C088%^a%^C124%^ndle%^CRST%^" });
    offerings_desc = "%^RESET%^%^CRST%^%^C109%^Placed with intention are traces of %^C245%^worldly vices%^C109%^, all %^C114%^reverently%^C109%^ left here by those seeking her favor. At a glance, you can see: %^CRST%^";
    for(i = 0; i < clutter; i++){
        selection = random(sizeof(offerings));
        if((i + 1) == clutter) offerings_desc += "and "+offerings[selection]+".%^CRST%^";
        else offerings_desc += offerings[selection]+", ";
        
        offerings -= ({ offerings[selection] });
        continue;
    }
    remove_item("offerings");
    add_item("offerings", offerings_desc);
    return;
}

