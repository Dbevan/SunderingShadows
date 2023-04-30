#include <std.h>
#include <spell.h>
#include <magic.h>
#include <priest.h>
#include <daemons.h>


inherit SPELL;

void second_hit();
void last_hit();

object* victims;

object oldenv, web, mass;
int waittime;

void dest_effect();
void waiter(int num);

void create()
{
    ::create();
    set_spell_name("holy phoenix");
    set_spell_level(([ "cleric" : 9 ]));
    set_spell_sphere("conjuration_summoning");
    set_domains(({ "good" }));
    set_syntax("cast CLASS holy phoenix");
    set_description("With this ability, the cleric sends a fervent prayer to their deity "
                    "sacrificing themselves for the lives of their friends and to the bane of "
                    "there enemies. Such a sacrifice always looks favorable by the gods.");
    set_arg_needed();
    set_damage_desc("divine");
    set_verbal_comp();
    set_somatic_comp();
    splash_spell(3);
    set_save("reflex");
}

string query_cast_string(){
   tell_object(caster,"%^C167%^Grabbing your holy symbol tightly "+
      "in your hand, shouting out a %^C111%^desperate prayer %^C167%^to your deity.");
   tell_room(place,"%^C167%^"+caster->QCN+" %^C167%^holds a holy symbol "+
      "tightly in "+caster->QP+" %^C167%^left hand as "+caster->QS+" %^C167%^they "+
      "shout out a %^C111%^desperate %^C167%^prayer to their deity.%^RESET%^",caster);
   return "display";
}

int preSpell()
{
    if(caster->cooldown("holy phoenix"))
    {
        tell_object(caster, "You can't use Holy Phoenix yet. Type <cooldowns> to see your wait time.");
        return 0;
	}else;{
	return 1;
    }
}

void spell_effect(int prof)
{
    object peep;

    victims = target_selector();

    if (!sizeof(victims))
    {
        tell_object(caster, "%^BOLD%^%^GREEN%^Your spell fails to connect to anyone.");
        dest_effect();
    }

    foreach(peep in victims)
    {
        if (!objectp(peep))
            continue;

        tell_object(peep, "%^C195%^Bright %^C190%^light %^C195%^hits you, starting to %^C088%^burn %^C195%^your skin!%^RESET%^");
        tell_room(place, "%^C195%^" + peep->QCN + "'s eyes flinches as the %^C190%^bright light %^C195%^begins to cover them!%^RESET%^", peep);

        if (!do_save(peep)) {
            damage_targ(peep, peep->return_target_limb(), sdamage, "divine");
        } else {
            tell_object(peep, "%^C109%^You steel yourself and shrug off the worst of the pain.%^RESET%^");
            damage_targ(peep, peep->return_target_limb(), sdamage / 2, "divine");
        }

    }
    spell_successful();
	caster->set_paralyzed(30, "%^C195%^You feel overwhelmed by %^C190%^divine power%^C195%^, you are losing control and unable to think or act!");
    call_out("second_hit", 15);
}

void second_hit()
{
    object peep, peep_environment;

    define_base_damage(0);

    foreach(peep in victims)
    {
        if (!objectp(peep))
            continue;

        if (!objectp(peep_environment = environment(peep)))
            continue;

        tell_object(peep, "%^C195%^Another surge of %^C190%^light %^C195%^bursts from" +caster->QCN+ "%^C202%^striking %^C195%^you with greater intensity!%^RESET%^");
        tell_object(peep_environment, "%^C195%^" + peep->QCN + " trembles violently as the %^C190%^light continues to blast into " + peep->QP + " body.", peep);

        if (!do_save(peep))
            peep->cause_typed_damage(peep, "torso", sdamage, "divine");
        else
        {
            tell_object(peep, "%^C109%^You steel yourself and shrug off the worst of the pain.%^RESET%^");
            peep->cause_typed_damage(peep, "torso", sdamage / 2, "divine");
        }
    }

    if (!arrayp(victims) || !sizeof(victims))
        dest_effect();

    caster->set_paralyzed(30, "%^C195%^Your body feels %^C088%^burning hot %^C195%^as if the %^C142%^energy %^C195%^is trying to escape! You are about to %^C124%^explode%^C195%^!");
    call_out("last_hit", 15);
}

void last_hit()
{
    object peep;

    define_base_damage(0);

    foreach(peep in victims)
    {
        tell_object(peep, "%^C195%^" +caster->QCN+ "%^C195%^explodes in a %^C197%^massive burst %^C195%^of %^C226%^divine energy%^C195%^!%^RESET%^");
        tell_object(environment(peep), "%^C195%^" + peep->QCN + " shudders %^C196%^violently %^C195%^as the %^C166%^explosion %^C195%^blasts into them!");
		tell_room(place,"%^C195%^" +caster->QCN+ "explodes in a burst of light, energy ripping through the room.%^RESET%^",caster);
        if (!do_save(peep)) {
            //damage_targ(peep, peep->return_peep_limb(), sdamage * 2, "divine");
            peep->cause_typed_damage(peep, "torso", sdamage, "divine");
        } else {
            tell_object(peep, "%^C195%^You steel yourself and shrug off the worst of the pain.%^RESET%^");
            //damage_targ(peep, peep->return_target_limb(), sdamage, "divine");
            peep->cause_typed_damage(peep, "torso", sdamage / 2, "divine");
        }
    }
	waittime= 5;
	oldenv=environment(caster);
	caster->add_cooldown("holy phoenix", 50);
	web=new("/d/magic/room/judgement.c");
    caster->move(web);
    addSpellToCaster();
    call_out("waiter",ROUND_LENGTH,1);
}

void waiter(int num) {
    object *tempinv;
    if(!caster || !objectp(web)){
        dest_effect();
        return;
    }
    if (!present(caster,web)) {
        mass->remove();
        web->move("/d/shadowgate/void.c");
        while ( tempinv=(object *)web->all_inventory() )
        {
            tempinv[0]->move(oldenv);
            return;
        }
    }
    if (num < waittime)
        call_out("waiter",ROUND_LENGTH,num+1);
    else
        dest_effect();
    return;
}
void dest_effect() {
    int i;
    object *tempinv;

    if (objectp(caster) && objectp(web))
        if (present(caster,web)) {
            tell_object(caster,"%^C156%^You feel %^C202%^warmth %^C195%^throughout your body as you step out of a %^C220%^ball of light%^C195%^!%^RESET%^");
            tell_room(place,"%^C195%^"+caster->QCN+" suddenly steps out of the %^C220%^glowing orb%^C195%^.%^RESET%^",caster );
            caster->move(oldenv);
            tempinv=all_inventory(web);
            for (i=0;i++;i<sizeof(tempinv)) {
                if (objectp(tempinv[i])) tempinv[i]->move(oldenv);
            }
        }
	if (find_call_out("second_hit") != -1) {
        remove_call_out("second_hit");
    }
    if (find_call_out("last_hit") != -1) {
        remove_call_out("last_hit");
    }
    if (objectp(caster) && objectp(web)) {
        if (!present(caster,web)) {
            tempinv=all_inventory(web);
            for (i=0;i++;i<sizeof(tempinv)) {
                if (objectp(tempinv[i])) tempinv[i]->move(oldenv);
            }
            TO->remove();
        }
    } else if(objectp(web)) {
        tempinv=all_inventory(web);
        for (i=0;i++;i<sizeof(tempinv)) {
            if (objectp(tempinv[i])) tempinv[i]->move(oldenv);
        }
        TO->remove();
        web->remove();
    } else {
        TO->remove();
    }
    ::dest_effect();
    if(objectp(TO)) TO->remove();
    return;
}
