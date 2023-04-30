//removed mention of non-specialized mages ~Circe~ 8/2/19
#include <magic.h>
#include <std.h>
#include <spell.h>
inherit SPELL;

object *animated, *monsters, control;
string arg;
int amount;

void make_sword();

void create() {
    ::create();
    set_author("nienne");
    set_spell_name("shadow double");
    set_spell_level(([ "innate" : 7, "monk" : 13, "psion" : 8, ]));
    set_spell_sphere("illusion");
    set_discipline("egoist");
    set_monk_way("way of the shadow");
    set_syntax("cast CLASS shadow double");
    set_damage_desc("greater summon");
    set_description("This spell will conjure a ghostly double of the caster, drawn from his or her own "
"shadow. The shadow will automatically follow and protect you. Should you lose it, though, simply go into the room with "
"it and <command shadow to follow>. This is a greater summon.");
    summon_spell();
    set_verbal_comp();
    set_somatic_comp();
    set_helpful_spell(1);
}

int preSpell(){
   if(caster->query_property("mages_sword") || caster->query_property("has_elemental")) {
        tell_object(caster,"%^CYAN%^You are incapable of controling two such powerful summonings.%^RESET%^");
        return 0;
   }
   return 1;
}

string query_cast_string(){
   tell_object(caster,"%^RESET%^%^C061%^You make a brief, %^RESET%^%^C244%^s%^C243%^u%^C244%^b%^C245%^t%^C243%^l%^RESET%^%^C242%^e m%^RESET%^%^C243%^o%^C245%^t%^C244%^i%^C243%^o%^RESET%^%^C242%^n %^RESET%^%^C061%^to the side with one hand.%^CRST%^");
   tell_room(place,"%^RESET%^%^C061%^"+caster->QCN+" %^RESET%^%^C061%^makes a brief, %^RESET%^%^C244%^s%^C243%^u%^C244%^b%^C245%^t%^C243%^l%^RESET%^%^C242%^e m%^RESET%^%^C243%^o%^C245%^t%^C244%^i%^C243%^o%^RESET%^%^C242%^n %^RESET%^%^C061%^to the side with one hand.%^CRST%^",caster);
   return "display";
}

void spell_effect(int prof){
	if(!objectp(environment(caster)))	{
		dest_effect();
		return;
	}
	place = environment(caster);
      make_sword();
      addSpellToCaster();
}

void make_sword() {
    object ob, thing;

    tell_object(caster,"%^RESET%^C062%^Your %^RESET%^%^C241%^s%^C242%^h%^C243%^a%^C244%^d%^C242%^o%^RESET%^%^C241%^w %^RESET%^%^C062%^moves, as if of its own accord, and %^RESET%^%^C241%^r%^C242%^i%^C243%^s%^C242%^e%^RESET%^%^C241%^s %^RESET%^%^C062%^to stand beside you.%^CRST%^");
    tell_room(place,"%^RESET%^%^C062%^You see "+caster->QCN+"%^RESET%^%^C062%^'s %^RESET%^%^C241%^s%^C242%^h%^C243%^a%^C244%^d%^C242%^o%^RESET%^%^C241%^w %^RESET%^%^C062%^move, as if of its own accord, and %^RESET%^%^C241%^r%^C243%^i%^C242%^s%^RESET%^%^C241%^e %^RESET%^%^C062%^to stand beside "+caster->QO+".%^CRST%^",caster);
    control = new("/d/magic/obj/shadowremote");
    control->set_caster(caster);
    control->move(caster);
    control->set_property("spell",TO);
    control->set_property("spelled", ({TO}) );
    ob=new("/d/magic/obj/ashadow.c");
    control->set_sword(ob);
    caster->add_follower(ob);
    ob->set_gender(caster->query_gender());
    ob->set_alignment(caster->query_alignment());
    ob->set_owner(caster);
    ob->setup_minion(clevel, spell_level, "greater");

    ob->set_heart(1);
    ob->set_stats("intelligence",8);
    ob->set_stats("dexterity",20);
    ob->set_stats("strength",20);
    ob->set_attack_limbs(({"left hand","right hand"}));
    ob->set_base_damage_type("slashing");
    ob->set_attacks_num(clevel/12+1);
    ob->set_damage(clevel/4,6);
    ob->set_overall_ac(-clevel);
    ob->set_hd(clevel,12);
    ob->set_stats("strength",15);
    //ob->set_max_hp(clevel*12+100);
    //ob->set_hp(ob->query_max_hp());
    ob->set_caster(caster);
    ob->move(environment(caster));
    ob->set_property("spell",TO);
    ob->set_property("spell", ({TO}) );
    ob->set_property("spell_creature", TO);
    ob->set_property("minion", caster);
    ob->set_property("effective_enchantment", ((int)CLEVEL / 7));
    ob->set_mylevel(clevel);
    caster->set_property("mages_sword",ob);
    return;
}

void dest_effect() {
    object sword;

    if (objectp(caster)) {
        tell_room(environment(caster),"%^RESET%^%^C069%^The %^RESET%^%^C241%^s%^C242%^h%^C243%^a%^C244%^d%^C243%^o%^C242%^w%^RESET%^%^C241%^y b%^RESET%^%^C242%^e%^C243%^i%^C242%^n%^RESET%^%^C241%^g %^RESET%^%^C069%^beside "+caster->QCN+" %^RESET%^%^C240%^f%^C242%^a%^C244%^d%^C245%^e%^RESET%^%^C246%^s a%^RESET%^%^C249%^w%^C252%^a%^RESET%^%^C255%^y%^RESET%^%^C069%^.%^CRST%^",caster);
        tell_object(caster,"%^RESET%^%^C069%^The %^RESET%^%^C241%^s%^C242%^h%^C243%^a%^C244%^d%^C242%^o%^RESET%^%^C241%^w %^RESET%^%^C240%^f%^C242%^a%^C244%^d%^C245%^e%^RESET%^%^C246%^s a%^RESET%^%^C249%^w%^C252%^a%^RESET%^%^C255%^y%^RESET%^%^C069%^.%^CRST%^");
    }

    if(objectp(control)) {
		if (objectp(sword = control->query_sword())){
			sword->remove();
			destruct(control);
		}
    }
    if(objectp(caster)) { caster->remove_property("mages_sword"); }
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}

