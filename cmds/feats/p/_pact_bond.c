/*
  _pact_bond.c
  
  Feat that allows a warlock to summon a heritage-specific familiar.
  
  -- Tlaloc -- 
*/

#include <std.h>
#include <daemons.h>
#include <magic.h>

inherit FEAT;
//Familiar helps with touch attacks, similar to spectral hand
//Format - type : ({ STR, DEX, CON, INT, WIS, CHA, SIZE, AC BONUS, }),
mapping valid_types = ([ 
                         "imp"           :    ({  4, 15, 10, 14, 12,  6, 1, 14,  }), //infernal
                         "quasit"        :    ({  6, 15,  8, 10, 12,  9, 1, 16,  }), //abyssal
                         "sprite"        :    ({  3, 18,  8, 12, 12,  7, 1, 14,  }), //fey
                         "gazer"         :    ({  4, 14, 10, 16, 11,  5, 1, 13,  }), //stars
                         "spirit"        :    ({  6, 15, 14, 8, 13,   4, 1, 15,  }), //gloom
                         //"coatl"         :    ({  3, 16, 10, 2, 12,   5, 1, 14,  }), //celestial
                       ]);

object companion;

void create()
{
    ::create();
    feat_type("instant");
    feat_category("PactBond");
    feat_name("pact bond");
    feat_prereq("Warlock L1");
    feat_syntax("pact_bond] [TYPE] to summon or pact_bond to dismiss");
    feat_desc("Your patron sends an emissary from beyond to aid you. This familiar will follow you through thick and thin and will level up with you. The TYPE of familiar will determine its base stats and natural armor.  The familiar can also assist with touch attack spells, much like spectral hand. The familiar is a non-combat companion, and won't fight or protect you in combat. While the familiar is present, the warlock gains +2 perception and +2 to reflex saves.

The available familiars are as follows:
%^CYAN%^BOLD%^ Gazer    %^RESET%^: A tiny beholder from the great beyond
%^CYAN%^BOLD%^ Imp      %^RESET%^: A devilish denizen from the nine hells
%^CYAN%^BOLD%^ Quasit   %^RESET%^: A demonic hordling from the abyss
%^CYAN%^BOLD%^ Spirit   %^RESET%^: A lost soul from the Plane of Shadows
%^CYAN%^BOLD%^ Sprite   %^RESET%^: A trickster from the Feywild

The Familiar will hide if you use the 'hide_in_shadows' command, allowing you to effectively sneak about with your faithful friend. It will also go invisible with you.

The Familiar can also be customized through several commands, which will allow you to change its description:

%^ORANGE%^<familiar short [DESCRIPTION]>%^RESET%^ - Changes the short description of the familiar.
%^ORANGE%^<familiar long  [DESCRIPTION]>%^RESET%^ - Changes the long description of the familiar.

  To have the familiar follow you, use 'familiar follow'.  
  To command the familiar, use %^ORANGE%^<familiar command %^ULINE%^ACTION%^RESET%^ORANGE%^>.%^RESET%^");
  
    set_target_required(0);
}

int allow_shifted() { return 0; }

int prerequisites(object ob)
{
    if(!objectp(ob))
        return 0;
    
    if(ob->query_class_level("warlock") < 1)
    {
        dest_effect();
        return 0;
    }
   
    return ::prerequisites(ob);
}

int cmd_pact_bond(string str)
{
    object feat;
    
    if(!objectp(this_player()))
        return 0;
    
    feat = new(base_name(this_object()));
    feat->setup_feat(this_player(), str);
    
    return 1;
}

void execute_feat()
{
    object ob;
    
    int class_level,
        comp_hd,
        comp_ac;
        
    string type;
    
    ::execute_feat();

    if (!objectp(caster))
    {
        dest_effect();
        return;
    }

    if(caster->query_property("using instant feat"))
    {
        tell_object(caster,"You are already in the middle of using a feat!");
        dest_effect();
        return;
    }

    
    if(sizeof(caster->query_attackers()))
    {
        tell_object(caster, "You can't summon your familiar during combat!");
        return;
    }
    
    
    companion = caster->query_property("familiar");
    
    if(objectp(companion))
    {
        companion && tell_object(caster, "You dismiss your familiar.");
        caster->remove_property("familiar");
        companion && companion->remove();               
        return;
    }
    
    if(!arg || member_array(arg, keys(valid_types)) < 0)
    {
        tell_object(caster, "That is not a valid choice for familiar.");
        tell_object(caster, "Valid options are: (" + implode(keys(valid_types), ",") + ") ");
        return;
    }
    
    tell_object(caster, sprintf("You summon your trusty %s familiar to your side.", arg));
    
    class_level = caster->query_prestige_level("mage");
    
    comp_hd = class_level + 2;
    comp_ac = class_level + 10;
    
    companion = new("/d/magic/mon/familiar");
    companion->set_race("outsider");
    companion->set_name(arg);
    companion->set_id( ({ arg, "familiar", caster->query_name() + "'s familiar" }) );
    companion->set_short(sprintf("%s's faithful %s familiar.", caster->query_cap_name(), arg));
    companion->set_level(class_level);
    companion->set_hd(comp_hd, 14);
    companion->set_attacks_num(2 + class_level / 8);
    companion->set_mlevel("fighter", comp_hd);
    companion->set_max_hp(14 + (14 * comp_hd));
    companion->set_hp(14 * comp_hd + 14);
    companion->set_alignment(caster->query_alignment());
    companion->set_owner(caster);
       
    caster->set_property("familiar", companion);
    caster->add_follower(companion);
    //caster->add_protector(companion);

    companion->set_property("minion", caster);
    companion->move(environment(caster));
    companion->set_heart_beat(1);
    
    //Setting companion stats based on type per SRD
    companion->set_stats("strength", valid_types[arg][0] + min( ({ class_level / 5, 6 }) ) );
    companion->set_stats("dexterity", valid_types[arg][1] + min( ({ class_level / 5, 6 }) ) );
    companion->set_stats("constitution", valid_types[arg][2]);
    companion->set_stats("intelligence", valid_types[arg][3]);
    companion->set_stats("wisdom", valid_types[arg][4]);
    companion->set_stats("charisma", valid_types[arg][5]);
    companion->set_size(valid_types[arg][6]);
    companion->set_overall_ac(0 - comp_ac - valid_types[arg][7]);
  
    //Based on SRD - companion gets "specials" at certain caster levels
    if(class_level >= 3)
        companion->set_monster_feats( ({ "evasion" }) );
    if(class_level >= 6)
        companion->set_monster_feats( ({ "evasion", "resistance" }) );
       
    return;
}

void dest_effect()
{
    remove_feat(this_object());
    ::dest_effect();
    return;
}
