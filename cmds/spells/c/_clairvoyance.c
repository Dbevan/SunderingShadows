//spell to allow a brief glimpse at a person's
//location without a scrying device ~Circe~ 8/28/05
#include <std.h>
#include <priest.h>

#define COOLDOWN 60

inherit SPELL;

object dest, mytarg, myplace;
int scrypower;

/*
int clairvoyance_delay()
{
    return 60;
}
*/

void create()
{
    ::create();
    set_spell_name("clairvoyance");
    //On way fewer spell lists in SRD...
    set_spell_level(([ "mage" : 2, "bard" : 3, "assassin" : 4, "inquisitor" : 3, "warlock" : 1, "magus" : 3, "druid" : 4 ]));
    set_spell_sphere("divination");
    set_syntax("cast CLASS clairvoyance on TARGET");
    set_description("With this spell, the caster will attempt to see the target and their immediate surroundings. If the target has a scry blocker active, an opposed roll will be made. If the caster fails the roll, the spell will immediately fail. If they succeed, they will see the target and the room they are in. Anyone else in the area of the target, with sufficient scry protection, can avoid detection on a successful opposed roll against this spell.");
    set_verbal_comp();
    set_somatic_comp();
    set_arg_needed();
    set_feats_required(([ "warlock" : "book of shadows"]));
}


int preSpell()
{
    if(caster->cooldown("clairvoyance"))
    {
        tell_object(caster, "You have to wait a while before using this type of spell again.");
        return 0;
    }
    /*
    if(spell_type != "psion" && ((int)caster->query_property("clairvoyance time") + clairvoyance_delay()) > time())
    {
        tell_object(caster,"You need to take a moment's rest before you can try that again.");
        return 0;
    }
    */
    return 1;
}


string query_cast_string()
{
    if(spell_type == "psion")
    {
        tell_object(caster,"%^BOLD%^Closing your eyes, you press your fingers"+
            " to your temple, and focus your energies.");
        tell_room(place,"%^BOLD%^%^WHITE%^"+caster->QCN+" closes "+caster->QP+" eyes"+
            ", pressing "+caster->QP+" fingers to "+caster->QS+" temples.", caster);
        return "display";
    }
    tell_object(caster,"%^BOLD%^Seeking inner calm, you open your mind to the world around you and the target you seek.");
    tell_room(place,"%^BOLD%^%^WHITE%^"+caster->QCN+" closes "+caster->QP+" eyes and takes on an expression of calm.", caster);
    return "display";
}


void spell_effect(int prof)
{
    string targ, real,*map_keys;
    object mytarg,myplace,blockobj;
    int matches, i, bonus, stop;
    mapping map;

    if(!arg || !stringp(arg))
    {
        tell_object(caster,"You must focus upon a target!");
        if(objectp(TO)) TO->remove();
        return 1;
    }

    targ = lower_case(arg);

    if(targ == (string)caster->QCN)
    {
        tell_object(caster,"%^BOLD%^That won't work.  You should know your own location.");
        dest_effect();
        return;
    }

    if (!find_player(caster->realNameVsProfile(targ)))
    {
        tell_object(CASTER,capitalize(targ)+" cannot be found to scry.\n");
        TO->remove();
        return;
    }


    if(avatarp(caster)) { write("You know who you're looking for."); }
    mytarg = find_player(caster->realNameVsProfile(targ));

    if(!objectp(mytarg))
    {
        tell_object(caster,"%^BOLD%^%^RED%^The target of your power is not available!");
        dest_effect();
        return;
    }

    if (mytarg->query_true_invis())
    {
        tell_object(caster,"%^BOLD%^%^RED%^The target of your power is not available!");
        dest_effect();
        return;
    }
    
    if(caster->query("no pk")){
        tell_object(caster,"%^YELLOW%^You are unable to scry while you have a %^MAGENTA%^NoPK %^YELLOW%^flag.");
        dest_effect();
        return;
    }

    scrypower = clevel + BONUS_D->query_stat_bonus(caster, get_casting_stat()) + query_spell_level(spell_type);
    //bonus = calculate_bonus(caster->query_stats(get_casting_stat()));
    //scrypower = CLEVEL + bonus + random(query_spell_level(spell_type) * 2); //this is here so the vision spell is more powerful

/*
    if(blockobj = present("blockerx111", environment(mytarg)) || blockobj = present("blockerx111",mytarg))
    {
        //if(!PLAYER_D->scry_check(mytarg, scrypower))
        if(scrypower < blockobj->query_block_power())
        {
            tell_object(CASTER, "%^BOLD%^RED%^There appears to be "+
                "interference blocking your scrying attempt in the "+
                "vicinity of the target!%^RESET%^");
            dest_effect();
            return 1;
        }
    }
*/
    spell_successful();
    caster->add_cooldown("clairvoyance", COOLDOWN);
    
    if(!mytarg->scry_check(caster, scrypower))
    {
        tell_object(caster, "%^BOLD%^RED%^You sense something blocking your scrying attempt!%^RESET%^");
        dest_effect();
        return;
    }
    
    if(spell_type == "psion")
    {
        tell_object(CASTER, "%^ORANGE%^You turn your mind inward, focusing upon "+capitalize(targ)+"'s location.");
        tell_object(CASTER,"%^YELLOW%^Your inner eye reveals "+capitalize(targ)+"'s location:");
    }
    else
    {
        tell_object(CASTER,"%^YELLOW%^A flash of insight reveals "+capitalize(targ)+"'s location:");
    }

    mytarg->long_look_room(caster, scrypower);
    dest_effect();
}


void dest_effect()
{
    if(objectp(caster)) { tell_object(caster,"%^ORANGE%^The image fades from your mind."); }
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}

/*
int long_look_room(object dest)
{
    string file, desc;
    object *inv;
    int i,j;
    message("room_description","\n%^YELLOW%^[S] %^RESET%^"+(string)dest->query_short()+"\n", caster);
    message("room_description","\n%^YELLOW%^[S] %^RESET%^"+(string)dest->query_long()+"\n", caster);
    inv = all_inventory(dest);

    for(i=0;i<sizeof(inv);i++)
    {
        if(!objectp(inv[i])) continue;
        if(inv[i]->query_invis()) continue;
        if(!inv[i]->is_detectable()) continue;
        //if(!PLAYER_D->scry_check(inv[i], scrypower))
        //    continue;
        TO->send_living_name(inv[i]);
    }
    return 1;
}
*/

/*
int send_living_name(object targ)
{
    string known, str;
    int i,j;

    if(targ->is_monster())
    {
        str = targ->query_short();
        message("living_item", "%^CYAN%^BOLD%^[S] %^RED%^"+str,caster);
        return 1;
    }
    
    if(objectp(caster) && caster->knownAs(targ->query_true_name())) known = caster->knownAs(targ->query_true_name());
    str = targ->getWholeDescriptivePhrase();
    if(known) str = capitalize(known)+", "+str;
    message("living_item", "%^CYAN%^BOLD%^[S] %^RED%^"+str, caster);
    known = 0;
    return 1;
}
*/

object find_miss(object play, object victim)
{
    object *userarray;
    object randobj;

    userarray = users();
    userarray -= ({ 0,play,victim });
    if (!sizeof(userarray)) return 0;
    randobj = userarray[random(sizeof(userarray))];
    if (avatarp(randobj)) return 0;
    else return randobj;
}
