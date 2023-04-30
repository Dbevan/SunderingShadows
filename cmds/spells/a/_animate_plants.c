#include <spell.h>
#include <magic.h>
#include <rooms.h>

inherit SPELL;

object *plants=({});
int duration;

void check();


void create()
{
    ::create();
    set_spell_name("animate plants");
    set_spell_level(([ "druid" : 7,"oracle":7, "cleric" : 7 ]));
    set_domains("plant");
    set_mystery("nature");
    set_spell_sphere("alteration");
    set_syntax("cast CLASS animate plants");
    set_description("This spell will animate any foliage, roots, or plants in an area for the purpose of protecting the caster. The animated plants will take on the essence of plants from all different aspects of the wild. The plants will have the same characteristics as those ordinarily found in the wilds, but will be of much smaller scale.");
    set_verbal_comp();
    set_somatic_comp();
    summon_spell();
    set_helpful_spell(1);
}


int preSpell()
{
    if(place->query_property("no sticks"))
    {
        tell_object(caster,"You can't seem to find any plants here at all.");
        return 0;
    }

    /*
    if(present("plant 4",place))
    {
        tell_object(caster,"There are already too many animated plants here. You can't create any more.");
        return 0;
    }
    */

    return 1;
}


string query_cast_string()
{
   return "%^GREEN%^"+caster->QCN+" waves "+caster->QP+" hand in a slow arc towards the ground and the foliage begins to come alive!";
}


spell_effect(int prof)
{
    object plant;
    int num,i;

    if(!objectp(caster))
    {
        dest_effect();
        return;
    }

    num = 10 + clevel / 5;

    for(i=0;i<num;i++)
    {
        plant = new("/d/magic/mon/plant.c");
        if(!objectp(plant)) { continue; }

        plants += ({ plant });

        plant->set("aggressive",1);
        plant->remove_property("swarm");
        plant->set_owner(caster);
        plant->setup_minion(clevel, spell_level, "lesser");
        //plant->set_mlevel("fighter",clevel);
        //plant->set_guild_level("fighter",clevel);
        plant->set_attacks_num(0);
        //plant->set_max_hp(clevel*8+30);
        //plant->set_hp(plant->query_max_hp());//kinda silly to be badly injured on summon
        plant->set_property("spelled",({TO}));
        plant->set_property("spell_creature",TO);
        plant->set_property("spell",TO);
        plant->add_id("summoned monster","miniature plant");
        plant->set_short("miniature plant");
        plant->set_stats("strength",14);
        plant->set_stats("dexterity",14);
        plant->set_stats("constitution",14);
        plant->set_new_exp(1,"low");
        plant->set_property("minion", caster);
        plant->move(environment(caster));
        //caster->add_follower(plant);
        //caster->add_protector(plant);

        tell_room(place,"%^BOLD%^%^GREEN%^A bit of foliage near "+caster->QCN+" comes to life and transforms into a miniature "+plant->QCN+"!",caster);
        tell_object(caster,"%^BOLD%^%^GREEN%^A bit of foliage near you comes to life and transforms into a miniature "+plant->QCN+"!");
    }

    environment(caster)->set_property("used sticks",1);

    addSpellToCaster();
    spell_successful();
    spell_duration = (clevel + roll_dice(1, 20)) * ROUND_LENGTH + 300;
    duration = spell_duration;
    set_end_time();
    call_out("dest_effect", duration);

    check();
}


void check()
{
    object plant,*attackers,att;
    int i,j;

    plants -= ({ 0 });

    if( (!objectp(caster)) || (!sizeof(plants)) || (!objectp(place = environment(caster))) )
    {
        dest_effect();
        return;
    }

    for(i=0;i<sizeof(plants);i++)
    {
        if(!objectp(plant = plants[i])) { continue; }

        if(!present(plant,place))
        {
            tell_room(environment(plant),"%^YELLOW%^With its creator gone, "+plant->QCN+" turns back into a piece of regular foliage.");
            plant->move("/d/shadowgate/void");
            plant->remove();
            continue;
        }

        caster->add_protector(plant);
        caster->add_follower(plant);

        if(!sizeof(attackers = (object*)caster->query_attackers())) { continue; }

        for(j=0;j<sizeof(attackers);j++)
        {
            if(!objectp(att = attackers[j])) { continue; }
            if(member_array(att,(object*)plant->query_attackers()) != -1) { continue; }

            plant->kill_ob(att,0);
        }
    }

    call_out("check",5);
}


void dest_effect()
{
    int i;
    
    remove_call_out("check");

    if(objectp(caster))
    {
        removeSpellFromCaster();
        tell_object(caster,"%^GREEN%^Your animated plants lose their magical energy and turn back into ordinary foliage.");
    }

    if(sizeof(plants))
    {
        for(i=0;i<sizeof(plants);i++)
        {
            if(!objectp(plants[i])) { continue; }

            tell_room(environment(plants[i]),"%^BOLD%^%^GREEN%^"+plants[i]->QCN+" stops moving and changes back into a piece of ordinary foliage.");
            plants[i]->move("/d/shadowgate/void");
            plants[i]->remove();
        }
    }

    ::dest_effect();
    if(objectp(TO)) TO->remove();
}
