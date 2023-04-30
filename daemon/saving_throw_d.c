/*
  saving_throw_d.c
  
  Rebuilt to solve the existing problems with saves.
  Previous saves were very difficult to balance. This
  effectively squishes the numbers down.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>
#include <dirs.h>
inherit DAEMON;

//All saves are in the format ({ fort, reflex, will })
// Using a mapping to track the numbers all through the daemon for debug purposes.
mapping save_info=([]);

void create() { ::create(); }

varargs int do_save(object ob, int dc, string type, raw_save)
{
    int *saves,num,save,roll1,i,level,statbonus,mod, *cls_save, max_mod, advantage;
    string *classes,file;
    object rider;
    
    if(!objectp(ob) || !stringp(type))
        return;
    
    saves = ({ 0, 0, 0 });
    save = 0;
    mod = 0;
    max_mod = 0;
    level = min( ({ (int)ob->query_base_character_level(), 60 }) );
    
    classes = ob->query_classes();
    if(!pointerp(classes))
        classes = ({  });
    
    foreach(string cls in classes)
    {
        file = DIR_CLASSES + "/" + cls + ".c";
        
        if(!load_object(file))
            continue;
        
        if(file->is_prestige_class())
            continue;
        
        cls_save = file->saving_throws(ob);
        
        if(!sizeof(cls_save))
            continue;
        
        saves[0] += cls_save[0] * 2;
        saves[1] += cls_save[1] * 2;
        saves[2] += cls_save[2] * 2;
    }
    
    saves[0] = max( ({ min( ({ saves[0], 2 }) ), -2 }) );
    saves[1] = max( ({ min( ({ saves[1], 2 }) ), -2 }) );
    saves[2] = max( ({ min( ({ saves[2], 2 }) ), -2 }) );
    
    switch(type)
    {
        //FORTITUDE SAVES
        case "fortitude": case "fort":
            save_info["save_type"] = "fort";
            if(ob->is_undead())
                statbonus = BONUS_D->query_stat_bonus(ob, "charisma");
            else
                statbonus = BONUS_D->query_stat_bonus(ob, "constitution");
            
            if(FEATS_D->usable_feat(ob, "divine grace"))
                mod += 5;
            
            if(FEATS_D->usable_feat(ob, "shroud of black steel"))
                mod += 4;
            
            mod += ob->query_saving_bonus("fortitude");
            mod += (saves[0] * 2);
            max_mod = saves[0];
        
            if(ob->query("subrace") == "aesatri")
                mod += 1;
            
            if(ob->query("subrace") == "ursine")
                mod += 2;
            
            if(PLAYER_D->check_familiar(ob, "lemming"))
                mod += 2;
            
        break;
        
        //REFLEX SAVES
        case "reflex":
            save_info["save_type"] = "reflex";
            statbonus = BONUS_D->query_stat_bonus(ob, "dexterity");
            
            if(FEATS_D->usable_feat(ob, "divine grace"))
                mod += 5;
            
            if(FEATS_D->usable_feat(ob, "danger sense"))
            {
                mod += (ob->query_class_level("barbarian") / 10 + 2);
                mod += (ob->query_class_level("thief") / 10 + 2);
            }
            
            if(FEATS_D->usable_feat(ob, "fighter reflexes"))
                mod += (1 + ob->query_class_level("fighter") / 10);
            
            if(ob->query_bloodline() == "kobold")
                mod += 2;
            
            mod += ob->query_saving_bonus("reflex");
            mod += (saves[1] * 2);
            max_mod = saves[1];
            
            if(ob->query("subrace") == "senzokuan")
                mod += 1;
            
            if(ob->query("subrace") == "canine" || ob->query("subrace") == "feline")
                mod += 2;

            if(PLAYER_D->check_familiar(ob, "fox") || PLAYER_D->check_familiar(ob, "weasel"))
                mod += 2;
            
            if(PLAYER_D->check_familiar(ob) && ob->is_class("warlock"))
                mod += 2;
        break;
        
        //WILL SAVES
        case "will":
            save_info["save_type"] = "will";
            //These don't stack
            if(ob->is_class("psion") || ob->is_class("psywarrior"))
                statbonus = BONUS_D->query_stat_bonus(ob, "intelligence");
            else if(FEATS_D->usable_feat(ob, "force of personality") && !FEATS_D->usable_feat(ob, "divine grace"))
                statbonus = BONUS_D->query_stat_bonus(ob, "charisma");
            else
                statbonus = BONUS_D->query_stat_bonus(ob, "wisdom");
            
            //Undead will save is always high (as if they had the right stat)
            if(ob->is_indead())
                statbonus = 10;
            
            if(FEATS_D->usable_feat(ob, "divine grace"))
                mod += 5;
            
            mod += ob->query_saving_bonus("will");
            mod += (saves[2] * 2);
            max_mod = saves[2];
        
            if(ob->query("subrace") == "maalish")
                mod += 1;
            
            if(ob->query("subrace") == "simian")
                mod += 2;
            
            if(ob->query_race() == "nymph")
                mod += 2;
            
            //Paladin Auras
            if(PLAYER_D->check_aura(ob, "courage") == 2)
                mod += 2;
            if(PLAYER_D->check_aura(ob, "resolve") == 2)
                mod += 2;

            //Vampires
            if (ob->is_vampire())
                mod -= (20000 - (int)ob->query_bloodlust()) / 2000;

            if(PLAYER_D->check_familiar(ob, "hedgehog"))
                mod += 2;
        break;
    }
    
    save = level;
    statbonus = statbonus > 10 ? 10 : statbonus;
    save += statbonus;
    
    //Saves at lower levels are completely skewed to have a huge spread with stats bonuses.
    //This is an attempt to rein that in to 150% of level.
    save = min( ({ save, (level * 3) / 2 }) );

    //SAVE ROLL MODIFIERS
    if (ob->query_race() == "halfling")
            mod += 1;
        
    if(ob->query_race() == "nightwing" && total_light(environment(ob)) > -1)
        mod -= 4;
    else
        mod += 4;
        
    if(FEATS_D->usable_feat(ob, "resistance"))
        mod += 2;
    
    if(FEATS_D->usable_feat(ob, "increased resistance"))
        mod += 2;
    
    if(FEATS_D->usable_feat(ob, "improved resistance"))
        mod += 2;

    if (FEATS_D->usable_feat(ob, "shadow master") && objectp(ENV(ob)) && ENV(ob)->query_light() < 2)
        mod += 2;
    
    if(userp(ob) && member_array("madness", ob->query_divine_domain()) >= 0)
        mod -= 1;

    save += mod;
    save = save > (level + 22 + max_mod) ? (level + 22 + max_mod) : save;

    if (raw_save) {
        return save;
    }

    if (ob->is_animal()) {
        rider = (object)ob->query_current_rider();
        if (objectp(rider)) {
            if (save < rider->query_skill("athletics") / 2 && FEATS_D->usable_feat(rider, "indomitable mount")) {
                save = rider->query_skill("athletics") / 2;
            }
        }
    }

    save_info["dc"] = dc;
    save_info["final_saving_throw"] = save;

    if (dc > 0)
        dc *= -1;

    //Determining advantage or disadvantage
    advantage = 0;
    
    if(FEATS_D->usable_feat(ob, "live to tell the tale") ||
        FEATS_D->usable_feat(ob, "dark ones luck") ||
        FEATS_D->usable_feat(ob, "spellcasting harrier") ||
        (FEATS_D->usable_feat(ob, "perpetual foresight") && ob->query("available focus")) ||
        ob->query_property("saves advantage") ||
        ob->query_property("roll advantage"))
            advantage++;
    
    if(ob->query_property("roll disadvantage")) advantage--;
    
    roll1 = roll_dice(1, 20, advantage);

    //Touch of Law makes the roll 11
    if(ob->query_property("touch of law"))
        roll1 = 11;

    save_info["saving_throw_roll"] = roll1;
    save_info["pass_or_fail_by"] = roll1 + save + dc;

    //Tlaloc changed this to try to reign in the eclipsing problem with saves vs DC.
    //This makes the minimum succes chance 10% and the maximum 90%
    if (roll1 <= 2) {
        save_info["save_result"] = 0;
    } else if (roll1 >= 19) {
        save_info["save_result"] = 1;
    } else if (roll1 + save + dc >= 0) {
        save_info["save_result"] = 1;
    } else {
        save_info["save_result"] = 0;
    }
    
    if(save_info["save_result"] == 0)
    {
        if(ob->query_class_level("cleric"))
        {
            //Fate domain has a chance to add 1d4 to save and try again
            if(member_array("fate", ob->query_divine_domain()) >= 0)
            {
                roll1 += roll_dice(1, 4);
                if(roll1 + save + dc >= 0)
                {
                    tell_object(ob, "%^BOLD%^MAGENTA%^You feel the hand of fate change the outcome!%^RESET%^");
                    save_info["save_result"] = 1;
                }
            }
        }
        //Barbarian with Eater of Magic can heal from a second save roll
        if(ob->is_class("barbarian"))
        {
            if(FEATS_D->usable_feat(ob, "eater of magic") && ob->query_property("raged"))
            {
                roll1 = roll_dice(1, 20);
                if(roll1 + save + dc >= 0)
                {
                    tell_object(ob, "%^GREEN%^Your body seems to absorb the magic!%^RESET%^");
                    ob->add_hp(roll_dice(level, 10));
                }
            }
        }
    }
    
    return save_info["save_result"];
}

int get_save(object who, string type)
{
    return do_save(who, 0, type, 1);
}

void check_save(object ob)
{
    object* weapons;
    int x;
    if (!objectp(ob)) {
        return 0;
    }
    if (save_info["save_result"] != 1) {
        if (FEATS_D->usable_feat(ob, "diamond soul")) {
            weapons = ob->query_wielded();
            for (x = 0; x < sizeof(weapons); x++) {
                if (!objectp(weapons[0])) {
                    continue;
                }
                if ((int)weapons[0]->query_size() > 1) {
                    return 0;
                }
                continue;
            }
            if (!ob->is_ok_armour("barb")) {
                return 0;
            }
            if (ob->spend_ki(1)) {
                return 1;
            }
        }
    }
    return 0;
}

int fort_save(object ob, int dc)
{
    do_save(ob, dc, "fort");
    if (check_save(ob)) {
        do_save(ob, dc, "fort");
    }
    return save_info["save_result"];
}

mapping debug_fort_save(object ob, int dc)
{
    do_save(ob, dc, "fort");
    return save_info;
}

int reflex_save(object ob, int dc)
{
    do_save(ob, dc, "reflex");
    if (check_save(ob)) {
        do_save(ob, dc, "reflex");
    }
    return save_info["save_result"];
}

mapping debug_reflex_save(object ob, int dc)
{
    do_save(ob, dc, "reflex");
    return save_info;
}

int will_save(object ob, int dc)
{
    do_save(ob, dc, "will");
    if (check_save(ob)) {
        do_save(ob, dc, "will");
    }
    return save_info["save_result"];
}

mapping debug_will_save(object ob, int dc)
{
    do_save(ob, dc, "will");
    return save_info;
}

//Keep here until all spells have updated
int magic_save_throw_adjust(object targ, object caster, object spell)
{
    int caster_bonus = 0;

    if (targ->query_race() == "dwarf") {
        if (targ->query("subrace") == "shield dwarf" || targ->query("subrace") == "gold dwarf") {
            caster_bonus -= 2;
        }
    }
    if (targ->query_race() == "orc" || targ->query_race() == "half-orc") {
        caster_bonus -= 1;
    }
    if (targ->query_race() == "human") {
        if (targ->query("subrace") == "heartlander") {
            caster_bonus -= 1;
        }
    }

    if (arrayp(targ->query_property("protection_from_alignment"))) {
        if (member_array(caster->query_alignment(), targ->query_property("protection_from_alignment")) != -1) {
            caster_bonus -= 2;
        }
    }

    if (FEATS_D->usable_feat(targ, "disruptive")) {
        caster_bonus -= 4;
    }

    if (FEATS_D->usable_feat(targ, "closed mind")) {
        caster_bonus -= 2;
    }
    
    if(FEATS_D->usable_feat(targ, "superstition") && targ->query_property("raged"))
    {
        caster_bonus -= 2;
    }
    
    if(targ->is_shade() && total_light(environment(targ)) < 2)
    {
        caster_bonus -= 1;
    }
    
    if(targ->query_mystery() == "spellscar" && targ->query_class_level("oracle") >= 15)
    {
        caster_bonus -= 2;
    }

    return caster_bonus;
}
