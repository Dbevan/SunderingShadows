//      /std/user/tsh.c
//      from the Nightmare Mudlib
//      tsh- the TMI-shell or Tru-shel
//      created by Truilkan@TMI 920205
//      bug in $h & $H options fixed by Pallando@Nightmare 930709

/**
 * @file
 * @brief Interactive shell
 */

#include <std.h>
#include <adt_defs.h>
#include <commands.h>
#include <daemons.h>
#include <tsh.h>
#include <new_exp_table.h>

#define ABLOCK_WHITELIST ({"quit", "help"})+TP->query_channels()
#define PREPARE_WHITELIST ({ "quit", "help", "score", "bio", "who", "hp", "review", "describe", "adjective", "feats", "master", "abandon", "gmcp", "ignore", "threaten", "biography", "pcnote", "speech", "advance", "game", "date", "limbs", "prompt", "skills", "reward", "crayon", "bboard", "follow", "stats", "money", "cooldowns", "followers", "corpselimbs", "account", "wimpydir", "set", "keep", "brief", "briefcombat", "elementalist", "thought", "uptime", "unignore", "nickname", "unalias", "mmap", "inventory", "last", "eq", "recall" }) + this_player()->query_channels()

#define DEFAULT_PROMPT "%^BOLD%^%^BLACK%^-%^RED%^> "
#define MAX_HIST_SIZE  50
#define MIN_HIST_SIZE  20
#define MAX_PUSHD_SIZE 50

/**
 * @file
 * @brief shell implementation
 */

inherit CSTACK_ADT;       /* for pushd and popd */

nosave string tsh_prompt;
nosave int cur, hist_size, pushd_size, custom_prompt, aBlock, timeBlock;

string do_nicknames(string arg);
string do_alias(string arg);
string handle_history(string arg);
int tsh(string file);

int do_new()
{
    string d1, d2;

	tsh_prompt = (string)this_object()->getenv("prompt");
	tsh_prompt = !tsh_prompt ? DEFAULT_PROMPT : tsh_prompt + " ";
	custom_prompt = (tsh_prompt != DEFAULT_PROMPT);

	d1 = (string)this_object()->getenv("pushd");
	pushd_size = 0;
	if (d1)
		sscanf(d1,"%d",pushd_size);
	if (pushd_size > MAX_PUSHD_SIZE)
		pushd_size = MAX_PUSHD_SIZE;

	d1 = (string)this_object()->getenv("history");
	hist_size = 0;
	if (d1)
		sscanf(d1,"%d",hist_size);
	if (hist_size > MAX_HIST_SIZE)
		hist_size = MAX_HIST_SIZE;
    if (!hist_size)
       hist_size = MIN_HIST_SIZE;
	return 1;
}

int pushd(string arg) {
    string path;

    path = (string)this_object()->get_path();
    if((int)CD->cmd_cd(arg) && cstack::enqueue(path) == -1) {
        cstack::dequeue();
        cstack::enqueue(path);
    }
    return 1;
}

int popd() {
    mixed dir;

    dir = cstack::pop();
    if((int)dir == -1) write("Directory stack is empty.\n");
    else CD->cmd_cd((string)dir);
    return 1;
}

void initialize() {
    string rcfile;

    do_new();
    if(pushd_size) cstack::alloc(pushd_size);
    if(hist_size) history::alloc(hist_size);
    rcfile = user_path((string)this_player()->query_name()) + ".login";
    if(file_size(rcfile) > -1) this_object()->tsh(rcfile);
}

string write_prompt()
{
    object shape;
    string path, prompt, am_invis, tmp, shape_race, light_level;
    int rage, expperc;
    if( custom_prompt )
    {
        prompt = tsh_prompt;
        path = (string)this_player()->get_path();
        tmp = user_path((string)this_player()->query_name());
        tmp = tmp[0 .. strlen(tmp)-2];
        if (stringp(path) && sscanf(path, "/realms/%s", tmp) == 1) {
            path = "~" + tmp;
        }
        if (!stringp(path)) {
            path = ">";
        }
        if (avatarp(this_player())) {
            am_invis = (!this_player()->query_true_invis()) ? "." : "T";
            am_invis += (!this_player()->query_magic_hidden()) ? "." : "M";
            am_invis += (!this_player()->query_hidden()) ? "." : "S";
        }
        shape = TP->query_property("shapeshifted");
        if (!objectp(shape)) {
            shape = TP->query_property("altered");
        }

        {
            int lvl = query_adjusted_character_level();
            if(lvl>1&&lvl<100)
            {
                int expcurlvl = total_exp_for_level(lvl);
                int expnextlvl = total_exp_for_level(lvl + 1);
                expperc = (query_exp()-expcurlvl)*100/(expnextlvl-expcurlvl);
            }
        }
        if (objectp(shape)) {
            shape_race = (string)shape->query_shape_race();
        }
        if (shape_race) {
            prompt = replace_string(prompt, "$f", shape_race);
        }else {
            prompt = replace_string(prompt, "$f", "");
        }
        rage = (int)TP->query_property("raged");
        if (rage) {
            prompt = replace_string(prompt, "$r", "Enraged");
        }else {
            prompt = replace_string(prompt, "$r", "");
        }

        if (TP->is_class("eldritch_knight")) {
            if (FEATS_D->is_active(TP, "eldritch warfare")) {
                prompt = replace_string(prompt, "$_EW", "Warfare");
            }
            else prompt = replace_string(prompt, "$_EW", "");
        }

        if(TP->is_class("bard")) {
            object* inv;
            string inspiration;

            inv = all_inventory(TP);
            inv = filter_array(inv, (: $1->is_inspiration() :));

            if (sizeof(inv)) {
                prompt = replace_string(prompt, "$_BI", inv[0]->query_active_inspiration());
            }
            else prompt = replace_string(prompt, "$_BI", "");
        }

        if(this_player()->is_class("magus")) {
            if(this_player()->query_property("magus cast")){
                prompt = replace_string(prompt, "$_SC", "Spell Combat");
            } else {
                prompt = replace_string(prompt, "$_SC", "");
            }

            if(this_player()->query_property("spell recall")){
                prompt = replace_string(prompt, "$_SR", "Spell Recall");
            } else {
                prompt = replace_string(prompt, "$_SR", "");
            }
        }
        
        if(this_player()->is_class("warlock"))
        {
            if(this_player()->query_property("eldritch shield"))
                prompt = replace_string(prompt, "$_ES", "Eldritch Shield");
            else
                prompt = replace_string(prompt, "$_ES", "");
        }

        if(this_player()->is_class("psion") || this_player()->is_class("psywarrior"))
        {
            if(this_player()->query("available focus") == 1)
                prompt = replace_string(prompt, "$_PF", "Focused");
            else if(this_player()->query("available focus") > 1)
                prompt = replace_string(prompt, "$_PF", "Firmly Focused");
            else
                prompt = replace_string(prompt, "$_PF", "");
        }

        if ((int)TP->query("maximum ki")) {
            prompt = replace_string(prompt, "$k", "" + (int)TP->query("available ki"));
            prompt = replace_string(prompt, "$K", "" + (int)TP->query("maximum ki"));
        }else {
            prompt = replace_string(prompt, "$k", "");
            prompt = replace_string(prompt, "$K", "");
        }
        if ((int)TP->query("maximum arcana")) {
            prompt = replace_string(prompt, "$a", "" + (int)TP->query("available arcana"));
            prompt = replace_string(prompt, "$A", "" + (int)TP->query("maximum arcana"));
        }
        else {
            prompt = replace_string(prompt, "$a", "");
            prompt = replace_string(prompt, "$A", "");
        }
        if ((int)TP->query("maximum grace")) {
            prompt = replace_string(prompt, "$z", "" + (int)TP->query("available grace"));
            prompt = replace_string(prompt, "$Z", "" + (int)TP->query("maximum grace"));
        }
        else {
            prompt = replace_string(prompt, "$z", "");
            prompt = replace_string(prompt, "$Z", "");
        }
        if((int)TP->query("maximum burn"))
        {
            prompt = replace_string(prompt, "$u", "" + (int)TP->query("available burn"));
            prompt = replace_string(prompt, "$U", "" + (int)TP->query("maximum burn"));
        }
        else
        {
            prompt = replace_string(prompt, "$u", "");
            prompt = replace_string(prompt, "$U", "");
        }
        if ((int)TP->query_mp()) {
            prompt = replace_string(prompt, "$p",
                                    "" + (int)this_player()->query_mp());
        }else {
            prompt = replace_string(prompt, "$p",
                                    "0");
        }
        if ((int)TP->query_max_mp()) {
            prompt = replace_string(prompt, "$P",
                                    "" + (int)this_player()->query_max_mp());
        }else {
            prompt = replace_string(prompt, "$P",
                                    "0");
        }
        prompt = replace_string(prompt, "$_UP", "" + this_player()->query_property("raised"));
        prompt = replace_string(prompt, "$_MP", "" + MAX_RAISE);
        prompt = replace_string(prompt, "$EL", strlen(this_player()->query("elementalist")) ? this_player()->query("elementalist") : "none");
        prompt = replace_string(prompt, "$D", path );
        prompt = replace_string(prompt, "$h",
                                 "" + (int)this_player()-> query_hp() );
        prompt = replace_string(prompt, "$H",
                                 "" + (int)this_player()-> query_max_hp() );
        prompt = replace_string(prompt, "\\n", "\n" );
        prompt = replace_string(prompt, "$N", lower_case(mud_name()) );
        prompt = replace_string(prompt, "$L", " "+this_object()->query_spoken());
        prompt = replace_string(prompt, "$C", ""+query_cmd_num() );
        prompt = replace_string(prompt, "$s", ""+this_object()->query_condition_percent());
        prompt = replace_string(prompt, "$S", ""+this_object()->query_condition_string());
        prompt = replace_string(prompt, "$W", ""+this_object()->query_wimpy()+"%");
        prompt = replace_string(prompt, "$x", ""+encumbrance2str(this_player()));
        prompt = replace_string(prompt, "$i", ""+hunger2str(this_player()));
        prompt = replace_string(prompt, "$_HP", ""+hunger_percent(this_player()));
        prompt = replace_string(prompt, "$o", ""+thirst2str(this_player()));
        prompt = replace_string(prompt, "$_TP", ""+thirst_percent(this_player()));
        prompt = replace_string(prompt, "$T", ""+EVENTS_D->query_time_of_day());
        prompt = replace_string(prompt, "$t", ""+sprintf("%2d",hour(time()))+":"+sprintf("%.2d",minutes(time())));
        prompt = replace_string(prompt, "$B", ""+bloodlust2string(this_player()));
        prompt = replace_string(prompt, "$e", ""+expperc);
        prompt = replace_string(prompt, "$d", ""+"cmds/mortal/_hp.c"->intox2str(TP->query_intox()*100 / TP->query_formula()));

        switch(total_light(environment(this_object()))) {
            case -20..-4:
                light_level = "Darkest";
                break;
            case -3..-2:
                light_level = "Darker";
                break;
            case -1..0:
                light_level = "Dark";
                break;
            case 1:
                light_level = "Normal";
                break;
            case 2..3:
                light_level = "Bright";
                break;
            case 4..5:
                light_level = "Brighter";
                break;
            case 6..20:
                light_level = "Brightest";
                break;
            default:
                light_level = "Unreasonable - stop it!";
                break;
        }
        if(stringp(light_level)) prompt = replace_string(prompt, "$_LL", light_level);

        if (stringp(this_object()->query("warlock_blast_type"))) {
            prompt = replace_string(prompt, "$E", "" + this_object()->query("warlock_blast_type") + "");
        } else {
            prompt = replace_string(prompt, "$E", "No Essence");
        }
        if (stringp(this_object()->query("cast and attack"))) {
            prompt = replace_string(prompt, "$w", "Warfare");
        } else {
            prompt = replace_string(prompt, "$w", "");
        }
        if (avatarp(this_player())) {
            prompt = replace_string(prompt, "$I", am_invis);
        }
        prompt += " ";
    } else {
        prompt = DEFAULT_PROMPT;
    }
    message("prompt", prompt, this_object());
    return prompt;
}

int adminBlock(){
    if (archp(this_object())) {
        return 0;
    }
    return aBlock || (timeBlock > time());
}


nomask string process_input(string arg)
{
    string first_arg;
    mapping my_aliases, my_nicks;  

    if (!stringp(arg) || arg == "")
        return arg;

    if(this_player()->query_property("memorizing") && strlen(arg))
    {
        first_arg = explode(do_alias(do_nicknames(arg)), " ")[0];
        
        /*
        if(strlen(arg))
        {
            first_arg = explode(arg, " ")[0];
            my_aliases = this_player()->query_aliases();
            if(member_array(first_arg, keys(my_aliases)) >= 0)
            {
                first_arg = my_aliases[first_arg];
            }
            else 
            {
                my_nicks = this_player()->query_nicknames(); 
                if(member_array(first_arg, keys(my_nicks)) >= 0)
                    first_arg = my_nicks[first_arg];
            }
        }
        */
        
        if(member_array(first_arg, PREPARE_WHITELIST) < 0)
        {
            this_player()->remove_property("memorizing");
            message("damage", "%^BOLD%^%^GREEN%^You stop your preparations to do something else!", this_object());
        }
    }
            
    /*
    if (this_player()->query_property("memorizing") && member_array(first_arg, PREPARE_WHITELIST) == -1) {
        this_player()->remove_property("memorizing");
        message("damage", "%^BOLD%^%^GREEN%^You stop your preparations to do something else!", this_object());
    }
    */
    USER_D->process_pkill_input(this_player(), arg);
    //TODO: log to syslog
    if (wizardp(TP)) {
        log_file("tshlog/wizard/" + TP->query_true_name(), do_alias(do_nicknames(handle_history(arg))) + "\n", 1);
    }else if (avatarp(TP)) {
        log_file("tshlog/avatar/" + TP->query_true_name(), do_alias(do_nicknames(handle_history(arg))) + "\n", 1);
    }else {
        log_file("tshlog/player/" + TP->query_true_name(), do_alias(do_nicknames(handle_history(arg))) + "\n", 1);
    }

    if (adminBlock()) {
        if (member_array(arg, ABLOCK_WHITELIST) == -1) {
            write("You are otherwise occupied.");
            arg = "";
        }
    }

    if (arg && arg != "") {
        return do_alias(do_nicknames(handle_history(arg)));
    }else {
        return arg;
    }
}

int tsh(string file) {
   string contents, *lines;
   int j, len, finished;

   if (geteuid(previous_object()) != geteuid()) return 0;
   if (!file) {
      notify_fail("usage: tsh filename\n");
      return 0;
   }
   contents = read_file(resolv_path((string)this_object()->get_path(),file));
   if (!contents) {
      notify_fail("tsh: couldn't read " + file + "\n");
      return 0;
   }
   lines = explode(contents,"\n");
   len = sizeof(lines);
   finished = 0;
   for (j = 0; j < len && !finished; j++) {
      command(lines[j]);
   }
   write("Script "+file+": ended successfully.\n");
   return 1;
}

varargs void setAdminBlock(int length){
    if (!length) {
        aBlock = 1;
    } else {
        timeBlock = (timeBlock > (time() + length))?timeBlock:(time()+length);
    }

}

void removeAdminBlock(){
    aBlock = 0;
    timeBlock = 0;
}

int queryTimeBlock(){
    return timeBlock;
}

int queryAdminBlock(){
    return aBlock;
}


string bloodlust2string(object obj)
{
    int max, hunger;
    max = 20000;
    hunger = obj->query_bloodlust();
    return ""+hunger/200;
}

string encumbrance2str(object player) {
    int max = player->query_max_internal_encumbrance();
    int cur = player->query_internal_encumbrance();
    int perc = cur*100/max;
    string enc = "";

    if(perc >= 10 && perc <= 90) {
        perc = random(1) ? (perc + random(6)) : (perc - random(6));
    }

    switch(perc) {
        case 0..25:
            enc = "Unburdened";
            break;
        case 26..50:
            enc = "Lightly Burdened";
            break;
        case 51..75:
            enc = "Moderately Burdened";
            break;
        case 76..100:
            enc = "Heavily Burdened";
            break;
        default:
            enc = "Overburdened";
            break;
    }

    return enc;
}

string hunger2str(object player)
{
    int max = player->query_formula();
    int cur, perc;
    cur = player->query_stuffed();
    max = max ? max : 1;
    perc = cur*100/max;

    if(perc > 100*5/6)
        return "%^BOLD%^%^GREEN%^Stuffed%^RESET%^";
    if(perc > 100*4/6)
        return "%^GREEN%^Not hungry%^RESET%^";
    if(perc > 50)
        return "%^YELLOW%^Hungry%^RESET%^";
    if(perc > 100/3)
        return "%^RED%^Really hungry%^RESET%^";
    if(perc > 100/6)
        return "%^RED%^Very hungry%^RESET%^";

    return "%^RED%^%^BOLD%^Starving%^RESET%^";
}

string thirst2str(object player)
{
    int max = player->query_formula();
    int cur, perc;
    cur = player->query_quenched();
    max = max ? max : 1;
    perc = cur*100/max;

    if(perc > 100*5/6)
        return "%^BOLD%^%^GREEN%^Quenched%^RESET%^";
    if(perc > 100*4/6)
        return "%^GREEN%^Not thirsty%^RESET%^";
    if(perc > 50)
        return "%^YELLOW%^Thirsty%^RESET%^";
    if(perc > 100/3)
        return "%^RED%^Really thirsty%^RESET%^";
    if(perc > 100/6)
        return "%^RED%^Very thirsty%^RESET%^";
    return "%^RED%^%^BOLD%^Parched%^RESET%^";
}

string hunger_percent(object player) {
    int current, percent, max;

    max = player->query_formula();
    current = player->query_stuffed();
    percent = current * 100 / max;

    return percent + "%";
}

string thirst_percent(object player) {
    int current, percent, max;

    max = player->query_formula();
    current = player->query_quenched();
    percent = current * 100 / max;

    return percent + "%";
}
