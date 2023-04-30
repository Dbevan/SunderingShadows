// Chernobog (11/10/21)
// Feywild Demiplane Boss

#include <std.h>
#include <daemons.h>

inherit MONSTER;

int powerlevel, bloodthirst, counter, handicap;

void create(){
    ::create();
    set_name("redcap");
    set_id(({ "redcap", "ualraig", "Ualraig" }));
    set_short("%^RESET%^%^CRST%^%^C196%^Ua%^C124%^lr%^C125%^aig%^C126%^, the %^C196%^Re%^C124%^dc%^C125%^ap%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C126%^This is a small but mighty man. Do not let his elderly visage and hunchback fool you, for those wiry muscles coupled with an insatiable %^C124%^bloodlust %^C126%^mean this redcap is ready to unleash the full wrath of his %^C196%^fury %^C126%^upon any and all who enter his vicinity. True to his name, he wears a bright %^C196%^red cap %^C126%^upon his head, still %^C124%^sopping wet %^C126%^from his last victim, and he wears %^C058%^heavy %^C244%^ir%^C059%^o%^C244%^n %^C058%^boots %^C126%^on his feet. His leathery lips creak into a wicked little smile, putting on full display his mouthful of protruding, %^RESET%^%^C255%^s%^C250%^h%^C255%^ar%^C250%^p %^C255%^te%^C250%^e%^C255%^t%^C250%^h%^C126%^.%^CRST%^\n");
    set_race("redcap");
    set_body_type("human");
    set_gender("male");
    set_size(1);
    set_alignment(9);
    set_attack_limbs(({ "left hand", "right hand" }));
    set_base_damage_type("slashing");
    set_func_chance(100);
    set_funcs(({ 
        "round_cleanup" }));
    set_property("no death", 1);
    set_property("no sleep", 1);
    set_property("fear_immunity", 1);
    set_property("confusion_immunity", 1);
    set_property("no bows", 1);
    set_property("no tripped", 1);
    set_property("no paralyze", 1);
    set_property("no steal", 1);
    set_property("no dominate", 1);
    set_property("no hold", 1);
    set_property("no_random_treasure", 1);
    set_property("hidden inventory", 1);
    set_property("function and attack", 1);
    set_property("darkvision", 1);
    set_resistance_percent("cold iron", -50);
    set_emotes(20, ({ 
        "%^RESET%^%^CRST%^%^C208%^The redcap %^C202%^flexes %^C208%^his hands, each twitching finger tipped by %^C059%^sha%^C244%^rpe%^C250%^ned %^C255%^tal%^C250%^o%^C255%^ns%^C208%^.%^CRST%^",
        "%^RESET%^%^CRST%^%^C196%^Blood %^C208%^drips from the soaked fabric of the redcap's %^C124%^ha%^C125%^t%^C208%^.%^CRST%^", }), 0);
    set_monster_feats(({
        "weapon training",
        "weapon focus",
        "greater weapon focus",
        "epic weapon focus", }));
    set_powerlevel(1);
    bloodthirst = 1;
    counter = 1;
    handicap = 0;
}

void set_powerlevel(int pwrlvl){
    powerlevel = pwrlvl;
    set_hd((powerlevel * 10), 0);
    set_class("fighter");
    set_mlevel("fighter", (powerlevel * 10));
    set_damage((powerlevel * 2), 10);
    set_attacks_num(powerlevel + 2);
    set_overall_ac(-((powerlevel * 10) + 5));
    set_max_hp(powerlevel * powerlevel * 300);
    set_hp(query_max_hp());
    set_new_exp((powerlevel * 10), "high");
    set_skill("athletics", (powerlevel * 10));
    tell_room(environment(this_object()), "%^RESET%^%^CRST%^%^C124%^The %^C196%^redcap %^C124%^seems to %^C220%^g%^C226%^l%^C220%^o%^C214%^w %^RESET%^%^C124%^as power manifests around it.%^CRST%^");
    return;
}

void round_cleanup(){
    if(bloodthirst > 10) bloodthirst = 10;
    switch(counter){
        case 1 :
            if(powerlevel > 4){
                bloody_slash();
            }
            bloodthirst++;
            break;
        case 2 :
            bloody_slash();
            break;
        case 3 :
            bloody_heal();
            break;
        default :
            tell_room(ETO, "Something is wrong, please contact Chernobog.");
            break;
    }
    if(bloodthirst > 1) bloodthirst--;
    counter++;
    if(counter > 3) counter = 1;
    return;
}

void bloody_slash(){
    int i;
    object* attackers;
    object targ;
    
    for(i=0; i < powerlevel; i++){
        attackers = query_attackers();
        targ = attackers[random(sizeof(attackers))];
        tell_room(ETO, "%^RESET%^%^CRST%^\n%^C208%^The redcap lunges forward, his talons outstretched!%^CRST%^");
        if(userp(targ)){
            if(SAVING_THROW_D->reflex_save(targ, ((powerlevel * 15) + bloodthirst + handicap))){
                tell_room(ETO, "%^RESET%^%^CRST%^%^C118%^"+targ->QCN+"%^RESET%^%^CRST%^%^C118%^ barely avoids the razor claws of the redcap!%^CRST%^\n", targ);
                tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You barely avoid the razor claws of the redcap!%^CRST%^\n");
                bloodthirst++;
                handicap++;
                return;
            }
            else{
                tell_room(ETO, "%^RESET%^%^CRST%^%^C196%^It slashes into "+targ->QCN+"%^RESET%^%^CRST%^%^C196%^, spraying blood into the air!%^CRST%^\n", targ);
                tell_object(targ, "%^RESET%^%^CRST%^%^C196%^It slashes into you, spraying blood into the air!%^CRST%^\n");
                targ->set_property("rend", powerlevel);
                bloodthirst += 3;
                tell_room(ETO, "%^RESET%^%^CRST%^%^C126%^The redcap seems to move faster and gain strength from all this %^C196%^blood%^C126%^...%^CRST%^");
                targ->cause_typed_damage(targ, "torso", bloodthirst * powerlevel * 5, "slashing");
                handicap--;
                return;
            }
        }
        tell_room(ETO, "%^RESET%^%^CRST%^%^C196%^It slashes into "+targ->QCN+"%^RESET%^%^CRST%^%^C196%^, spraying blood into the air!%^CRST%^");
        targ->set_property("rend", powerlevel);
        targ->cause_typed_damage(targ, "torso", bloodthirst * powerlevel * 5, "slashing");
        bloodthirst++;
        tell_room(ETO, "%^RESET%^%^CRST%^%^C126%^The redcap seems to move faster and gain strength from all this %^C196%^blood%^C126%^...%^CRST%^");
        continue;
    }
    return;
}

void bloody_heal(){
    int i, healing;
    object* attackers;
    object targ;
    
    attackers = query_attackers();
    for(i=0; i < sizeof(attackers); i++){
        targ = attackers[i];
        if(attackers[i]->query_property("rend")){
            healing = targ->query_property("rend");
            TO->cause_typed_damage(TO, "torso", healing * powerlevel * bloodthirst * 10, "positive energy");
            targ->remove_property("rend");
            tell_room(ETO, "%^RESET%^%^CRST%^\n%^C208%^The redcap swings his %^C124%^st%^C125%^ai%^C124%^ne%^C125%^d c%^C124%^ap %^C208%^in the air towards "+targ->QCN+"%^RESET%^%^CRST%^%^C208%^, the %^C196%^blood %^C208%^from their wounds flying towards the %^C124%^ha%^C125%^t %^C208%^and %^C196%^soaking %^C208%^it!%^CRST%^\n", targ);
            tell_object(targ, "%^RESET%^%^CRST%^\n%^C208%^The redcap swings his %^C124%^st%^C125%^ai%^C124%^ne%^C125%^d c%^C124%^ap %^C208%^in the air towards you, the %^C196%^blood %^C208%^from your wound flying towards the %^C124%^ha%^C125%^t %^RESET%^%^C208%^and %^C196%^soaking %^C208%^it!%^CRST%^\n");
        }
        continue;
    }
    if((random(5) < 1) && (powerlevel > 4)){
        targ = attackers[random(sizeof(attackers))];
        tell_room(ETO, "%^RESET%^%^CRST%^\n%^C208%^The redcap leaps towards "+targ->QCN+"%^RESET%^%^CRST%^%^C208%^, trying to stomp down on their feet with his %^C244%^heavy iron boots%^C208%^!%^CRST%^", targ);
        tell_object(targ, "%^RESET%^%^CRST%^\n%^C208%^The redcap leaps towards you, trying to stomp down on your feet with his %^C244%^heavy iron boots%^C208%^!%^CRST%^");
        if(SAVING_THROW_D->reflex_save(targ, (powerlevel * 14) + bloodthirst)){
            tell_room(ETO, "%^RESET%^%^CRST%^%^C118%^"+targ->QCN+"%^RESET%^%^CRST%^%^C118%^ dodges to the side!%^CRST%^\n", targ);
            tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You dodge to the side!%^CRST%^\n");
        }
        else{
            tell_room(ETO, "%^RESET%^%^CRST%^%^C196%^"+targ->QCN+"%^RESET%^%^CRST%^%^C196%^ isn't fast enough, and the redcap crushes their foot!%^CRST%^\n", targ);
            tell_object(targ, "%^RESET%^%^CRST%^%^C196%^You aren't fast enough, and the redcap crushes your foot!%^CRST%^\n");
            targ->cause_typed_damage(targ, "torso", bloodthirst * powerlevel * 5, "bludgeoning");
            targ->set_tripped(random(2), "%^RESET%^%^CRST%^%^C124%^Your foot is still throbbing in pain!%^CRST%^");
        }
    }
    return;
}

void die(object ob){
    object room, pet;
    room = environment(this_object());
    
    tell_room(room, "%^RESET%^%^CRST%^%^C059%^The %^C196%^re%^C124%^dc%^C125%^ap %^C059%^collapses, reaching out with a %^C124%^viciously %^C255%^ta%^C250%^l%^C255%^one%^C250%^d %^RESET%^%^C059%^hand before dying.%^CRST%^");
    if(!random(10)){
        tell_room(room, "\n%^RESET%^%^CRST%^%^C126%^The nearby %^C036%^ve%^C040%^ge%^C046%^t%^C036%^at%^C040%^io%^C046%^n %^RESET%^%^C126%^rustles for a moment, breaking for a %^C196%^b%^C160%^ri%^C255%^g%^C160%^ht %^C196%^re%^C160%^d m%^C255%^u%^C196%^s%^C160%^hr%^C196%^o%^C255%^o%^C196%^m %^RESET%^%^C126%^that meanders closer on its animated %^C144%^stalk%^C126%^.%^CRST%^");
        pet = new("/d/common/obj/pets/loot_pets/shroomling");
        pet->move(room);
    }
    ::die();
}

