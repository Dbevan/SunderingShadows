// Chernobog (9/27/21)
// Offal Marsh Demiplane Boss

#include <std.h>
#include <daemons.h>
#include <magic.h>

inherit MONSTER;

int powerlevel, buffed, handicap;

void create(){
    ::create();
    set_name("plague lord");
    set_id(({ "elemental", "plague lord", "filth elemental", "stercus", "Stercus", "lord" }));
    set_short("%^RESET%^%^CRST%^%^C118%^St%^C112%^er%^C106%^cus%^C144%^, the %^C143%^P%^C137%^la%^C131%^gue %^C143%^L%^C137%^o%^C131%^rd%^CRST%^");
    set_long("%^RESET%^%^CRST%^%^C144%^This heap of %^C118%^f%^C112%^i%^C106%^l%^C118%^t%^C112%^h %^C144%^and %^C143%^r%^C137%^o%^C131%^t %^C144%^rises in a vaguely humanoid upper body, with thick flabby arms extending from a %^C106%^corpulent %^C144%^torso and a many-jowled face sitting squat between %^C157%^o%^C151%^oz%^C145%^in%^C157%^g %^C144%^shoulders. A series of discarded %^C255%^bon%^C252%^e%^C255%^s %^C144%^and %^C130%^branches %^C144%^protrude from the top of his head like some debased and foul %^C184%^c%^C190%^r%^C226%^o%^C190%^w%^C184%^n%^RESET%^%^C144%^. From his hips, the rest of his fetid form is a pile of sludge, oozing a trail of slime and muck as he crawls about.%^CRST%^\n");
    set_race("elemental");
    set_body_type("human");
    set_gender("male");
    set_size(3);
    set_alignment(8);
    set_base_damage_type("bludgeoning");
    set_func_chance(75);
    set_funcs(({ 
        "flies_fun",
        "flies_fun",
        "flies_fun",
        "flies_fun",
        "wave_fun" }));
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
    set_property("cast and attack", 1);
    set_property("darkvision", 1);
    set_property("disease immunity", 1);
    set_property("poison immunity", 1);
    set_spell_chance(75);
    set_emotes(20, ({ 
        "%^RESET%^%^CRST%^%^C118%^St%^C112%^er%^C106%^cus%^C144%^ shudders, the wave of shifting %^C143%^f%^C137%^i%^C131%^l%^C143%^t%^C137%^h %^C144%^cascading across his %^C106%^bl%^C112%^o%^C106%^ate%^C112%^d %^C144%^form.%^CRST%^",
        "%^RESET%^%^CRST%^%^C144%^Clouds of %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s %^C144%^swarm about %^C118%^St%^C112%^er%^C106%^cus%^C144%^, mirrored on his body by thousands of wriggling %^C255%^ma%^C252%^g%^C255%^go%^C252%^t%^C255%^s %^C144%^feasting on rot.%^CRST%^", }), 0);
    set_monster_feats(({
        "spell focus",
        "spell penetration",
        "greater spell penetration",
        "spell power",
        "perfect caster", }));
    set_powerlevel(1);
    set_resistance_percent("acid", 125);
    set_property("swarm", 1);
    handicap = 0;
    set("elementalist", "acid");
}

void set_powerlevel(int pwrlvl){
    powerlevel = pwrlvl;
    set_hd((powerlevel * 10), 0);
    set_class("mage");
    set_mlevel("mage", (powerlevel * 10));
    set_guild_level("mage", (powerlevel * 10));
    set_class("inquisitor");
    set_mlevel("inquisitor", (powerlevel * 10));
    set_guild_level("inquisitor", (powerlevel * 10));
    set_damage((powerlevel * 2), 10);
    set_attacks_num(powerlevel);
    set_overall_ac(-((powerlevel * 10) + 5));
    set_max_hp(powerlevel * powerlevel * 400);
    set_hp(query_max_hp());
    set_new_exp((powerlevel * 10), "high");
    
    switch(powerlevel){
        case 1..2:
            set_spells(({
                "burning hands",
                "acid arrow",
                "contagion",
                }));
                break;
        case 3:
            set_spells(({
                "mass fester",
                "chill touch",
                "acid arrow",
                "contagion",
                }));
                break;
        case 4:
            set_spells(({
                "mass fester",
                "chill touch",
                "fireball",
                "contagion",
                }));
                break;
        case 5:
            set_spells(({
                "mass fester",
                "acid fog",
                "fireball",
                "contagion",
                }));
                break;
    }
    return;
}

void init(){
    ::init();
    if((!buffed) && (powerlevel > 4)){
        new("/cmds/spells/e/_elemental_aura.c")->use_spell(this_object(), "acid", (powerlevel * 10), 100, "mage");
        buffed = 1;
    }
}

void flies_fun(){
    object room, ob, ob2, targ, stercus;
    object* attackers;
    
    stercus = this_object();
    room = environment(stercus);
    attackers = query_attackers();
    targ=attackers[random(sizeof(attackers))];
    
    tell_room(room, "%^RESET%^%^CRST%^\n%^C118%^St%^C112%^er%^C106%^cus%^C130%^ opens his maw wide, spewing out a cloud of %^C244%^f%^C248%^l%^C244%^i%^C250%^e%^C246%^s%^C130%^!");
    
    if(!userp(targ)){
        if(powerlevel > 4){
            tell_room(room, "%^RESET%^%^CRST%^%^C196%^Several swarms erupt from his bloated lips!\n%^CRST%^");
            ob2 = new("/d/common/obj/daily/mon/plaguelord_flies");
            ob2->move(room);
            ob2->set_powerlevel(powerlevel);
            stercus->add_follower(ob2);
            stercus->add_protector(ob2);
        }
    }
    
    ob = new("/d/common/obj/daily/mon/plaguelord_flies");
    ob->move(room);
    ob->set_powerlevel(powerlevel);
    stercus->add_follower(ob);
    stercus->add_protector(ob);
    return;
}

void wave_fun(){
    object* attackers;
    object targ, room;
    int i, count;
    
    room = environment(this_object());
    attackers = query_attackers();
    count = sizeof(attackers);
    
    tell_room(ETO, "%^RESET%^%^CRST%^\n%^C118%^St%^C112%^er%^C106%^cus%^C106%^ raises his horrid limbs up, and the corrupt turgid waters gather behind him in a %^C143%^ma%^C137%^ss%^C131%^iv%^C143%^e t%^C137%^id%^C131%^al %^C143%^wa%^C137%^ve %^C106%^of %^C112%^filth %^C106%^and %^C118%^putrescence%^C106%^. It rushes forward with his gesture, slamming into everything in sight!%^CRST%^");
    
    for(i = 0; i < count; i++){
        targ = attackers[i];
        if(!userp(targ)){
            tell_room(room, "%^RESET%^%^CRST%^%^C106%^"+targ->QCN+"%^RESET%^%^CRST%^%^C106%^ is %^C124%^washed away %^C106%^in a tidal wave of %^C143%^f%^C137%^i%^C131%^l%^C143%^t%^C137%^h%^C106%^!%^CRST%^");
            targ->die();
            continue;
        }
        if(powerlevel > 4){
            if(SAVING_THROW_D->reflex_save(targ, (powerlevel * 15) + handicap)){
                tell_object(targ, "%^RESET%^%^CRST%^%^C118%^You manage to keep your footing against the foul tidal wave!\n%^CRST%^");
                tell_room(room, "%^RESET%^%^CRST%^%^C118%^"+targ->QCN+"%^RESET%^%^CRST%^%^C118%^ manages to keep their footing against the foul tidal wave!\n%^CRST%^", targ);
                handicap++;
                continue;
            }
            else{
                tell_object(targ, "%^RESET%^%^C106%^The %^C143%^f%^C137%^o%^C131%^u%^C143%^l %^C137%^w%^C131%^a%^C143%^v%^C137%^e %^C106%^slams into you, throwing you off your feet!\n");
                tell_room(room, "%^RESET%^%^CRST%^%^C106%^"+targ->QCN+"%^RESET%^%^CRST%^%^C106%^ is thrown to the ground by the force of the %^C143%^f%^C137%^o%^C131%^u%^C143%^l %^C137%^w%^C131%^a%^C143%^v%^C137%^e%^C106%^!\n", targ);
                targ->set_tripped(roll_dice(1, 3), "%^C130%^You keep slipping on the %^C118%^sl%^C112%^i%^C106%^m%^C118%^y f%^C112%^i%^C106%^l%^C118%^th%^C106%^!");
                handicap--;
                continue;
            }
        }
        continue;
    }
    return;
}

void die(object ob){
    object room, pet;
    room = environment(this_object());
    
    tell_room(room, "%^RESET%^%^CRST%^%^C059%^The %^C143%^ele%^C137%^men%^C131%^tal %^C143%^f%^C137%^o%^C131%^rm %^RESET%^%^C059%^sinks, melting away to join the surrounding %^C118%^b%^C112%^o%^C106%^g%^C059%^.%^CRST%^");
    if(!random(10)){
        tell_room(room, "\n%^RESET%^%^CRST%^%^C131%^Not all of it seems to have dissipated, as %^C118%^bu%^C112%^lbo%^C106%^us %^C112%^o%^C118%^o%^C112%^z%^C106%^e %^RESET%^%^C131%^seems to move about with a disgusting sucking sound.%^CRST%^");
        pet = new("/d/common/obj/pets/loot_pets/disgusting_oozeling");
        pet->move(room);
    }
    ::die();
}

