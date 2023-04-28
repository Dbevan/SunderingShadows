/*
  circlet.c
  
  Circlet of the Expansive Mind
  
  Psion-specific head item.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>

#define MAX_MANA 500

inherit ARMOUR;

int mana;

void create()
{
    ::create();
    
    set_name("circlet");
    set_id( ({ "circlet", "mind circlet", "circlet of the expansive mind", "expansive mind circlet" }) );
    set_short("%^C033%^C%^C039%^i%^C045%^r%^C051%^c%^C045%^l%^C039%^e%^C033%^t of the E%^C039%^x%^C045%^p%^C051%^a%^C045%^n%^C039%^s%^C033%^ive Mind%^CRST%^");
    set_obvious_short("%^C033%^c%^C039%^r%^C045%^y%^C051%^s%^C045%^t%^C039%^a%^C033%^l-i%^C039%^n%^C045%^f%^C051%^u%^C045%^s%^C039%^e%^C033%^d circlet%^CRST%^");
    set_long("This is a circlet that almost seems to be made of transparent glass. Within the glass seem to be floating stars and other celestial bodies in a swirl of slow motion. The circlet it rounded, with no edges and is completely smooth to the touch, save for a ridge along the top made of various-sized psi-crystals, which pulse with a violet aura. It is otherwise featureless but emanates a strange, otherworldly power. This is truly a grand prize for the consummate psychic.");
    set_lore("%^C195%^Around the year 750 SG, the psionic practitioners of the crystal tower in Seneca learned how to store latent psionic energy within items, in order to have a reservoir to %^CYAN%^draw power%^RESET%^ upon in a time of great need. One might also %^CYAN%^check power%^RESET%^ to see how full the psychic reservoir is. They created several items like this one, which proved to be invaluable in emergency situations. It is also said that these items provide the wearer incredible resistance to mental attacks from enemies.%^CRST%^");
    set_property("lore difficulty", 40);
    set_property("id difficulty", 40);
    set_ac(0);
    set_value(10000);
    set_flag_required("boss avatar");
    set_limbs( ({ "head" }) );
    set_property("enchantment", 7);
    set_item_bonus("intelligence", 6);
    set_item_bonus("academics", 5);
    set_item_bonus("mental resistance percent", 50);
    set_type("clothing");
    set_heart_beat(1);
    //set_wear((:TO,"wear_fun":));
}

void init()
{
    ::init();
    add_action("draw", "draw");
    add_action("check_mana", "check");
}

void heart_beat()
{   
    mana = mana >= MAX_MANA ? MAX_MANA : mana + 1;
}   

int draw(string str)
{
    if(str != "power")
        return 0;
    
    if(mana <= 0)
    {
        write("The circlet appears to be empty of power.");
        return 1;
    }
    
    write("%^BOLD%^CYAN%^You tap into the circlet's energy, and feel the power flow into you.%^RESET%^");
    this_player()->add_mp(mana);
    mana = 0;
    
    return 1;
}

int check_mana(string str)
{
    if(str != "power")
        return 0;
     
    printf("The circlet currently has %d power points stored in it.\n", mana);
    
    return 1;
}

/*
int wear_fun()
{
    object holder = environment(this_object());
    
    if(!holder->query("boss avatar"))
    {
        tell_object(holder, "The circlet rejects your touch!");
        return 0;
    }
    
    return 1;
}  
*/ 