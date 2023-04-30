// Chernobog (9/22/21)
// Clockwork Demiplane Boss

#include <std.h>
#include <daemons.h>

inherit MONSTER;

int powerlevel, handicap;

void create(){
    ::create();
    set_name("steam golem");
    set_id(({ "golem", "steam golem", "ix", "ix axos", "Ix", "Ix Axos" , "Nine" }));
    set_short("%^RESET%^%^CRST%^%^C184%^I%^C178%^x %^C184%^A%^C178%^x%^C172%^os%^C244%^, the %^C250%^St%^C248%^eam%^C246%^wo%^C244%^rk %^C250%^G%^C248%^o%^C246%^l%^C244%^em%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C136%^This is a %^C250%^st%^C248%^eam%^C246%^wo%^C244%^rk %^C250%^g%^C248%^o%^C246%^l%^C244%^em%^C136%^, vaguely humanoid in shape with %^C250%^b%^C252%^l%^C255%^ad%^C252%^e%^C250%^d %^C136%^arms and a giant %^C250%^b%^C255%^u%^C250%^z%^C255%^z %^C250%^s%^C255%^a%^C250%^w %^C136%^scything about on a support arm. %^C255%^Steam %^RESET%^%^C136%^is vented periodically from between the joints of its limbs. The %^C172%^b%^C178%^r%^C184%^a%^C178%^s%^C172%^s p%^C178%^l%^C184%^at%^C178%^e%^C172%^s %^C136%^covering its body are wet with condensation and gleam in the light.%^CRST%^\n");
    set_race("construct");
    set_body_type("human");
    add_limb("left hand blade", "left hand", 0, 0, 0);
    add_limb("right hand blade", "right hand", 0, 0, 0);
    add_limb("buzz saw", "torso", 0, 0, 0);
    set_gender("neuter");
    set_size(2);
    set_alignment(2);
    set_attack_limbs(({ "left hand blade", "right hand blade", "buzz saw" }));
    set_base_damage_type("slashing");
    set_func_chance(75);
    set_funcs(({ 
        "steam_fun",
        "buzz_fun",
        "buzz_fun",
        "buzz_fun" }));
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
    set_true_seeing(1);
    set_emotes(20, ({ 
        "%^RESET%^%^CRST%^%^C130%^A clinking, clattering noise rises up from the %^C250%^g%^C248%^o%^C246%^l%^C244%^em %^C130%^as it shudders, before blasting out a %^C255%^puff %^C250%^of %^C255%^steam%^C130%^.%^CRST%^",
        "%^RESET%^%^CRST%^%^C130%^The %^C250%^b%^C255%^u%^C250%^z%^C255%^z %^C250%^s%^C255%^a%^C250%^w %^C130%^on the golem lets out a %^C214%^kee%^C220%^ni%^C226%^ng %^C214%^wa%^C220%^i%^C226%^l %^C130%^as it speeds up! ..then slows back down a moment later.%^CRST%^", }), 0);
    set_monster_feats(({
        "blind fight",
        "weapon training",
        "weapon focus",
        "greater weapon focus",
        "epic weapon focus", }));
    set_powerlevel(1);
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
    set_skill("athletics", (powerlevel * 10) + 10);
    tell_room(environment(this_object()), "%^RESET%^%^CRST%^%^C130%^The %^C250%^g%^C248%^o%^C246%^l%^C244%^em %^C130%^shudders as it reconfigures itself.%^CRST%^");
    return;
}

void steam_fun(object targ){
    object room = environment(this_object());
    
    tell_room(room, "%^RESET%^%^CRST%^\n%^C202%^The %^C250%^g%^C248%^o%^C246%^l%^C244%^em %^C202%^shudders, vents opening along its sides to expel huge clouds of %^C255%^steam %^C202%^towards %^C230%^"+targ->QCN+"%^RESET%^%^CRST%^%^C255%^!%^CRST%^", targ);
    tell_object(targ, "%^RESET%^%^CRST%^\n%^C202%^The %^C250%^g%^C248%^o%^C246%^l%^C244%^em %^C202%^shudders, vents opening along its sides to expel huge clouds of %^C255%^steam %^C202%^towards %^C230%^you%^C255%^!%^CRST%^");

    if(userp(targ)){
        if(powerlevel > 4){
            if(SAVING_THROW_D->reflex_save(targ, (powerlevel * 15) + handicap)){
                tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You manage to dive away, avoiding the hot blast!%^CRST%^\n");
                tell_room(room, "%^RESET%^%^CRST%^%^C118%^"+targ->QCN+"%^RESET%^%^CRST%^%^C118%^ manages to dive away!%^CRST%^\n", targ);
                handicap++;
                return;
            }
            tell_object(targ, "%^RESET%^%^CRST%^%^C196%^A blast of superheated %^C255%^steam %^C196%^catches you in the face, searing and %^C214%^bl%^C220%^i%^C226%^nd%^C220%^i%^C214%^ng %^C196%^you!%^CRST%^\n");
            tell_room(room, "%^RESET%^%^CRST%^%^C230%^"+targ->QCN+"%^RESET%^%^CRST%^%^C196%^ catches a blast of %^C255%^steam %^C196%^in the face!%^CRST%^\n", targ);
            targ->cause_typed_damage(targ, "head", roll_dice(powerlevel,10), "fire");   
            targ->set_temporary_blinded(roll_dice(1,powerlevel));
            handicap--;
            return;
        }
        return;
    }
    else{
        tell_room(room, "%^RESET%^%^CRST%^%^C124%^"+targ->QCN+" %^RESET%^%^CRST%^%^C124%^is engulfed in superheated steam and is incinerated!%^CRST%^\n");
        targ->die();
        return;
    }
}

void buzz_fun(object targ){
    int i;
    object room, *attackers;
    
    attackers = query_attackers();
    room = environment(this_object());
    
    tell_room(room, "%^RESET%^%^CRST%^\n%^C202%^The %^C250%^g%^C248%^o%^C246%^l%^C244%^em %^C202%^shifts, sending the buzz saw down in a low arc!%^CRST%^");

    for(i=0; i < powerlevel; i++){
        targ=attackers[random(sizeof(attackers))];
        if(userp(targ)){
            if(SAVING_THROW_D->reflex_save(targ, (powerlevel * 14) + handicap)){
                tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You dodge, and the buzz saw blade only grazes you!%^CRST%^\n");
                tell_room(room, "%^RESET%^%^CRST%^%^C118%^"+targ->QCN+"%^RESET%^%^CRST%^%^C118%^ dodges out of the way!%^CRST%^\n", targ);
                targ->cause_typed_damage(targ, "torso", roll_dice(powerlevel, 10), "slashing");
                handicap++;
                return;
            }
        
            tell_object(targ, "%^RESET%^%^CRST%^%^C196%^The buzz saw catches you, digging in to %^C124%^rip %^C196%^and %^C124%^tear!%^CRST%^\n");
            tell_room(room, "%^RESET%^%^CRST%^%^C196%^The saw blade digs into %^C230%^"+targ->QCN+"%^RESET%^%^CRST%^%^C196%^, %^C124%^ripping %^C196%^and %^C124%^tearing%^C196%^!%^CRST%^\n", targ);
            targ->cause_typed_damage(targ, "torso", roll_dice((powerlevel * 2),10), "slashing");
            targ->set_property("rend", powerlevel);
            handicap--;
            return;
        }
        else{
            if(!objectp(targ)) continue;
            tell_room(room, "%^RESET%^%^CRST%^%^C124%^"+targ->QCN+" %^RESET%^%^CRST%^%^C124%^is torn apart by the whirling buzz saw!%^CRST%^");
            targ->die();
            attackers = query_attackers();
            continue;
        }
    }
    return;
}

void die(object ob){
    object room, pet;
    room = environment(this_object());
    
    tell_room(room, "%^RESET%^%^CRST%^%^C059%^The %^C220%^go%^C214%^le%^C220%^m %^C059%^falls apart into a pile of %^C244%^broken parts %^C059%^and %^C244%^cogs%^C059%^.");
    if(!random(10)){
        tell_room(room, "\n%^RESET%^%^CRST%^%^C136%^All the commotion seems to have attracted one of the denizens of the demiplane.%^C059%^.");
        pet = new("/d/common/obj/pets/loot_pets/clockwork_hummingbird");
        pet->move(room);
    }
    ::die();
}

