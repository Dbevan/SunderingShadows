/*
  dark_omen.c
  
  Dark Omen.
  
  Spontaneous caster specific item. Should allow a spontaneous caster to use spell slots to cast specific spells on the ring.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>

inherit "/d/common/obj/jewelry/ring.c";

string owner;

void create()
{
    ::create();
    
    set_name("dark omen");
    set_id( ({ "ring", "omen", "dark omen", "dark ring" }) );
    set_short("%^C060%^D%^C066%^a%^C072%^r%^C060%^k O%^C066%^m%^C072%^e%^C060%^n%^CRST%^");
    set_obvious_short("%^C060%^A d%^C066%^a%^C072%^r%^C060%^k and f%^C066%^o%^C072%^r%^C078%^b%^C072%^i%^C066%^d%^C060%^ding o%^C066%^n%^C072%^y%^C060%^x b%^C066%^a%^C072%^n%^C060%^d%^CRST%^");
    set_long("%^C060%^The band set before you has an almost p%^C066%^a%^C072%^l%^C078%^p%^C072%^a%^C066%^b%^C060%^le and e%^C066%^n%^C072%^e%^C078%^r%^C072%^v%^C066%^a%^C060%^ting weight that seems to %^C066%^pull%^C060%^ and %^C078%^scratch%^C060%^ at your very soul. It would seem rather innocuous from a distance if it were not for the %^C066%^cloying shadows%^C060%^ which seem to almost dance around it. There are no real markings upon this band, and it seems to be made of a seamless piece of polished o%^C066%^n%^C072%^y%^C060%^x.%^CRST%^");
    set_lore("%^C066%^A truly dark and forbidding ring filled with %^C072%^forbidding power%^C066%^. The origin of this ring is unknown, but its dark nature seems to suggest it was wrought in the %^C060%^Plane of Shadows%^C066%^. The ring itself lends its power to spontaneous casters of the weave, allowing them to cast %^C078%^fear%^C066%^, %^C078%^ray of exhaustion%^C066%^, %^C078%^waves of fatigue%^C066%^, %^C078%^ray of enfeeblement%^C066%^, and %^C078%^cause fear%^C066%^ as if they were mastered. Using this ring will consume the appropriate prepared spell slot.%^CRST%^");
    set_property("lore difficulty", 40);
    set_property("id difficulty", 40);
    set_value(100000);
    set_flag_required("boss avatar");
    set_wear((:this_object(),"wear_fun":));
    set_remove( (: this_object(),"remove_fun" :) );
}

int wear_fun()
{
    int align;
    
    align = this_player()->query_true_align();
    
    if(align == 1 || align == 4 || align == 7)
    {
        tell_object(this_player(), "%^C084%^Your conscience pulls your hand away from the evil ring with revulsion!%^CRST%^");
        return 0;
    }
    
    if(!strlen(owner))
    {
        owner = this_player()->query_name();
        tell_object(this_player(), "%^C066%^The %^C060%^dark ring%^C066%^ %^C072%^binds%^C066%^ itself with your %^C078%^soul%^C066%^!%^CRST%^");
    }
    else
    {
        if(owner != this_player()->query_name())
        {
            tell_object(this_player(), "This is not your ring!");
            return 0;
        }
    }
    
    this_player()->set_property("bonus_mastered", ({ "fear", "ray of exhaustion", "waves of fatigue", "ray of enfeeblement", "cause fear" }));
    tell_object(this_player(), "The ring slides onto your finger, lending you its dark power!");
    
    return 1;
}

int remove_fun()
{
    this_player()->remove_property_value("bonus_mastered", ({ "fear", "ray of exhaustion", "waves of fatigue", "ray of enfeeblement", "cause fear" }));
    tell_object(this_player(), "%^C078%^You remove the ring and feel its dark power leave you.%^CRST%^");
    return 1;
}
    
    