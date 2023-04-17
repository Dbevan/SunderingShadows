#include <std.h>
#include <daemons.h>
#include <magic.h>

#include "../graez.h"

inherit MONSTER;

void create() {

  object room,nasty;
  
  nasty = this_object();
  room = environment(nasty);

  ::create();
  set_name("undead minotaur");
  set_id( ({"minotaur", "undead minotaur", "monster", "zovvut", "cavemon"}) );
  set_short("%^C251%^u%^C244%^n%^C059%^d%^C251%^e%^C059%^"+
    "a%^C251%^d %^C246%^m%^C251%^i%^C060%^n%^C251%^ota%^C060%^u%^C251%^r%^CRST%^");
  set_long("The beast that stands before you is a %^C059%^monstrosity of "+
    "undeath%^CRST%^ given %^C088%^rage%^CRST%^. It was a %^C094%^minotaur%^CRST%^ "+
    "in its life and it was an insanely large specimen of one. It wields "+
    "a %^C239%^large battleaxe%^CRST%^ easily in its hands. Its form has "+
    "been %^C059%^ruined with undeath %^CRST%^and whole chunks of %^C243%^gray "+
    "flesh%^CRST%^ seem to slough off every time it moves but there is "+
    "still %^C152%^strength%^CRST%^ in it above most mortal men.%^RESET%^\n");
  set_race("undead");
  set_body_type("minotaur");

  set_gender("male");
  set_size(3);
  set_class("fighter");
  set_mlevel("fighter", 30);
  set_hd(30,5);
  set_attack_bonus(10);
  set_max_hp(1000+random(1000));
  set_hp(query_max_hp());
  set_overall_ac(-34);
  set_property("function and attack", 1);
  set_property("full attacks", 1);
  set_property("swarm",1);
  set_stats("strength", 25);
  set("aggressive", 30);
  set_mob_magic_resistance("high");
  set_alignment(6);

  set_property("no hold", 1);
  set_property("no death", 1);
  set_property("no bows", 1);
  set_sight_bonus(5);

  set_funcs(({"overhead", "spew", "charge"}));
  set_func_chance(50);

  set_monster_feats(({
    "weapon focus",
    "expertise",
    "cleave",
  }));

  set_property("add quest", "%^C244%^Put the %^C059%^Undead "+
    "Minotaur%^C244%^ of Graez to Rest%^CRST%^");
  set_property("quest exp", 1000000);
  set_new_exp(25, "boss");
  
  if(!clonep())
      return;

  new(OBJ"minoaxe.c")->move(TO);
  command("wield axe");
  new(OBJ"thunderous_hooves.c")->move(TO);
  command("wear shoes");
  add_money("gold", random(1500));
  add_money("silver", random(4500));

}


void overhead(object targ) {

  object room,nasty;
  
  nasty = this_object();
  room = environment(nasty);

  tell_object(targ, "%^C243%^The minotaur holds its giant axe above its head and brings it down on you with a mighty growl!%^RESET%^");
  tell_room(room, "%^C243%^The minotaur delivers an overhead strike on "+targ->query_cap_name()+"%^C243%^ with its axe!%^RESET%^",targ);
  targ->cause_typed_damage(targ, targ->return_target_limb(), roll_dice(10,8)+50,"slashing");

  return;
}

void slash(object targ) {

  object room,nasty;
  
  nasty = this_object();
  room = environment(nasty);

  tell_object(targ, "%^C070%^The %^C059%^minotaur%^C070%^ opens its maw and "+
    "out spews a %^C241%^cloud of horrific darkness%^C070%^ that envelop you "+
    "and sucks the %^C194%^life%^C070%^ out of you!%^CRST%^");
  tell_room(room, "%^C070%^The minotaur engulfs%^CRST%^ "+targ->query_cap_name()+" %^C070%^in "+
    "a%^CRST%^ %^C240%^horrific cloud of darkness%^C070%^!%^CRST%^",targ);
  targ->cause_typed_damage(targ, targ->return_target_limb(), roll_dice(10,8)+100,"negative energy");

  return;

}

void charge(object targ) { // 58-130 damage on save, 108-260 + para on fail)

  object room,nasty;
  
  nasty = this_object();
  room = environment(nasty);


  tell_object(targ, "The minotaur lowers its head and charges directly at you!");
  tell_room(room, "The minotaur charges head first into "+targ->query_cap_name()+"!",targ);

  if(SAVING_THROW_D->reflex_save(targ, (40))){
    tell_object(targ, "%^C100%^You manage to get mostly out of the way of "+
      "the %^C059%^minotaur's%^C100%^ %^C229%^horns%^C100%^ and only take a glancing blow%^CRST%^");
    tell_room(room, "%^C100%^The %^C060%^minotaur%^C100%^ slams "+
      "into%^CRST%^ "+targ->query_cap_name()+"%^C100%^, who barely gets "+
      "out of the way of the %^C229%^horns%^C100%^!%^CRST%^",targ);
    targ->cause_typed_damage(targ, targ->return_target_limb(), roll_dice(10,8)+50,"bludgeoning");
    return;
  } 

  tell_object(targ, "%^C100%^You are impaled on the %^C059%^minotaur's%^C100%^ %^C229%^horns%^C100%^ "+
    "and thrown violently to the %^C094%^ground%^C100%^!%^CRST%^");
  tell_room(room, "%^C100%^The %^C059%^minotaur%^C100%^ impales %^CRST%^"+targ->query_cap_name()+" %^C100%^on "+
    "its horns then throws them to the %^C094%^ground%^C100%^!%^CRST%^",targ);
  targ->cause_typed_damage(targ, targ->return_target_limb(), roll_dice(20,8)+100,"piercing");
  targ->set_paralyzed(10, "%^C100%^You are trying to recover from being %^C160%^gored%^C100%^!%^CRST%^");

}

