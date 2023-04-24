// Chernobog (11/15/21)
// Deep Caverns Demiplane Boss

#include <std.h>
#include <daemons.h>

inherit MONSTER;

int powerlevel, counter, handicap;

void create(){
    ::create();
    set_name("grue");
    set_id(({ "grue", "Grue", "vrudril", "Vrudril", "V'rud'ril", "v'rud'ril", "aberration" }));
    set_short("%^RESET%^%^CRST%^%^C037%^V'%^C030%^ru%^C024%^d'r%^C023%^il%^%^C019%^, the %^C037%^G%^C030%^r%^C024%^u%^C023%^e%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C024%^It is hard to discern what you are looking at, as the %^C059%^da%^C244%^r%^C059%^kne%^C244%^s%^C059%^s %^C024%^writhing about this creature seeks to %^C030%^obfuscate %^C024%^and interrupt any inspection. You get a sense of %^C019%^size%^C024%^... this thing is massive, and there seems to be a scrape of %^C250%^cla%^C244%^w%^C250%^s %^C024%^on stone as it moves.%^CRST%^\n");
    set_race("grue");
    set_body_type("quadruped");
    set_gender("neuter");
    set_size(3);
    set_alignment(9);
    add_limb("teeth", "head", 0, 0, 0);
    set_attack_limbs(({ "teeth" }));
    set_base_damage_type("piercing");
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
    set_property("cast and attack", 1);
    set_property("darkvision", 1);
    set_resistance_percent("void", 125);
    set_true_seeing(1);
    set_emotes(15, ({ 
        "%^RESET%^%^CRST%^%^C024%^A low %^C019%^growl %^C024%^seems to emanate from within the %^C059%^sw%^C244%^i%^C059%^rli%^C244%^n%^C059%^g da%^C244%^r%^C059%^kne%^C244%^s%^C059%^s%^C024%^.%^CRST%^", }), 0);
    set_monster_feats(({
        "spell focus",
        "spell penetration",
        "greater spell penetration",
        "spell power",
        "perfect caster", }));
    set_spell_chance(50);
    set_spells(({
        "interstellar void",
        "tremor",}));
    set_powerlevel(1);
    counter = 1;
    handicap = 0;
}

void set_powerlevel(int pwrlvl){
    powerlevel = pwrlvl;
    set_hd((powerlevel * 10), 0);
    set_class("fighter");
    set_mlevel("fighter", (powerlevel * 10));
    set_class("oracle");
    set_mlevel("oracle", (powerlevel * 10));
    set_guild_level("oracle", (powerlevel * 10));
    set_mystery("heavens");
    set_damage((powerlevel * 3), 10);
    set_attacks_num(powerlevel);
    set_overall_ac(-((powerlevel * 10) + 5));
    set_max_hp(powerlevel * powerlevel * 350);
    set_hp(query_max_hp());
    set_new_exp((powerlevel * 10), "high");
    set_skill("athletics", (powerlevel * 10));
    return;
}

void round_cleanup(){
    switch(counter){
        case 1 :
            darkness_spread();
            warning();
            break;
        case 2 :
            slam_attack();
            break;
        case 3 :
            darkness_spread();
            warning();
            break;
        case 4 :
            if(powerlevel > 4){
                slam_attack();
            }
            darkness_heal();
            break;
        default :
            tell_room(ETO, "Something is wrong, please contact Chernobog.");
            break;
    }
    counter++;
    if(counter > 4) counter = 1;
    return;
}

void warning(){
    object room;
    
    room = environment(this_object());
    
    if(room->query_property("light") < -2)
        tell_room(room, "\n%^RESET%^%^CRST%^%^C030%^Those orbs seem to be radiating %^C059%^darkness%^C030%^. You should probably %^C037%^snuff %^C030%^them.%^CRST%^\n");
    return;
}

void darkness_spread(){
    object ob, room;
    
    room = environment(this_object());
    
    if(room->query_property("light") <= -10) return;
    ob = new("/d/common/obj/daily/obj/grue_orb");
    ob->move(room);
    tell_room(room, "%^RESET%^%^CRST%^%^C244%^The grue shudders... and coughs out an %^C059%^orb of darkness %^C244%^that drifts away!\n\n%^RESET%^%^C037%^The %^C030%^dark%^C024%^ness inte%^C023%^nsif%^C019%^ies...\n%^CRST%^");
    return;
}

void darkness_heal(){
    int healing, darkness;
    object room, grue;
    
    grue = this_object();
    room = environment(grue);
    
    darkness = (room->query_property("light") * -1);
    if(darkness < 0) return;
    healing = (powerlevel * powerlevel * 35 * darkness);
    grue->cause_typed_damage(grue, "torso", healing, "positive energy");
    tell_room(room, "%^RESET%^%^CRST%^\n%^C060%^The grue shivers, shaking its massive body as the %^C059%^darkness %^C060%^seeps into it and its %^C062%^wounds heal%^C060%^.%^CRST%^\n");
    return;
}

void slam_attack(){
    int i, darkness;
    object* attackers;
    object targ, room;
    
    room = environment(this_object());
    
    darkness = (room->query_property("light") * -1);
    if(darkness < 0) darkness = 0;
    
    for(i=0; i < powerlevel; i++){
        attackers = query_attackers();
        targ = attackers[random(sizeof(attackers))];
        tell_room(room, "%^RESET%^%^CRST%^\n%^C093%^With a piercing screech, the grue charges forward!%^CRST%^");
        if(userp(targ)){
            if(SAVING_THROW_D->reflex_save(targ, ((powerlevel * 15) + darkness + handicap))){
                tell_room(room, "%^RESET%^%^CRST%^%^C118%^"+targ->query_cap_name()+"%^RESET%^%^CRST%^%^C118%^ barely jumps out of the way!%^CRST%^\n", targ);
                tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You barely jump out of the way!%^CRST%^\n");
                handicap++;
                return;
            }
            else{
                tell_room(room, "%^RESET%^%^CRST%^%^C196%^It rushes, slamming its body into "+targ->query_cap_name()+"%^RESET%^%^CRST%^%^C196%^!%^CRST%^\n", targ);
                tell_object(targ, "%^RESET%^%^CRST%^%^C196%^It rushes, slamming its body into you!%^CRST%^\n");
                targ->cause_typed_damage(targ, "torso", roll_dice((darkness * 5), (powerlevel * 2)), "bludgeoning");
                targ->set_paralyzed(2, "%^RESET%^%^CRST%^%^C125%^That last hit left you winded!%^CRST%^");
                handicap--;
                return;
            }
        }
        if(!objectp(targ)) continue;
        tell_room(ETO, "%^RESET%^%^CRST%^%^C124%^With a quick snap of its mouth, it swallows "+targ->query_cap_name()+" %^RESET%^%^CRST%^%^C124%^whole!%^CRST%^");
        targ->die();
        attackers = query_attackers();
        continue;
    }
    return;
}

void die(object ob){
    object room, pet;
    room = environment(this_object());
    
    tell_room(room, "%^RESET%^%^CRST%^%^C059%^The %^C023%^dar%^C024%^kness %^C030%^rece%^C037%^des%^C059%^, and whatever %^C124%^lurked %^C059%^within seems to dissolve in the light.%^CRST%^");
    if(!random(10)){
        tell_room(room, "\n%^RESET%^%^CRST%^%^C023%^A small patch of %^C059%^darkness %^C023%^seems to have persisted, and it lets out a strange mewling noise...%^CRST%^");
        pet = new("/d/common/obj/pets/loot_pets/baby_grue");
        pet->move(room);
    }
    ::die();
}

