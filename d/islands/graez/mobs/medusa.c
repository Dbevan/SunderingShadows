#include <std.h>
#include "../graez.h"

inherit MONSTER;

void create() {

  object room,nasty;
  
  nasty = this_object();
  room = environment(nasty);

  ::create();
  set_name("medusa");
  set_id( ({"medusa", "Medusa", "monster"}) );
  set_short("%^C028%^me%^C040%^d%^C028%^u%^C040%^s%^C028%^a%^CRST%^");
  set_long("From a distance this is quite possibly the %^C215%^most "+
    "beautiful woman%^CRST%^ you have ever seen. Her statuesque human "+
    "form is dressed in a %^C195%^loose toga%^CRST%^ and moves with "+
    "the %^C068%^grace%^CRST%^ of a %^C075%^dancer%^CRST%^. However "+
    "her perfection ends as you realize instead of a full %^C094%^head "+
    "of hair%^CRST%^, her head is covered in hundreds of %^C109%^snarling%^CRST%^ "+
    "and %^C227%^hissing%^CRST%^ %^C035%^snakes%^C028%^.%^CRST%^ As she "+
    "looks you over you feel the %^C062%^malign curse%^CRST%^ of her "+
    "gaze start to eat away at you, turning you into %^C239%^stone%^CRST%^.\n");
  set_race("medusa");
  set_body_type("human");

  add_limb("snake head 1","head",0,0,0);
  add_limb("snake head 2","head",0,0,0);
  add_limb("snake head 3","head",0,0,0);
  add_limb("snake head 4","head",0,0,0);

  set_gender("female");
  set_size(3);
  set_class("oracle");
  set_mlevel("oracle", 30);
  set_hd(30,5);
  set_attack_bonus(10);
  set_max_hp(1000+random(1000));
  set_hp(query_max_hp());
  set_overall_ac(-34);
  set_property("function and attack", 1);
  set_property("cast and attack", 1);
  set_property("full attacks", 1);
  set_property("swarm",1);
  set_stats("charmisma", 25);
  set("aggressive", 30);
  set_mob_magic_resistance("high");
  set_alignment(6);

  set_property("no hold", 1);
  set_property("no death", 1);
  set_property("no bows", 1);
  set_sight_bonus(5);

  set_funcs(({"gaze"}));
  set_func_chance(33);

  set_property("add quest", "%^C107%^cleansed %^C194%^Haven%^C107%^ of the %^C028%^Medusa%^CRST%^");
  set_property("quest exp", 1000000);
  set_new_exp(25, "boss");

  set_monster_feats(({
    "spell focus",
    "spell penetration",
    "greater spell penetration",
    "spell power",
    "greater spell power",
    "perfect caster",
    "blind fight",
    "force of personality",
     "deadeye",                       
     "manyshot",                      
     "point blank shot",              
     "preciseshot",
   "parry",
     "shot on the run" 
  }));

  
  set_spells(({ 
    "eyebite",
    "fear",
    "wave of pain"
    "powerword kill",
    "shadow vortex",
    "oppression",
    "crushing despair",
    "weird",
    "black tentacles",
    "overwhelming presence", 
  }));
  set_spell_chance(50);
  
  if(!clonep())
      return;

  new(OBJ"aspbow.c")->move(nasty);
  command("wield bow");
  new("/d/common/obj/lrweapon/limitlessquiver")->move(nasty);
  command("wear arrows");

}


void gaze(object target){
    // Targetted damage or death

    object room;

    room = environment(this_object());

  if(!objectp(target)) return 0;
  if(!objectp(TO)) return 0;
  if(!target->will_save(30)) {
    if(target->query_property("no death")){

      //damage and stun  
      tell_object(target,"%^C186%^The Medusa turns to gaze in your direction. You cannot look away and you feel your whole body get tense as her magic tries to turn you to stone!%^CRST%^");
      tell_room(room,"%^C186%^The medusa turns to gaze at "+target->QCN+"%^C186%^ who stands slack and does not move!%^CRST%^",target);
      target->cause_typed_damage(target, target->return_target_limb(), random(200)+100, "void");
      target->set_tripped(1,"%^C186%^You are still trying to catch your breath!%^CRST%^");
      return(random(3)+1);
    }
 
    // deaders 
    tell_object(target,"%^C186%^The medusa turns her gaze in your direction and you feel your whole body get tense. You find you cannot turn away from her beauty and your whole body quickly turns to stone!");
    tell_room(room,"%^C186%^The medusa turns to gaze at "+target->QCN+"%^C186%^! They stand enthralled for a moment before "+target->QCN+"%^C186%^'s body turns to stone!%^CRST%^",target);
    target->cause_typed_damage(target, "torso", random(500)+5500, "untyped");
    tell_object(target,"%^C160%^You die!%^CRST%^");
    return 1;
  }
    
    // damage
    tell_object(target,"%^C186%^The Medusa turns to gaze in your direction. You manage to look away in time to not take the brunt of her curse!%^CRST%^");
    tell_room(room,"%^C186%^The medusa turns to gaze at "+target->QCN+"%^C186%^!%^CRST%^",target);
    target->cause_typed_damage(target, target->return_target_limb(), random(200)+50, "void");
    return 1;
}

