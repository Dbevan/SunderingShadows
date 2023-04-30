#include <std.h>

inherit MONSTER;

void go_home(object to);

string my_gender, my_race;

void create() {
   object ob;
   ::create();
   set_name("captive");

    switch(random(2)) {
      case 0: set_gender("male"); my_gender = "male"; break;
      default: set_gender("female"); my_gender = "female"; break;
        }

    switch(random(2)) {
      case 0: set_race("human"); my_race = "human"; break;
      default: set_race("half-elf"); my_race = "half-elf"; break;
        }

   set_id(({ "prisoner","captive","Captive","slave"}));

   set_long("Dressed in %^C060%^rags%^CRST%^ and %^C144%^malnutritioned%^CRST%^, "+
    "this poor soul has been %^C242%^chained%^CRST%^ to a %^C094%^stake%^CRST%^ on "+
    "the %^C144%^beach%^CRST%^ and left out to the elements. Without any shelter "+
    "their skin has become red with the start of a %^C088%^vicious burn%^CRST%^ and t"+
    "hey have not eaten well in some time so they are starting to get "+
    "thin. %^C055%^Bruises%^CRST%^ and %^C160%^open sores%^CRST%^ show that "+
    "the %^C241%^Hounds%^CRST%^ are also mistreating them. You could %^C055%^>free<%^CRST%^ them.\n");

   set_short("%^C059%^dirty %^C143%^" +my_gender+ " %^C152%^" +my_race+ " %^C060%^captive%^CRST%^");


   set_exp(0);
   set_max_hp(200);
   set_hp(20);
   set_property("no random treasure", 1);
   set_property("not random monster", 1);
   set_spoken("wizish");
   command("speech mumble hoarsely");
}



void init() {
    ::init();
    add_action("free_fun","free");
}

int free_fun(string str)
{
    object me = this_object();

    if (!objectp(me))
        return 0;

    if (!stringp(str) || !(str == "captive" || str == "prisoner" || str == "slave"))
        return 0;

    tell_object(TP,"%^RESET%^%^ORANGE%^You unbind the person chained to the stake in the beach...%^RESET%^");
    tell_room(ETO,"%^RESET%^RED%^\nAs they are unbound, they cough \n%^RESET%^");

    switch(random(5))
    {
    case 0:
        force_me("say Thank you.. Thank you..\n");
        force_me("say I was captured by pirates, they brought us here..\n");
    break;
        
    case 1:
        force_me("say Kismet be praised.\n");
        force_me("say They took more of us through that archway to the north..\n");
    break;

    case 2:
        force_me("say They powered the gate with something..\n");
        force_me("say Then they marched mounted troops through. I wonder where the archway leads?\n");
    break;

    default:
        force_me("emote groans, unable to speak...");
    break;
    }

    return go_home(me);
}

int go_home(object to)
{
    object my_environment;

    if (!objectp(to))
        return 0;

    if (objectp(my_environment = environment(to)))
        tell_room(my_environment, "%^RESET%^RED%^\nThe now freed prisoner runs down the beach to safety...\n%^RESET%^");

    to->remove();

    return 1;
}
