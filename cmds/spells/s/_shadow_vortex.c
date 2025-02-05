#include <std.h>
#include <spell.h>
#include <magic.h>
#include <daemons.h>

inherit SPELL;

int timer,flag,stage,toggle,counter;


void create()
{
    ::create();
    set_spell_name("shadow vortex");
    set_spell_level(([ "innate" : 3 ]));
    set_spell_sphere("invocation_evocation");
    set_syntax("cast CLASS shadow vortex");
    set_damage_desc("void, half on reflex save");
    set_description("This spell snatches part of the great maelstorm from the seas on shadow plane. Summoned vortices will harm everything in the area with void energy.");
    set_verbal_comp();
    set_somatic_comp();
    set_save("reflex");
    aoe_spell(1);
    set_aoe_message("%^RESET%^%^MAGENTA%^(%^BOLD%^%^BLACK%^p%^RESET%^%^MAGENTA%^o%^BOLD%^%^BLACK%^p%^RESET%^%^MAGENTA%^u%^BOLD%^%^BLACK%^l%^RESET%^%^MAGENTA%^a%^BOLD%^%^BLACK%^t%^RESET%^%^MAGENTA%^e%^BOLD%^%^BLACK%^d w%^RESET%^%^MAGENTA%^i%^BOLD%^%^BLACK%^th sh%^RESET%^%^MAGENTA%^a%^BOLD%^%^BLACK%^d%^RESET%^%^MAGENTA%^o%^BOLD%^%^BLACK%^w v%^RESET%^%^MAGENTA%^o%^BOLD%^%^BLACK%^rt%^RESET%^%^MAGENTA%^i%^BOLD%^%^BLACK%^c%^RESET%^%^MAGENTA%^e%^BOLD%^%^BLACK%^s%^RESET%^%^MAGENTA%^)%^RESET%^");
}

string query_cast_string()
{
    tell_room(place,"%^BOLD%^%^BLACK%^"+caster->QCN+" voices few syllables.");
    return "display";
}

void spell_effect(int prof)
{
    tell_room(place,"%^MAGENTA%^Multiple shadow vertices pop into existence.%^RESET%^");
    counter = clevel * 3;
    addSpellToCaster();
    spell_successful();
    execute_attack();
}

void execute_attack()
{
    object *foes=({}),targ;
    int i;

    if(!flag)
    {
        flag = 1;
        ::execute_attack();
        return;
    }

    if(!objectp(caster) || !objectp(place) || !present(caster,place)|| counter<0)
    {
        dest_effect();
        return;
    }

    foes = target_selector();
    foes -= ({ caster });

    define_base_damage(0);
    tell_room(place,"%^MAGENTA%^Cold shadow vortices rage with undulating void energy!%^RESET%^");
    for(i=0;i<sizeof(foes);i++) {
        if(!objectp(targ = foes[i]))
            continue;
        if(do_save(targ,2))
        {
            tell_object(targ,"%^MAGENTA%^Shadow vortices caress you, but you avoid some of the damage!");
            damage_targ(targ,targ->return_target_limb(),sdamage/2,"void");
        }
        else
        {
            tell_object(targ,"%^MAGENTA%^Shadow vortices caress you with powerful void energy!");
            damage_targ(targ,targ->return_target_limb(),sdamage,"void");
        }
    }
    counter--;
    place->addObjectToCombatCycle(TO,1);
}


void dest_effect() {
    if(objectp(place)) tell_object(place,"%^MAGENTA%^The shadow storm recedes.%^RESET%^");
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}
