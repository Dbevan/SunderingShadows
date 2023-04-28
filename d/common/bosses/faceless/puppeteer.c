/*
  puppeteer.c
  
  Faceless One avatar boss.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>
#include <magic.h>
#include "puppet.h"

#define ENRAGE_TIMER 300

inherit "/d/common/bosses/avatar_boss.c";

int coreparty = 4;
int ticker = 0;
int enrage;
int psize;
int madness = 0;
int wave;
int buffed = 0;

mapping checkpoints = ([
                        "melee"   : 0,
                        "spells"  : 0,
                        "shadows" : 0,
                        "madness" : 0,
                        "dopple"  : 0,
                      ]);

object puppet, dopple;

//Prototyping
void madness_check(object room);
void melee(object room);
void spells(object room);
void shadows(object room);
void madness(object room);
void dopple(object room);
void enrage(object room);

int set_puppet(object ob) { puppet = ob; return 1; }
int remove_puppet()       { puppet = 0;  return 1; }

void create()
{
    object ob;
    ::create();

    set_name("puppeteer");
    set_id(({ "puppeteer", "demigod", "shadow god", "outsider" }));
    set_short("%^C091%^The%^C128%^ %^C097%^P%^C103%^u%^C109%^p%^C103%^p%^C097%^e%^C091%^teer%^CRST%^");
    set_long("Before you stands a creature that, by all reasoning, belongs in the Plane of Shadows. It is a mass of writhing black tentacles, seemingly formed into a ball that never seems to stop twisting and rolling in the air. It feels about itself, almost frantically, perhaps searching for the mental energies of those around it.");
    
    add_limb("tendril one", "", 0, 0, 0);
    add_limb("tendril two", "", 0, 0, 0);
    add_limb("tendril three", "", 0, 0, 0);
    add_limb("tendril four", "", 0, 0, 0);
    add_limb("mouth", "", 0, 0, 0);
    
    set_gender("neuter");
    set_race("outsider");
    set_size(2);
    set_hd(75, 9);
    set_class("oracle");
    set_class("shadow_adept");
    set_mlevel("oracle", 75);
    set_mlevel("shadow_adept", 10);
    set("base_class", "oracle");
    set_mystery("shadow");
    set_alignment(8);
    //set_limbs(({ "mouth", "tendril one", "tendril two", "tendril three", "tendril four" }));
    set_attack_limbs(({ "mouth", "tendril one", "tendril two", "tendril three", "tendril four" }));
    set_hit_funcs(([ "mouth" : (: this_object(), "bite" :), "tendril one" : (: this_object(), "tendril" :),"tendril two" : (: this_object(), "tendril" :),"tendril three" : (: this_object(), "tendril" :),"tendril four" : (: this_object(), "tendril" :) ]));
    set_base_damage_type("void");
    set_damage(16, 7);
    set_attacks_num(5);
    set_property("no death", 1);
    set_property("no bows", 1);
    set_property("no knockdown", 1);
    set_property("no trip", 1);
    set_property("no blind", 1);
    set_property("no tripped", 1);
    set_property("no steal", 1);
    set_property("no dominate", 1);
    set_property("no hold", 1);
    set_property("no paralyze", 1);
    set_property("confusion_immunity");
    set_property("hidden inventory", 1);
    set_property("cast and attack", 1);
    set_property("function and attack", 1);
    set_property("full attacks", 1);
    
    set_stats("strength", 16);
    set_stats("constitution", 16);
    set_stats("charisma", 30);
    set_stats("wisdom", 18);
    
    //set_func_chance(102);
    set_skill("perception", 50);
    set_mob_magic_resistance("high");
    set_overall_ac(-70);
    set_sight_bonus(10);
    set_max_hp(100000);
    set_hp(query_max_hp());
    set_property("add kits", 40);
    set_new_exp(75, "boss");
    set_property("add quest", "%^C244%^Faced the P%^C124%^u%^C160%^p%^C196%^p%^C197%^e%^C244%^teer!%^CRST%^");
    set_property("quest exp", 12000000);
    add_money("gold", random(100000) + (80000));
    add_money("platinum", random(20000) + (40000));
    set_emotes(30, ({ "",
                      "",
                      "", }), 20);
    set_monster_feats(({
        "spell focus",
        "spell penetration",
        "greater spell penetration",
        "spell power",
        "greater spell power",
        "perfect caster",
        "blind fight",
        "force of personality",
    }));
    set_spells(({ "shadow nova",
                  "fear",
                  "wave of pain",
                  "waves of exhaustion",
                  "powerword blind",
                  "shadow vortex",
                  "insanity",
                  "entropic storm",
                  "oppression",
                  "crushing despair",
                  "weird",
                  "black tentacles",
                  "overwhelming presence", }));
                  
    set_spell_chance(0);
    set_resistance_percent("void", 100);
    add_money("platinum", 100000 + random(20000));
    set_property("no paralyze", 1);
    set_acquired_template("shade");
    set_sight_bonus(5);
    command("shadow_apotheosis");
}

void set_paralyzed(int time, string message)
{
    if (!objectp(this_object()))
    {
        return;
    }
    if (!objectp(environment(this_object()))) 
    {
        return;
    }
    tell_room(environment(this_object()), "The Puppeteer seems unaffected by paralysis!");
    return;
}

void init()
{
    object player, room;
    
    ::init();
    
    player = this_player();
    player && room = environment(this_object());
    
    if(!player || !room)
        return;
    
    if (wizardp(player) || player->query_true_invis()) {
        return;
    }
    
    if(!buffed)
    {
        new("/cmds/spells/u/_unholy_aura.c")->use_spell(this_object(), 0, 70, 100, "mage");
        new("/cmds/spells/s/_shadow_body.c")->use_spell(this_object(), 0, 70, 100, "mage");
        new("/cmds/spells/s/_shadowform.c")->use_spell(this_object(), 0, 70, 100, "mage");
        buffed = 1;
    }        
}

void boss_death_event()
{
    object *attackers;
    
    if(objectp(puppet))
    {
        puppet->move_player(environment(this_object()));
        puppet->kill_ob(this_object());
    }
    
    environment(this_object())->return_exits();
    
    tell_room(environment(this_object()), "%^BOLD%^CYAN%^You feel a great shadow lift from the area as the puppeteer dissipates into nothingness!%^RESET%^");
    broadcast_area("/d/common/bosses/faceless/rooms/", "%^WHITE%^BOLD%^An insidious, creeping shadow has been cleansed from this land as the Puppeteer is defeated!%^RESET%^");
    //message("broadcast", "%^WHITE%^BOLD%^An insidious, creeping shadow has been cleansed from this land as the Puppeteer is defeated!%^RESET%^", users());
    message("broadcast", "%^WHITE%^BOLD%^The balance of power in the world shifts towards %^CYAN%^GOOD%^RESET%^", users());
    WORLD_EVENTS_D->kill_event("Puppeteer has been defeated");
    WORLD_EVENTS_D->inject_event((["Puppeteer has been defeated" : (["start message" : "%^BOLD%^%^BLACK%^The Puppeteer has been defeated!", "event type" : "exp bonus", "length" : 720, "notification" : "5% Bonus Exp", "event name" : "Puppeteer has been defeated", "modifier" : 5, "announce" : 1, "announce to" : "world", "alignments" : ({ 1, 4, 7 }) ]), ]));
}

void heart_beat()
{
    object attackers, room;
    int percent;

    if (!objectp(this_object()))
    {
        return;
    }

    ::heart_beat();
    
    room = environment(this_object());
    
    if(!room)
        return;

    attackers = this_object()->query_attackers();
    
    if(sizeof(attackers))
        ticker++;
    else
        return;
    
    madness_check(room, attackers);
    
    if(!enrage && ticker >= ENRAGE_TIMER)
    {
        enrage(room);
    }
    else if(enrage)
    {
        for(int x = 0; x < psize; x++)
            this_object()->execute_attack();
        
        if((query_hp() < query_max_hp()))
            add_hp(1000);
    }
    
    percent = (query_hp() * 100) / query_max_hp();
    
    switch(percent)
    {
        case 90..100:
        if(sizeof(attackers))
            melee(room);
        break;
        
        case 70..89:
        spells(room);
        break;
        
        case 40..69:
        shadows(room);
        break;
        
        case 20..39:
        madness(room);
        break;
            
        default:
        dopple(room);
        break;
    }
}

void madness_check(object room, object *attackers)
{
    madness = ticker / 25;
    
    foreach(object ob in attackers)
    {
        if(!userp(ob))
            continue;
        
        if(ob->query_property("effect_confused"))
            madness++;
        if(ob->query_property("effect_staggered"))
            madness++;
        if(ob->query_property("effect_panicked"))
            madness++;
        if(ob->query_property("effect_cowering"))
            madness++;
        if(ob->query_property("effect_exhausted"))
            madness++;
        if(ob->query_property("effect_frightened"))
            madness++;
        if(ob->query_property("effect_fatigued"))
            madness++;
        if(ob->query_property("effect_sickened"))
            madness++;
        if(ob->query_property("effect_staggered"))
            madness++;
        if(ob->query_property("effect_shaken"))
            madness++;
        if(ob->query_paralyzed())
            madness++;
        if(ob->query_property("effect_dazzled"))
            madness++;
        if(ob->query_temporary_blinded())
            madness++;
    }
    
    if(objectp(puppet))
        madness++;
    
    if(madness)
    {
        tell_room(room, "%^MAGENTA%^BOLD%^The Puppeteer is emboldened by the madness it is causing!%^RESET%^");
        tell_room(room, "%^YELLOW%^MADNESS : %^CYAN%^" + madness);
        add_hp(madness * 50);
    }
}

//Physical attacks phase
void melee(object room)
{
    int spike_chance;
    
    if(!checkpoints["melee"])
    {
        tell_room(room, "%^YELLOW%^The Puppeteer says : %^RESET%^%^CYAN%^BOLD%^Let's start with something light, shall we?");
        tell_room(room, "%^C244%^The P%^C097%^u%^C103%^p%^C109%^p%^C115%^e%^C109%^t%^C103%^e%^C097%^e%^C244%^r launches a series of attacks with his %^C109%^shadowy tentacles%^C244%^!%^CRST%^");        
        checkpoints["melee"] = 1;
    }
    
    spike_chance = max( ({ 0, 5 - madness }) );
    
    if(!random(spike_chance))
        shadow_spikes();
}

//Casts some random spells
void spells(object room)
{
    string spell;
    object target;
    
    if(!checkpoints["spells"])
    {
        tell_room(room, "%^YELLOW%^The Puppeteer says : %^RESET%^%^BOLD%^CYAN%^Still alive are we? Perhaps a little magic to liven things up...%^RESET%^");
        tell_room(room, "%^C141%^The Puppeteer begins casting a series of dark spells!%^CRST%^"); 
        //set_spell_chance(75);
        checkpoints["spells"] = 1;
    }
    
    target = pick_random_target("user");
    spell = get_random_spell();
    
    objectp(target) && new("/cmds/spells/" + spell[0..0] + "/_" + replace_string(spell, " ", "_") + ".c")->use_spell(this_object(), target, 70, 100, "sorcerer");
}

//Summons shadow minions to attack the party
void shadows(object room)
{
    object shadow;
    
    if(!checkpoints["shadows"])
    {
        tell_room(room, "%^YELLOW%^The Puppeteer says : %^RESET%^BOLD%^CYAN%^The shadows will become your friends, as they, so long ago, became mine...");
        tell_room(room, "%^C103%^The S%^C109%^h%^C103%^a%^C109%^d%^C103%^o%^C109%^w%^C103%^s all around you move and f%^C109%^l%^C103%^i%^C109%^t%^C103%^ and f%^C109%^e%^C103%^e%^C109%^l%^C103%^ and %^C115%^attack%^C103%^!%^CRST%^"); 
        set_spell_chance(35);
        checkpoints["shadows"] = 1;
    }
    
    if(present("shadow spawn", room))
        return;
    
    tell_room(room, "%^BLACK%^BOLD%^The Puppeteer beckons to the shadows around you!%^RESET%^");
    
    for(int x = 0; x < (5 + madness * 2); x++)
    {
        tell_room(room, "%^YELLOW%^A figure emerges from the shadows!%^RESET%^");
        shadow = new("/d/common/bosses/faceless/shadow_spawn");
        shadow->move(room);
        this_object()->add_protector(shadow);
    }
}

//Causes widespread confusion every round
void madness(object room)
{
    object target;
    
    if(!checkpoints["madness"])
    {
        tell_room(room, "%^YELLOW%^The Puppeteer says : %^RESET%^BOLD%^CYAN%^Your feeble minds cannot handle the harsh reality of this existence.");
        tell_room(room, "%^C171%^A c%^C177%^l%^C183%^ou%^C171%^d of pure m%^C177%^a%^C183%^d%^C189%^n%^C177%^e%^C171%^ss descends upon you!%^CRST%^"); 
        checkpoints["madness"] = 1;
    }
    
    target = pick_random_target("user");
    
    if(SAVING_THROW_D->will_save(target, 80 + madness))
    {
        tell_object(target, "%^CYAN%^%^BOLD%^You cough and struggle and are able to shake off the madness!%^RESET%^");
        tell_room(room, "%^CYAN%^BOLD%^" + target->query_cap_name() + " coughs and struggles and is able to shake off the madness!.%^RESET%^", target);
        target->cause_typed_damage(target, "head", roll_dice(10, 10) + madness, "mental");
    }
    else
    {
        tell_room(room, "%^MAGENTA%^BOLD%^" + target->query_cap_name() + " inhales a gulp of the maddening cloud and goes completely berserk!%^RESET%^", target);
        tell_object(target, "%^MAGENTA%^BOLD%^You breath in a lung full of the cloud and suddenly lose all control!%^RESET%^");
        "/std/effect/status/confused"->apply_effect(target, 8 + madness, this_object());
    }
}

//Replaces a player with a doppleganger, which they have to kill.
void dopple(object room)
{
    object prison, *attackers;
    
    if(!objectp(room))
        return;
    
    if(!checkpoints["dopple"])
    {
        tell_room(room, "%^YELLOW%^The Puppeteer says : %^RESET%^BOLD%^CYAN%^You've managed to endure the madness of this world, but what of the madness inside of you?%^RESET%^");
        set_spell_chance(50);
        
        attackers = this_object()->query_attackers();
        
        foreach(object ob in attackers)
            ob->cease_all_attacks();
            
        tell_room(room, "%^CYAN%^BOLD%^For a moment, your heads seem to clear and there is a pause in the madness.....%^RESET%^");
            
        checkpoints["dopple"] = 1;
    }
    
    if(catch(prison = load_object("/d/common/bosses/faceless/prison_room")))
    {
        //tell_room(room, "BOSS ERROR IN LOADING ROOM. CONTACT AN IMM.");
        return;
    }
    
    if(!objectp(puppet) && !random(3 - madness))
    {
        puppet = pick_random_target("user");
        
        if(objectp(puppet))
        {
            string *classes;
            
            tell_object(puppet, "%^RED%^BOLD%^The puppeteer touches your head and you black out%^RESET%^!");
            tell_room(room, "\n\%^BOLD%^MAGENTA%^The puppeteer touches " + puppet->query_cap_name() + "'s head and " + puppet->query_objective() + " goes limp.%^RESET%^", puppet);
            puppet->move_player(prison);
            puppet->set_paralyzed(30, "You are blacked out!");
            
            dopple = new("/d/common/bosses/faceless/dopple");
            dopple->set_name(puppet->query_cap_name());
            dopple->set_id( ({ "doppleganger", puppet->query_cap_name(), puppet->query_name(), puppet->query_race() }) );
            dopple->set_race(puppet->query_race());
            dopple->set_short(puppet->query_short());
            dopple->set_long(puppet->query_long());
            dopple->set_player_height(puppet->query_player_height());
            dopple->set_player_weight(puppet->query_player_weight());
            dopple->set_gender(puppet->query_gender());
            dopple->set_level(60 + madness);
            dopple->set_max_hp(10000);
            dopple->set_hp(10000);
            dopple->set_puppet(puppet);
            
            classes = puppet->query_classes();
            
            foreach(string pclass in classes)
                dopple->set_mlevel(pclass, puppet->query_class_level(pclass));
            
            dopple->move(room);
        }
    }
}

void enrage(object room)
{   
    if(!objectp(room))
        return;
    
    tell_room(room, "%^YELLOW%^Puppeteer says : %^BLACK%^BOLD%^Your time is up, foolish mortals...%^RESET%^");
    tell_room(room, "%^RED%^BOLD%^THE PUPPETEER ENRAGES!%^RESET%^");
    enrage = 1;
}

object pick_random_target(string type)
{
    object* attackers;
    if (!objectp(TO)) {
        return 0;
    }

    attackers = this_object()->query_attackers();
    
    if(type == "user")
        attackers = filter_array(attackers, (: userp($1) :));

    if (!sizeof(attackers)) {
        return 0;
    }
    return attackers[random(sizeof(attackers))];
}   

/*
  MELEE ATTACK SPECIALS
*/

int bite(object ob)
{   
    if(!ob)
        return 0;
    
    if(random(3))
        return roll_dice(1, 8);
    
    set_base_damage_type("piercing");
    
    tell_object(ob, "%^RED%^BOLD%^Puppeteer bites you ferociously with its maw.%^RESET%^");
    tell_room(environment(this_object()), "%^RED%^BOLD%^The Puppeteer bites " + ob->query_cap_name() + " ferociously with its maw!%^RESET%^", ({ ob }));
    
    return (roll_dice(10 + madness, 10) + 10) * (1 + enrage);
}

int tendril(object ob)
{   
    if(!ob)
        return 0;
    
    if(random(3))
        return roll_dice(1, 8);
    
    set_base_damage_type("void");
    
    if(SAVING_THROW_D->reflex_save(ob, 85))
    {
        tell_object(ob, "%^BOLD%^You slip away from the tendril's grasp!%^RESET%^");
        return 0;
    }
    
    tell_object(ob, "%^BLACK%^BOLD%^The Puppeteer reaches out with a shadow tendril and grabs you!%^RESET%^");
    tell_room(environment(this_object()), "%^BLACK%^BOLD%^The Puppeteer reaches out with a shadowy tendril and grabs " + ob->QCN + "!%^RESET%^", ({ ob }));
    
    ob->set_paralyzed(roll_dice(1,4) * 6, "%^YELLOW%^The tendril has you wrapped tight!%^RESET%^");
    
    return (roll_dice(12 + madness, 10) + 10) * (1 + enrage);
}

void shadow_spikes()
{
    object room, *attackers;
    int dam;
    
    room = environment(this_object());
    
    if(!objectp(room))
        return;
    
    attackers = this_object()->query_attackers();
    
    if(!pointerp(attackers) || !sizeof(attackers))
        return;
    
    tell_room(room, "%^C245%^S%^C184%^h%^C185%^a%^C187%^d%^C185%^o%^C184%^w%^C245%^y s%^C184%^p%^C185%^i%^C186%^k%^C245%^es launch themselves from all directions, threatening to %^C166%^skewer%^C245%^ you all!%^CRST%^");
    
    dam = roll_dice(50 + madness, 10);
    
    foreach(object ob in attackers)
    {   
        if(!SAVING_THROW_D->reflex_save(ob, 80 + madness))
        {
            tell_room(room, "%^BOLD%^RED%^A spike impales " + ob->query_cap_name() + "!%^RESET%^");
            ob->cause_typed_damage(ob, "torso", dam, "void");
        }
        else
        {
            ob->cause_typed_damage(ob, "torso", dam / 2, "piercing");
        }
    }
}
