/**
 * @file
 * @brief a daemon to keep track of player progress
 */

//      /adm/daemon/player_d.c
//      from the Nightmare Mudlib
//      a daemon to keep track of player progress
//      created by Descartes of Borg 10 july 1993

#include <objects.h>
#include <save.h>
#include <security.h>
#include <daemons.h>

string* monsters, * monsters2, * monsters3, * monsters4, * monsters5;

mapping* player_list;

void create()
{
    seteuid(getuid());
    monsters = ({});
    player_list = ({});
    seteuid(UID_DAEMONSAVE);
    restore_object(SAVE_PLAYER_LIST);
    seteuid(getuid());
}

mapping* query_player_list()
{
    return player_list;
}

varargs void add_player_info(object ob)
{
    int exp, x;
    int i;

    if (!objectp(ob)) {
        if (!(ob = previous_object()) || !userp(ob)) {
            return;
        }
    }
    if (avatarp(ob)) {
        i = sizeof(player_list);
        while (i--) {
            if (player_list[i]["name"] == (string)ob->query_name() && avatarp(ob)) {
                player_list -= ({ player_list[i] });
            }
        }
        return;
    }
    x = -1;
    i = sizeof(player_list);
    while (i--) {
        if (player_list[i]["name"] == (string)ob->query_name()) {
            x = i;
        }
    }
    if ((sizeof(player_list) == 20) && (x == -1) &&
        (((int)ob->query_exp()) < player_list[19]["experience"])) {
        return;
    }
    if (x > -1) {
        player_list -= ({ player_list[x] });
    }
    if (!arrayp(player_list)) {
        player_list = ({});
    }

    player_list += ({ ([ "name" : (string)ob->query_name(), "experience" : (int)ob->query_exp(),
                         "kills" : sizeof((string*)ob->query_kills()),
                         "deaths" : sizeof((mixed*)ob->query_deaths()),
                         "quests" : sizeof((string*)ob->query_quests()),
                         "major accomplishments" : sizeof((mixed*)ob->query_mini_quests()),
                         "level" : (int)ob->query_level(),
                         "class" : (string)ob->query_class(),
                         "guild" : (string)ob->query_guild()
                       ]) });
    player_list = sort_array(player_list, "sort_list", this_object());
    if (sizeof(player_list) > 20) {
        player_list = player_list[0..19];
    }
    seteuid(UID_DAEMONSAVE);
    save_object(SAVE_PLAYER_LIST);
    seteuid(getuid());
}

int sort_list(mapping alpha, mapping beta)
{
    if (alpha["experience"] > beta["experience"]) {
        return -1;
    }else if (alpha["experience"] < beta["experience"]) {
        return 1;
    }else {
        return strcmp(alpha["name"], beta["name"]);
    }
}

int count_unique_monsters()
{
    int tmp, x;
    monsters -= ({ 0 });
    monsters2 -= ({ 0 });
    monsters3 -= ({ 0 });
    monsters4 -= ({ 0 });
    monsters5 -= ({ 0 });
    seteuid(UID_DAEMONSAVE);
    save_object(SAVE_PLAYER_LIST);
    seteuid(getuid());
    return (sizeof(monsters) + sizeof(monsters2) + sizeof(monsters3) + sizeof(monsters4) + sizeof(monsters5));
}

int add_kill(string str)
{
    int x;

    if (!monsters) {
        monsters = ({});
    }
    if (!monsters2) {
        monsters2 = ({});
    }
    if (!monsters3) {
        monsters3 = ({});
    }
    if (!monsters4) {
        monsters4 = ({});
    }
    if (!monsters5) {
        monsters5 = ({});
    }

    if ((x = member_array(str, monsters)) == -1) {
        if ((x = member_array(str, monsters2)) == -1) {
            if ((x = member_array(str, monsters3)) == -1) {
                if ((x = member_array(str, monsters4)) == -1) {
                    if ((x = member_array(str, monsters5)) == -1) {
                        monsters5 += ({ str });
                        x = member_array(str, monsters5);
                        seteuid(UID_DAEMONSAVE);
                        save_object(SAVE_PLAYER_LIST);
                        seteuid(getuid());
                    }
                    x += 15000;
                }
                x += 15000;
            }
            x += 15000;
        }
        x += 15000;
    }
    return x;
}

string query_monster(int x)
{
    if(!intp(x) || x < 0)
        return "unknown";
    
    if (x <= sizeof(monsters)) {
        if (!monsters[x]) {
            return "unknown";
        }else {
            return monsters[x];
        }
    }else if (x - 15000 <= sizeof(monsters2)) {
        x = x - 15000;
        if (!monsters2[x]) {
            return "unknown";
        }else {
            return monsters2[x];
        }
    }else if (x - 30000 <= sizeof(monsters3)) {
        x = x - 30000;
        if (!monsters3[x]) {
            return "unknown";
        }else {
            return monsters3[x];
        }
    }else if (x - 45000 <= sizeof(monsters4)) {
        x = x - 45000;
        if (!monsters4[x]) {
            return "unknown";
        }else {
            return monsters4[x];
        }
    }else if (x - 60000 <= sizeof(monsters5)) {
        x = x - 60000;
        if (!monsters5[x]) {
            return "unknown";
        }else {
            return monsters5[x];
        }
    }
    return "unknown";
}

void remove_player(int i)
{
    mapping tmp;
    player_list[i]["experience"] = 0;
    player_list -= ({ player_list[i] });
    seteuid(UID_DAEMONSAVE);
    save_object(SAVE_PLAYER_LIST);
    seteuid(getuid());
}

int calc_height(object play, int hpct)
{
    int heightbase, heightmod;
    string myfile, myrace, mygender;

    if (!objectp(play)) {
        return 0;
    }
    myrace = (string)play->query_race();
    if (!myrace) {
        return 0;
    }
    myfile = "/std/races/" + myrace + ".c";
    if (!file_exists(myfile)) {
        return 0;
    }
    mygender = (string)play->query_gender();

    heightbase = (int)myfile->height_base(mygender);
    if (!heightbase) {
        heightbase = 60;           // default setting
    }
    heightmod = (int)myfile->height_mod(mygender);
    if (!heightmod) {
        heightmod = 20;          // default setting
    }
    return (heightbase + ((hpct * heightmod) / 80));
}

int* calc_weight(object play, int wpct)
{
    int base, num, adjust, die, max;
    int* vals;
    string gender;
    int height, weight;

    vals = allocate(2);
    gender = play->query_gender();
    height = play->query_height();
    switch ((string)play->query_race())
    {
    case "dwarf":
        if (gender == "male")
        {
            base = 150;
            adjust = height - 45;

        }
        else
        {
            base = 120;
            adjust = height - 43;
        }
        die = 4;
        break;

    case "elf":
        if (gender == "male")
        {
            base = 100;
            adjust = height - 64;
        }
        else
        {
            base = 90;
            adjust = height - 64;
        }
        die = 8;
        break;

    case "drow":
        if (gender == "male")
        {
            base = 100;
            adjust = height - 64;
        }
        else
        {
            base = 90;
            adjust = height - 64;
        }
        die = 8;
        break;

    case "half-elf":
        if (gender == "male") {
            base = 110;
            adjust = height - 62;
        }
        else
        {
            base = 90;
            adjust = height - 60;
        }
        die = 8;
        break;

    case "half-drow":
        if (gender == "male") {
            base = 100;
            adjust = height - 62;
        }
        else
        {
            base = 90;
            adjust = height - 60;
        }
        die = 8;
        break;

    case "gnome":
        if (gender == "male")
        {
            base = 35;
            adjust = height - 36;
        }
        else
        {
            base = 30;
            adjust = height - 34;
        }
        die = 4;
        break;

    case "halfling":
        if (gender == "male")
        {
            base = 30;
            adjust = height - 32;
        }
        else
        {
            base = 25;
            adjust = height - 30;
        }
        die = 4;
        break;

    case "human":
    case "yuan-ti":
    case "shade":
    case "deva":
        if (gender == "male") {
            base = 120;
            adjust = (height - 58);

        }else {
            base = 85;
            adjust = (height - 53);
        }
        die = 10;
        break;
        
    case "beastman":
        if (gender == "male")
        {
            base = 105;
            adjust = height - 58;
        }
        else
        {
            base = 85;
            adjust  = height - 53;
        }
        die = 10;
        break;
        
    case "bugbear":
        if (gender == "male")
        {
            base = 210;
            adjust = height - 78;
        }
        else
        {
            base = 180;
            adjust  = height - 74;
        }
        die = 10;
        break;

    case "voadkyn":
    case "firbolg":
        if (gender == "male")
        {
            base = 675;
            adjust = height - 111;
        }
        else
        {
            base = 650;
            adjust  = height - 105;
        }
        die = 15;
        break;

    case "minotaur":
    case "gnoll":
        if (gender == "male")
        {
            base = 180;
            adjust = height - 87;
        }
        else
        {
            base = 160;
            adjust  = height - 83;
        }
        die = 10;
        break;

    case "goblin":
    case "ratkin":
        if (gender == "male")
        {
            base = 72;
            adjust = height - 46;
        }
        else
        {
            base = 68;
            adjust  = height - 41;
        }
        die = 8;
        break;
        
    case "hobgoblin":
        if (gender == "male")
        {
            base = 150;
            adjust = height - 79;
        }
        else
        {
            base = 130;
            adjust  = height - 75;
        }
        die = 4;
        break;
        
    case "kobold":
        if (gender == "male")
        {
            base = 130;
            adjust = height - 42;
        }
        else
        {
            base = 105;
            adjust  = height - 33;
        }
        die = 6;
        break;

    case "ogre":
        if (gender == "male")
        {
            base = 650;
            adjust = height - 100;
        }
        else
        {
            base = 600;
            adjust  = height - 94;
        }
        die = 12;
        break;

    case "half-ogre":
        if (gender == "male") {
            base = 270;
            adjust = height - 91;
        }
        else
        {
            base = 220;
            adjust  = height - 97;
        }
        die = 15;
        break;
        
    case "ogre-mage":
        if (gender == "male") {
            base = 810;
            adjust = height - 124;
        }
        else
        {
            base = 780;
            adjust  = height - 99;
        }
        die = 15;
        break;

    case "orc":
        if (gender == "male") {
            base = 130;
            adjust = height - 61;
        }
        else
        {
            base = 95;
            adjust  = height - 59;
        }
        die = 10;
        break;
        
    case "half-orc":
        if (gender == "male") {
            base = 135;
            adjust = height - 63;
        }
        else
        {
            base = 95;
            adjust  = height - 61;
        }
        die = 6;
        break;

    case "centaur":
    case "wemic":
        if (gender == "male")
        {
            base = 700;
            adjust = height - 85;
        }
        else
        {
            base = 670;
            adjust = height - 82;
        }
        die = 10;
        break;

    default:
        base = 140;
        adjust = height - 72;
        die = 10;
    }
    
    max = base + (2 * die * adjust);
    //max = base + (num * die) / 2 + adjust; // average weight
    //num = wpct * num / 100;
    //base = wpct * base / 100;
    //adjust = (wpct > 100 ? wpct : 100) * adjust / 100;
    //weight = base + roll_dice(num, die) + adjust;
    weight = base + (roll_dice(2, die) * adjust);
    vals[0] = weight;
    vals[1] = max;
    return vals;
}

// For new lighting system / race night blindness...
// 12/31/2003 - garrett
string* night_races()
{
    return ({ "drow", "half-drow", "bugbear", "kobold", "hobgoblin", "gnoll", "orc", "lich", "ogre", "ogre-mage", "goblin", "minotaur", "ratkin", "troll", "nightwing"});
}

string* list_base_classes()
{
    string* possible_classes, * classlist = ({}), myname;
    int i;
    object class_ob;
    myname = TP->query_name();
    possible_classes = get_dir("/std/class/*.c");
    possible_classes = explode(implode(possible_classes, ""), ".c");
    if (sizeof(possible_classes)) {
        for (i = 0; i < sizeof(possible_classes); i++) {
            if (possible_classes[i] == "cavalier" ||
                possible_classes[i] == "antipaladin") {
                continue;
            }
            class_ob = find_object_or_load("/std/class/" + possible_classes[i] + ".c");
            if (objectp(class_ob) &&
                class_ob->is_prestige_class()) {
                continue;
            }
            classlist += ({ possible_classes[i] });
        }
    }
    return classlist;
}

string* list_classes()
{
    string* possible_classes, * classlist = ({}), myname;
    int i;
    object class_ob;
    myname = TP->query_name();
    possible_classes = get_dir("/std/class/*.c");
    possible_classes = explode(implode(possible_classes, ""), ".c");
    possible_classes -= ({ "cavalier", "antipaladin" });
    if (sizeof(possible_classes)) {
        for (i = 0; i < sizeof(possible_classes); i++) {
            class_ob = find_object_or_load("/std/class/" + possible_classes[i] + ".c");
            classlist += ({ possible_classes[i] });
        }
    }
    return classlist;
}

int sizeof_monsters()
{
    return sizeof(monsters4);
}

/*
int opposed_alignment(object me, object you)
{
    string *opposed;
    int my_align, your_align;
    
    if(!me || !you)
        return 0;
    
    my_align = me->query_true_align();
    your_align = you->query_true_align();
    
    if(my_align == 5 || your_align == 5)
        return 0;
    
    //If you're the polar opposite
    if(my_align - (10 - your_align) == 0)
        return 2;
    
    opposed = ({ "000", "78936", "789", "78914", "369", "000", "147", "12369", "123", "12347" });
    
    if(strsrch(opposed[my_align], your_align + "") >= 0)
        return 1;
    
    return 0;
}
*/
    

int check_aura(object target, string type)
{
    object *allies;
    string aura;
    int prot;

    if(!target || !type)
        return 0;

    allies = PARTY_D->query_party_members(target->query_party());

    if(!sizeof(allies))
        allies = ({ target });

    //Immunity
    if(FEATS_D->usable_feat(target, "aura of " + type))
        return 1;

    allies -= ({ target });

    //+4
    foreach(object ally in allies)
    {
        if(FEATS_D->usable_feat(ally, "aura of " + type))
            return 2;
    }

    return 0;
}

mixed check_familiar(object target, string type)
{
    object fam;
    
    if(!target)
        return 0;
    
    if(!objectp(target))
        return 0;
    
    fam = target->query_property("familiar");
    
    if(!fam || !objectp(fam))
        return 0;
    
    if(environment(fam) != environment(target))
        return 0;

    //To check what type of familiar they have if we don't know
    if(!strlen(type))
        return fam->query_name();
    
    if(fam->query_name() != type)
        return 0;
    
    return 1;
}
    
int immunity_check(object obj, string type)
{
    string myrace, mysubrace;
    int num;
    if (!objectp(obj)) {
        return 0;
    }
    
    if(archp(obj))
        return 1;
    
    myrace = obj->query_race();
    mysubrace = obj->query("subrace");

    switch (type) {
    case "sleep":
    {
        if (obj->is_undead()) {
            return 1;
        }
        if(obj->query_property("no sleep"))
            return 1;
        if(obj->query_mystery() == "dragon" && obj->query_class_level("oracle") >= 31)
            return 1;
        
        switch (myrace) {
        case "elf":
        case "drow":
        case "half-elf":
        case "half-drow":
        case "golem":
        case "soulforged":
            return 1;
        default:
            return 0;
        }
        return 0;
    }
    break;
    
    case "blindness":
    {
        if(obj->true_seeing())
            return 1;
        
        if(obj->query_property("no blind"))
            return 1;
        
        return 0;
    }

    case "charm":
    {
        if(obj->is_undead())
            return 1;
        
        if(obj->query_property("no charm"))
            return 1;
        
        switch (myrace) {
            case "barrus":
                return 1;
                break;
        }

        if (check_aura(obj, "resolve") == 1) {
            return 1;
        }
    }
    break;
    
    case "confusion":
    {
        if(obj->query_mystery() == "battle" && obj->query_class_level("oracle") >= 10)
            return 1;
        
        if(obj->query_property("confusion_immunity"))
            return 1;
        
        if(obj->is_undead())
            return 1;
    }
    break;
    
    case "disease":
    {
        if(obj->is_undead())
            return 1;
        
        if (obj->query_property("disease immunity"))
            return 1;
        
        if(member_array("plague", obj->query_divine_domain()) >= 0)
            return 1;
        
        if(obj->query_mystery() == "life" && obj->query_class_level("oracle") >= 10)
            return 1;
        
        if(obj->query_bloodline() == "pestilence" && obj->query_class_level("sorcerer") > 30)
            return 1;
        
        if(FEATS_D->usable_feat(obj, "divine health"))
            return 1;
        
        if(FEATS_D->usable_feat(obj, "purity of body"))
            return 1;
        
        if(FEATS_D->has_feat(obj, "undead graft"))
            return 1;
    }

    case "fear":
    {
        if(obj->is_undead())
            return 1;

        if (FEATS_D->usable_feat(obj, "no fear of the flame")) {
            return 1;
        }

        if (FEATS_D->usable_feat(obj, "bravery")) {
            return 1;
        }

        if(check_aura(obj, "courage") == 1)
            return 1;

        if (obj->query_property("fear_immunity")) {
            return 1;
        }
        
        if(obj->query_property("intellect fortress"))
            return 1;
        
        if(FEATS_D->usable_feat(obj, "fearless rage") && obj->query_property("raged"))
            return 1;
        
        if(obj->query_mystery() == "heavens")
        {
            if(obj->query_class_level("oracle") > 30)
                return 1;
        }
        
        if(member_array("glory", obj->query_divine_domain()) >= 0)
            return 1;

        switch (myrace) {

        case "halfling":
            return 1;

        case "troll":
            return 1;

        case "firbolg":
            return 1;
            
        case "saurian":
            return 1;

        case "human":
            if (mysubrace == "attayan") {
                return 1;
            }
            break;

        default:
            return 0;
        }
        return 0;
    }
    
    break;

    case "fatigue":
    {
        if (obj->is_undead()) {
            return 1;
        }
        if (member_array(obj->query_race(),({"golem", "construct", "soulforged"})) >= 0) {
            return 1;
        }
        
        if(obj->query_mystery() == "life" && obj->query_class_level("oracle") >= 31)
            return 1;
        
        if(obj->query_bloodline() == "boreal" && obj->query_class_level("sorcerer") > 30)
            return 1;

        return 0;
    }
    
    break;
    
    case "negative_level":
    {
        if(obj->is_undead())
            return 1;
        
        if(obj->query_mystery() == "life" && obj->query_class_level("oracle") >= 31)
            return 1;
        
        if(obj->query("subrace") == "dhampir")
            return 1;
        
        return 0;
    }
    break;
    
    case "paralysis":
    {
        if(obj->query_property("stun_immunity") || obj->query_property("no paralyze"))
            return 1;
        
        if(obj->is_class("barbarian") && !obj->cooldown("unrestrained rage"))
        {
            if(obj->query_property("raged") && FEATS_D->has_feat(obj, "unrestrained rage"))
            {
                tell_object(obj, "%^YELLOW%^Your rage allows you to shake off the paralysis!%^RESET%^");
                obj->add_cooldown("unrestrained rage", 120);
                obj->set_property("stun_immunity", 1);
                call_out("remove_stun_immunity", 24, obj);
                return 1;
            }
        }
        
        if(obj->query_mystery() == "dragon" && obj->query_class_level("oracle") >= 31)
        {
            if(!obj->cooldown("dragon resistance"))
            {
                tell_object(obj, "%^YELLOW%^Your draconic resistance helps you shrug off the paralysis!");
                obj->add_cooldown("dragon resistance", 120);
                return 1;
            }
        }

        if(obj->query_bloodline() == "ghoul" && obj->query_class_level("sorcerer") > 30)
        {
            if(!obj->cooldown("ghoulish aspect"))
            {
                tell_object(obj, "%^YELLOW%^Your ghoulish aspect helps you shrug off the paralysis!");
                obj->add_cooldown("ghoulish aspect", 120);
                return 1;
            }
        }

        if(member_array("freedom", obj->query_divine_domain()) >= 0)
        {
            if(!obj->cooldown("freedom boon"))
            {
                tell_object(obj, "%^YELLOW%^Your dedication to freedom at all costs protects you from the paralysis!");
                obj->add_cooldown("freedom boon", 120);
                return 1;
            }
        }         
    
        return 0;
    }
    break;
    
    case "poison":
    {
        if (obj->query_property("poison immunity"))
            return 1;
            
        if (FEATS_D->usable_feat(obj, "venom immunity"))
            return 1;
        
        if (FEATS_D->usable_feat(obj, "undead graft"))
            return 1;
    
        if (FEATS_D->usable_feat(obj, "purity of body"))
            return 1;
    
        if (FEATS_D->usable_feat(obj, "earthen blood"))
            return 1;
        
        if(obj->query_class_level("sorcerer") > 30)
        {
            if(obj->query_bloodline() == "abyssal" ||
               obj->query_bloodline() == "infernal" ||
               obj->query_bloodline() == "fey")
               return 1;
        }

        if (obj->is_undead())
            return 1;
        
        if(obj->query_race() == "yuan-ti")
            return 1;
        
        if(obj->query_race() == "devil")
            return 1;
        
        if(obj->query_mystery() == "life" && obj->query_class_level("oracle") >= 10)
            return 1;
            
        return 0;
    }
    
    case "rend":
    {
        if(obj->is_undead())
            return 1;
        
        if(obj->query_mystery() == "life" && obj->query_class_level("oracle") >= 31)
            return 1;
        
        if(obj->query_mystery() == "bones" && obj->query_class_level("oracle") >= 21)
            return 1;
        
    }
    break;
    
    case "sickened":
    {
        if(obj->query_mystery() == "life" && obj->query_class_level("oracle") >= 31)
            return 1;
        
        if(obj->query_bloodline() == "pestilence" && obj->query_class_level("sorcerer") > 30)
            return 1;
        
        if(FEATS_D->has_feat(obj, "internal fortitude") && obj->query_property("raged"))
            return 1;
    }
    break;    
    

    default:
        return 0;
    }

    return 0;
}

void remove_stun_immunity(object ob)
{
    ob && ob->remove_property("stun_immunity");
}

/*
  New scrying system handler.
  
  -- Tlaloc --


//This is a spot scry check
int scry_check(object target, int power)
{
    object room;
    int block_power, room_block;
    
    if(!objectp(target) || !power)
        return 0;
    
    room = environment(target);
    
    if(!objectp(room))
        return 0;
    
    if(room->query_property("no scry"))
        return 0;
    
    if(target->query_property("block scrying"))
        return 0;
    
    room_block = room->query_property("scry proof");    
    block_power = target->query_property("scry block power");
    
    block_power = max( ({ room_block, block_power }) );   
   
    if(block_power + 10 > power + roll_dice(1, 20))
        return 0;
    
    return 1;
}

int long_look_room(object victim, object detector)
{
    string file, desc;
    object *inv, dest;
    int i,j;
    
    if(!objectp(victim))
        return 0;
    
    dest = environment(victim);
    
    if(!dest || !objectp(dest))
        return 0;
    
    if(!detector || !objectp(detector))
        return 0;
    
    message("room_description","\n%^YELLOW%^[S] %^RESET%^"+(string)dest->query_short()+"\n", detector);
    message("room_description","\n%^YELLOW%^[S] %^RESET%^"+(string)dest->query_long()+"\n", detector);
    inv = all_inventory(dest);

    foreach(object ob in inv)
    {
        if(!objectp(ob)) continue;
        if(ob->query_invis()) continue;
        if(ob->is_detectable()) continue;
    	tell_object(target, 
        tell_object(detector, "%^WHITE%^BOLD%^[S] %^RED%^"+inv[i]->query_short());
    }
    return 1;
}
*/
