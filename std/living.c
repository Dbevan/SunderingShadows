//  from the Nightmare mudlib
//  inheritable code for living things
//  created by Descartes of Borg september 1992

/**
 * @file
 * @brief All living beings inherit
 */


#include <security.h>
#include <daemons.h>
#include <party.h>
#include <dirs.h>
#include <living.h>
#include <rooms.h>
inherit "/std/lang";
#include <std.h>
#include <move.h>

#define TIME_TO_HEAL 1000
#define HEALING_FORMULA (stats["strength"] + stats["constitution"] + stats["dexterity"] + tolerance) * 250
#define PASSED_OUT "You have passed out from exhaustion."

#define MAX_ENCUMBRANCE ({ 15, 25, 35, 45, 60, 85, 105, 125, 145, 160, 180, 195, 210, 225, 235, 245, 255, 270, 285, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, \
                           360, 365, 370, 375, 380, 385, 390, 395, 400, 405, 410, 415, 420, 425, 430, 435, 440, 445, 450, 455, 460, 465, 470, 475, 480, 485, 495, 500, 505, 510, \
                           515, 520, 525, 530, 535, 540, 545, 550, 555, 565, 576, 585, 590, 595, 600, 605, 610 })

#define SAVING_THROW ({ "paralyzation_poison_death", "rod_staff_wand", "breath_weapon", "petrification_polymorph", "spell", "will", "reflex", "fortitude" })

int invis;
int ok_to_heal, player_age, spell_interrupted;
nosave int parrying, scrambling;
int num_attacks;
nosave int forced, sight_bonus;
string description;
string background;
string acquired_template;
string sphere_of_influence;
string school_of_magic;
string school_of_opposition;
string sorc_bloodline;
string oracle_mystery;
string psion_discipline;
//string druid_circle;
string paladin_oath;
string inquisition;
//by Circe, for psions 7/16/05
string* divine_domain = ({}); // For cleric domains -Ares 4/7/07
string* quest_spells = ({}); // For priest quest spells - Ares 4/8/08
nosave string party;
nosave string* search_path;
private string gender;
mapping stats;
int temp_flag;
nosave int abonus, dbonus; // For true AD&D-style attack and damage bonuses
nosave mapping stat_bonus;
mapping save_bonus;
object in_vehicle;
object poisoner;
object draggee;
nosave int living_ticker = 0;
int tolerance, tolerance_flag, used_stamina;
nosave int detecting_invis;
nosave int true_seeing;
mapping time_delay;
nosave function* pre_exit_funcs;
nosave function* post_exit_funcs;
private nosave object* protectors;
string diety;

string fighter_style;

void create()
{
    combat::create();
    time_delay = ([]);
}

/*
varargs int query_id_no(string id_name)
{
    int i, limit;
    if (!objectp(environment(this_object()))) {
        return 0;
    }
    if (!stringp(id_name)) {
        id_name = query_race();
    }
    limit = sizeof(all_living(environment(this_object())));
    for (i = 1; i < limit; i++) {
        if (present(id_name + " " + i) == this_object()) {
            return i;
        }
    }
    return 0;
}
*/

int do_typed_damage_effects(victim, limb, damage, damage_type)
{
    //overide this if you want your mob to do something special when it takes typed damage. Return the amount of damage you want done (priort to application of any standard resistances)
    return damage;
}

string adjust_targeted_limb(object victim, string limb)
{
    return limb;
}

//Fix this to use deity
void set_diety(string str)
{
    diety = str;
}

//Fix this to use deity
string query_diety()
{
    if (stringp(diety)) {
        return diety;
    } else {
        return "godless";
    }
}

varargs void set_nwp(string name, int level, int seed)
{
// placeholder that does nothing to prevent NPCs bugging as many are still set with nwps. N, 12/13
}

string query_race()
{
    if (!objectp(this_object())) {
        return "";
    }
    if (!avatarp(this_object())) {
        return query("race");
    }
    if ((query("race") == "with") && query("race_mask")) {
        return query("race_mask");
    }
    return query("race");
}

string query_visual_race()
{
    object shape;

    if (objectp(shape = query_property("shapeshifted"))) {
        return shape->query_shape_race();
    }

    if (objectp(shape = query_property("altered"))) {
        return shape->query_shape_race();
    }

    return query_race();
}

void set_race(string str)
{
    set("race", str);
    add_id(str);
}

void set_num_attacks(int i)
{
    if (!interactive(this_object())) {
        num_attacks = i;
    }
}

string query_message_in()
{
    mapping message_map;
    string profile;

    if (!interactive(this_object())) {
        return 0;
    }

    profile = (string)query("relationship_profile");
    message_map = (mapping)query("message_map");

    if (!profile) {
        return 0;
    }
    if (!mapp(message_map)) {
        return 0;
    }
    if (!mapp(message_map[profile])) {
        return 0;
    }

    return message_map[profile]["in"];
}

string query_message_out()
{
    mapping message_map;
    string profile;

    if (!interactive(this_object())) {
        return 0;
    }

    profile = (string)query("relationship_profile");
    message_map = (mapping)query("message_map");

    if (!profile) {
        return 0;
    }
    if (!mapp(message_map)) {
        return 0;
    }
    if (!mapp(message_map[profile])) {
        return 0;
    }

    return message_map[profile]["out"];
}

//Doesn't seem to be used anymore
int query_num_attacks()
{
    int lev;

    if (!interactive(this_object())) {
        return num_attacks;
    } else {
        lev = query_level();
        if (lev < 8) {
            return 1;
        }
        if ((lev < 15) && (lev > 7)) {
            if (temp_flag) {
                temp_flag = 0;
                return 1;
            } else {
                temp_flag = 1;
                return 2;
            }
        }
        if ((lev > 14)) {
            return 2;
        }
    }
}

//Move to combat.c
void set_parrying(int i)
{
    parrying = i;
}

//Move to combat.c
int query_parrying(object att)
{
    object* weapons, *eweapons;
    weapons = query_wielded();
    
    objectp(att) && eweapons = att->query_wielded();
    
    //can't parry if both weapons ranged
    if(sizeof(eweapons) && eweapons[0]->is_lrweapon() && !FEATS_D->usable_feat(this_object(), "deflect arrows") && !FEATS_D->usable_feat(this_object(), "canny defense"))
    {
        if(sizeof(eweapons) == 1)
            return 0;
            
        if(sizeof(eweapons) > 1 && eweapons[1]->is_lrweapon())
            return 0;
    }
    
    if (sizeof(weapons) && !weapons[0]->is_lrweapon()) {
            
        if (FEATS_D->usable_feat(this_object(), "parry")) {
            return 1;
        }
        if (FEATS_D->usable_feat(this_object(), "opportunistic parry")) {
            if (validate_combat_stance("one hander")) {
                return 1;
            }
        }
        if (FEATS_D->usable_feat(this_object(), "unassailable parry")) {
            if (validate_combat_stance("dual wield") && !weapons[1]->is_lrweapon()) {
                return 1;
            }
        }
        if (FEATS_D->usable_feat(this_object(), "blade block")) { // this should not allow parrying with bows!
            if (validate_combat_stance("two hander")) {
                return 1;
            }
        }
    }
    if (FEATS_D->usable_feat(this_object(), "unarmed parry")) {
        if (!sizeof(weapons)) {
            return 1;
        }
    }
    if (!parrying) {
        return 0;
    }
    return parrying;
}

void set_scrambling(int i)
{
    scrambling = i;
}

int query_scrambling()
{
    if (query_tripped() || query_in_vehicle() || (!is_ok_armour("thief"))) {
        return 0;
    }
    if (FEATS_D->usable_feat(this_object(), "scramble")) {
        return 1;
    }
    return scrambling;
}

void init_living()
{
    add_action("cmd_hook", "", 1);
    init_path();
    init_attack();
}

void reinit_path()
{
    init_path();
}

void heart_beat()
{
    int myskill, mylevel, regen_amt, i;
    object me = this_object(), attacker;

    if (!objectp(me))
    {
        return;
    }

    POISON_D->ProcessPoisons(me);

    // No longer used
    // new stab resets available chances once per round.
    if (objectp(me) && sizeof(query_attackers()))
    {
        if (query_property("stabs_available"))
        {
            remove_property("stabs_available");
        }
        if (FEATS_D->usable_feat(this_object(), "combat reflexes"))
        {
            /*            i = (max(({query_guild_level("thief"),
                                            query_class_level("thief") + query_class_level("arcane_trickster")
                                            }))+9)/10; */
                                            // there's a lib query for this now to get ANY thief PrC inherits, let's use that for efficiency! N, 1/3/20
            i = (query_prestige_level("thief") + 9) / 10;
            set_property("stabs_available", i);
        }
    }
    if (is_class("monk"))
    {
        USER_D->regenerate_pool(me, (1 + random(2)), 1, "ki");
    }
    if (is_class("magus"))
    {
        USER_D->regenerate_pool(me, (1 + random(2)), 1, "arcana");
    }
    if (is_class("paladin") || is_class("cleric"))
    {
        USER_D->regenerate_pool(me, 1, 1, "grace");
    }
    if (is_class("psion") || is_class("psywarrior"))
    {
        USER_D->regenerate_pool(me, 1, 1, "focus");
    }
    if (is_class("warlock"))
    {
        USER_D->regenerate_pool(me, 1, 1, "burn");
    } 

    //enhancement effects
    "/cmds/mortal/_enhance.c"->run_enhances_timer(me, "weapon");
    "/cmds/mortal/_enhance.c"->run_enhances_timer(me, "armor");

    if (used_stamina > 0)
    {
        if (!userp(me))
        {
            used_stamina -= 10;
            if (used_stamina < 0)
            {
                used_stamina = 0;
            }
            return;
        }
        if (!query_property("inactive"))
        {
            myskill = query_skill("endurance");
            if (sizeof(query_attackers()) < 1)
            {
                if (myskill < 0)
                {
                    myskill = 1;
                }
                used_stamina -= ((myskill / 4) + 2);
                if (used_stamina < 0)
                {
                    used_stamina = 0;
                }
            }
            else
            {
                used_stamina -= ((myskill / 4) + 2);
                if (used_stamina < 0)
                {
                    used_stamina = 0;
                }
            }
        }
        if (query_condition() < 0)
        {
            set_condition(query_max_stamina());
        }

        if (userp(me))
        {
            gmcp_update_character("vitals", ([ "stamina": "" + (query_max_stamina() - used_stamina), "max_stamina": "" + query_max_stamina() ]));
        }
    }

    if (!(living_ticker % 3))
    {
        //Central way to remove instant feat locks when out of combat
        if(!query_current_attacker())
        {
            remove_property("using instant feat");
        }
        
        if (FEATS_D->usable_feat(me, "regeneration"))
        {
            if (query_hp() < query_max_hp())
            {
                regen_amt = roll_dice(1, query_level()) / 2 + 1;
                if(query_property("fester")) regen_amt = regen_amt / 2;
                add_hp(regen_amt);
            }
        }
        if (query_property("fast healing"))
        {
            if (query_hp() < query_max_hp())
            {
                regen_amt = query_property("fast healing") * roll_dice(1, query_level() / 2 + 1);
                if(query_property("fester")) regen_amt = regen_amt / 2;
                add_hp(regen_amt);
            }
        }

        if(is_class("metamind"))
        {
            if(query("available focus"))
            {
                add_mp(1);
            }
        }

        if(FEATS_D->usable_feat(me, "psychic vampire") && !avatarp(me) && !wizardp(me) && !query("no pk"))
        {
            object targs = all_inventory(environment(me));
            targs = filter_array(targs, (: userp($1) :));
            targs -= ({ me });

            /*
            foreach(object ob in targs)
            {
                if(ob->query("no pk"))
                    continue;

                if(ob->query_mp() && !ob->query("no pk") && !wizardp(ob) && !avatarp(ob))
                {
                    
                    if(!random(5))
                        tell_object(ob, "%^MAGENTA%^You feel something pull on your mind.");
                    ob->add_mp(-1);
                    
                }
            }
            */

            add_mp(sizeof(targs));
        }
        
        if(is_shade() &&
        !random(25) &&
        !avatarp(me) &&
        !wizardp(me) &&
        !query_hidden() &&
        !query_property("altered") &&
        !query_invis())
        {
            object room = environment(me);
            
            switch(total_light(room))
            {
                case 0..1:
                tell_object(me, "%^BLACK%^BOLD%^Whisps of darkness coalesce from the shadows around you.%^RESET%^");
                tell_room(room, "%^BOLD%^BLACK%^Whisps of darkness coalesce from the shadows around " + query_cap_name(), me);
                break;
                case -5..-1:
                tell_object(me, "%^BOLD%^BLACK%^You feel your body meld with the darkness around you.");
                tell_room(room, "%^BOLD%^BLACK%^" + query_cap_name() + "'s body seems to meld with the darkness around " + query_objective() + ".", me);
                break;
                case -10..-6:
                tell_object(me, "%^BOLD%^BLACK%^You are engulfed in a miasma of shadowy energy.");
                tell_room(room, "%^BOLD%^BLACK%^" + query_cap_name() + " is engulfed in a miasma of shadowy energy.", me);
                break;
            }
        }
        
        //Screen Reader Support. Tells screen reader users in the room, briefly, what we are attacking.
        //attacker = query_current_attacker();
        
        /*
        if(attacker && userp(me))
        {
            object *readers;
            
            readers = filter_array(all_inventory(environment(me)), (: $1->query("reader") :));
            
            foreach(object person in readers)
                tell_object(person, query_cap_name() + " is fighting " + attacker->query_cap_name() + ".");
        }
        */

        if(is_undead())
        {
            remove_property("rend");
        }
        
        if(PLAYER_D->immunity_check(me, "rend"))
        remove_property("rend");

        if(query_property("rend"))
        {
            tell_room(environment(me), "%^RED%^BOLD%^" + query_cap_name() + "'s wounds bleed profusely!%^RESET%^", ({ me }));
            tell_object(me, "%^RED%^BOLD%^Your wounds bleed profusely!%^RESET%^");
            cause_typed_damage(me, "torso", roll_dice(query_property("rend"), query_level() / 5 + 1), "untyped");
            set_property("rend", -1);
            if(query_property("rend") <= 0)
            {
                tell_room(environment(me), "%^WHITE%^BOLD%^" + query_cap_name() + "'s wounds stop bleeding.%^RESET%^", ({ me }));
                tell_object(me, "%^WHITE%^BOLD%^Your wounds stop bleeding.%^RESET%^");
                remove_property("rend");
            }
        }

        if (is_vampire()) {
            if (is_in_sunlight()) {
                int todamage = query_max_hp() / 4 + 1;
                if (!query_property("sunlight_umbrella")) {
                    if (query_hp() < -(query_max_hp() * 4 / 5)) {
                        add_death("Sunlight");
                        die();
                    }else {
                        cause_typed_damage(me, "torso", todamage, "divine");
                        tell_object(me, "%^ORANGE%^The sun burns your putrid flesh!");
                    }
                }
            }
            if (!random(10)) {
                if (is_vampire()) {
                    if (query_bloodlust() < (5000)) {
                        write("%^RED%^Bloodlust drives you insane.");
                        tell_room(environment(me), "%^RED%^" + query_cap_name() + "'s eyes glow dark red.", me);
                    }
                }
            }
        }
    }

    living_ticker++;
}

void init_path()
{
    string* DIR_CLASS;
    string tmp;
    int i;
    string* classhold;

    classhold = query_classes();
    DIR_CLASS = ({});

    for (i = 0; i < sizeof(classhold); i++) {
        DIR_CLASS += ({ DIR_CMDS + "/" + classhold[i] });
        if (classhold[i] == "antipaladin" || classhold[i] == "paladin") {
            DIR_CLASS += ({ DIR_CMDS + "/cavalier" });
        }
        continue;
    }

    if (query("is_assassin")) {
        DIR_CLASS += ({ DIR_CMDS + "/assassin" });
    }
    search_path = DIR_CLASS + ({ DIR_MORTAL_CMDS, DIR_PROF_CMDS, DIR_CLASS_CMDS });
    search_path += ({ "/cmds/feats" });
    if (ambassadorp(this_object()) || wizardp(this_object())) {
        search_path += ({ DIR_AMBASSADOR_CMDS, DIR_SYSTEM_CMDS });
    }
    if (avatarp(this_object()) || wizardp(this_object())) {
        search_path += ({ "/cmds/avatar" });
    }
    if (wizardp(this_object())) {
        if (member_group(getuid(), "over")) {
            search_path += ({ DIR_OVER_CMDS });
        }
        search_path += ({ DIR_CREATOR_CMDS });
        if (file_size(user_path(query_name()) + "bin") == -2) {
            search_path += ({ user_path(query_name()) + "bin" });
        }
        if (archp(this_object())) {
            search_path += ({ DIR_ADMIN_CMDS, DIR_OVER_CMDS });
        }
        if (member_group(getuid(), "law_c")) {
            search_path += ({ DIR_LAW_CMDS });
        }
    }
}

int move(mixed dest)
{
    int hold;
    object myCastedSpell;

    if (!objectp(this_object())) {
        return MOVE_OK;
    }

    if (draggee && objectp(draggee)) {
        if (draggee->query_ghost()) {
            draggee = 0;
        } else {
            if (environment(draggee) != environment(this_object())) {
                draggee = 0;
            }
        }
    }

    hold = ::move(dest);

    if (!objectp(this_object())) {
        return MOVE_OK;
    }

    if (hold == MOVE_OK) {
        if (in_vehicle && objectp(in_vehicle)) {
            in_vehicle->move(dest);
        }
        if (draggee && objectp(draggee)) {
            if (draggee->query_ghost()) {
                draggee = 0;
            } else {
                draggee->move_player(dest,draggee->query_cap_name() + " is dragged along.");
            }
        }

        if (!objectp(this_object())) {
            return MOVE_OK;
        }

        if (objectp(myCastedSpell = query_property("spell_casting"))) {
            if (myCastedSpell->query_has_been_cast()) {
                myCastedSpell->dest_effect();
            } else {
                myCastedSpell->before_cast_dest_effect();
            }
            remove_property("spell_casting");
            if (query_casting()) {
                set_casting(0);
            }
            // TELL PLAYER
            tell_object(this_object(), "Your movement causes your casting to fail!");
        }
    }
    return hold;
}

object query_in_vehicle()
{
    return in_vehicle;
}

void set_in_vehicle(object ob)
{
    set_scrambling(0);
    in_vehicle = ob;
}

void init_stats()
{
    stats = ([]);
}

nomask protected int cmd_hook(string cmd)
{
    string file, verb;
    object shapeshift_restricted_commands;
    if ((verb = query_verb()) != "quit" && (verb = query_verb()) != "second_wind" && (verb = query_verb()) != "stillness_of_mind" && query_paralyzed()) {
        this_player()->send_paralyzed_message("info", this_object());
        //message("my_action", sprintf("%s", (string)this_player()->query_paralyze_message()),
        return 1;
    }

    if(query_property("shapeshifted"))
    {
        if (objectp(shapeshift_restricted_commands = find_object_or_load("/std/races/shapeshifted_races/restricted_commands"))) {
            if (!shapeshift_restricted_commands->allow_shifted_command(this_object(), verb, cmd)) {
                if (!query("no_commands_allowed")) {
                    if (cmd) {
                        tell_object(this_object(), "You can't " + verb + " " + cmd + " while shapeshifted.");
                    }else {
                        tell_object(this_object(), "You can't " + verb + " while shapeshifted.");
                    }
                }
                return 1;
            }
        }
    }

    if (!(file = (string)CMD_D->find_cmd(verb, search_path))) {
        if (!((int)SOUL_D->do_cmd(verb, cmd))) {
            return (int)CHAT_D->do_chat(verb, cmd);
        } else {
            return 1;
        }
    }

    if (strsrch(file, "/cmds/feats/") == 0) {
        if (!FEATS_D->usable_feat(this_object(), file->query_feat_name())) {
            return 0;
        }
    }

    if (this_object()->query_ghost() && !avatarp(this_object())) {
        if (objectp(environment(this_object()))) {
            if (base_name(environment(this_object())) == DEATH_ROOM) {
                return (int)call_other(file, "cmd_" + verb, cmd);
            }else {
                if (verb != "look" && verb != "tell" && verb != "reply" && verb != "pray") {
                    tell_object(this_object(), "You wail miserably.\n");
                    tell_room(environment(this_object()), "A ghost wails miserably.\n", this_object());
                    return 1;
                }
            }
        }else {
            if (verb != "look" && verb != "tell" && verb != "reply" && verb != "pray") {
                tell_object(this_object(), "You wail miserably.\n");
                tell_room(environment(this_object()), "A ghost wails miserably.\n", this_object());
                return 1;
            }
        }
    }
    return (int)call_other(file, "cmd_" + verb, cmd);
}

// Does not appear to be used
// Added to support quest spells
void set_quest_spells(string* spells)
{
    quest_spells = spells;
}

void add_quest_spell(string spell)
{
    if (!pointerp(quest_spells) || !quest_spells) {
        quest_spells = ({});
    }
    quest_spells += ({ spell });
    return;
}

void remove_quest_spell(string spell)
{
    if (member_array(spell, quest_spells) == -1) {
        return;
    }
    quest_spells -= ({ spell });
}

string* query_quest_spells()
{
    if (!quest_spells || quest_spells == ({})) {
        return ({});
    }
    return quest_spells;
}

// Added to deal with priest spell domains.
void set_divine_domain(string* domain)
{
    divine_domain = domain;
}

string* query_divine_domain()
{
    if (!divine_domain || divine_domain == ({})) {
        return ({});
    }
    return divine_domain;
}

string set_inquisition(string str)
{
    inquisition = str;

    return inquisition;
}

string query_inquisition()
{
    return inquisition;
}
/*
string set_druid_circle(string str)
{
    druid_circle = str;
    
    return druid_circle;
}

string query_druid_circle()
{
    return druid_circle;
}
*/

set_paladin_oath(string str)
{
    paladin_oath = str;
    
    return paladin_oath;
}

string query_paladin_oath()
{
    return paladin_oath;
}

void set_acquired_template(string str)
{
    acquired_template = str;
}

string query_acquired_template(string str)
{
    return acquired_template;
}

string query_school()
{
    return school_of_magic;
}

void set_school(string str)
{
    school_of_magic = str;
}

string query_opposing_school()
{
    return school_of_opposition;
}

void set_opposing_school(string str)
{
    school_of_opposition = str;
}

string query_bloodline()
{
    return sorc_bloodline;
}

void set_bloodline(string str)
{
    sorc_bloodline = str;
}

string query_mystery()
{
    return oracle_mystery;
}

void set_mystery(string str)
{
    oracle_mystery = str;
}

string query_discipline()
{
    return psion_discipline;
}

void set_discipline(string str)
{
    psion_discipline = str;
}

string query_fighter_style()
{
    return fighter_style;
}

void set_fighter_style(string str)
{
    fighter_style = str;
}

string query_sphere()
{
    return sphere_of_influence;
}

//No longer used
void set_sphere(string str)
{
    sphere_of_influence = str;
}

int query_spell_interrupted()
{
    return spell_interrupted;
}

void set_spell_interrupted(int x)
{
    spell_interrupted = x;
}

int force_me(string cmd)
{
    string tmp;
    int res;
    if (!objectp(this_object())) {
        return 0;                 // added by *Styx* 11/4/02 for mobs bugging
    }
    if (objectp(previous_object())) {
        tmp = geteuid(previous_object());
    }
//if(tmp != UID_ROOT && tmp != UID_FORCE && tmp != geteuid()) return 0;
    forced = 1;
    res = command(cmd);
    forced = 0;
    return res;
}

int query_ok_to_heal()
{
    return ok_to_heal;
}

void adjust_ok_to_heal(int x)
{
    ok_to_heal += x;
}

void do_healing(int x)
{
    int tmp;

    if (query_poisoning()) {
        set_heal_rate(-(query_poisoning() / 9 + 1));
    }
    ok_to_heal = player_age + TIME_TO_HEAL;
}

int calculate_healing()
{
    object me = this_object();
    string msg;
    if (query_intox() && !(query_property("inactive")))
    {
        healing["intox"]--;
        if (is_undead())
        {
            healing["intox"] = 0;
        }
        if (query_race() == "soulforged")
        {
            healing["intox"] = 0;
        }
        if (healing["intox"] < HEALING_FORMULA / 3)
        {
            tolerance_flag = 0;
        }
        if (healing["intox"] < 0)
        {
            healing["intox"] = 0;
        }
        if (!healing["intox"])
        {
            write("You are not intoxicated..");
        }
        else if ((healing["intox"] > HEALING_FORMULA) && !query_tripped() && !query_unconscious())
        {
            if (!random(5))
            {
                write("You begin to be sick and vomit from your excessive drinking.");
                tell_room(environment(me), this_player()->query_cap_name() + " begins to be sick and vomit from the drinking.", me);
                do_damage("torso", roll_dice(1, 4));
                set_tripped(1, "The alcohol is revolting against you.");
                healing["intox"] -= roll_dice(4, 5);
            } 
            else if (!random(4))
            {
                write("You feel a wave of nausea that suddenly brings a blanket of darkness.");
                tell_room(environment(me), query_cap_name() + " suddenly gets a blank look on " + query_possessive() + " face.", me);
                write("You pass out cold into your drink.");
                tell_room(environment(me), query_cap_name() + " passes out cold, face first into " + query_possessive() + " drink.", me);
                set_unconscious((healing["intox"] - HEALING_FORMULA) / 70 + 1, "You have passed out cold, you're sleeping off part of your drinking binge.");
                if (query_unconscious() > 8)
                {
                    set_unconscious(8, "You have passed out cold, you're sleeping off part of your drinking binge.");
                }
                healing["intox"] = HEALING_FORMULA;
            }
            if (!tolerance_flag)
            {
                tolerance += 1;
                tolerance_flag = 1;
            }
        }
        else if (!userp(me))
        {
            if (2 > random(101))
            {
                string* drunkemotes = ({ "stumble", "hiccup", "look", "burp", "grin", "blink", "sway" });
                msg = drunkemotes[random(sizeof(drunkemotes))];
                if (userp(me))
                {
                    tell_object(me, "You " + msg + (msg == "look" ? " drunk." : "."));
                    tell_room(environment(me), "" + this_player()->query_cap_name() + " " + msg + "s" + (msg == "look" ? " drunk." : "."), me);
                }
                else {
                    write("You " + msg + (msg == "look" ? " drunk." : "."));
                    say(query_cap_name() + " " + msg + "s" + (msg == "look" ? " drunk." : "."));
                }
            }
        }
    }

    if (!(query_property("sustenance") ||
          query_property("inactive") ||
          FEATS_D->usable_feat(me, "timeless body") ||
          is_undead() ||
          query_race() == "soulforged"
          ))
    {
        healing["stuffed"]--;
        healing["quenched"]--;
        if (healing["stuffed"] < 0)
        {
            healing["stuffed"] = 0;
        }
        if (healing["quenched"] < 0)
        {
            healing["quenched"] = 0;
        }
    }

    if (query_bloodlust())
    {
        if (is_vampire())
        {
            if (!query_property("inactive"))
            {
                healing["bloodlust"] -= random(2) + 1;
                if (healing["bloodlust"] < 0) {
                    healing["bloodlust"] = 0;
                }

                if (userp(me))
                {
                    gmcp_update_character("survival", ([ "bloodlust": "" + healing["bloodlust"] ]));
                }
            }
        }
    }

    if (query_poisoning())
    {
        add_poisoning(-1);
    }

    if (userp(me))
    {
        gmcp_update_character("survival", ([ "intox": "" + healing["intox"], "hunger": "" + healing["stuffed"], "thirst": "" + healing["quenched"] ]));
    }

    return query_intox() + query_stuffed() + query_quenched();
}

void set_party(string str)
{
    party = str;
}

void add_poisoning(int x)
{
    //if(!healing) healing = ([]);
    //healing["poisoning"] += x;
    //if(healing["poisoning"] < 0) healing["poisoning"] = 0;*/
    //log what objects are calling this and
    //hopefully making it so that they
    //use the new poison system - Saide
    if (x < 1) {
        return;
    }
    POISON_D->ApplyPoison(this_object(), "any", previous_object());
}

void setPoisoner(object person)
{
    if (!objectp(poisoner)) {
        poisoner = person;
    }
}

object queryPoisoner()
{
    return poisoner;
}

void set_stats(string str, int x)
{
    if (x > 55 && str != "exceptional_strength") {
        x = 55;
    }
    stats[str] = x;
    if (str == "strength") {
        set_max_internal_encumbrance(MAX_ENCUMBRANCE[x]);
    }
}

void set_hidden(int xxx)
{
    if (!xxx && query_hidden()) {
        ::set_hidden(xxx);
        if (wizardp(this_object())) {
            tell_object(this_object(), "You step out of the shadows.\n");
        }
        tell_room(environment(this_object()), query_mvis(), this_object());
    } else if (xxx && !query_hidden()) {
        tell_room(environment(this_object()), query_minvis() + "", this_object());
        ::set_hidden(xxx);
        if (wizardp(this_object())) {
            tell_object(this_object(), "You fade into the shadows.\n");
        }
    }
}

void set_magic_hidden(int xxx)
{
    if (!xxx && query_magic_hidden()) {
        ::set_magic_hidden(xxx);
        if (wizardp(this_object())) {
            tell_object(this_object(), "You step out of the shadows.\n");
        }
        tell_room(environment(this_object()), query_mvis(), this_object());
    } else if (xxx && !query_magic_hidden()) {
        if (wizardp(this_object())) {
            tell_object(this_object(), "You fade into the shadows.\n");
        }
        tell_room(environment(this_object()), query_minvis() + "", this_object());
        ::set_magic_hidden(xxx);
    }
}

void set_invis()
{
    if (!objectp(this_object())) {
        return;
    }
    if (avatarp(this_object())) {
        if (query_invis()) {
            invis = 0;
            ::set_hidden(0);
            ::set_magic_hidden(0);
            if (wizardp(this_object())) {
                tell_object(this_object(), "You step out of the shadows.\n");
            }
            tell_room(environment(this_object()), query_mvis(), this_object());
        } else {
            if (wizardp(this_object())) {
                tell_object(this_object(), "You fade into the shadows.\n");
            }
            tell_room(environment(this_object()), query_minvis() + "", this_object());
            invis = 1;
        }
        return;
    } else {
        if (query_hidden() || query_magic_hidden()) {
            set_hidden(0);
            set_magic_hidden(0);
        } else {
            set_hidden(0);
            set_magic_hidden(1);
        }
    }
}

void set_description(string str)
{
    description = str;
}

void set_background(string str)
{
    background = str;
}

void add_sight_bonus(int x)
{
    sight_bonus += x;
}

void set_sight_bonus(int x)
{
    sight_bonus = x;
}

void add_search_path(string dir)
{
    if (member_array(dir, search_path) == -1) {
        search_path += ({ dir });
    }
}

void delete_search_path(string dir)
{
    if (getuid(previous_object()) != UID_ROOT) {
        return;
    }
    if (member_array(dir, search_path) != -1) {
        search_path -= ({ dir });
    }
}

nosave private int traceFlag;


void add_alignment(int x)
{
    return 1;
}

void adj_alignment(int x)
{

}

void set_alignment(int x)
{
    player_data["general"]["alignment"] = x;
}

int add_intox(int x)
{
    object me = this_object();

    if (x > 0)
    {
        x = x * 3 + x / 2;
    }
    if (healing["intox"] > HEALING_FORMULA)
    {
        return 0;
    }
    healing["intox"] += x;
    if (healing["intox"] < 0)
    {
        healing["intox"] = 0;
    }

    if (userp(me))
    {
        gmcp_update_character("survival", ([ "intox": "" + healing["intox"] ]));
    }

    return 1;
}

int add_stuffed(int x)
{
    object me = this_object();
    if (x > 0)
    {
        x = x * 250;
    }
    if ((HEALING_FORMULA - healing["stuffed"]) < (HEALING_FORMULA / 6))
    {
        return 0;
    }
    if (x + healing["stuffed"] > HEALING_FORMULA)
    {
        healing["stuffed"] = HEALING_FORMULA;
    } 
    else
    {
        healing["stuffed"] += x;
    }
    if (healing["stuffed"] < 0)
    {
        healing["stuffed"] = 0;
    }

    if (userp(me))
    {
        gmcp_update_character("survival", ([ "hunger": "" + healing["stuffed"] ]));
    }

    return 1;
}

int set_stuffed(int x)
{
    healing["stuffed"] = x;
    return 1;
}

int set_quenched(int x)
{
    object me = this_object();

    healing["quenched"] = x;

    if (userp(me))
    {
        gmcp_update_character("survival", ([ "thirst": "" + healing["quenched"] ]));
    }

    return 1;
}

int add_quenched(int x)
{
    object me = this_object();

    if (x > 0)
    {
        x = x * 250;
    }
    if ((HEALING_FORMULA - healing["quenched"]) < (HEALING_FORMULA / 6))
    {
        return 0;
    }
    if (x + healing["quenched"] > HEALING_FORMULA)
    {
        healing["quenched"] = HEALING_FORMULA;
    }
    else
    {
        healing["quenched"] += x;
    }
    if (healing["quenched"] < 0)
    {
        healing["quenched"] = 0;
    }

    if (userp(me))
    {
        gmcp_update_character("survival", ([ "thirst": "" + healing["quenched"] ]));
    }

    return 1;
}

int add_bloodlust(int x)
{
    object me = this_object();

    if (!is_vampire())
    {
        return 1;
    }
    if (x + healing["bloodlust"] > 20000)
    {
        healing["bloodlust"] = 20000;
    } else
    {
        healing["bloodlust"] += x;
    }
    if (healing["bloodlust"] < 0)
    {
        healing["bloodlust"] = 0;
    }

    if (userp(me))
    {
        gmcp_update_character("survival", ([ "bloodlust": "" + healing["bloodlust"] ]));
    }

    return 1;
}

void add_stat_bonus(string stat, int amount)
{
    if (!stat_bonus) {
        stat_bonus = ([]);
    }
    if (stat_bonus[stat]) {
        stat_bonus[stat] += amount;
    } else {
        stat_bonus[stat] = amount;
    }
    if (!stat_bonus[stat]) {
        map_delete(stat_bonus, stat);
    }
    if (interactive(this_object()) && stat == "strength") {
        do_encumbrance();
    }
}

// added to be able to pull description for corpses or anything needing it without all the added stuff below  *Styx*  2/15/03
string base_desc()
{
    return ::query_long();
}

string query_long(string unused)
{
    object* inv, shape;
    string* tmp;
    string pre, stuff, extra, reg, short, sub, race, the_race, dis;
    int i, x, height, weight;

    if (this_object()->query_ghost()) {
        return "An ethereal presence.\n";
    }

    if (objectp(shape = query_property("shapeshifted"))) {
        the_race = (string)shape->query_shape_race();
    } else if (objectp(shape = query_property("altered"))) {
        the_race = (string)shape->query_shape_race();
    }
    if (!the_race) {
        the_race = query("race");
    }
    reg = "";
    pre = "%^CYAN%^%^BOLD%^You look over the " + the_race + ".%^RESET%^\n";

    if (::query_long("junk")) {
        pre += "%^CYAN%^" + ::query_long("junk") + "%^RESET%^\n";
    }

    if (this_object()->query_disguised() || (avatarp(this_object()) && this_object()->query_disguised())) {
        if (description) {
            pre += "%^GREEN%^" + capitalize(query_vis_name()) + " " + description + "%^RESET%^\n";
        }
    }else {
        if (description) {
            if (userp(this_object())) {
                pre += "%^GREEN%^" + capitalize(getNameAsSeen(this_player())) + " " + description + "%^RESET%^\n";
            }else {
                pre += "%^GREEN%^" + capitalize(query_name()) + " " + description + "%^RESET%^\n";
            }
        }
    }
    /* sub = capitalize(query_subjective()); */
    
    sub = "They";
    /*
    if (userp(this_object())) {
        height = query_player_height();
        if (objectp(shape)) {
            height = (int)shape->query_shape_height();
        }
        weight = query_player_weight();
        if (objectp(shape)) {
            weight = (int)shape->query_shape_weight();
        }
        if (query_size_bonus()) {
            height = to_int(height * pow(2, query_size_bonus()));
            weight = to_int(weight * pow(pow(2, query_size_bonus()), 2));
        }
        reg += "%^BOLD%^" + sub + " are approximately " + height + " inches (" + height / 12 + "ft " + height % 12 + "in) tall and " + weight + " pounds.%^RESET%^\n";
    }
    */

    x = ((player_data["general"]["hp"] * 100) / max( ({ player_data["general"]["max_hp"], 1 }) ));
    //x = ((player_data["general"]["hp"] * 100) / (player_data["general"]["max_hp"]) + 1);
    if (x > 90) {
        reg += "%^YELLOW%^" + sub + " are in top shape.%^RESET%^\n";
    } else if (x > 75) {
        reg += "%^WHITE%^%^BOLD%^" + sub + " are in decent shape.%^RESET%^\n";
    } else if (x > 60) {
        reg += "%^WHITE%^" + sub + " are slightly injured.%^RESET%^\n";
    } else if (x > 45) {
        reg += "%^MAGENTA%^" + sub + " are hurting.%^RESET%^\n";
    } else if (x > 30) {
        reg += "%^ORANGE%^" + sub + " are badly injured.%^RESET%^\n";
    } else if (x > 15) {
        reg += "%^RED%^%^BOLD%^" + sub + " are terribly injured.%^RESET%^\n";
    } else {
        reg += "%^RED%^" + sub + " are near death.%^RESET%^\n";
    }
    stuff = "";
    extra = "";
    
    if(!query_property("hidden inventory"))
        stuff = describe_item_contents(({}));
    
    if (stuff == "") {
        reg += sub + " are empty handed.\n";
    } else {
        stuff = " " + stuff;
        stuff = replace_string(stuff, ",", "\n");
        stuff = replace_string(stuff, " are here.", "");
        reg += "%^GREEN%^%^BOLD%^" + sub + " are carrying:%^RESET%^\n" +
               "%^GREEN%^" + stuff + "%^RESET%^";
    }

    if (extra != "") {
        pre = pre + extra;
    }
    reg = pre + reg;
    return reg;
}

string query_desc(string unused)
{
    string pre, short, reg, sub, the_race;
    int x, height, weight;
    object shape;

    if (this_object()->query_ghost()) {
        return "An ethereal presence.\n";
    }
    reg = "";

    if (objectp(shape = query_property("shapeshifted"))) {
        the_race = (string)shape->query_shape_race();
    }
    if (objectp(shape = query_property("altered"))) {
        the_race = (string)shape->query_shape_race();
    }
    if (!the_race) {
        the_race = query("race");
    }

    pre = "%^CYAN%^%^BOLD%^You look over the " + the_race + ".%^RESET%^\n";
    if (::query_long("junk")) {
        pre += "%^CYAN%^" + ::query_long("junk") + "%^RESET%^\n";
    }
    if (avatarp(this_object()) && this_object()->query_disguised()) {
        if (description) {
            pre += "%^GREEN%^" + capitalize(query_vis_name()) + " " + description + "%^RESET%^\n";
        }
    } else {
        if (description) {
            if (userp(this_object())) {
                pre += "%^GREEN%^" + capitalize(getNameAsSeen(this_player())) + " " + description + "\n";
            } else {
                pre += "%^GREEN%^" + capitalize(query_name()) + " " + description + "\n";
            }
        }
    }
    sub = capitalize(query_subjective());
    
    /*
    if (userp(this_object())) {
        height = query_player_height();
        if (objectp(shape)) {
            height = (int)shape->query_shape_height();
        }
        weight = query_player_weight();
        if (objectp(shape)) {
            weight = (int)shape->query_shape_weight();
        }
        reg += "%^BOLD%^" + sub + " are approximately " + height + " inches (" + height / 12 + "ft " + height % 12 + "in) tall and " + weight + " pounds.%^RESET%^\n";
    }
    */

    x = ((player_data["general"]["hp"] * 100) /( player_data["general"]["max_hp"] + 1));
    if (x > 90) {
        reg += "%^YELLOW%^" + sub + " is in top shape.%^RESET%^\n";
    } else if (x > 75) {
        reg += "%^WHITE%^%^BOLD%^" + sub + " is in decent shape.%^RESET%^\n";
    } else if (x > 60) {
        reg += "%^WHITE%^" + sub + " is slightly injured.%^RESET%^\n";
    } else if (x > 45) {
        reg += "%^MAGENTA%^" + sub + " is hurting.%^RESET%^\n";
    } else if (x > 30) {
        reg += "%^ORANGE%^" + sub + " is badly injured.%^RESET%^\n";
    } else if (x > 15) {
        reg += "%^RED%^%^BOLD%^" + sub + " is terribly injured.%^RESET%^\n";
    } else {
        reg += "%^RED%^" + sub + " is near death.%^RESET%^\n";
    }
    return pre + reg;
}

int query_stats(string stat)
{
    int x, y, z, num, res;

    if (stat_bonus) {
        x = stat_bonus[stat];
    } else {
        x = 0;
    }

    z = (int)POISON_D->query_poison_effect(this_object(), stat);
    if (!intp(z)) {
        z = 0;
    }
    y = query_base_stats(stat);
    if (!intp(y)) {
        y = 0;
    }
    if (!intp(x)) {
        x = 0;
    }
    //return stats[stat] + x;
    res = x + y + z;
    res = WORLD_EVENTS_D->monster_modification_event(res, stat, this_object());
    if (is_vampire()) {
        if (stat == "strength" ||
            stat == "charisma") {
            int blst = (20000 - (int)query_bloodlust()) / 2000 - 1;
            res -= blst < 0 ? 0 : blst;
        }
    }

    if(is_class("cleric"))
    {
        if(stat == "strength" && member_array("strength", query_divine_domain()) >= 0)
            res += 2;
    }
    
    if(query_class_level("oracle") >= 10)
    {
        if(stat == "strength" && query_mystery() == "dragon")
            res += 2;
    }

    if(stat == "charisma" && FEATS_D->usable_feat(this_object(), "spiritual body"))
        res += 2;

    res += EQ_D->gear_bonus(this_object(), stat);

    if (res > 29 && userp(this_object())) {
        return 30;
    }
    if (res > 39) {
        return 40;
    }
    if (res < 1) {
        return 1;
    } else {
        return res;
    }
}

int query_base_stats(string stat)
{
    int amt;
    
    if (!stats || !stats[stat])
        return 0;
        
    amt = stats[stat];
    
    return amt;
}

int query_formula()
{
    return HEALING_FORMULA;
}

nomask int query_forced()
{
    return forced;
}

string* query_search_path()
{
    if (previous_object() != this_object() && geteuid(previous_object()) != UID_ROOT) {
        return search_path + ({});
    } else {
        return search_path;
    }
}

int query_invis()
{
    return (invis || query_magic_hidden() || query_hidden());
}

int query_free_exp()
{
    int used;
    int i;
    for (i = 0; i < sizeof(query_classes()); i++) {
        used += ADVANCE_D->get_real_exp(query_class_level(query_classes()[i]), query_classes()[i], this_object());
    }
    return query_exp() - used;
}

int query_alignment()
{
    if (query_property("align mask")) {
        return query_property("align mask");
    } else if (query_property("hidden alignment")) {
        return query_property("hidden alignment");
    } else {
        return player_data["general"]["alignment"];
    }
}

int query_true_align()
{
    return player_data["general"]["alignment"];
}

int query_intox()
{
    if (healing && healing != ([])) {
        return healing["intox"];
    } else {
        return 0;
    }
}

mapping query_healing()
{
    return healing;
}

int query_stuffed()
{
    if (healing) {
        return healing["stuffed"];
    } else {
        return 0;
    }
}

int query_quenched()
{
    if (healing) {
        return healing["quenched"];
    } else {
        return 0;
    }
}

int query_bloodlust()
{
    if (!is_vampire()) {
        return 0;
    }
    if (healing) {
        return healing["bloodlust"];
    } else {
        return 0;
    }
}

int query_poisoning()
{
    //old poison code, no longer applies - Saide
    return 0;
    //if(healing) return healing["poisoning"];
    //else return 0;
}

string query_party()
{
    return party;
}

//hidden alignment added because align mask did not work for
//some reason.  Trying this. Circe 8/1/05
string query_al_title()
{
    int al;

    al = player_data["general"]["alignment"];
    if (query_property("align mask")) {
        al = query_property("align mask");
    }
    if (query_property("hidden alignment")) {
        al = query_property("hidden alignment");
    }
    if (al == 1) {
        return "Lawful Good";
    }
    if (al == 2) {
        return "Lawful Neutral";
    }
    if (al == 3) {
        return "Lawful Evil";
    }
    if (al == 4) {
        return "Neutral Good";
    }
    if (al == 5) {
        return "True Neutral";
    }
    if (al == 6) {
        return "Neutral Evil";
    }
    if (al == 7) {
        return "Chaotic Good";
    }
    if (al == 8) {
        return "Chaotic Neutral";
    }
    if (al == 9) {
        return "Chaotic Evil";
    }
}

int can_fly()
{
    object file;
    string* flyraces =    ({ "deva", "faery", "nightwing", "imp", "strix", "dragonkin" });
    string* flysubraces = ({ "fey'ri", "rock gnome", "trixie", "sildruath", "avian"});
    string* flyprofiles = ({ "druid_bird_999", "druid_dragon_999", "mage_red_dragon_999", "mage_demon_999", "vampire_bat_999", "vampire_vampire_999", "vampire_varghulf_999", "mage_pixie_999" });

    if (!objectp(this_object()))
        return 0;

    if(is_deva())
        return 1;   
    if(query_bloodline() == "celestial" && query_class_level("sorcerer") > 30)
        return 1;
    if(query_bloodline() == "infernal" && query_class_level("sorcerer") > 30)
        return 1;
    if(query_bloodline() == "draconic" && query_class_level("sorcerer") > 30)
        return 1;
    if(FEATS_D->usable_feat(this_object(), "air totem"))
        return 1;
    if(query_property("flying"))
        return 1;
    if (!catch(file = load_object("/std/races/" + query("race"))) && objectp(file))
        if (file->is_flying())
            return 1;
        
    return member_array(query_visual_race(), flyraces) >= 0 || member_array(query("subrace"), flysubraces) >= 0 || member_array(query("relationship_profile"), flyprofiles) >= 0;
}
    
int query_sight_bonus()
{
    string temp;
    object file;
    int temp_bonus = 0;
    
    temp = query_acquired_template();
    
    if(temp)
    {
        file = find_object("/std/acquired_template/" + temp + ".c");
        
        if(objectp(file))
            temp_bonus = (file->sight_bonus());
    }
    
    if(strlen(temp = query("race")) && temp != "unborn")
    {   
        if(file_size("/std/races/" + temp + ".c") > 0 && objectp(file = load_object("/std/races/" + temp + ".c")))
            temp_bonus += file->sight_bonus(query("subrace"));
    }

    if(member_array("cavern", query_divine_domain()) >= 0 ||
       member_array("moon", query_divine_domain()) >= 0 ||
       member_array("darkness", query_divine_domain()) >= 0)
       temp_bonus += 2;
              
    return (EQ_D->gear_bonus(this_object(), "sight bonus") + sight_bonus + temp_bonus);
}

int query_age()
{
    return player_age;
}

void set_age(int x)
{
    player_age = x;
}

string* query_all_stats()
{
    return keys(stats);
}

string query_description()
{
    return description;
}

string query_background()
{
    return background;
}

int remove()
{
    int i;
    object me;
    object* inv;

    if (!objectp(me = this_object()))
        return 0;

    inv = all_inventory(me);
    for (i = 0; i < sizeof(inv); i++)
    {
        if (!inv[i])
            continue;

        if (inv[i]->drop() && inv[i])
            inv[i]->remove();
            /* Some objects call remove() in drop() */
    }

    if (objectp(previous_object()))
        log_file("living/remove", query_name() + " removed " + file_name(previous_object()) + ".\n");
    else
        log_file("living/remove", query_name() + " removed " + file_name(this_object()) + ".\n");

    ::remove();

    return 1;
}

//No longer used
void adjust_biorhythms()
{
    float freq, temps;

//    What the F*CK is this???   - Kai
//    temps = to_float(player_age/1000);
//    freq = to_float( (int)query_stats("wisdom"));
//    spiritual = to_int( 5.0 * sin(freq*temps) );
//    freq = to_float( (int)query_stats("strength") );
//    physical = to_int( 5.0 * sin(freq*temps) );
}

void set_gender(string str)
{
    if (str != "male" && str != "female" && str != "neuter" && str != "other") {
        return;
    }
    gender = str;
}

string query_gender()
{
    return (gender ? gender : "neuter");
}

string query_subjective()
{
    return nominative(gender);
}

string query_possessive()
{
    return possessive(gender);
}

string query_objective()
{
    return objective(gender);
}

//This should be in combat.c
// For attack and damage bonuses -- Thorn 950620
/////////////////////////////

void set_attack_bonus(int a)
{
    abonus = a;
}

void add_attack_bonus(int a)
{
    abonus += a;
}

void set_damage_bonus(int a)
{
    dbonus = a;
}

void add_damage_bonus(int a)
{
    dbonus += a;
}

int query_attack_bonus()
{
    int enc, bonus;
    int ret, x, favored;
    object* weap;
    object attacker;

    weap = query_wielded();
    for (x = 0; x < sizeof(weap); x++) {
        enc += weap[x]->query("enchantment");
    }
    bonus = abonus;
    ret = bonus + enc;
    ret += EQ_D->gear_bonus(this_object(), "attack bonus");
    if (query_invis()) {
        ret += 2;
    }
    if (FEATS_D->usable_feat(this_object(), "weapon training")) {
        ret += query_prestige_level("fighter") / 10 + 1;
    }

    if (FEATS_D->usable_feat(this_object(), "weapon focus")) {
        ret += 1;
    }

    if (FEATS_D->usable_feat(this_object(), "greater weapon focus")) {
        ret += 1;
    }

    if (FEATS_D->usable_feat(this_object(), "epic weapon focus")) {
        ret += 1;
    }

    if(is_class("psywarrior") && query("available focus"))
        ret += 1;

    attacker = query_current_attacker();

    //Added by Tlaloc to handle favored enemies for Rangers
    if (FEATS_D->usable_feat(this_object(), "favored enemy") && attacker)
    {

        if(is_favored_enemy(attacker))
        {
            ret += 2;

            //Favored enemy improves with additional feats
            FEATS_D->usable_feat(this_object(), "second favored enemy") && ret += 2;
            FEATS_D->usable_feat(this_object(), "third favored enemy") && ret += 2;
        }

        //Are we fighting our quarry?
        if(query_property("quarry") == attacker)
        {
            FEATS_D->usable_feat(this_object(), "quarry") && ret += 2;
            FEATS_D->usable_feat(this_object(), "improved quarry") && ret += 2;
        }
    }
    
    if(is_class("druid"))
    {
        if(FEATS_D->has_feat(this_object(), "guardian of nature"))
        {
            if(!USER_D->is_valid_terrain(environment(this_object())->query_terrain(), "city"))
                ret += 2;
        }
    }
    
    if(attacker && query_race() == "dwarf")
    {
        if(query("subrace") != "gray dwarf")
        {
            if(USER_D->is_valid_enemy(attacker->query_race(), "orcs") || USER_D->is_valid_enemy(attacker->query_race(), "goblins"))
                ret += 1;
        }
        else
        {
            if(attacker->query_race() == "drow")
                ret += 1;
        }
    }
    
    if(attacker && query_race() == "gnome")
    {
        if(query("subrace") == "forest gnome" || query_race() == "rock gnome")
        {
            if(USER_D->is_valid_enemy(attacker->query_race(), "lizardfolk") || USER_D->is_valid_enemy(attacker->query_race(), "goblins"))
                ret += 1;
        }
        else if(query("subrace") == "deep gnome")
        {
            if(USER_D->is_valid_enemy(attacker->query_race(), "dwarves") || USER_D->is_valid_enemy(attacker->query_race(), "lizardfolk"))
                ret += 1;
        }        
    }
    
    if(attacker && query_race() == "strix")
    {
        if(USER_D->is_valid_enemy(attacker->query_race(), "humans"))
            ret += 1;
    }
   
    if(attacker && attacker->is_vulnerable_to(this_object()))
    {
        if(FEATS_D->usable_feat(this_object(), "prime strike"))
        {
            ret += 1;
            
            if(FEATS_D->usable_feat(this_object(), "inevitable steel"))
                ret += 2;
        }
    }
    
    if(is_class("warlock"))
    {
        if(query_property("eldritch strikes") && sizeof(weap))
        {
            object strikes;
        
            strikes = query_property("eldritch strikes");
        
            if(objectp(strikes))
            {
                if(weap[0] == strikes)
                    ret += 1;
                else if(sizeof(weap) == 2 && weap[1] == strikes)
                    ret += 1;
            }
        }
        
        ret += query("available burn");
    }            

    //Inquisitor Bane
    if(query_property("bane weapon") && sizeof(weap) && attacker)
    {
        int valid;
        string *ids = attacker->query_id();
        mixed *bane = query_property("bane weapon");

        if(sizeof(bane) == 2 && weap[0] == bane[0])
        {
            foreach(string id in ids)
            {
                if(USER_D->is_valid_enemy(id, bane[1]))
                valid = 1;
            }
        }

        if(valid)
            ret += 2;
    }
    
    //Paladin capstone
    //ret += query_property("hand of the gods");

    if (FEATS_D->usable_feat(this_object(), "true strikes") &&
        validate_combat_stance("one hander")) {
        ret += 3;
    }
    
    //Crimson Templar
    if(FEATS_D->usable_feat(this_object(), "fiendish studies") && attacker)
    {
        string *ids = attacker->query_id();
        int valid;
        
        if(member_array(attacker->query_true_align(), ({ 3, 6, 7, 8, 9 })) >= 0)
        {     
            foreach(string id in ids)
            {
                if(USER_D->is_valid_enemy(id, "outsiders"))
                    valid = 1;
            }
        
            if(valid)
                ret += 2;
        }
    }

    if(FEATS_D->usable_feat(this_object(), "smite the lifeless") && attacker && attacker->is_undead())
        ret += 2;
    
    if(query_race() == "nightwing" && total_light(environment(this_object())) > -1)
        ret -= 4;

    return ret;
}

int query_damage_bonus()
{
    int enc, bonus;
    int ret, x, favored;
    object* weap;
    object attacker;

    bonus = dbonus;
    ret = bonus;// + enc;
    ret += EQ_D->gear_bonus(this_object(), "damage bonus");
    if (FEATS_D->usable_feat(this_object(), "weapon training")) {
        ret += query_prestige_level("fighter") / 10 + 1;
    }

    if (FEATS_D->usable_feat(this_object(), "weapon specialization")) {
        ret += 2;
    }

    if (FEATS_D->usable_feat(this_object(), "greater weapon specialization")) {
        ret += 2;
    }

    if (FEATS_D->usable_feat(this_object(), "epic weapon specialization")) {
        ret += 2;
    }

    if(is_class("psywarrior") && query("available focus"))
        ret += 1;

    attacker = query_current_attacker();

    //Added by Tlaloc to handle favored enemies for Rangers
    if (FEATS_D->usable_feat(this_object(), "favored enemy"))
    {

        if(is_favored_enemy(attacker))
        {
            ret += 2;

            //Favored enemy improves with additional feats
            FEATS_D->usable_feat(this_object(), "second favored enemy") && ret += 2;
            FEATS_D->usable_feat(this_object(), "third favored enemy") && ret += 2;
        }
      
        //Are we fighting our quarry?
        if(query_property("quarry") == attacker)
        {
            FEATS_D->usable_feat(this_object(), "quarry") && ret += 2;
            FEATS_D->usable_feat(this_object(), "improved quarry") && ret += 2;
        }
    }
    
    if(is_class("druid"))
    {
        if(FEATS_D->has_feat(this_object(), "guardian of nature"))
        {
            if(!USER_D->is_valid_terrain(environment(this_object())->query_terrain(), "city"))
                ret += 2;
        }
    }
    
    if(FEATS_D->is_active(this_object(), "rending blows"))
        ret -= 5;

    if(FEATS_D->usable_feat(this_object(), "smite the lifeless") && attacker && attacker->is_undead())
        ret += 2;

    weap = query_wielded();

    if(query_property("bane weapon") && sizeof(weap) && attacker)
    {
        int valid;
        string *ids = attacker->query_id();
        mixed *bane = query_property("bane weapon");

        if(sizeof(bane) == 2 && weap[0] == bane[0])
        {
            foreach(string id in ids)
            {
                if(USER_D->is_valid_enemy(id, bane[1]))
                valid = 1;
            }
        }

        if(valid)
            ret += 2;
    }
    
    //Crimson Templar
    if(FEATS_D->usable_feat(this_object(), "fiendish studies") && attacker)
    {
        string *ids = attacker->query_id();
        int valid;
        
        if(member_array(attacker->query_true_align(), ({ 3, 6, 7, 8, 9 })) >= 0)
        {     
            foreach(string id in ids)
            {
                if(USER_D->is_valid_enemy(id, "outsiders"))
                    valid = 1;
            }
        
            if(valid)
                ret += 2;
        }
    }

    //Plant domain gets +2 to unarmed fighting damage
    if (!sizeof(weap) || weap[0] == this_object() || query_property("shapeshifted"))
    {
        if(query_class_level("cleric"))
        {
            if(member_array("plant", query_divine_domain()) >= 0)
                ret += 2;
        }
    }
    
    if(query_race() == "nightwing" && total_light(environment(this_object())) > -1)
        ret -= 4;

    return ret;
}

void init_saving_bonus()
{
    int i;
    save_bonus = ([]);
    //   write("in");
    for (i = 0; i < sizeof(SAVING_THROW); i++) {
        save_bonus[SAVING_THROW[i]] = 0;
    }
    set_property("saving_init", 1);
}

//doesn't seem to be used
void add_saving_bonus(string throw, int bonus)
{
    int i;
    if (!query_property("saving_init")) {
        init_saving_bonus();
    }
    if ((member_array(throw, SAVING_THROW) != -1) || (throw == "all")) {
        if (throw == "all") {
            for (i = 0; i < sizeof(SAVING_THROW); i++) {
                save_bonus[SAVING_THROW[i]] += bonus;
            }
        } else {
            save_bonus[throw] += bonus;
        }
    }
}

int query_saving_bonus(string throw)
{
    int x;
    object attacker;

    if ((!query_property("saving_init")) || (!save_bonus)) {
        init_saving_bonus();
    }
    if (member_array(throw, SAVING_THROW) != -1) {
        return save_bonus[throw] + EQ_D->gear_bonus(this_object(), throw);
    }

    x = EQ_D->gear_bonus(this_object(), throw);

    attacker = query_current_attacker();

    if(FEATS_D->usable_feat(this_object(), "seen it before") && attacker)
    {
        if(is_favored_enemy(attacker))
            x += 6;
    }

    /*
    if(FEATS_D->usable_feat(this_object(), "resist undead") && attacker && attacker->is_undead() && (throw == "fortitude" || throw == "will"))
        x += 4;
    */

    if(FEATS_D->usable_feat(this_object(), "canny defense") && throw == "reflex")
        x += 4;

    return x;
}

void remove_stat_bonuses()
{
    stat_bonus = ([]);
    return;
}

void remove_save_bonuses()
{
    init_saving_bonus();
    return;
}

//int add_named_effect( string unique_name, string type, mapping effects) {
//}

int query_hp_percent()
{
    if (!query_hp() || !query_max_hp()) {
        return 0;
    }
    return (query_hp() * 100) / query_max_hp();
}

object query_draggee()
{
    return draggee;
}

void set_draggee(object drag)
{
    draggee = drag;
}

int detecting_invis()
{
    return detecting_invis || true_seeing || environment(this_object())->query_property("no invis") || avatarp(this_object());
}

void set_detecting_invis(int xx)
{
    detecting_invis = xx;
}

int true_seeing()
{
    if(!objectp(this_object()) || !objectp(environment(this_object())))
        return 0;
    
    return true_seeing || environment(this_object())->query_property("no invis") || avatarp(this_object());
}

void set_true_seeing(int xx)
{
    true_seeing = xx;
}

int query_max_internal_encumbrance()
{
    int res, encumbrance, extra;
    if (!objectp(this_object())) {
        return 0;
    }
    res = query_stats("strength");
    if (res < 0) {
        res = 0;
    }
    extra = query_property("extra encumbrance");
    encumbrance = MAX_ENCUMBRANCE[res];
    return encumbrance + extra;
}

int query_encumbrance_percent()
{
    return (query_true_internal_encumbrance() * 100) / query_max_internal_encumbrance();
}

void increment_stamina(int x)
{
    object me = this_object();

    if (is_undead())
    {
        return;
    }
    used_stamina += x;
    if (used_stamina < 0)
    {
        used_stamina = 0;
    }
    if (query_condition() < 0)
    {
        send_paralyzed_message("info", this_object());
        used_stamina = query_max_stamina();
    }

    if (userp(me))
    {
        gmcp_update_character("vitals", ([ "stamina": "" + (query_max_stamina() - used_stamina), "max_stamina": "" + query_max_stamina() ]));
    }
}

void use_stamina(int x)
{
    object me = this_object();

    if (!x)
    {
        x = 1;
    }
    x = x * (query_encumbrance_percent() / 25 + 1);

    //had redundant code that did the same thing as the following function
    //Saide - June 2016
    increment_stamina(x);

    if (userp(me))
    {
        gmcp_update_character("vitals", ([ "stamina": "" + (query_max_stamina() - used_stamina), "max_stamina": "" + query_max_stamina() ]));
    }
}

void continue_attack()
{
    ::continue_attack();
    if (sizeof(query_attackers()) > 0) {
        set("adreniline", 11);
    } else {
        set("adreniline", query("adreniline") > 0 ? query("adreniline") - 1 : 0);
    }
}

int query_max_stamina()
{
    int bonus;

    bonus = query_property("bonus stamina");
    if (query("adreniline")) {
        return (query_skill("endurance") * 10 + 500 + bonus) * 2;
    } else {
        return query_skill("endurance") * 10 + 500 + bonus;
    }
}

int query_used_stamina()
{
    return used_stamina;
}

void reset_condition()
{
    object me = this_object();

    used_stamina = 0;

    if (userp(me))
    {
        gmcp_update_character("vitals", ([ "stamina": "" + (query_max_stamina() - used_stamina), "max_stamina": "" + query_max_stamina() ]));
    }
}

void set_condition(int x)
{
    object me = this_object();

    used_stamina = x;

    if (userp(me))
    {
        gmcp_update_character("vitals", ([ "stamina": "" + (query_max_stamina() - used_stamina), "max_stamina": "" + query_max_stamina() ]));
    }
}

int query_condition()
{
    return query_max_stamina() - query_used_stamina();
}

int query_condition_percent()
{
    if (query_max_stamina() > 0) {
        return ((query_max_stamina() - query_used_stamina()) * 100) / query_max_stamina();
    } else {
        return 0;
    }
}

string query_condition_string()
{
    string stamina;

    switch (query_condition_percent()) {
    case 1..20:
        stamina = "Collapsing";
        break;
    case 21..40:
        stamina = "Spent";
        break;
    case 41..60:
        stamina = "Tired";
        break;
    case 61..80:
        stamina = "Vigorous";
        break;
    case 81..100:
        stamina = "Energetic";
        break;
    default:
        stamina = "Fainted";
    }

    return stamina;
}

int query_unconscious()
{
    return ::query_unconscious();
}

string query_unconscious_message()
{
    return ::query_unconscious_message();
}

int query_true_invis()
{
    return invis;
}

int set_true_invis()
{
    if (!avatarp(this_object())) {
        set_invis();
        return 0;
    }else {
        return invis = !invis;
    }
}

string knownAs(string str)
{
    return str;
}

void add_protector(object ob)
{
    if (!objectp(ob)) {
        return;
    }
    if (!protectors) {
        protectors = ({});
    }
    protectors += ({ ob });
    protectors = distinct_array(protectors);
}

void remove_protector(object ob)
{
    if (!objectp(ob)) {
        return;
    }
    if (!protectors) {
        protectors = ({});
    }
    protectors -= ({ ob });
    protectors = distinct_array(protectors);
}

object* query_protectors()
{
    if (!protectors) {
        protectors = ({});
    }
    protectors = filter_array(protectors, "is_non_immortal", "/daemon/filters_d.c");
    return protectors;
}

void set_protectors(object* blah)
{
    protectors = blah;
}

void set_time_delay(string name)
{
    if (!time_delay) {
        time_delay = ([]);
    }
    time_delay[name] = time();
}

varargs int query_time_delay(string name, int t)
{
    if (!time_delay) {
        time_delay = ([]);
    }
    if (!time_delay[name]) {
        return 1;
    }
    if (t == 0) {
        return time_delay[name];
    }
    if (time_delay[name] + t > time()) {
        return 0;
    }
    map_delete(time_delay, name);
    return 1;
}

void add_id(string str)
{
    if (id(str)) {
        return;
    }
    ::add_id(str);
}

void add_post_exit_func(function f)
{
    if (!post_exit_funcs) {
        post_exit_funcs = ({});
    }
    post_exit_funcs += ({ f });
}

void post_exit_func()
{
    int i;
    for (i = 0; i < sizeof(post_exit_funcs); i++) {
        function f = post_exit_funcs[i];
        if (!objectp(f[0])) {
            continue;
        }
        catch((*f)());
    }
}

string query_tripped_message()
{
    if (query_max_internal_encumbrance() < query_internal_encumbrance() && !this_object()->query_ghost()) {
        return "%^RED%^You are over encumbered and cannot move!%^RESET%^";
    }
    return ::query_tripped_message();
}

int query_tripped()
{
    int i = ::query_tripped();
    return i || (query_max_internal_encumbrance() < query_internal_encumbrance() && !this_object()->query_ghost());
}

void set_all_stats(int* all_stats)
{
    string* valid_stats = ({ "strength", "dexterity", "constitution", "intelligence", "wisdom", "charisma" });
    int x;
    if (!pointerp(all_stats)) {
        return;
    }
    for (x = 0; x < sizeof(valid_stats); x++) {
        if (x >= sizeof(all_stats)) {
            return;
        }
        if (!intp(all_stats[x])) {
            continue;
        }
        set_stats(valid_stats[x], all_stats[x]);
        continue;
    }
}

int query_prestige_level(string the_class)
{
    string* base, * classes;
    object class_ob;
    int i, class_level;

    classes = query_classes();
    if (sizeof(classes == 1)) {
        return query_class_level(the_class);
    }
    for (i = 0; i < sizeof(classes); i++) {
        class_ob = find_object_or_load(DIR_CLASSES + "/" + classes[i] + ".c");
        if (!objectp(class_ob)) {
            continue;
        }
        if (!class_ob->is_prestige_class()) {
            continue;
        }
        base = class_ob->query_base_classes(this_object());
        if (member_array(the_class, base) == -1) {
            continue;
        }
        class_level = class_ob->caster_level_calcs(this_object(), the_class);
    }
    if (!class_level) {
        return query_class_level(the_class);
    }
    return class_level;
}

int is_living()
{
    return 1;
}

/**
 * Is this living undead based on the ways mud used to define
 * undead. If you wish to mark something as undead use undead property
 * , not the race.
 */
int is_undead()
{
    return (query("undead") ||
            query_property("undead") ||
            query_race() == "undead" ||
            query_race() == "nightwing" ||
            member_array("undead", query_id()) != -1 ||
            query_acquired_template() == "undead" ||
            query_acquired_template() == "mortuum" ||
            query_acquired_template() == "vampire");
}

/**
 * Used by vampire related mexhanics.
 */
int is_vampire()
{
    return (query_acquired_template() == "vampire") || 0;
}

int is_in_sunlight()
{
    if(EVENTS_D->query_time_of_day()!="day")
        return 0;
    if(environment(this_object())->query_property("indoors"))
        return 0;
    if(WEATHER_D->query_clouds(this_object())>3)
        return 0;
    if(ASTRONOMY_D->query_eclipse())
        return 0;
    return 1;
}

int is_were()
{
    if (query_acquired_template() == "weretiger" || query_acquired_template() == "werewolf" || query_acquired_template() == "wererat") {
        return 1;
    }
    return 0;
}

int is_shade()
{
    if(query_acquired_template() == "shade")
        return 1;

    return 0;
}

int is_feyborn()
{
    if(query_acquired_template() == "feyborn")
        return 1;

    return 0;
}

int is_deva()
{
    if(query_acquired_template() == "deva")
        return 1;

    return 0;
}

int is_feared()
{
    if(query_property("effect_cowering") ||
       query_property("effect_frightened") ||
       query_property("effect_panicked") ||
       query_property("effect_shaken"))
        return 1;

    return 0;
}

void reset_all_status_effects()
{
    object* myFeats;
    int x;

    if (!objectp(this_object()))
        return;

    POISON_D->ClearAllPoisons(this_object());

    set_bound(0, 0);
    set_blind(0);
    set_blindfolded(0);
    set_temporary_blinded(0);
    set_tripped(0, 0);
    set_unconscious(0, 0);
    set_gagged(0, 0);
    set_asleep(0, 0);
    remove_paralyzed();
    this_object()->remove_all_disable();

    myFeats = (object*)query_property("active_feats");

    if (pointerp(myFeats))
    {
        for (x = 0; x < sizeof(myFeats); ++x)
        {
            if (objectp(myFeats[x]))
                myFeats[x]->dest_effect();

            continue;
        }
    }

    if (query_invis())
    {
        set_magic_hidden(0);
        set_invis();
    }
}

int is_good(object obj)
{
    int align;

    if (!objectp(obj)) {
        return 0;
    }
    align = (int)obj->query_alignment();
    if (align == 1 || align == 4 || align == 7) {
        return 1;
    }
    return 0;
}

int is_evil(object obj)
{
    int align;

    if (!objectp(obj)) {
        return 0;
    }
    align = (int)obj->query_alignment();
    if (align == 3 || align == 6 || align == 9) {
        return 1;
    }
    return 0;
}

int is_neutral(object obj)
{
    int align;

    if (!objectp(obj)) {
        return 0;
    }
    align = (int)obj->query_alignment();
    if (align == 2 || align == 5 || align == 8) {
        return 1;
    }
    return 0;
}

/*generic combat stance
unarmed
unarmed and shield
weapon and shield
one hander
two hander
dual wield -> dual wield includes double weapon
double weapon
validate lrweapon separately for now
*/
void set_combat_stance(string stance) {
    if (query_property("combat stance")) {
        remove_property("combat stance");
    }
    set_property("combat stance", stance);
}

string query_combat_stance() {
    string stance;
    if (stance = query_property("combat stance")) {
        return stance;
    }
    return "unarmed";
}

int validate_combat_stance(string stance) {
    if (query_combat_stance() == stance) {
        return 1;
    }
    if (stance == "dual wield" && query_combat_stance() == "double weapon") {
        return 1;
    }
    return 0;
}
