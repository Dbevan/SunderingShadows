//Graez monster core

#include <std.h>
#include "../graez.h"

#include <daemons.h>
inherit MONSTER;

int rank, faction, resting, resting_time, recruited;
int what_should_I_do();
int move_me();
int resting();


void create() {
   object ob;
   ::create();
   set_name("loki monster");
   set_id(({"loki monster"}));
   set_short("loki monster");
   set_long("loki monster");
   set_race("human");
   set_body_type("humanoid");
   set_max_hp(100);
   set_hp(query_max_hp());
   set_size(2);
   faction = 0;
   recruited = 0;
   set_moving(1);
   set_speed(60);
}


void heart_beat()
{
    ::heart_beat();

    if (!objectp(TO)) {
        return;
    }
    if(!clonep())
        return;
    
// am I in combat?
    if (sizeof(TO->query_attackers())) {
        return;
    }

// am I resting?
    if (resting == 1){
		//force_me("say I am STILL resting for "+resting_time+" seconds!");
		resting_time --;
		if(resting_time <= 0){
			resting = 0;
		}
		return;
	}
what_should_I_do();
return;
}


int what_should_I_do()
{
int i;

i = random(20);
switch(i)
{
	case 0..5 : 
		move_me();
		break;
	case 6..7 : 
		rally();
		break;
	case 8..9 :
		resting();
		break;
	case 10..19 :
	    break;
}
return;
}

int move_me()
{
object this_room;

this_room = ETO;
// Turn back on when you're ready - this_room->set_faction(faction);

//force_me("say I am moving!");
if(rank==4){
	return 1; // commanders dont move
}

switch(faction)
{	
	case 1:
	if(random(10)) force_me("northeast");
	else force_me("southeast");
	break;
	case 2:
		if(random(10)) force_me("southwest");
	else force_me("northwest");
	break;
	
}
}

int resting()
{
if (rank == 1){
move_me();
return;
}
resting_time = random(10)+3;

//force_me("say I am resting for "+resting_time+" seconds!");

return;

}


int rally(object ob)
{
object mon;
	
//force_me("say I am rallying!");

if(recruited > 1){
	return 1;
	//they already recruited twice 
}

switch(rank)
{
	case 1: 
	// too low to recruit more forces, just stop!
	break;
	
	case 2: 
	// corporals can only recruit grunts
		switch(faction)
		{	
	case 1:
    //force_me("say I am a corporal!");
	mon=new(MOBS"human_grunt.c");
    mon->move(ETO);
	mon=new(MOBS"human_grunt.c");
    mon->move(ETO);
    recruited++;
	break;
	case 2:
		//add undead mobs
    //force_me("say I am a corporal!");
	mon=new(MOBS"undead_grunt.c");
    mon->move(ETO);
	mon=new(MOBS"undead_grunt.c");
    mon->move(ETO);
    recruited++;
	break;
	
}
	break;
	
	case 3: 
	// captains can recruit some grunts.. and some corporals
		switch(faction)
		{	
			case 1:
            //force_me("say I am a Captain!");
			mon=new(MOBS"human_corporal.c");
			mon->move(ETO);
			mon=new(MOBS"human_grunt.c");
			mon->move(ETO);
			mon=new(MOBS"human_grunt.c");
			mon->move(ETO);
			recruited++;
			break;
			case 2:
			//add undead mobs
            //force_me("say I am a Captain!");
			mon=new(MOBS"undead_corporal.c");
			mon->move(ETO);
			mon=new(MOBS"undead_grunt.c");
			mon->move(ETO);
			mon=new(MOBS"undead_grunt.c");
			mon->move(ETO);
			recruited++;
	break;
	
}
	break;
	
	case 4: 
	// commanders can recruit captains and some grunts
		switch(faction)
		{	
			case 1:
            //force_me("say I am a commander!");
			mon=new(MOBS"human_captain.c");
			mon->move(ETO);	
			mon=new(MOBS"human_captain.c");
			mon->move(ETO);	
			mon=new(MOBS"human_corporal.c");
			mon->move(ETO);	
			mon=new(MOBS"human_grunt.c");
			mon->move(ETO);	
			mon=new(MOBS"human_grunt.c");
			mon->move(ETO);
			recruited++;	
			break;
			case 2:
			//add undead mobs
            //force_me("say I am a commander!");
			mon=new(MOBS"undead_captain.c");
			mon->move(ETO);	
			mon=new(MOBS"undead_captain.c");
			mon->move(ETO);
			mon=new(MOBS"undead_corporal.c");
			mon->move(ETO);
			mon=new(MOBS"undead_grunt.c");
			mon->move(ETO);
			mon=new(MOBS"undead_grunt.c");
			mon->move(ETO);
			recruited++;	
		break;
	
}
	break;
	


}
}

void die(object ob){

object war_room;

war_room=find_object_or_load("/realms/loki/graez/storage/special_war.c");

    if(objectp(war_room) && rank && faction)
        war_room->something_died(rank,faction);

   ::die(ob);
}

void init()
{
	
	if(!objectp(TO)) { return; }
    if(!objectp(TP)) { return; }
    if(!objectp(ETO)) { return; }

    ::init();
	
    if(TP->query_true_invis()) return;
	
switch(faction)
{	
	case 1:
{

    if(TP->query_true_invis()) return;
    if((int)TP->query_race() == "undead")
    {
        kill_ob(TP,1);
    }
}


 	break;
	case 2:

    if(TP->query_true_invis()) return;
    if((int)TP->query_race() != "undead")
    {
        kill_ob(TP,1);
    }

	break;
	
}
}

