//shadowlisten.c
//Silverwatcher, shadowlistener, and innereye are 
//essentially the same file.  If you make a change to
//one, please check to see if the others need it as well.

#include <std.h>

inherit MONSTER;

object caster, follower;
void self_destruct();
void set_scry_power(int x);
int query_scry_power();
object query_mycaster();
void query_caster();

int power;

void create(){
   ::create();

    set_name("shadowlistener");
   set_property("no detect",1);
   set_id(({"ShadowListener"}));
      set_short(0);
   set_invis();
   add_limb("body","",0,0,0);
}

int move(mixed dest){
   ::move(dest);

}

set_caster(object cas){
   caster = cas;
}

void query_mycaster(){
   return caster;
}

set_target(object ob){
   follower = ob;
}

void die(object ob){
}

void heart_beat()
{
    ::heart_beat();
   if(!objectp(caster)) return;
   if(!objectp(follower)) return;
   if(!objectp(TO)) { return; }
   if(ETO != environment(follower))
      move(environment(follower));
   if(ETO->query_property("no scry")) 
   {
       self_destruct();
       return;
   }
   if(follower->query_true_invis()) 
   {
       self_destruct();
       return;
   }
   if(present("blockerx111", follower)) 
   {
       self_destruct();
       return;
   }
}

dest_me(){
//   tell_room(ETO,"%^BLACK%^%^BOLD%^A shadow off to one side shimmers and disappears.",TO);
//Removing this since the spells are now detectable through normal
//scrying detections ~Circe~ 5/27/08
   remove();
}

catch_tell(string str){
   if(!objectp(caster)){
      dest_me();
      return;
   }

    if(userp(previous_object()))
    {
        if(!previous_object()->scry_check(caster, query_scry_power()))
            return;
    }
    
   tell_object(caster,"%^MAGENTA%^Your shadow whispers to you:%^RESET%^   "+str);

}

int do_damage(string str, int i){
    return 0;
}

int is_invincible(){ return 1;}
int is_detectable(){ return 0;}

int id(string str){
    if(objectp(TP) && !avatarp(TP)) return 0;
    else return ::id(str);
}

void query_caster() { return caster; }

void set_invis() {
  if (TO->query_invis())
    return;
  else ::set_invis();
  return;
}

void set_scry_power(int x){
   power = x;
}

void query_scry_power(){
   return power;
}

void self_destruct() {
int i;
object casterobj,*spells = ({});
   casterobj = find_player(caster);
   if(objectp(casterobj)){
      spells += casterobj->query_property("dispellable spells");
      for(i=0;i<sizeof(spells);i++){  
         if(objectp(spells[i])){
            if(!spells[i]->query_target_object()){
               if((string)spells[i]->query_spell_name() == "listening shadow"){
                  if(!objectp(spells[i])) return notify_fail("Your listening shadow seems to have been lost.\n");
                  tell_object(casterobj,"Something seems to be blocking your listening shadow!", TP);
                  spells[i]->dest_effect();
                  spells[i]->removeSpellFromCaster();
               }
            }
         }
      }
   }
   if(objectp(TO)) dest_me();
   return;
}

int is_priest_scry(){ return 1; }

//Adding this so that Detect Scrying, etc. work correctly with this spell
//~Circe~ 8/17/13
int is_scry_object(){ return 1; }
