/*
  krasus.c
  
  Great golden dragon. Avatar boss of Krey.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>
#include <magic.h>

#define COREPARTY 4
#define HEALER_WAVE 5

inherit "/d/common/bosses/avatar_boss.c";

int buffed,
    ticker,
    enrage;
    
object room;

mapping checkpoints = ([
                        "shield"  : 0,
                        "spear"   : 0,
                        "barrage" : 0,
                        "dragon"  : 0,
                        "waves"   : 0,
                      ]);
                      
object barrage_room;

void create()
{
    object obj;
    
    set_name("krasus");
    set_id( ({ "dragonkin", "defender", "warrior", "golden defender" }) );
    set_short("Krasus, the Great Golden Defender");
    set_long("");
    set_race("human");
    set_gender("male");
    set_hd(75, 10);
    set_class("paladin");
    set_mlevel("paladin", 75);
    set_class("immortal_defender");
    set_mlevel("immortal_defender", 10);
    set_class("cleric");
    set_mlevel("cleric", 75);
    set_class("oracle");
    set_mlevel("oracle", 75);
    set_mystery("dragon");
    set("base_class", "paladin");
    set("dragon_affinity", "gold");
    set_alignment(1);
    set_damage(20, 10);
    set_attacks_num(10);
    
    set_stats("strength", 30);
    set_stats("constitution", 30);
    set_stats("charisma", 30);
    set_stats("wisdom", 10);
    set_stats("dexterity", 14);
    set_stats("intelligence", 10);    
    set_skill("perception", 50);
    set_mob_magic_resistance("high");
    set_overall_ac(-80);
    set_sight_bonus(10);
    set_max_hp(125000);
    set_hp(125000);
    //set_property("add quest", "Faced the Krasus, The Great Golden Defender!");
    
    set_monster_feats( ({ "damage resistance", "improved damage resistance", "weapon focus", "rush", "shield focus", "shieldbash", "resistance", "improved resistance", "increased resistance", "expertise", "parry", "shieldwall", "counter", "weapon bond", "armor bond", "penetrating strike", "layonhands", "smite", "dreadful carnage", "cornugon smash", "shatter defenses", "intimidating prowess", "dazzling display", "improved shieldbash", "improved rush" }) );
    
    set_spell_chance(25);
    
    set_spells( ({ "shield of dawn", "angelic aspect", "archon aura", "prayer", "seeking sword" }) );
    
    if(!clonep())
        return;
    
    obj = new("/d/magic/obj/shields/kreysneothosies");
    obj->set_property("enchantment", 10);
    obj->move(this_object());
    obj->set_property("monster weapon", 1);
    force_me("wear shield");

    obj = new("/d/magic/obj/weapons/kreysneothosies");
    obj->set_property("enchantment", 10);
    obj->move(this_object());
    obj->set_property("monster weapon", 1);
    force_me("wield weapon");
}

void init()
{
    object player;
    
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
        command("shieldwall max");
        command("enhance add axiomatic");
        command("enhance add holy");
        command("enhance weapon");
        command("enhance add light fortification");
        command("enhance armor");
        command("defenders_presence");
        new("/cmds/spells/s/_shield_of_dawn.c")->use_spell(this_object(), 0, 70, 100, "paladin");
        new("/cmds/spells/a/_angelic_aspect.c")->use_spell(this_object(), 0, 70, 100, "paladin");
        new("/cmds/spells/a/_archon_aura.c")->use_spell(this_object(), 0, 70, 100, "paladin");
        new("/cmds/spells/s/_seeking_sword.c")->use_spell(this_object(), 0, 70, 100, "paladin");
        buffed = 1;
    }        
}

void enrage(object room)
{   
    if(!objectp(room))
        return;
    
    tell_room(room, "%^YELLOW%^Krasus says : %^RED%^%^BOLD%^Your time is up, foolish mortals...%^RESET%^");
    tell_room(room, "%^RED%^BOLD%^KRASUS ENRAGES!%^RESET%^");
    set_attacks_num(20);
    set_spell_chance(100);
    enrage = 1;
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
    
    if(!enrage && ticker >= ENRAGE_TIMER)
    {
        enrage(room);
    }
    else if(enrage)
    {   
        if((query_hp() < query_max_hp()))
            add_hp(1000);
    }
    
    percent = (query_hp() * 100) / query_max_hp();
    
    switch(percent)
    {
        case 90..100:
        if(sizeof(attackers))
            shield(room);
        break;
        
        case 70..89:
        spear(room);
        break;
        
        case 40..69:
        barrage(room);
        break;
        
        case 20..39:
        dragon(room);
        break;
            
        default:
        waves(room);
        break;
    }
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

//SHIELD PHASE
void shield(object room)
{
    object target;
    
    if(!objectp(room))
        return;
    
    if(!checkpoints["shield"])
    {
        tell_room(room, "MESSAGE START SHIELD PHASE");
        set_spells( ({ "earthquake", "prayer", "seeking sword", "overwhelming presence", "earth reaver" }) );
        set_spell_chance(10);
        checkpoints["shield"] = 1;
    }
    
    target = pick_random_target("user");
    
    if(!objectp(target))
        return;
    
    switch(random(4))
    {
        case 0:
        command("smite " + target->query_name());
        break;
        case 1:
        command("shieldbash " + target->query_name());
        break;
        case 2:
        command("rush " target->query_name());
        break;
        case 3:
        command("shield_charge");
        break;
    }
}

//SPEAR PHASE
void spear(object room)
{
    object target, weapon;
    
    if(!objectp(room))
        return;
    
    if(!checkpoints["spear"])
    {
        tell_room(room, "YOU FACE THE INDOMITABLE MIGHT OF THE GREAT GOLDEN DRAGON EMPEROR HIMSELF!");
        
        if(objectp(weapon = new("/d/common/bosses/loot/regalith")))
        {
            tell_room(room, "A GREAT GOLDEN SPEAR APPEARS IN KRASUS'S HANDS, AND MIGHTY CLAWS APPEAR ON HIS HANDS");
            command("shieldwall min");
            command("unwield weapon");
            command("remove shield");
            weapon->move(this_object());
            weapon->set_property("monster weapon", 1);
            set_spell_chance(15);
            set_monster_feats( ({ "damage resistance", "improved damage resistance", "weapon focus", "rush", "resistance", "improved resistance", "increased resistance", "parry", "weapon bond", "armor bond", "penetrating strike", "layonhands", "smite", "dreadful carnage", "cornugon smash", "shatter defenses", "intimidating prowess", "dazzling display", "improved rush", "sweepingblow", "strength of arm", "light weapon", "impale" }) );
            command("powerattack max");
        }
        
        checkpoints["spear"] = 1;
    }
    
    target = pick_random_target("user");
    
    switch(random(4))
    {
        case 0:
        command("sweepingblow");
        break;
        case 1:
        command("impale " + target->query_name());
        break;
        case 2:
        command("smite " + target->query_name());
        break;
        case 3:
        command("rush " + target->query_name());
        break;
    }
}

//BARRAGE PHASE
void barrage()
{
    object *targets;
    
    if(!checkpoints["barrage"])
    {
        tell_room(room, "MESSAGE START BARRAGE PHASE");
        set_spells( ({ "bolt of force", "prayer", "overwhelming presence", "dictum", "slow" }) );
        set_spell_chance(50);
        checkpoints["barrage"] = 1;
    }

    if(!sizeof(targets = this_object()->query_attackers()))
        return;
    
    spear_barrage();
    
    target = pick_random_target("user");
    
    if(!objectp(target))
        return;

    switch(random(4))
    {
        case 0:
        command("sweepingblow");
        break;
        case 1:
        command("impale " + target->query_name());
        break;
        case 2:
        command("smite " + target->query_name());
        break;
        case 3:
        command("rush " + target->query_name());
        break;
    }
}

void dragon()
{
    int num_healer;
    object healer;
    
    if(!objectp(room))
        return;
    
    if(!checkpoints["dragon"])
    {
        tell_room(room, "MESSAGE START DRAGONKIN PHASE");
        command("unwield weapon");
        set_race("dragonkin");
        set_short("DRAGONKIN SHORT DESC");
        set_long("DRAGONKIN LONG DESC");
        set_spells( ({ "bolt of force", "overwhelming presence", "dictum", "slow", "holy smite", "crushing hand" }) );
        set_monster_feats( ({ "damage resistance", "improved damage resistance", "weapon focus", "rush", "resistance", "improved resistance", "increased resistance", "parry", "weapon bond", "armor bond", "penetrating strike", "layonhands", "smite", "dreadful carnage", "cornugon smash", "shatter defenses", "intimidating prowess" }) );
        set_spells( ({ "obsidian flow", "overwhelming presence", "fear", "powerword kill", "earthquake", "bolt of force" }) );
        set_spell_chance(50);
        command("dragon_aspect");
        checkpoints["dragon"] = 1;
    }

    if(num_healer = present("krey_healer", room))
    {
        if((query_hp() * 100) / query_max_hp() < 100)
        {
            tell_room(room, "KRASUS GETTING HEALED MESSAGE");
            add_hp(100 * num_healer);
        }
    }
    else if(!random(10))
    {
        tell_room(room, "KRASUS CALLS FOR HEALERS MESSAGE");
        
        for(int x = 0; x < HEALER_WAVE)
        {
            if(objectp(healer = new("/d/common/bosses/krey/healer")))
                healer->move(environment());
        }
    }
    
    switch(random(2))
    {
        case 0:
        command("smite " + target->query_name());
        break;
        case 1:
        command("rush " + target->query_name());
        break;
    }
}

void waves()
{
    if(!objectp(room))
        return;
    
    if(!checkpoints["waves"])
    {
        tell_room(room, "MESSAGE START DRAGON/WAVES PHASE");
        set_race("dragon");
        set_short("DRAGON SHORT DESC");
        set_long("DRAGON LONG DESC");
        set_spells( ({ "bolt of force", "overwhelming presence", "dictum", "clashing rocks", "crushing hand", "fire storm", "globe of invulnerability" }) );
        set_spell_chance(100);
        new("/cmds/spells/g/_globe_of_invulnerability.c")->use_spell(this_object(), 0, 70, 100, "cleric");
        checkpoints["waves"] = 1;
    }
    
    switch(random(3))
    {
        case 0:
        wing_flap();
        break;
        case 1:
        fire_breath();
        break;
        default:
        spear_barrage();
        break;
    }   
        
}

void wing_flap()
{
    object *attackers;
    
    attackers = query_attackers;
    
    if(!sizeof(attackers))
        return;
    
    tell_room(room, "KRASUS FLAPS HIS WINGS CREATING A HUGE WING GUST!");
    
    foreach(object ob in attackers)
    {
        if(!SAVING_THROW_D->reflex_save(ob, 85);
        {
            tell_object(ob, "YOURE THROWN ON YOUR ASS");
            tell_room(room, ob->query_cap_name() + " IS THROWN ON " + ob->query_possessive() + " ASS!", ob);
            ob->cause_typed_damage(ob, "torso", roll_dice(10, 10) + 100, "force");
            ob->set_tripped(6);
        }
        
    }
}

void fire_breath()
{
    object *attackers;
    int dam;
    
    attackers = query_attackers;
    
    if(!sizeof(attackers))
        return;
    
    tell_room(room, "KRASUS BREATHES A BUNCH OF FIRE ON EVERYONE!");
    dam = roll_dice(10, 10) + 300 + (enrage * 100);
    
    foreach(object ob in attackers)
    {
        tell_object(ob, "YOURE BURNED TO A CRISP");
        tell_room(room, ob->query_cap_name() + " IS BURNED TO A CRISP!", ob);
        
        if(!SAVING_THROW_D->reflex_save(ob, 85);
            ob->cause_typed_damage(ob, "torso", dam, "fire");
        else
            ob->cause_typed_damage(ob, "torso", dam / 2, "fire");        
    }
}  

void spear_barrage()
{
    object *targets;
    
    targets = query_attackers();
    
    if(!sizeof(targets))
        return;
    
    tell_room(room, "SPEAR BARRAGE MESSAGE");
    
    foreach(object ob in targets)
    {
        if(!SAVING_THROW_D->fort_save(ob, 85))
        {
            ob->set_property("rend", 2);
            tell_object(ob, "BLEEDING MESSAGE");
        }
        
        ob && ob->cause_typed_damage(ob, "torso", roll_dice(10, 10) + 200 + (enrage * 100), "piercing");
    }
}

void boss_death_event()
{
    object *attackers;
    
    environment(this_object())->return_exits();
    
    tell_room(environment(this_object()), "KRASUS DIES ROOM MESSAGE");
    //broadcast_area("/d/common/bosses/faceless/rooms/", "FLASHY KRASUS DIES MESSAGE");
    message("broadcast", "%^BLACK%^BOLD%^The balance of power in the world shifts towards %^CYAN%^EVIL%^RESET%^", users());
    WORLD_EVENTS_D->kill_event("Krasus has been defeated");
    WORLD_EVENTS_D->inject_event((["Krasus has been defeated" : (["start message" : "%^BOLD%^%^BLACK%^Krasus has been defeated!", "event type" : "exp bonus", "length" : 720, "notification" : "5% Bonus Exp", "event name" : "Krasus has been defeated", "modifier" : 5, "announce" : 1, "announce to" : "world", "alignments" : ({ 6, 7, 9 }) ]), ]));
}  
    
        

