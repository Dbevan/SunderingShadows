//adapted from Ares' testdummy for new Seneca
//used in the mage guild
//~Circe~ 1/9/08
#include <std.h>

inherit "/std/monster.c";

void create(){
    ::create();
    set_name("dummy");
    set_id(({ "dummy", "test dummy", }));
    set_race("test dummy");
    set_body_type("human");
    set_short("ooc test dummy");
    set_long("Come at me, bro!");
    set_hd(1, 1);
    set_class("fighter");
    set_mlevel("fighter", 1);
    set_max_hp(10000000);
    set_hp(query_max_hp());
    set_property("no death", 1);
    set_property("no bows", 1);
    set_property("no dominate", 1);
    set_property("fear_immunity", 1);
    //set_func_chance(100);
    //set_funcs(({ "round_cleanup" }));
}

int do_typed_damage_effects(victim, limb, damage, damage_type){
    tell_room(ETO, "%^MAGENTA%^Dummy says%^RESET%^: I got hit to " + limb + " for " + damage + " " + damage_type + " damage!");
    return damage;
}

void round_cleanup(){
    object* attackers, target, room;
    int count, i;
    
    room = environment(this_object());
    attackers = query_attackers();
    count = sizeof(attackers);
    
    if(count == 1) return;
    for(i = 0; i < count; i++){
        target = attackers[i];
        if(userp(target)) continue;
        tell_room(room, "%^RESET%^%^CRST%^%^C196%^The dummy swings about wildly, cracking "+target->query_cap_name()+"'s head open with a wooden limb.%^CRST%^");
        target->die();
        continue;
    }
    
    return;
}

