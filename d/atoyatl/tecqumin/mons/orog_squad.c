//Coded by Lujke
#include <std.h>
#include <daemons.h>
#include <move.h>

#include "../tecqumin.h"
#include <move.h>
#define STOCK_ROOMS ({"stockade", "centre", "archery", "elder",\
         "kitchen", "foodhall", "north_section", "east_section",\
         "south_section", "west_section", "northeast_corner",\
         "northwest_corner", "southwest_corner", "southeast_corner",\
         "battlement1", "battlement2", "battlement3", "battlement4",\
         "battlement5", "battlement6", "battlement7", "battlement8"})

inherit MONSTER;

object * orogs, waystation;
int tactic, flag, rolling, waitcount;
string env_name;
string * directions;
int is_squad_member(object orog);
int check_grouped();
void review_tactic();
int is_squad_member(object orog);
void regroup();
void die(object ob);
void die();
int do_damage(string str, int i);
int is_invincible();
int is_detectable();
void set_invis();
int is_squad_member(object orog);
int count_orogs();
void clear_directions();
void roll_ram();
void set_entry_method(string meth);
void climb_orogs();
void add_orog(object orog);
void remove_orog(object orog);
void log(string str);
void set_orogs(object * or);
void change_tactic(int tac);
object * query_orogs();
int query_tactic();
int set_tactic(int tac);
int force_me(string str);
void regroup();
void broadcast(string str, object * rooms);
string * query_walk_path();
int orog_arrives(object orog);
int check_grouped();
int move(mixed dest);
int climb_trees();
int count_ladders();
int pickup_ladder();
int reach_destination();
void start_climbing();
void goto_field();
void get_ladder();
void review_tactic();
void retreat();
void clear_rolling();
void heart_beat();

nosave int BEAT, heartcount;


void create(){
   ::create();
   waitcount = 0;
   set_property("not random monster", 1);
   set_true_invis();
   set_name("orog squad");
   set_id(({"xxsquadxx"}));
   set_property("no detect",1);
   set_short(0);
   set_hidden(1);
   add_limb("body","",0,0,0);
        set_race("no_race");
      set_speed(0);
      orogs = ({});
      tactic = random(4);
      flag = 1;
      set_no_clean(1);
   directions = ({});
   set_property("no random treasure", 1);
   BEAT = 0;
   heartcount = 0;
   check_my_heart();
}

void report(string str){
//  "/daemon/reporter_d"->report("lujke", str);
}

string short_desc(){
  if (TP->query_true_invis() || wizp(TP)){
    return "An orog squad object, filename: " + base_name(TO);
  } else {
    return "An orog squad object you shouldn't be seeing. Please make a bug report";
  }
}

void check_my_heart()
{
    if (heartcount>200){
      return;
    }
    if(BEAT > 7)
    {
        set_heart_beat(1);
        BEAT = 0;
        call_out("check_my_heart",1);
        return;
    }
    BEAT++;
    call_out("check_my_heart",1);
    return;
}

void do_walk(){
  object * critters;
  int i, num;
  num = sizeof(orogs);
  if (num<1){
    move("/d/shadowgate/void");
  }
  for (i=0;i<num;i++){
    if (!objectp(orogs[i])){ continue;}
    critters = orogs[i]->query_attackers();
    if (sizeof(critters)>0){
      return;
    }
  }
  ::do_walk();
}

void die(object ob){

}
int do_damage(string str, int i){
    return 0;
}

int is_invincible(){ return 1;}
int is_detectable(){ return 0;}

void set_invis(){
   if(TO->query_invis()) return;
   else ::set_invis();
   return;
}

int is_squad_member(object orog){
  if (member_array(orog, orogs)!=-1){
    return 1;
  }
  return 0;
}

int count_orogs(){
  return sizeof(orogs);
}

void clear_directions(){
  directions = ({});
}

string query_destination(){
  return destination;
}

string query_tactic_name(){
  switch(tactic)
  {
  case RAM:
    return "RAM";
    break;
  case SWING:
    return "SWING";
    break;
  case LADDER:
    return "LADDER";
    break;
  case WAIT:
    return "WAIT";
    break;
  case CLIMB:
    return "CLIMB";
    break;
  case STOPPED:
    return "STOPPED";
    break;
  case ARCHERY:
    return "ARCHERY";
    break;
  case WALK_IN:
    return "WALK_IN";
    break;
  case REGROUPING:
    return "REGROUPING";
    break;
  }
}

varargs void roll_ram(int repeat){
  object field, * rooms, ram, env;
  string * path, * finalpath;
  int num, i;
  report("starting to roll ram");
  if (sizeof(directions)<1){
    report("no stored directions. getting some new ones");
    if (objectp(ETO)){
      ram = (object)ETO->query_ram();
      if (objectp(ram)){
        report("valid ram available. Getting its environment");
        env = environment(ram);
        if (objectp(env)){
          env_name = file_name(env);
          if (file_name(env)==TABAXROOM + "main_field"){
            report("Orog squad trying to approach gate");
            force_me("approach gate");
            return;
          }
          report("environment valid. Getting next waystation");
          waystation = PATHFINDER_D->find_waystation(env, TABAXROOM + "main_field", 10);
          if (objectp(waystation)){
            field = find_object_or_load(TABAXROOM + "main_field");
            if (!objectp(field)){
              report("field not valid. Ending ram rolling");
              return;
            }
            report("waystation valid: " + file_name(waystation)
                     + ". Getting directions");
            path = ({});
            finalpath = ({});
            rooms = ({});
            directions = PATHFINDER_D->findpath (env, waystation, path, finalpath, rooms, 13, 0);
            if (sizeof(directions)<1){
              report("no directions found. Ending ram rolling!");
              report("env = " + file_name(env));
              report("field = " + file_name(field));
              report("path[0] = " + path[0]);
              report("finalpath[0] = " + finalpath[0]);
              report("rooms[0] = " + file_name(rooms[0]));
              return;
            }
            num = sizeof(directions);
            for (i=0;i<num;i++){
              report("go = " + directions[i]);
            }
          } else report("%^BOLD%^%^CYAN%^Oh no! Waystation is not valid! (orog squad)");
        } else report("%^BOLD%^%^YELLOW%^Oh no! Ram's environment is not valid! (orog squad)");
      } else
      {
        if (repeat >0)
        {
          report("Making the new ram clearly didn't work");
        }
        ram = new(MOB + "ram_mob");
        ram->move(env);
        report("%^BOLD%^%^MAGENTA%^Oh no! My ram is not valid! (orog squad)");
        return roll_ram(1);
      }
    } else report("%^BOLD%^%^MAGENTA%^Oh no! My environment is not valid! (orog squad)");
  } else report("%^BOLD%^%^CYAN%^We have stored directions");
  force_me("roll " + directions[0]);
  num = sizeof(directions);
  if (num == 1){
    directions = ({});
  } else {
    directions = directions[1..num-1];
  }
}

void set_entry_method(string meth){
  int i;
  if (sizeof(orogs)>0){
    for (i=0;i<sizeof(orogs);i++){
      if (objectp(orogs[i])){
        orogs[i]->set_entry_method(meth);
      }
    }
  }
}

void climb_orogs(){
  int i, num;
  object room, * removes;
  string roomname;
  num = sizeof(orogs);
  removes = ({});
  report("tell lujke attempting to make the damn orogs climb");
  if (num>0){
    for (i=0;i<num;i++){
      room = environment(orogs[i]);
      if (!objectp(room)){
        removes += ({orogs[i]});
        report("tell lujke one orog is in an invalid room");
        continue;
      }
      roomname = file_name(room);
      switch(roomname){
      case TABAXROOM + "main_field":
      case TABAXROOM + "low_field":
      case TABAXROOM + "high_field": //deliberately falling through to here
        report("tell lujke this orog is in a field. Making it approach, or climb stuff");
        orogs[i]->force_me("approach fence");
        orogs[i]->force_me("raise ladder");
        orogs[i]->force_me("climb ladder");
        orogs[i]->force_me("climb fence");
        break;
      default:
        report("tell lujke orog is not in a field. Making it start walking");
        orogs[i]->start_walking(TABAXROOM + "main_field");
      }
    }
    orogs -= removes;
  }
}

void add_orog(object orog){
  orogs += ({orog});
}

void remove_orog(object orog){
  orogs -= ({orog});
}

void crash(){
  call_out("crash_recover",9);
}

void crash_recover(){
  int i, num;
  num = sizeof(orogs);
  if (num>0){
    for (i=0;i<num;i++){
      if (present(orogs[i], ETO)){
        if (orogs[i]->query_paralyzed()>0){
          call_out("crash_recover", 4);
          return;
        }
      }
    }
  }
  force_me("approach fence");
}

void receive_orog(){
  string roomname;
  object field;
  if (!objectp(ETO)){
    return;
  }
  roomname = file_name(ETO);
  switch (roomname){
  case TABAXROOM + "battlement1":
  case TABAXROOM + "battlement2":
  case TABAXROOM + "battlement3":
  case TABAXROOM + "battlement4":
  case TABAXROOM + "battlement5":
  case TABAXROOM + "battlement6":
  case TABAXROOM + "battlement7":
  case TABAXROOM + "battlement8"://deliberately falling through to here
    if (check_grouped()==1){
      field = find_object_or_load(TABAXROOM + "main_field");
      if (objectp(field)){
        if ((string)field->query_exit("north")=="/d/shadowgate/void"){
          start_walking(TABAXROOM + "stockade");
          return;
        }
      }
      start_walking(TABAXROOM+"granary");
      set_tactic(WALK_IN);
    } else {
      if (tactic ==LADDER || tactic == CLIMB){
        climb_orogs();
      }
    }
    break;
  }
}

void log(string str){
  object battler, *kids;
  int i;
  kids = children(TABAXOBJ + "tabaxi_battler.c");
  if (sizeof(kids)>0){
    for (i=0;i<sizeof(kids);i++){
      if (objectp(kids[i])){
        battler = kids[i];
        break;
      }
    }
  }
  if (objectp(battler)){
    battler->log(str);
  }
}

void set_orogs(object * or){
  orogs = or;
}

void change_tactic(int tac){
  tactic = tac;
  review_tactic();
}

object * query_orogs(){
  return orogs;
}

int query_tactic(){
  return tactic;
}

int set_tactic(int tac){
  tactic = tac;
}

int force_me(string str) {
  int i;
  object ram;
  string * exits;
  if (sizeof(orogs)>0){
    for (i=0;i<sizeof(orogs);i++){
      if (!objectp(ETO) ||!objectp(orogs[i]) || !present(orogs[i], ETO)){
        continue;
      }
      orogs[i]->force_me(str);
    }
  }

  report("Squad being forced to : " + str);
  return ::force_me(str);
}


void regroup(){  // for getting the squad back together if they get separated
  int i, num;
  object env, * conflict_points;
  mapping dests;
  string where_squad, where_orog;
  conflict_points = ({});
  if (!objectp(ETO)){
    return;
  }
  if (sizeof(orogs)<1){
    return;
  }
  set_tactic(REGROUPING);
  where_squad = file_name(ETO);
  report("%^BOLD%^%^YELLOW%^Attempting to regroup at " + where_squad);
  dests = DESTINATIONS_D->query_waystations(where_squad);
  if (sizeof(dests)<1){
    report("No waystations for " + where_squad + ". Making some new ones");
    DESTINATIONS_D->generate_waystations(where_squad,3, 3);
  }
  stop_walking();
  num = sizeof(orogs);
  for (i=0;i<num;i++){
    if (!objectp(orogs[i])){
      continue;
    }
    env = environment(orogs[i]);
    if (!objectp(env)){
      report("BOLD%^%^CYAN%^There's a problem with one of the orogs' environment. Moving it here cheatily.");
      orogs[i]->move(ETO);
      tell_room(ETO, "An orog bursts out of the the thick jungle");
    } else{
      where_orog = file_name(env);
    }
    if (sizeof(orogs[i]->query_attackers())>0){
      report("%^BOLD%^%^WHITE%^This orog is in a conflict point");
      if (member_array(env, conflict_points)==-1){
        conflict_points += ({env});
      }
    }
    if (where_orog != where_squad){
      report("%^BOLD%^%^YELLOW%^This orog is not where the squad is");
      if (where_orog == TABAXROOM + "treetop1" ||
          where_orog == TABAXROOM + "treetop2" ||
          where_orog == TABAXROOM + "treetop3"  ||
          where_orog == TABAXOBJ + "ram_interior"){
        report("%^BOLD%^%^GREEN%^But it is either up a tree or in a ram, so it is okay. Carrying on...");
        continue;
      }
      if (sizeof(orogs[i]->query_attackers())>0){
        report("%^BOLD%^%^WHITE%^This orog is in a conflict point, so it can't move at the moment.");
        conflict_points += ({env});
      } else {
        report("%^BOLD%^%^BLUE%^Starting orog walking to " + where_squad);
        orogs[i]->start_walking(where_squad);
      }
    }
  }
  if (sizeof(conflict_points)>0){
    if (member_array(ETO, conflict_points)==-1){
      report("Some orogs are in conflict, and the squad is not there. Moving squad to " + conflict_points[0] + ", so the orogs can regroup there.");
      move(conflict_points[0]);
    }
  }
}

void broadcast(string str, object * rooms){
  int i;
  object room;
  if (sizeof(rooms)>0){
    for (i=0;i<sizeof(rooms);i++){
      room = find_object_or_load(rooms[i]);
      if (objectp(room)){
        tell_room(room, str);
      }
    }
  }
}

string * query_walk_path(){
  return path;
}

int orog_arrives(object orog){
  string where;
  if (!objectp(orog)){
    return 0;
  }
  if (check_grouped()==1){
    review_tactic();
  }
}

int check_grouped(){
  int i;
  object room;
  if (sizeof(orogs)<1 ){
    remove();
    return -1;
  }
  if (!objectp(ETO)){
    remove();
    return -1;
  }
  for (i=0;i<sizeof(orogs);i++){
    if (!objectp(orogs[i])){
      continue;
    }
    room = environment(orogs[i]);
    if (room != ETO){
      report("%^BOLD%^%^MAGENTA%^Dammit - Orog Squad "+ orogs[i]->query_property("squad name") + " is not grouped!");
      return 0;
    }
  }
  return 1;
}

void report_status(){
  object orog;
  string loc, result;
  if (objectp(ETO)) loc = file_name(ETO);
  else loc = "Invalid";
  result = "location: " + loc;
  result += " tactic: " + query_tactic_name();
  if (!catch(query_destination())) result += "destination: " + query_destination();
  else result += "No destination set";
  result += "\n\nNo of orogs: " + sizeof(orogs) + " locations:\n";
  foreach(orog in orogs)
  {
    if (objectp(orog) && objectp(environment(orog)))
    {
      result += file_name(environment(orog)) + " ";
    } else result += " invalid location ";
    if (!catch(orog->query_destination())) result += "destination: " + orog->query_destination();
    else result += "No destination set";
  }
  result += "\n";
  report(result);
}

int climb_trees(){
  int i, num;
  object room, treetop;
  string roomname, treetop_name;
  num = sizeof(orogs);
  if (num<1){
    report("tell lujke I'm trying to climb_trees but there are no orogs in my squad!");
    return 0;
  }
  for (i=0;i<num;i++){
    if (!objectp(orogs[i])){ continue;}
    room = environment(orogs[i]);
    if (!objectp(room)){
      report("tell lujke An %^GREEN%^orog%^RESET%^ is stuck in a faulty room in the"
              +" orog_squad climb_trees function!");
      orogs[i]->move("/d/shadowgate/void");
      continue;
    }
    if (!objectp(ETO)){
      return -1;
    }
    if (room == ETO){
      orogs[i]->force_me("climb");
      break;
    } else {
      treetop_name= ETO->query_climb_exit("climb");
      treetop = find_object_or_load(treetop_name);
      roomname = file_name(room);
      if (roomname == (string)treetop->query_target_room_name("south")){
        continue;
      }
    }
  }
  if (i<num-1){
    call_out("climb_trees", 2+random(3));
  } else {
    report("climb tree");
    report("swing to south");
  }
  return 1;
}

int count_ladders(){
  int i, num, count;
  num = sizeof(orogs);
  count = 0;
  if (num>0){
    for (i=0;i<num;i++){
      if (present("ladder", orogs[i])){
        count ++;
      }
    }
  }
  return count;
}

int pickup_ladder(){
  if (count_ladders()>1){
    return -1;
  }
  orogs[0]->force_me("get ladder");
  call_out("goto_field", 2);
  return 1;
}

int reach_destination(){
  string where;
  mapping dests;
  object * battlers,battler, ram, lujke, field, * rooms, * ladders;
  int i, num;
  lujke = find_player("lujke");
  where = file_name(ETO);
  switch (where){
  case TABAXROOM + "granary":
    num = sizeof(orogs);
    if (num<1){
      report("tell lujke I have no orogs left. Vaping!");
      move("/d/shadowgate/void");
    }
    for (i=0;i<num;i++){
      orogs[i]->reach_destination();
    }
    break;
  case TABAXROOM + "treetop_base1": //deliberately falling through
  case TABAXROOM + "treetop_base2": //deliberately falling through
  case TABAXROOM + "treetop_base3":
    if (tactic == SWING){
      switch(random(3)){
      case 0:
        break;
      case 1:
        force_me("west");
        break;
      case 2:
        force_me("emote breaks into a run");
        force_me("west");
        force_me("west");
        break;
      }
      if (check_grouped()!=1){
        regroup();
        return 0;
      }
      climb_trees();
//      report("climb");
//     report("swing to south");
      return 1;
      break;
    }
    break;
  case TABAXROOM + "main_field": // deliberately falling through
  case TABAXROOM + "low_field": // deliberately falling through
  case TABAXROOM + "high_field":
    if ((string)ETO->query_exit("north")== TABAXROOM + "stockade"){
      set_entry_method("walking right in through the open gate.");
      start_walking(TABAXROOM + "granary");
      break;
    }
    if (tactic == LADDER || tactic ==CLIMB){
      force_me("approach fence");
      return 1;
    } else {
      report("tell lujke arrived at a field, but the tactic was NOT ladders");
      if (tactic == ARCHERY){
        rooms = ({TABAXROOM + "battlement1", TABAXROOM + "battlement2",
                  TABAXROOM + "battlement3", TABAXROOM + "battlement4",
                  TABAXROOM + "battlement5", TABAXROOM + "battlement6",
                  TABAXROOM + "battlement7", TABAXROOM + "battlement8",
                  TABAXROOM + "watchtower"});
        broadcast("%^BOLD%^%^RED%^A squad of %^RESET%^%^GREEN%^orog%^BOLD%^%^RED%^"
        +" crossbow archers arrives in %^RESET%^"+ ETO->query_name()
        +"%^BOLD%^%^RED%^ and begins preparing to fire on the battlements!"
          , rooms);
        battlers = children(TABAXOBJ + "tabaxi_battler");
        if (sizeof(battlers)>0){
          for (i=0;i<sizeof(battlers);i++){
            battler = battlers[i];
            if (objectp(battler) && objectp(environment(battler))){
              battler->call_out("orog volley", 7, ETO);
              break;
            }
          }
        }
        return 1;
      }
      if (tactic != RAM ){
        if (waitcount > 15){
          switch(random(2)){
          case 0:
            tactic = RAM;
            break;
          case 1:
            tactic = LADDER;
            break;
          }
          call_out("reach_destination", 10);
          return 1;
        }
      }
    }
    break;
  }
  if (tactic ==RAM){
    report("tell lujke %^BOLD%^%^YELLOW%^Checking to see whether a"
                 +" ram is present at %^RESET%^" + ETO->query_name());
    ram = present("ram", ETO);
    if (objectp(ram)){
      report("tell lujke Ram present. Checking whether there are"
                          +" orogs available");
      if (sizeof(orogs)>0){
        field = find_object_or_load(TABAXROOM + "main_field");
        report("tell lujke Orogs present. Entering Ram");
        waystation = PATHFINDER_D->find_waystation(ETO, TABAXROOM + "main_field");
        directions = ({});
        force_me("enter ram");
        move(TABAXOBJ + "ram_interior");
        review_tactic();
        return 1;
      }
    }
  }
  if (tactic ==LADDER){
    if (present("ladder", ETO)){
      pickup_ladder();
      return 0;
    }else {
      get_ladder();
    }
  }
  return 0;
}

void start_climbing(){
  force_me("raise ladder");
  call_out("force_me", 4, "climb ladder");
  call_out("force_me", 6, "climb fence");
}

void goto_field(){
  int i;
  string field_name, where;
  mapping dests;
  i = random(3);
  switch(i){
  case 0:
    field_name = TABAXROOM  + "main_field";
    break;
  case 1:
    field_name = TABAXROOM  + "low_field";
    break;
  case 2:
    field_name = TABAXROOM  + "high_field";
    break;
  }
  dests = DESTINATIONS_D->query_waystations(field_name);
  if (sizeof(dests)<1){
    DESTINATIONS_D->generate_waystations(field_name,3, 4);
  }
  if (is_walking>0){
    return;
  }
  start_walking(field_name);
  return;
}

void get_ladder(){
  object * ladders, room, storage, ladder;
  int i, j, num;
  string roomname, * waystations;
  report("%^B_CYAN%^OROG SQUAD Attempting to get ladders");
  storage = find_object_or_load(TABAXROOM + "ladder_store");
  if (objectp(storage)){
    ladder  = present("ladder", storage);
    if (objectp(ladder)){
      start_walking(TABAXROOM + "ladder_store");
      return;
    }
  }
  roomname = "nowhere";
  ladders = children(TABAXOBJ + "ladder");
  num = sizeof(ladders);
  if (num>0){
    for (i=0;i<num;i++){
      room = environment(ladders[i]);
      if (!objectp(room)){
        continue;
      }
      if (living(room)){
        continue;
      }
      roomname = file_name(room);
      if (roomname == "/d/shadowgate/void"){
        roomname = "nowhere";
        continue;
      }
      break;
    }
  }
  if (roomname == "nowhere"){
    report("Orog Squad: There are no ladders anywhere. We are going to climb the fence");
    tactic = CLIMB;
    goto_field();
    return;
  }
  waystations = DESTINATIONS_D->query_waystations(roomname);
  if (sizeof(waystations)<1){
    DESTINATIONS_D->generate_waystations(roomname,4, 4);
  }
  report("Orog Squad: Starting to walk to " + roomname +" to get ladders");
  start_walking(roomname);
}

int am_in_stockade(){
    if(!environment(this_object()))
        return 0;
    
    if (!stringp(ETO->get_name()))
        return 0;
    if (member_array((string)ETO->get_name(), STOCK_ROOMS)!=-1) return 1;
    return 0;
}

void move_through_stockade()
{
  mapping orig_exits;
  string * orig_directions, gran_dir;
  report("%^B_RED%^%^BOLD%^%^WHITE%^Moving through the stockade");
  orig_exits = ETO->query_orig_exits();
  orig_directions = keys(orig_exits);
  gran_dir = ETO->query_granary_dir();
  if (member_array(gran_dir, orig_directions)!=-1)
  {
    force_me("dig " + gran_dir);
    return;
  } else
  {
    report("%^B_ORANGE%^Walking " + gran_dir + " toward the granary");
    force_me(gran_dir);
  }
}

void review_tactic(){
  int i, flag, num;
  string room,* steps, squadname, * orig_directions, gran_dir;
  object * rams, * battlers, ram, newenv, field, ladder, *orog_room;
  mapping dests, orig_exits;
  if (am_in_stockade())
  {
    move_through_stockade();
  } else report("%^B_MAGENTA%^%^BOLD%^%^WHITE%^I am not in a stock room: " + file_name(ETO));


  if (!objectp(ETO)){
    num = sizeof(orogs);
    if (num>0){
      for (i=0;i<num;i++){
        if (!objectp(orogs[i])){
          continue;
        }
        if (objectp(environment(orogs[i]))){
          TO->move(environment(orogs[i]));
          if (check_grouped()<1 ){
            if (tactic != REGROUPING && tactic != SWING && tactic !=LADDER){
              regroup();
            }
            return;
          }
          break;
        }
      }
    }
  }
  if (!objectp(ETO)) { return;}
  field = find_object_or_load(TABAXROOM + "main_field");
  if (objectp(field)){
    if ((string)field->query_exit("north") == TABAXROOM + "stockade"){
      tactic = WALK_IN;
      start_walking(TABAXROOM + "granary");
      return;
    }
  }
  room = file_name(ETO);
  if (room == TABAXROOM + "battlement1" ||
      room == TABAXROOM + "battlement2" ||
      room == TABAXROOM + "battlement3" ||
      room == TABAXROOM + "battlement4" ||
      room == TABAXROOM + "battlement5" ||
      room == TABAXROOM + "battlement6" ||
      room == TABAXROOM + "battlement7" ||
      room == TABAXROOM + "battlement8" ||
      room == TABAXROOM + "stockade" ||
      room == TABAXROOM + "southeast_corner" ||
      room == TABAXROOM + "southwest_corner" ){
    if (check_grouped()==1){
      start_walking(TABAXROOM + "granary");
      return;
    } else {
      report("tell lujke we're not grouped. Can't walk to granary yet");
      report("tell lujke tactic is " + tactic);
      climb_orogs();
      num = sizeof(orogs);
      for (i=0;i<num;i++){
        if (!objectp(orogs[i])){continue;}
        orog_room = environment(orogs[i]);
        report("tell lujke one orog at " + file_name(orog_room));
      }
      return;
    }
  }
  switch(tactic){
  case WAIT:
    if (random(4) <3){
      return;
    }
    goto_field();
    return;
  case SWING:
    room = file_name(ETO);
    if (room == TABAXROOM + "treetop_base1" ||
        room == TABAXROOM + "treetop_base2" ||
        room == TABAXROOM + "treetop_base3"){
      climb_trees();
      break;
    }
    if (query_walking()==1){
      return;
    }
    dests = DESTINATIONS_D->query_waystations(TABAXROOM + "treetop_base1");
    if (sizeof(dests)<1){
      DESTINATIONS_D->generate_waystations(TABAXROOM + "treetop_base1",4, 4);
    }
    start_walking(TABAXROOM + "treetop_base1");
    break;
  case LADDER:
    if (query_walking()==1){
      return;
    }
    num = sizeof(orogs);
    if (!objectp(ETO)||num<1){
      remove();
      break;
    }
    flag = -1;
    for (i=0;i<num;i++){
      if (!objectp(orogs[i])){
        continue;
      }
      ladder = present("ladder", orogs[i]);
      if (objectp(ladder)){
        flag = 1;
        break;
      }
    }
    if (flag ==1){
      if (file_name(ETO)==TABAXROOM + "main_field" ||
          file_name(ETO)==TABAXROOM + "high_field" ||
          file_name(ETO)==TABAXROOM + "low_field" ){
        force_me("approach fence");
        return;
      }
      report("%^B_CYAN%^OROG SQUAD Has a ladder. Attempting to goto to a field");
      goto_field();
      return;
      break;
    }
    report("Orog squad: We don't have a ladder. Trying to get one.");
    get_ladder();
    break;
  case RAM:
    if (objectp(ETO) && present("ram", ETO)){
      if (check_grouped()==1){
        force_me("enter ram");
        call_out("review_tactic", 2);
      } else {
        regroup();
      }
    }
    if (objectp(ETO)){
      if (file_name(ETO) == TABAXOBJ + "ram_interior"){
        roll_ram();
        return;
      }
    }
    rams = children(MOB + "ram_mob");
    flag = -1;
    if (sizeof(rams)>1){
      for (i=0;i<sizeof(rams);i++){
        if (objectp(rams[i])){
          if (rams[i]->query_hp()>1 && objectp(environment(rams[i]))){
            flag = i;
          }
        }
      }
    }
    if (flag <0 || !objectp(environment(rams[flag]))){
      broadcast_area(TABAXROOM,"%^BOLD%^%^RED%^A howl of %^GREEN%^anguish%^RED%^ rises through the"
                    +" %^RESET%^%^GREEN%^j%^BOLD%^%^GREEN%^u%^RESET%^%^GREEN%^ng%^BOLD%^%^GREEN%^l"
                    +"%^RESET%^%^GREEN%^e as the %^BOLD%^%^BLACK%^orogs %^RED%^realise their"
                    +" %^RESET%^%^ORANGE%^battering ram %^BOLD%^%^RED%^is broken!");
      switch(random(3)){
      case 0:
        tactic = SWING;
         break;
      case 1:
        tactic = WAIT;
        break;
      case 2:
        tactic = LADDER;
        break;
      }
      return;
    }
    room = file_name(environment(rams[flag]));
    dests = DESTINATIONS_D->query_waystations(room);
    if (sizeof(dests)>0){
      DESTINATIONS_D->generate_waystations(room,4, 4);
    }
    start_walking(room);
    break;
  case STOPPED:
    return;
  case ARCHERY:
    room = file_name(ETO);
    if (room == TABAXROOM + "high_field" || room == TABAXROOM + "main_field"
                                         || room == TABAXROOM + "low_field"){
      if (random(3)>0){
        battlers = children(TABAXOBJ + "tabaxi_battler");
        if (sizeof(battlers)>0){
          for (i=0;i<sizeof(battlers);i++){
            if (objectp(battlers[i])){
              battlers[i]->orog_volley(ETO);
              report("tell lujke %^BOLD%^%^YELLOW%^Firing a blooming volley!");
              break;
            }
          }
        }
      }
    } else {
      goto_field();
    }
    break;
  case WALK_IN:
    if (!query_walking()){
      start_walking(TABAXROOM + "granary");
    }
    break;
  case REGROUPING:
    if (check_grouped()<1){
      break;
    } //otherwise deliberately falling through
  case CLIMB:
    if (room != TABAXROOM + "main_field" && room != TABAXROOM + "high_field" &&
        room != TABAXROOM + "low_field") {
      goto_field();
      return;
    } else {
      force_me("approach fence");
    }
  default:
    if (check_grouped()<1 ){
      regroup();
    } else {
      tactic = random(4);
    }
  }
}

void retreat(){
  object room;
  string * dests;
  room = TABAXROOM + "orog_entry";
  dests = DESTINATIONS_D->query_waystations(room);
  if (sizeof(dests)>0){
    DESTINATIONS_D->generate_waystations(room,4, 4);
  }
  start_walking(room);
}

void clear_rolling(){
  rolling = 0;
}

void heart_beat(){
  int i;
  object * removes;
  string room;
  ::heart_beat();
  removes = ({});
  for (i=0;i<sizeof(orogs);i++){
    if (!objectp(orogs[i])){
      removes += ({orogs[i]});
    }
  }
  rolling --;
  orogs -= removes;
  if (objectp(ETO)){
    if (file_name(ETO)== TABAXOBJ + "ram_interior"){
      if ( rolling <1){
        rolling =2;
        roll_ram();
      }
      return;
    }
    if (present("player", ETO)){
      heartcount = 0;
    }
  }
  if (am_in_stockade() && check_grouped())
  {
    report("%^B_WHITE%^%^BOLD%^%^BLACK%^I am in the stockade: " + file_name(ETO));
    move_through_stockade();
    return;
  } else {
    if (am_in_stockade()) report("%^B_MAGENTA%^%^BOLD%^%^WHITE%^I am in the stockade but not grouped");
  }
  if (random(7)<2){
    review_tactic();
  }
}
