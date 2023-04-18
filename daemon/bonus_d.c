// Changed the numbers around to reflect the 3.5 edition rules.  They havethe same effect
// but are still backwards.  I'm going to leave a copy of the current ones commented out, in
// case we want to change them back.  -Ares 8/22/06
// added in BAB for LA characters. Transformation (mage/sorc spell) also picks up here. N, 5/15.
// added psywarriors ~Circe~ 9/1/15
#include <std.h>
#include <dirs.h>
#include <daemons.h>

#define PO previous_object()
int query_stat_bonus(object who, string stat);

int high_thaco(int level)
{
    return (level - 20);
}

/**
 * This used to calculate number of attacks with old style compat,
 * used in attack_bonus() and some feats/items.x
 */
varargs int thaco(int level, string myclass, object ob)
{
    string file, * classes;
    int ret, i, bab;

    if (!intp(level)) {
        return 0;
    }
    if (!stringp(myclass)) {
        return 0;
    }

    if (objectp(ob)) {
        if (ob->query("new_class_type")) {
            ret = 0;
            // fighter number of attacs
            //if ((int)ob->query_property("transformed") || (int)ob->query_property("dance-of-cuts")) {
            //    ret = (int)ob->query_level();
            //    ret = ret - 20;
            //    ret = ret * -1;
            //    return ret;
            //}
            //classes = (string*)ob->query_classes();
            //if (!sizeof(classes)) {
            //    return 0;
            //}
            
            bab = ob->base_attack();
            //for (i = 0; i < sizeof(classes); i++) {
            //    file = DIR_CLASSES + "/" + classes[i] + ".c";
            //    if (!file_exists(file)) {
            //        continue;
            //    }
            //    bab = fileob->attack_bonus(ob);
                ret += (bab < 0 ? 0 : bab);
            //}

            ret = ret - 20;
            ret = ret * -1;
            return ret;
        }
    }

    if (myclass == "fighter" || myclass == "monk" || myclass == "ranger" || myclass == "paladin" || myclass == "antipaladin" || myclass == "cavalier" || myclass == "barbarian") {
        ret = 20 - (level - 1);
    }
    if (myclass == "cleric" || myclass == "oracle" || myclass == "druid" || myclass == "warlock" || myclass == "psywarrior" || myclass == "inquisitor" || myclass == "magus") {
        if (level < 21) {
            ret = (20 - (2 * ((level - 1) / 3)));
        }else {
            ret = 8 - high_thaco(level);
        }
    }
    if (myclass == "mage" || myclass == "illusionist" || myclass == "psion") {
        if (level < 21) {
            ret = (20 - ((level - 1) / 3));
        }else {
            ret = 14 - high_thaco(level);
        }
    }
    if (myclass == "thief" || myclass == "bard" || myclass == "assassin") {
        if (level < 21) {
            ret = (20 - ((level + 1) / 2));
        }else {
            ret = 10 - high_thaco(level);
        }
    }
    return ret;
}

/**
 * This returns _number_ of attacks one gets every round in
 * /std/battle.c and has nothing to do with attack bonus.
 */
int attack_bonus(string myclass, int level, object player)
{
    int num;
    num = thaco(level, myclass, player);
    num = 20 - (num);
    return num;
}

int monster_thaco(int hd)
{
    return thaco((int)PO->query_level(), (string)PO->query_class());
}

int query_con_bonus(int num)
{
    if (!intp(num)) {
        return 0;
    }
    return (num - 10) / 2;
}

int query_dex_bonus(object target)
{
    object* torso = ({});
    int i, dex, ret, enc, bonus, max = 10;

    if (!objectp(target)) {
        return 0;
    }
    dex = (int)target->query_stats("dexterity");

    ret = (dex - 10) / 2 * -1;

    if (!userp(target)) {
        //limiting this to a max of 3 from dex for mobs, simply because mobs end up with stupidly high dex stats
        //unless they are manually set and hopefully this might
        //help balance things out since I removed the double dex bonus to attack that everyone got - Saide
        if (target->query_property("full ac")) {
            if (ret > 5) {
                ret = 5;
            }
            return ret;
        }else {
            return 0;
        }
    }

    // gets the max dex bonus based on armor type
    torso = (object*)target->query_armour("torso");

    if (!sizeof(torso) || FEATS_D->usable_feat(target, "armor training")) {
        max = 10;
    } else {
        for (i = 0; i < sizeof(torso); i++) {
            if (!objectp(torso[i])) {
                continue;
            }
            if (!torso[i]->is_armour()) {
                continue;
            }
            bonus = (int)torso[i]->query_max_dex_bonus();
            if (!bonus) {
                log_file("armor_no_max", "" + base_name(torso[i]) + " no set_max_dex_bonus() or is 0.\n");
                continue;
            }
            if (bonus < max) {
                max = bonus;
            }
        }
    }

    // prevents the bonus from being higher than the max allowed by the armor
    max = -max;
    if (ret < max) {
        ret = max;
    }

    return ret;
}

int query_stance_bonus(object victim)
{
    int fnord = 0;

    fnord -= (int)victim->query_defensive_bonus();
    fnord += (int)previous_object()->query_offensive_bonus();
    return fnord;
}

//this could have been accomplished much much easier - simply by doing 1 per level for fighters, 0.75 for clerics and the like, and 0.50 for mages
//adding that up, diving by the number of classes * total level - Saide
int new_bab(int level, object ob)
{
    string* classes, file;
    int i, ret = 0, bab = 0;
    if (objectp(ob)) {
        if ((int)ob->query_property("transformed") || (int)ob->query_property("dance-of-cuts")) { // fighter BAB under transformation/dance.
            //ret = (int)ob->query_level();
            //changing to query_base_character_level()
            ret = (int)ob->query_base_character_level();
            return ret;
        }
        classes = (string*)ob->query_classes();
        if (!sizeof(classes)) {
            return 0;
        }
        for (i = 0; i < sizeof(classes); i++) {
            file = DIR_CLASSES + "/" + classes[i] + ".c";
            if (!file_exists(file)) {
                continue;
            }
            bab = file->attack_bonus(ob);
            ret += (bab < 0 ? 0 : bab);
            //since mobs are usually much higher level than players - Saide
            if (!userp(ob)) {
                return ret;
            }
        }

        return ret;
    }
    return ret;
}

int tohit_bonus(int dex, int str, object target)
{
    int ret, exc_str, enc_percent;
    int abonus;

/*   tell_object(PO, "You are the previous object!! [attack]"); */
    abonus = PO->query_attack_bonus();
    if (!abonus) {
        abonus = 0;
    }

    // Putting these in functions all their own for easier maintenance and...
    // sanity. - G.

    ret = query_dex_bonus(target) * (-1);
    //this was decreasing their bonus for some reason. *(-1) to see if it fixes. Nienne, 07/10
    //ret += query_stance_bonus(target);
    ret += abonus;
    ret += (str - 10) / 2;

    return ret;
}

//AC only starting at 10 and going up - Saide
varargs effective_ac(object who)
{
    int MyAc = 0, tmp = 0;

    if (!objectp(who)) {
        return 0;
    }

    //AC is starting at 10 now - Saide
    //AC starts at 10 and goes up
    //however, our AC goes down by default
    //so someone with a 2 AC should have an 18 now
    //or be 8 points away from the base of 10,
    //someone with a -1 AC would have 21 AC -
    //because they are 11 points away from the base
    //I had actually forgot this, then fixed it, then
    //realized I was right the first time - hopefully
    //this will explain why AC is 10 and then
    //the difference from 10 and AC is added
    //if the query_ac is above 0 and why
    //an additional 10 is added if it's below 0 - Saide
    MyAc = 10;
    tmp = (int)who->query_ac();
    if (tmp >= 0) {
        MyAc += (10 - tmp);
    } else {
        MyAc += 10 + (tmp * -1);
    }

    return MyAc;
}

/**
 * This function accepts second target if you ever need
 */
varargs ac_bonus(object who, object attacker)
{
    int MyBonus, dexb;
    if (!objectp(who)) {
        return 0;
    }
    if (!objectp(attacker)) {
        return 0;
    }
    MyBonus = 0;

    dexb = query_dex_bonus(who);
    dexb = -dexb;
    
    //Nature oracle feature
    if(who->query_mystery() == "nature")
    {
        if(who->query_class_level("oracle") > 20)
            dexb = query_stat_bonus(who, "charisma");
    }
    if(who->query_mystery() == "lunar")
    {
        if(who->query_class_level("oracle") >= 10)
            dexb = query_stat_bonus(who, "charisma");
    }

    if (who->query_temporary_blinded() || who->query_blind()) {
        if (!FEATS_D->usable_feat(who, "blindfight")) {
            dexb = 0;
        }
    }
    if (who->query_unconscious() || who->query_prone() || who->query_paralyzed() || who->query_asleep() &&
        !FEATS_D->usable_feat(who, "dodge")) {
        dexb = 0;
    }
    if (attacker->query_invis() && attacker != who) {
        if (!who->detecting_invis()) {
            dexb = 0;
        }
    }

    MyBonus += dexb;
    return MyBonus;
}

varargs int hit_bonus(object who, object targ, int attack_num, object current, int touch)
{
    int th, to_hit, tmp;
    int i, j, min, hold, mysize, fired, bab_scale, pen;
    string* classes;
    object ammo;

    th = 0;

    if (!objectp(who)) {
        return 0;
    }
    if (who->query_unconscious() || who->query_bound()) {
        return 0;
    }
    if (!objectp(targ)) {
        return 0;
    }
    //in 3ed you get base attack bonus for your first attack and then
    //-5 for every subsequent attack which is actually included
    //in the bab tables so attack 1 is full bonus, attack 2
    //is -5, attack 3 is -10 - I am not entirely certain how
    //this will translate here since there are many more
    //attacks for some classes - but will try it out and adjust
    //accordingly - Saide
    //adjusting to -2 penalty per subsequent attack -
    //capping at -6 for everything now - Saide, November 2016
    if (sizeof(distinct_array((object*)who->query_wielded())) > 1) {
        if (attack_num == 1) {
            pen = 2;
        }
    }
    if (attack_num >= 2) {
        pen = 2 * (attack_num - 1);
    }
    if (pen > 6) {
        pen = 6;
    }

    th -= pen;

    th += (int)who->query_attack_bonus();

    tmp = new_bab((int)who->query_level(), who);
    //tmp *= -1;
    th += tmp;

    to_hit = th;

    if ((current && objectp(current)) && !who->query_property("shapeshifted") && current != who) { // weapon finesse calculations if wielding a weapon
        if (current->is_lrweapon()) {
            ammo = present(current->query_ammo(), who);
            if (FEATS_D->usable_feat(who, "point blank shot") && objectp(ammo) && ammo->query_shots()) {
                to_hit += (query_dex_bonus(who) * -1);
                fired = 1;
            }
        }
        if (!fired) { // if they didn't fire the ranged wep, it must be a melee weapon
            mysize = (int)who->query_size();
            if (mysize == 1) {
                mysize++;           //run small creatures as normal size please.
            }
            mysize -= (int)current->query_size();
            //if (FEATS_D->usable_feat(who, "weapon finesse") && ((mysize >= 0) || current->query_property("finesse") || touch == 1)) { // if has-feat & weapon is smaller/same size as user - Odin 5/24/2020 or weapon has the property - Venger dec20
            //Or it's a ranged touch attack (uses dex)
            if (FEATS_D->usable_feat(who, "weapon finesse") && ((mysize >= 0) || touch == 1 || current->query_property("finesse"))) {
                to_hit += (query_dex_bonus(who) * -1);
            }
            else if(FEATS_D->usable_feat(who, "fighter finesse"))
            {
                to_hit += (query_dex_bonus(who) * -1);
            }
            else if(FEATS_D->usable_feat(who, "cunning strikes"))
            {
                to_hit += query_stat_bonus(who, "charisma");
            }
            else {
                to_hit += query_stat_bonus(who, "strength");
            }
        }
    }
    // if no weapon (unarmed), run the same check
    else {
        if (FEATS_D->usable_feat(who, "weapon finesse")) {
            to_hit += (query_dex_bonus(who) * -1);
        }
        else if(FEATS_D->usable_feat(who, "cunning strikes"))
        {
            to_hit += query_stat_bonus(who, "charisma");
        }
        else {
            to_hit += (query_stat_bonus(who, "strength"));
        }
    }
    
    //Point blank shot gives +1 to ranged touch attacks
    //Spectral hand gives a further +1 bonus to touch attacks
    if(touch == 1)
    {
        to_hit += FEATS_D->usable_feat(who, "point blank shot");
        to_hit += who->query_property("spectral_hand");
    }
    if(touch == 2)
    {
        to_hit += who->query_property("spectral_hand");
    }
    
    //Paladin smite against opposed alignment adds cha mod to attack rolls
    if(who->query_guild_level("paladin") && targ->query_property("paladin smite") == who)
        to_hit += query_stat_bonus(who, "charisma");

    // +1 BAB to rock gnome, racial, vs goblinoids
    if ((string)who->query_race() == "gnome" && (string)who->query("subrace") == "rock gnome") {
        if ((string)targ->query_race() == "goblin" || (string)targ->query_race() == "hobgoblin" || (string)targ->query_race() == "bugbear") {
            to_hit += 1;
        }
    }
    // +1 BAB to morinnen human ethnicity if mounted
    if ((string)who->query_race() == "human" && (string)who->query("subrace") == "morinnen" && who->query_in_vehicle()) {
        to_hit += 1;
    }

    if (current && objectp(current) && current != who) {
        if (current->query_property("enchantment")) {
            to_hit += (int)current->query_property("enchantment");
        }
    }
    if ((!objectp(current) || current == who) && (who->is_class("monk") || FEATS_D->usable_feat(who, "enchanted fists")) && !who->query_property("shapeshifted")) {
        //if(find_player("saide")) tell_object(find_player("saide"), "Should be using unarmed enchantments... equal to "+COMBAT_D->unarmed_enchantment(who));
        to_hit += COMBAT_D->unarmed_enchantment(who);
    }
    if (who->query("protecting")) {
        to_hit -= random(7) + 1;
    }
    if (who->query_blind() && !FEATS_D->usable_feat(who, "blindfight")) {
        to_hit -= 8 + random(4);
    }


    // trying to scale bab with each attack, might take some tweaking -Ares
    // only scales on attacks with main hand, so dual wielders don't get penalized more
    bab_scale = (int)who->query_property("bab_scale");

    if (!who->query_property("testing_bab")) {
        to_hit += bab_scale;
    }
    return to_hit;
}

//flag = 1 for a touch attack - Saide
varargs int process_hit(object who, object targ, int attack_num, mixed current, object DebugOb, int flag, int mod)
{
    object PlayerBoss;
    int attack_roll, bon, AC = 0, pFlag;
    int ac_excess;
    if (!objectp(who)) {
        return 0;
    }
    if (!objectp(targ)) {
        return 0;
    }
    if (!intp(attack_num)) {
        attack_num = 1;
    }
    if (!flag) {
        AC += effective_ac(targ);
    }
    AC += ac_bonus(targ, who);
    
    //Tlaloc added this cap 9/2/21 to address super high AC
    //Diminished returns above 80
    if(AC > 80)
        AC = 80 + (AC - 80) / 2;
    //If it's still above 90, even more diminished
    if(AC > 90)
        AC = 90 + (AC - 90) / 4;
    
    AC += targ->query_property("sundered");

    if (!userp(who)) {
        if (objectp(PlayerBoss = who->query_property("minion"))) {
            if (userp(PlayerBoss)) {
                pFlag = 1;
            }
        }
        if (who->query_static_bab() && !pFlag) { // giving monsters a static base attack bonus to see if this helps armor classes vs monsters -Ares
            bon = (int)who->query_static_bab();
            bon += ((int)who->query_max_hp() / 250); // give some extra chance to hit based on monster health, so bosses don't miss as often
            bon += ((int)hit_bonus(who, targ, attack_num, current) / 2, flag);
        }else {
            bon = hit_bonus(who, targ, attack_num, current, flag);
        }
    }else {
        bon = hit_bonus(who, targ, attack_num, current, flag);
    }

    if (intp(current)) {
        bon += current;
    }
    attack_roll = roll_dice(1, 20);

    if (objectp(DebugOb)) {
        tell_object(DebugOb, "Attack roll = " + attack_roll);
        tell_object(DebugOb, "Attack bonus = " + bon);
        tell_object(DebugOb, "AC = " + AC);
    }
    if (attack_roll == 20) {
        return 20;
    }
    
    if(attack_roll == 1)
        return 1;
    
    attack_roll += mod;
    
    //if(attack_roll == 1) return -1;
    //does this change make AC less OP? - Saide, August 2017
    //Might have to reconsider this since I added the change above to diminished returns - Tlaloc
    /*
    if ((bon + 15) < AC) {
        if (random(bon + AC + attack_roll) >= AC) {
            return attack_roll;
        }
        return 0;
    }
    */
    if ((attack_roll + bon) >= AC) {
        return attack_roll;
    }
    return 0;
}

int query_stat_bonus(object who, string stat)
{
    int ret;
    if (!objectp(who)) {
        return 0;
    }
    if (!stringp(stat)) {
        return 0;
    }
    return ret = (((int)who->query_stats(stat) - 10) / 2);
}

int damage_bonus(int str)
{
    int ret;
    int dbonus;
    int estr;

/*   tell_object(PO, "You are the previous object!! [damage]"); */
    dbonus = PO->query_damage_bonus();
    if (!dbonus) {
        dbonus = 0;
    }

    ret += (str - 10) / 2;
    ret += dbonus;
    return ret;
}

//added to allow this to still function since
//the function that called damage_bonus() is located
//in combat_d.c now - which would mean combat_d.c is
//now the previous object - Saide, September 2016
int new_damage_bonus(object attacker, int str)
{
    int dbonus;
    if (objectp(attacker)) {
        dbonus = (int)attacker->query_damage_bonus();
    }
    if (!dbonus) {
        dbonus = 0;
    }

    dbonus += ((str - 10) / 2);
    return dbonus;
}

//Combat Maneuvers Calcs
int query_combat_maneuver_bonus(object ob)
{
    int cmb, mysize;
    
    if(!objectp(ob))
        return 0;
    
    mysize = ob->query_size();
    
    cmb = new_bab(ob->query_level(), ob);  
    if(FEATS_D->has_feat(ob, "agile maneuvers"))
        cmb += query_stat_bonus(ob, "dexterity");
    else
        cmb += query_stat_bonus(ob, "strength");
    
    if(ob->query_acquired_template() == "chaotic")
        cmb += 2;
    
    cmb += ob->query("gladiator trance") / 2;
    
    cmb += (mysize - 2);
    
    return cmb;
}

int query_combat_maneuver_defense(object ob)
{
    int cmd, mysize;
    
    if(!objectp(ob))
        return 0;
    
    mysize = ob->query_size();
    cmd = new_bab(ob->query_level(), ob);
    cmd += ob->query("gladiator trance");
    cmd += max( ({ query_stat_bonus(ob, "strength"), query_stat_bonus(ob, "dexterity") }) );    
    cmd += (mysize - 2);
    
    if(ob->query_race() == "dwarf")
        cmd += 4;
    
    if(ob->query_race() == "hobgoblin")
        cmd += 1;
    
    if(ob->query_acquired_template() == "chaotic")
        cmd += 2;
    
    return cmd;
}

int combat_maneuver(object victim, object attacker, int mod)
{
    int result, CMB, CMD, diff, psybonus;
    
    if(victim->query_paralyzed() || victim->query_bound() || victim->query_unconscious())
        return 1;
    
    result = roll_dice(1, 20);
    
    if(result == 1)
        return 0;
    if(result == 20)
        return 1;
    
    //psybonus = attacker->query("gladiator trance") / 2;
    //result += psybonus;
    result += mod;
    
    if(!userp(victim))
        CMD = attacker->query_level() + 10;
    else
        CMD = query_combat_maneuver_defense(victim);

    if(!userp(attacker))
        CMB = victim->query_level() + 10;
    else
        CMB = query_combat_maneuver_bonus(attacker);
    
    CMB += result;
    CMD += 10;
    
    CMD += (FEATS_D->usable_feat(victim, "unmoving") * 3);
    
    result = CMB >= CMD ? 1 : 0;
    
    return result;
}
    
int intimidate_check(object victim, object attacker, int mod)
{
    int DC, result, influence, diminish;
    
    if(!objectp(victim) || !objectp(attacker))
        return 0;
    
    if(PLAYER_D->immunity_check(victim, "fear"))
        return 0;
    
    DC = min( ({ 50, victim->query_level() }) );
    DC += (10 + query_stat_bonus(victim, "wisdom"));
    
    result = roll_dice(1, 20);
    influence = attacker->query_skill("influence");
    diminish = attacker->query_level() + 10;
    
    if(influence > diminish)
        influence = diminish + (influence - diminish) / 2;
    
    if(result == 1)
        return 0;
    if(result == 20)
        return 1;
    
    if(attacker->query_race() == "half-orc" || attacker->query_race() == "orc" || attacker->query_race() == "hobgoblin" || attacker->query_race() == "dragonkin" || attacker->query_race() == "half-ogre")
        mod += 2;
    
    if(FEATS_D->has_feat(attacker, "intimidating prowess"))
        mod += query_stat_bonus(attacker, "strength");
    
    if(FEATS_D->has_feat(attacker, "persuasive"))
        mod += 2;
    
    result = influence + result;
    result += mod;
    
    result = result >= DC ? 1 : 0;
    
    return result;
}
    