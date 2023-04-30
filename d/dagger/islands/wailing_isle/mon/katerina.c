// Chernobog & Titania (3/1/22)
// Wailing Isle - Katerina, Captain of the Zephyr

#include <std.h>
#include <daemons.h>
#include "../defs.h"
inherit MONSTER;

int powerlevel, counter, buffed, psize, charge, adjusted;

void create(){
    ::create();
    set_name("katerina");
    set_id(({"katerina", "Katerina", "captain", "katerina, captain of the zephyr", "Katerina, Captain of the Zephyr"}));
    set_short("%^RESET%^%^CRST%^%^C051%^K%^C045%^ate%^C039%^rina%^RESET%^%^C101%^, %^C144%^C%^C101%^aptain of the %^RESET%^%^C144%^Z%^C101%^ephyr%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C101%^Katerina is a slender but sturdy looking woman, her charismatic presence lending a sense of %^C106%^authority%^C101%^ to her surroundings. Her hair is pulled into a %^C058%^se%^C088%^n%^C124%^si%^C088%^b%^C058%^le br%^C088%^a%^C058%^id%^RESET%^%^C101%^ that falls to midback, keeping the %^C058%^l%^C130%^o%^C124%^n%^C058%^g ci%^C130%^n%^C124%^n%^C058%^am%^C124%^o%^C058%^n lo%^C124%^c%^C130%^k%^C058%^s%^RESET%^%^C101%^ from distracting from her comely, %^C058%^swarthy%^C101%^ visage. She bares %^C095%^aristocratic%^C101%^ features, softened by the %^C105%^p%^C039%^a%^C045%^l%^C117%^e %^C111%^b%^C039%^l%^C045%^u%^C117%^e%^RESET%^%^C101%^ of her eyes, which hold a glimmer of %^C098%^am%^C097%^us%^C098%^em%^C097%^ent%^RESET%^%^C101%^. She is dressed in loose-fitting loungewear, but accents her features in %^C226%^g%^C220%^o%^C142%^ld%^C220%^e%^C226%^n j%^C220%^e%^C142%^wel%^C220%^r%^C226%^y%^RESET%^%^C101%^ that looks anything but cheap.%^CRST%^\n");
    set_race("human");
    set_gender("female");
    set_alignment(2);
    set_func_chance(100);
    set_funcs(({ 
        "round_cleanup" }));
    set_property("no death", 1);
    set_property("no sleep", 1);
    set_property("fear_immunity", 1);
    set_property("confusion_immunity", 1);
    set_property("no knockdown", 1);
    set_property("no trip", 1);
    set_property("no tripped", 1);
    set_property("no bows", 1);
    set_property("no paralyze", 1);
    set_property("no steal", 1);
    set_property("no dominate", 1);
    set_property("no hold", 1);
    set_property("no disarm", 1);
    set_property("no_random_treasure", 1);
    set_property("hidden inventory", 1);
    set_property("function and attack", 1);
    set_property("full attacks",1);
    set_monster_feats(({
        "opportunity strikes",
        "positioning",
        "opportunistic parry",
        "weapon training",
        "weapon focus",
        "greater weapon focus",
        "epic weapon focus",
        "weapon specialization",
        "greater weapon specialization",
        "epic weapon specialization",}));
    set_spell_chance(25);
    set_spells(({
        "airbolt", }));
    force_me("speech speak with a heavy Tsarven accent");
    set_powerlevel(5);
    counter = 1;
    buffed = 0;
    psize = 1;
    adjusted = 0;
}

void set_powerlevel(int pwrlvl){
    object katerina, zephyr;
    katerina = this_object();
    
    powerlevel = pwrlvl;
    set_hd((powerlevel * 10), 0);
    set_class("fighter");
    set_mlevel("fighter", (powerlevel * 10));
    set_class("thief");
    set_mlevel("thief", (powerlevel * 10));
    set_class("cleric");
    set_mlevel("cleric", (powerlevel * 10));
    set_damage((powerlevel * 2), 5);
    set_attacks_num(powerlevel);
    set_overall_ac(-((powerlevel * 10) + 5));
    set_max_hp(powerlevel * powerlevel * 300);
    set_hp(query_max_hp());
    set_new_exp((powerlevel * 10), "high");
    set_skill("athletics", (powerlevel * 13));
    
    if(!zephyr = present("cutlass", katerina)) zephyr = new(OBJ"zephyr_talon");
    zephyr->move(katerina);
    zephyr->remove_property("enchantment");
    zephyr->set_property("enchantment", powerlevel * 2);
    force_me("unwield cutlass");
    force_me("wield cutlass");
    
    tell_room(environment(katerina), "\n%^RESET%^%^CRST%^Katerina sizes you up and adjusts her stance.%^CRST%^");
    return;
}

void init(){
    ::init();
    if((!buffed) && (powerlevel > 3)){
        new("/cmds/spells/d/_displacement.c")->use_spell(this_object(), 0, ((powerlevel) * 10), 100, "mage");
        new("/cmds/spells/e/_elemental_aura.c")->use_spell(this_object(), "electricity", (powerlevel * 10), 100, "mage");
        buffed = 1;
    }
}

void round_cleanup(){
    object room;
    room = environment(this_object());
    
    check_psize();
    if(!adjusted && (psize > 1)) adjust_hp();
    switch(counter){
        case 1 :
            call_backup();
            create_dustdevil();
            break;
        case 2 :
            charge_weapon();
            break;
        case 3 :
            discharge_weapon();
            break;
        case 4 :
            discharge_weapon();
            break;
        default :
            tell_room(room, "Something is wrong, please contact Chernobog.");
            break;
    }
    counter++;
    if(counter > 4) counter = 1;
    return;
}

void adjust_hp(){
    int multiplier;
    if(psize == 1) return;
    multiplier = 300 + (75 * psize);
    set_max_hp(powerlevel * powerlevel * multiplier);
    set_hp(query_max_hp());
    adjusted = 1;
    return;
}

void call_backup(){
    object katerina, room, sailor;
    int i, buildto;
    
    katerina = this_object();
    room = environment(katerina);
    buildto = 3 * psize;
    
    if(!present( "sailor 2", room)){
        tell_room(room, "\n%^RESET%^%^CRST%^%^C101%^Katerina steps back, letting out a %^C144%^shout %^C101%^and calling some of her %^C038%^crew %^C101%^to her aid!%^CRST%^");
        for( i = 0; i < buildto; i++){
            sailor = new(MON"sailor");
            sailor->set_powerlevel(powerlevel + psize);
            sailor->move(room);
            katerina->add_follower(sailor);
            katerina->add_protector(sailor);
        }
    }
    
    return;
}

void discharge_weapon(){
    object katerina, room, target;
    object* attackers;
    int i, damage;
    katerina = this_object();
    room = environment(katerina);
    
    if(charge == 0){ // wind, aoe that wipes out fodder
        tell_room(room, "\n%^RESET%^%^CRST%^%^C101%^Katerina gives her %^C153%^cutlass %^RESET%^%^C101%^a low swing, the wisps of cloud flying free and twirling into %^C250%^c%^C255%^ycl%^C250%^o%^C255%^ne%^C250%^s %^RESET%^%^C101%^that sweep across the floor...%^CRST%^");
        attackers = katerina->query_attackers();
        damage = roll_dice(powerlevel * psize * 2, 10);
        for( i = 0; i < (powerlevel * psize); i++){
            if(sizeof(attackers) <= i) continue;
            if(!objectp(attackers[i])) continue;
            target = attackers[i];
            if(!objectp(target)) continue;
            if(userp(target)){
                tell_object(target, "%^RESET%^%^CRST%^%^C100%^The winds push and pull at you, knocking you into some of the furniture!%^CRST%^");
                tell_room(room, "%^RESET%^%^CRST%^%^C100%^The winds push and pull at "+target->query_cap_name()+"%^RESET%^%^CRST%^%^C100%^, knocking them into some of the furniture!%^CRST%^", target);
                target->cause_typed_damage(target, "torso", damage, "bludgeoning");
            }
            else{
                tell_room(room, "%^RESET%^%^CRST%^%^C100%^"+target->query_cap_name()+"%^RESET%^%^CRST%^%^C100%^ is caught up in one and sent flying out a window!%^CRST%^");
                target->die();
            }
            continue;
        }
    }
    if(charge == 1){ // lightning, burst damage to players
        tell_room(room, "\n%^RESET%^%^CRST%^%^C101%^Katerina brings her cutlass down, and the roiling tongues of lightning scatter across the floor in snakes of %^C228%^el%^C231%^e%^C228%^ctr%^C226%^i%^C231%^c%^C228%^ity...%^CRST%^");
        attackers = katerina->query_attackers();
        damage = roll_dice(powerlevel * psize * 6, 10);
        for( i = 0; i < sizeof(attackers); i++){
            target = attackers[i];
            if(!objectp(target)) continue;
            if(SAVING_THROW_D->reflex_save(target, (powerlevel * 17 + psize))){
                tell_room(room, "%^RESET%^%^CRST%^%^C118%^"+target->query_cap_name()+"%^RESET%^%^CRST%^%^C118%^ dodges out of the way, escaping the brunt of the blast!%^CRST%^", target);
                tell_object(target, "%^RESET%^%^CRST%^%^C118%^You dodge out of the way, escaping the brunt of the blast!%^CRST%^");
                target->cause_typed_damage(target, "torso", damage / 2, "electricity");
            }
            else{
                tell_room(room, "%^RESET%^%^CRST%^%^C196%^Jagged bolts dart along the ground and strike "+target->query_cap_name()+"%^RESET%^%^CRST%^%^C196%^!%^CRST%^", target);
                tell_object(target, "%^RESET%^%^CRST%^%^C196%^Jagged bolts dart along the ground and strike you!%^CRST%^");
                target->cause_typed_damage(target, "torso", damage, "electricity");
            }
            continue;
        }
    }
    return;
}

void charge_weapon(){
    object katerina, room;
    object* attackers;
    katerina = this_object();
    room = environment(katerina);
    
    attackers = katerina->query_attackers();
    if(sizeof(attackers) > 2 * psize) charge = 0;
    else charge = 1;
    
    switch(charge){
        case 0 : // wind, aoe that wipes out fodder
            tell_room(room, "\n%^RESET%^%^CRST%^%^C101%^Raising her %^C153%^cutlass %^RESET%^%^C101%^overhead, Katerina screams out strange words of magic as %^C250%^s%^C255%^ma%^C250%^l%^C255%^l clo%^C250%^u%^C255%^ds %^C101%^begin to %^RESET%^%^C255%^fo%^C250%^r%^C255%^m %^C101%^and %^C250%^t%^C255%^wi%^C250%^s%^C255%^t %^RESET%^%^C101%^about her blade!%^CRST%^\n");
            break;
        case 1 : // lightning, burst damage to players
            tell_room(room, "\n%^RESET%^%^CRST%^%^C101%^Raising her %^C153%^cutlass %^RESET%^%^C101%^overhead, Katerina screams out strange words of magic as %^C228%^el%^C231%^e%^C228%^ctr%^C226%^i%^C231%^c%^C228%^ity p%^C231%^o%^C228%^ps %^C101%^and %^C228%^ar%^C226%^c%^C228%^s %^RESET%^%^C101%^about her blade!%^CRST%^\n");
            break;
    }
    return;
}

void create_dustdevil(){
    object dustdevil, katerina, room;
    int strength;
    katerina = this_object();
    room = environment(katerina);
    
    tell_room(room, "%^RESET%^%^CRST%^%^C101%^The %^C039%^g%^C045%^emstone%^C039%^s %^RESET%^%^C101%^in Katerina's cutlass %^C244%^darken %^C101%^as a strange wind whips about the area, gathering debris and spinning it about in a small %^C144%^wh%^C252%^i%^C255%^r%^C144%^lw%^C101%^i%^C144%^n%^C255%^d!%^CRST%^");
    
    strength = powerlevel * 2;
    
    dustdevil = new(MON"dustdevil");
    dustdevil->set_powerlevel(strength);
    dustdevil->move(room);
    dustdevil->set_property("minion", katerina);
    katerina->add_follower(dustdevil);
    katerina->add_protector(dustdevil);
    return;
}

void check_psize(){
    object* attackers;
    
    attackers = query_attackers();
    psize = sizeof(filter_array(attackers, (: userp($1) :)));
    if(!psize) psize = 1;
    return;
}

void die(){
    object katerina, room, cutlass;
    katerina = this_object();
    room = environment(katerina);
    
    tell_room(environment(katerina), "\n%^RESET%^%^CRST%^%^C101%^Katerina retrieves a small %^C144%^orb %^C101%^from a pocket and hurls it towards the floor! A great %^C250%^f%^C255%^las%^C250%^h %^C101%^blinds you, and when your vision returns... she has escaped.%^CRST%^\n");
    if(!random(4)){
        tell_room(room, "%^RESET%^%^CRST%^%^C101%^It appears Katerina dropped %^C144%^something %^C101%^in her haste.%^CRST%^\n");
        cutlass = new(OBJ"zephyr_talon");
        cutlass->move(room);
    }
    katerina->move("/d/shadowgate/void.c");
    katerina->remove();
    
    return;
}

