//holder.c

#include <std.h>

inherit OBJECT;

object caster, mon, *foes, mySpell;
int freed=0, count=0;

void save_me(string file){return 1;}

void create(){
    ::create();
    set_name("device");
    set("id", ({ "devicex999" }) );
    set("short", "");
    set("long", "");
    set_weight(0);
}

int move(mixed dest){
	if(ETO && objectp(ETO))
		if(interactive(ETO))
			return 0;
	::move(dest);
	set_heart_beat(10);
}

set_caster(object ob)
{
	if(objectp(ob))
		caster = ob;
	else
		remove();
}

int query_freed(){return freed;}

object query_mon(){return mon;}

object query_caster(){return caster;}

set_elemental(object ob){
	if(objectp(ob))
		mon = ob;
	else
		remove();
}

set_freed(int x){
	freed = x;
}

void defend(){
    int num,numfoes;
    object *tmp;
    tmp = ({});
    numfoes=sizeof(tmp = caster->query_attackers());
	for(num =0;num < numfoes;num++){
		if(!objectp(tmp[num])) continue;
		if(member_array(tmp[num], (object *)mon->query_attackers()) != -1)
			continue;
		mon->kill_ob(tmp[num],1);
	}
    return;
}

void heart_beat()
{
	if(!objectp(mon))
		remove();
	if(!objectp(caster))
		remove();
    if(sizeof(caster->query_attackers())) { defend(); }
    // taking this out to make the elementals more useful -Ares
    /*
	if(!freed)
    {
		if(random(100) < 5)
        {
			freed = 1;
			tell_room(environment(mon),"%^BOLD%^%^YELLOW%^The elemental frees itself from the power of "+caster->query_cap_name()+"!",caster);
			tell_object(caster,"%^BOLD%^%^YELLOW%^The elemental frees itself of your power!");
			mon->kill_ob(caster);
		}
		if(!freed && (object *)caster->query_attackers() != ({}))
			defend();
	}
	if(freed){
		count++;
		if(count > 5){
			tell_room(environment(mon),"%^BOLD%^The pain being to great on this plane. The elemental returns to it's plane!");
			remove();
		}
	}*/
}

int remove()
{
    object mySpell;

    if (!objectp(this_object()))
        return;

    if (objectp(mySpell = query_property("spell")))
        mySpell->dest_effect();

    if (objectp(mon))
        mon->remove();

    return ::remove();
}

void init(){
	::init();
	add_action("cmd","command");
	add_action("dismiss","dismiss");
}

int cmd(string str){
	object ob;
	string what, who, what2, holder;
	if(!objectp(mon))
    {
        remove();
        return 0;
    }
	if(!str) return notify_fail("Care to tell it what to do?\n");
	//if(freed)
	//	return notify_fail("It has broken your spell!\n");
	if(sscanf(str, "%s to %s", who, what) != 2)
		return notify_fail("Syntax: <command OBJECT to ACTION>\n");
	if(!mon->id(who))
		return 0;
    if(what == "follow")
    {
        caster->add_follower(mon);
        tell_object(caster,"The "+mon->query_short()+" is now following you.");
        return 1;
    }
	if(what[0..3] == "kill") {
		if(sscanf(what, "kill %s",who) == 1)
			if(ob = present(who,environment(caster)))
				if(!caster->ok_to_kill(ob))
					return notify_fail("You are not allowed to kill that creature!\n");
	}
    if(what[0..3] == "wear")
        return notify_fail("This summon cannot wear anything.");
	if(!mon->force_me(what))
		return notify_fail("You fail to command the "+mon->query_name()+" to "+what+"!\n");
	return 1;
}

int dismiss(string str){
	if(!str || !mon->id(str))
		return 0;
	if(freed)
		return notify_fail("You have no power over this being any more.\n");

	tell_room(environment(caster),"%^BOLD%^"+caster->query_cap_name()+" dismisses the "+mon->query_name()+"!",caster);
    if(objectp(caster)) { caster->remove_property("has_elemental"); }
	tell_object(caster,"%^BOLD%^You dismiss the "+mon->query_name()+"!\n");

    remove();

	return 1;
}
