// --==** /cmds/priest/_heal.c **==--
// Modified by Pator@ShadowGate
// August 25 1995
#include <priest.h>

inherit SPELL;

void create()
{
    ::create();
    set_spell_name("heal");
    set_spell_level(([ "cleric" : 6, "druid" : 7 ]));
    set_spell_sphere("healing");
    set_syntax("cast CLASS heal on TARGET");
    set_damage_desc("positive energy");
    set_description("%^RESET%^This spell will channel huge amount of positive energy into target and will allow the caster to cure massive amount damage from wounds and diseases.

See also: heal *player commands");
    set_verbal_comp();
    set_somatic_comp();
    set_target_required(1);
    set_casting_time(15);
    set_helpful_spell(1);
}


string query_cast_string()
{
    string cast;
    if (interactive(caster))
    {
        cast = "%^BOLD%^%^WHITE%^"+caster->QCN+" folds "+caster->QP+
            " hands as "+caster->QS+" prays for a divine spell!\n";
    }
    else
    {
        cast = "%^BOLD%^%^WHITE%^"+caster->QCN+" calls for a divine spell!\n";
    }
    return "\n"+cast;
}


spell_effect(int prof)
{
    int rnd;
    if(!!target->is_undead())
        set_helpful_spell(0);
    if (interactive(caster))
    {
        // CAST UPON ONESELF
        if ( caster == target )
        {
            tell_object(caster, "%^WHITE%^%^BOLD%^You reach out and channel energy at yourself.");
            tell_room(place, "%^WHITE%^%^BOLD%^"+YOU+" reaches out and channels energy at "+caster->QO+"self.", ({ caster, target}) );
        }
        else
        {
            tell_object(caster, "%^WHITE%^%^BOLD%^You reach out and channel energy at "+target->QCN+".");
            tell_room(place, "%^WHITE%^%^BOLD%^"+YOU+" channels energy at "+target->QCN+".",({ caster}) );
        }
    }
    else
    {
        // OBJECT CALLS HEAL
        tell_room(environment(caster), "%^WHITE%^%^BOLD%^"+caster->QCN+" sends a beam of energy at "+target->QCN+".",({ caster, target }) );
        tell_object(target, "%^WHITE%^%^BOLD%^"+caster->QCN+" sends a beam of energy at you.");
    }

    rnd = - sdamage * 5/4;
    damage_targ(target,target->return_target_limb(),rnd,"positive energy");
    target->remove_paralyzed();
    target->set_poisoning((-1)*(int)target->query_poisoning());
    dest_effect();
}

void dest_effect()
{
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}
