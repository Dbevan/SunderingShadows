#include <spell.h>
#include <daemons.h>
inherit SPELL;
int bonus;
string target_shape, target_name;

string *valid_forms()
{
    return ({"dragon","demon","golem"});
}

void create()
{
    ::create();
    set_spell_name("greater polymorph");
    set_spell_level(([ "mage" : 9 ]));
    set_spell_sphere("alteration");
    set_syntax("cast CLASS greater polymorph on TARGET | dragon/demon/golem");
    set_description("With this spell you transform yourself or a party member into one of several fearsome creatures. In the new form you won't be able to access your inventory, but you will possess mighty melee abilities. The potency of the form will grow with your power, but it won't benefit from transformation spell. In addition winged forms can use <wing> command to travel. You may only maintain one polymorph at any given time. You cannot polymorph constructs or golems).

%^BOLD%^%^RED%^N.B.%^RESET%^ You can set alternative description, speech string and adjective for these forms.");
    set_verbal_comp();
    set_somatic_comp();
    set_arg_needed();
    set_helpful_spell(1);
    set_arg_needed();
}

int preSpell()
{
    object shape, *allies;
    string *forms;
    
    if(sscanf(arg, "%s | %s", target_name, target_shape) != 2)
    {
        tell_object(caster, query_syntax());
        return 0;
    }
    
    if(caster->query_property("greater polymorph"))
    {
        tell_object(caster, "You can only use Greater Polymorph on one target at a time.");
        return 0;
    }
    
    target = present(target_name, place);
    
    if(!objectp(target))
    {
        tell_object(caster, "Your target is not here.");
        return 0;
    }
    
    if(target->query_race() == "construct" || target->query_race() == "golem")
    {
        tell_object(caster, "You cannot polymorph constructs or golems.");
        return 0;
    }
    
    if(!userp(target))
    {
        tell_object(caster, "You can only polymorph player characters.");
        return 0;
    }
    
    allies = ({  });
    allies += PARTY_D->query_party_members(caster->query_party());
    
    if(!sizeof(allies))
        allies += ({ caster });
    
    allies += caster->query_protectors();
    allies += caster->query_followers();
    
    if(member_array(target, allies) < 0)
    {
        tell_object(caster, "You can only cast greater polymorph on yourself, party members, or minions.");
        return 0;
    }
    
    if(objectp(shape = target->query_property("shapeshifted")) ||
       objectp(shape = target->query_property("transformed")) ||
       objectp(shape = target->query_property("dance-of-cuts")) ||
       objectp(shape = target->query_property("altered")))
    {
        tell_object(caster,"Your target is already in an alternative form!");
        return 0;
    }
    
    forms = valid_forms();
     
    if(member_array(target_shape,forms)==-1)
    {
        tell_object(caster,"Invalid form, valid forms are: "+implode(valid_forms(),", "));
        return 0;
    }

    return 1;
}

void spell_effect(int prof)
{
    object shape;
    
    if (!objectp(caster)){
        TO->remove();
        return;
    }

    if (!objectp(target))
    {
        tell_object(caster, "Your target is no longer here.");
        dest_effect();
        return;
    }

    new("/std/races/shapeshifted_races/mage_" + target_shape + ".c")->init_shape(target,target_shape);

    shape = target->query_property("shapeshifted");
    //Can't completely boost up a person
    shape->set_clevel(min( ({ clevel, target->query_character_level() + 10 }) ));
    bonus = clevel/4+1;
    spell_successful();

    caster->set_property("greater polymorph", target);
    target->set_property("spelled", ({TO}) );
    
    addSpellToCaster();
    
    if(userp(target) && target != caster)
        addSpellToTarget();
    
    /*
    if(!userp(target))
        addSpellToCaster();
    else
        addSpellToTarget();
    */
}

void dest_effect()
{
    object shape, me = this_object();

    if (objectp(target))
    {
        if (objectp(shape = target->query_property("shapeshifted")))
            shape->reverse_shape(target);

        if(objectp(caster))
        {
            if(caster != target && objectp(target))
                tell_object(caster, "You feel your polymorph magic slip away from " + target->query_cap_name() + ".");
        }
    }
    
    if(objectp(caster))
        caster->remove_property("greater polymorph");
    
    ::dest_effect();

    if (objectp(me))
        me>remove();
}
