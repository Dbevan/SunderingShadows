// Chernobog (9/27/21)
// Offal Marsh Demiplane Minion

#include <std.h>
#include <daemons.h>

inherit MONSTER;

int powerlevel;

void create(){
    ::create();
    set_name("swarm of flies");
    set_id(({ "swarm of flies", "flies" }));
    set_short("%^RESET%^%^CRST%^%^C144%^swarm of %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C144%^The sheer number of %^C244%^l%^C250%^a%^C244%^r%^C248%^g%^C244%^e bl%^C246%^a%^C248%^c%^C244%^k f%^C248%^l%^C244%^i%^C248%^e%^C246%^s %^C144%^in this swarm darken the air, filling the it with a mind-numbing insectoid %^C252%^bu%^C255%^z%^C252%^zin%^C255%^g%^C144%^.%^CRST%^\n");
    set_race("insect");
    set_body_type("insect");
    set_gender("neuter");
    set_size(1);
    set_alignment(8);
    set_base_damage_type("piercing");
    set_func_chance(50);
    set_funcs(({ "buzz_fun" }));
    set_property("no_random_treasure", 1);
    set_powerlevel(1);
    set_resistance_percent("acid", 100);
    set_property("aggressive", 1);
    set_property("swarm", 1);
    set_property("function and attack", 1);
    set_property("darkvision", 1);
}

void set_powerlevel(int pwrlvl){
    powerlevel = pwrlvl;
    set_hd((powerlevel * 10), 0);
    set_class("fighter");
    set_mlevel("fighter", (powerlevel * 10));
    set_damage(powerlevel, 10);
    set_attacks_num(1);
    set_overall_ac(-(powerlevel * 10));
    set_max_hp(powerlevel * 50);
    set_hp(query_max_hp());
    set_new_exp((powerlevel * 10), "very low");
    return;
}

void buzz_fun(object targ){
    object room;
    room = environment(this_object());
    
    tell_object(targ, "%^RESET%^%^CRST%^%^C144%^The %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s %^C144%^swarm about your face, crawling over your eyes, into your mouth!%^CRST%^");
    tell_room(room, "%^RESET%^%^CRST%^%^C144%^The %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s %^C144%^swarm about "+targ->QCN+"%^RESET%^%^CRST%^%^C144%^, covering their face!%^CRST%^", targ);
    
    if(SAVING_THROW_D->fort_save(targ, (powerlevel * 14))){
        tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You manage to shake them off, spitting out a couple as well.%^CRST%^");
        tell_room(room, "%^RESET%^%^CRST%^%^C118%^"+targ->QCN+"%^RESET%^%^CRST%^%^C118%^ shakes free of the swarm.%^CRST%^", targ);
        return;
    }
    
    switch(random(2)){
        case 0 :
            if(powerlevel > 4){
                tell_object(targ, "%^RESET%^%^CRST%^%^C137%^They buzz and crawl all over and around your face, causing you to %^C226%^stagger %^C137%^about!%^CRST%^");
                tell_room(room, "%^RESET%^%^CRST%^%^C137%^"+targ->QCN+"%^RESET%^%^CRST%^ %^C226%^staggers %^C137%^about, unable to avoid the swarm of %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s%^C137%^!%^CRST%^", targ);  
                load_object("/std/effect/status/staggered")->apply_effect(targ, roll_dice(1, 2), TO);
            }
            break;
        case 1 :
            tell_object(targ, "%^RESET%^%^CRST%^%^C137%^The %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s %^C137%^crawl inside your clothing, into your mouth and across your body! It is %^C102%^si%^C108%^ck%^C114%^e%^C102%^ni%^C108%^ng%^C137%^, and you start to %^C102%^p%^C108%^u%^C114%^k%^C102%^e %^C137%^uncontrollably!%^CRST%^");
            tell_room(room, "%^RESET%^%^CRST%^%^C137%^"+targ->QCN+"%^RESET%^%^CRST%^%^C137%^ is absolutely covered in flies, and begins to %^C102%^p%^C108%^u%^C114%^k%^C102%^e%^C137%^!%^CRST%^", targ);  
            load_object("/std/effect/status/sickened")->apply_effect(targ, roll_dice(1, 2), TO);
            break;
        default :
            tell_room(room, "%^RESET%^%^CRST%^%^C196%^Something went wrong, please contact Chernobog.%^CRST%^");
            break;
    }
    return;
}

