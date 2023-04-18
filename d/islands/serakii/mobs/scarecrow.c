#include <std.h>
inherit MONSTER;
#include "../serakii.h"

object ob;	

void create()
{
    int lev;
    object ob;
    string me;
    ::create();
    set_name("monstrous scarecrow");
    set_id( ({"monstrous scarecrow", "scarecrow", "monster"}) );
    
    set_short("%^C226%^m%^C220%^o%^C226%^n%^C249%^s%^C226%^"+
    "tro%^C214%^u%^C226%^s s%^C215%^c%^C226%^ar%^C249%^"+
    "e%^C226%^cr%^C214%^o%^C226%^w%^CRST%^");    
    set_long("%^C220%^This %^C249%^monstrosity%^C220%^ "+
    "at one point could have been a %^C095%^scarecrow%^C220%^, "+
    "but some %^C164%^fe%^C092%^l%^C164%^l en%^C093%^e%^C164%^"+
    "rgy%^C220%^ has infected it and twisted it into a %^C058%^"+
    "grotesque abomination%^C220%^. It's %^C226%^straw body%^C220%^ "+
    "leaks a %^C093%^miasma%^C220%^ that if it touches you or you "+
    "breath it in seems to suck the life and emotion out of you. "+
    "It was dressed in a simple %^C095%^farmer's overalls%^C220%^ "+
    "and %^C178%^homespun shirt%^C220%^ that have stretched and "+
    "torn. %^C058%^Long arms%^C220%^ drape to the ground with "+
   "wicked looking %^C093%^fell claws%^C220%^.%^CRST%^\n");
   
    set_gender("neuter");
    set_race("human");
    set_body_type("human");
    set_overall_ac(-40);
    set_property("swarm",1);
    set_property("no death", 1);
    lev = 35 + random(9);
    set_class("cleric");
    set_mlevel("cleric",lev);
    set_guild_level("cleric",lev);
    set_stats("strength", 20);
    set_stats("dexterity", 8);
    set_stats("constitution", 20);
    set_stats("intelligence", 3);
    set_stats("wisdom", 25);
    set_stats("charisma", 3);
    set("aggressive", 1);
    set_property("add kits", 40);
    set_property("hidden inventory", 1);
    set_property("add quest", "%^C220%^Cleansed the Fallow Field of the Scarecrow!%^CRST%^");
    me = TO->query_short();
    
    set_emotes(3, ({"%^C061%^A %^C092%^thick miasma%^C061%^ spreads around area.%^CRST%^",
        me+" %^C095%^drags its %^C092%^claws%^C095%^ against the ground%^CRST%^",
        me+" %^C059%^grabs%^C244%^ at you%^CRST%^",}),1);

    
    set_alignment(9);
    set_size(3);
    set_max_hp(lev * 200);
    set_hp(query_max_hp());
    set_property("full attacks", 1);
    add_money("gold", 400 + random(601));
    set_new_exp(lev,"boss");
   
    set_spell_chance(100);
    
    set_spells(({"earthquake", "flame strike", "call lightning", "unholy orb"}));

    set_monster_feats(({"perfect caster", "spell power", "spell focus"}));
    
    set_func_chance(20);
    set_funcs(({"grab"}));
    
    if(!clonep())
        return;

      ob = new(OBJ"dragonrobes");
    ob->move(TO);
    if(random(4)) ob->set_property("monsterweapon",1);

      ob = new(OBJ"jaquinstears.c");
    ob->move(TO);
    if(random(4)) ob->set_property("monsterweapon",1);
ob = new(OBJ"shoulderplate");
    ob->move(TO);
    if(random(4)) ob->set_property("monsterweapon",1);
	
ob = new(OBJ"moon_plate");
    ob->move(TO);
    if(random(4)) ob->set_property("monsterweapon",1);
	
ob = new(OBJ"iron_knuckles");
    ob->move(TO);
    if(random(4)) ob->set_property("monsterweapon",1);

ob = new(OBJ"tikelen_belt");
    ob->move(TO);
    if(random(4)) ob->set_property("monsterweapon",1);

}

void grab(object targ)
{
    string me;
    if(!objectp(targ)) return;
    if(!objectp(TO)) return;
    if(!objectp(ETO)) return;
    me = TO->query_short();
    
    tell_object(targ, "%^C142%^You are grabbed by "+me+"%^C142%^ "+
    "and it holds you against the ground!%^CRST%^");
    
    tell_room(ETO, "%^C142%^With an otherworldly scream "+me+"%^C142%^ "+
    "reaches out and grabs onto "+targ->QCN+"%^C142%^ and "+
    "slams them against the ground!%^CRST%^", targ);
    
    targ->set_paralyzed(20, "%^C142%^You are pinned to the ground!%^CRST%^");

    return;
}

