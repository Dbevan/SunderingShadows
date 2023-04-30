//generic inheritable cigar by Odin 5/2/2020
// Chernobog (10/7/22)
// cleaned up code and removed unused variables

#include <std.h>
#include <move.h>
inherit OBJECT;

string *mymsgs, *yourmsgs;
int lit, hasmsgs, intox, tracker;

void create(){
    ::create();
    set_name("cigar");
    set_id(({ "cigar" }));
    set_short("%^RESET%^%^ORANGE%^cigar%^RESET%^");
    set_long("This is a hand rolled cigar.\n\n%^RESET%^%^ORANGE%^Try smoking it. Douse it to put it out.%^RESET%^");
    set_weight(0);
    set_value(50);
    lit = 0;
    hasmsgs = 1;
    tracker = 0;
    mymsgs = ({ "%^RESET%^%^ORANGE%^You enjoy the pungent taste of the tobacco.%^RESET%^" });
    yourmsgs = ({ "%^RESET%^%^ORANGE%^You smell the pungent aroma of the burning tobacco from the cigar.%^RESET%^" });
    intox = 0;
}

void init(){
    ::init();
    add_action("extinguish", "douse");
    add_action("smoke", "smoke");
}

int smoke(string str){
    object cigar, player;
    cigar = this_object();
    player = environment(cigar);
    
    if(!id(str)){
        tell_object(player, "%^RESET%^%^CRST%^%^C058%^What do you want to smoke?%^CRST%^");
        return 0;
    }
    if(lit){
        tell_object(player, "%^RESET%^%^CRST%^%^C058%^It is already lit.%^CRST%^");
        return 0;
    }
    tell_object(player, "%^RESET%^%^CRST%^%^C100%^You begin %^C243%^s%^C245%^m%^C247%^o%^C249%^k%^C251%^i%^C253%^n%^C255%^g%^C100%^ your cigar.%^CRST%^");
    tell_room(environment(player), "%^RESET%^%^CRST%^%^C100%^"+player->query_cap_name()+"%^RESET%^%^CRST%^%^C100%^ begins %^C243%^s%^C245%^m%^C247%^o%^C249%^k%^C251%^i%^C253%^n%^C255%^g%^C100%^ their cigar.%^CRST%^", player);
    TO->set_property("added short", ({ " %^RESET%^%^CRST%^%^C214%^(%^C202%^l%^C196%^i%^C202%^t%^C214%^)%^CRST%^" }));
    call_out("take_drag", 5);
    TO->set_property("lit cigar", 1);
    lit = 1;
    return 1;
}

int extinguish(string str){
    object cigar, player;
    cigar = this_object();
    player = environment(cigar);
    
    if(!id(str)){
        tell_object(player, "%^RESET%^%^CRST%^%^C058%^What do you want to douse?%^CRST%^");
        return 0;
    }
    if(!lit){
        tell_object(player, "%^RESET%^%^CRST%^%^C058%^The cigar is not lit.%^CRST%^");
        return 0;
    }
    tell_object(player, "%^RESET%^%^CRST%^%^C058%^You douse your cigar.%^CRST%^");
    tell_room(environment(player), "%^RESET%^%^CRST%^%^C058%^"+player->query_cap_name()+"%^RESET%^%^CRST%^%^C100%^ douses a cigar.%^CRST%^", player);
    remove_call_out("take_drag");
    cigar->remove_property("lit cigar");
    lit = 0;
    cigar->remove_property_value("added short", ({ " %^RESET%^%^CRST%^%^C214%^(%^C202%^l%^C196%^i%^C202%^t%^C214%^)%^CRST%^" }));
    cigar->remove_property("added short");
    return 1;
}

void take_drag(){
    object cigar, player, room;
    int i;
    cigar = this_object();
    player = environment(cigar);
    room = environment(player);
    
    if(!living(player)){
        go_out();
        return;
    }
    if(tracker > 30 + random(16)){ 
        go_out();
        return;
    }
    
    tell_object(player, "%^RESET%^%^CRST%^%^C124%^You take a long, gentle drag from your cigar.%^CRST%^");
    tell_room(room, "%^RESET%^%^CRST%^%^C124%^"+player->query_cap_name()+"%^RESET%^%^CRST%^%^C124%^ takes a long, gentle drag from a cigar.%^CRST%^", player);
    
    if(intox){
        player->add_intox(intox);
        tell_object(player, "%^RESET%^%^CRST%^%^C144%^You start to feel %^C076%^s%^C070%^t%^C064%^r%^C076%^a%^C070%^n%^C064%^g%^C076%^e%^C144%^, and your mind wanders...%^CRST%^");
    }
    if(hasmsgs){
        i = random(sizeof(mymsgs));
        tell_object(player, mymsgs[i]);
        tell_room(room, yourmsgs[i], player);
    }
    tracker++;
    call_out("take_drag", random(30) + 60);
}

void go_out(){
    object cigar, player, room;
    cigar = this_object();
    player = environment(cigar);
    room = environment(player);
    
    if(living(player)){
        tell_room(room, "%^RESET%^%^CRST%^%^C058%^"+player->query_cap_name()+"%^RESET%^%^CRST%^%^C058%^'s cigar goes out, the tobacco finished; They dispose of the remaining butt.%^CRST%^\n", player);
        tell_object(player, "%^RESET%^%^CRST%^%^C058%^Your cigar goes out, the tobacco finished. You dispose of the remaining butt.%^CRST%^\n");
    }
    else tell_room(player, "%^RESET%^%^CRST%^%^C058%^The cigar goes out, completely spent.%^CRST%^");
    cigar->remove_property("lit cigar");
    remove_call_out("take_drag");
    lit = 0;
    intox = 0;
    hasmsgs = 0;
    tracker = 0;
    cigar->remove_property_value("added short", ({ " %^RESET%^%^CRST%^%^C214%^(%^C202%^l%^C196%^i%^C202%^t%^C214%^)%^CRST%^" }));
    cigar->remove_property("added short");
    cigar->remove();
}

remove(){
    if (lit) {
        lit = 0;
        remove_call_out("take_drag");
        this_object()->remove_property("lit cigar");
    }
    return ::remove();
}

int get_tracker(){ return tracker;}
int is_cigar(){ return 1;}

