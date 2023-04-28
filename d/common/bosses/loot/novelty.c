/*
  novelty.c
  
  Hammer weapon dropped by the Nightwalker Boss.
  
  -- Tlaloc --
*/

#include <std.h>
#include <daemons.h>

//Hit interval instead of proc chance. Procs every 7 hits.
#define HIT_INTERVAL 5

inherit "/d/common/obj/weapon/warhammer.c";

string owner;
int hit_count;
object holder;

string color(string str)
{
    return CRAYON_D->color_string(str, "lightning yellow");
}

void create()
{
    ::create();
    
    set_name("novelty");
    set_id( ({ "hammer", "healing hammer", "glowing hammer", "novel hammer" }) );
    set_short("%^C160%^N%^C196%^o%^C208%^v%^C220%^e%^C226%^l%^C228%^t%^C231%^y%^CRST%^ - %^C220%^a%^CRST%^ %^C214%^b%^C220%^ri%^C226%^l%^C220%^li%^C214%^a%^C208%^n%^C202%^t%^CRST%^ %^C160%^f%^C196%^l%^C202%^a%^C208%^m%^C214%^i%^C220%^n%^C226%^g%^CRST%^ %^C220%^w%^C226%^a%^C220%^rh%^C214%^a%^C220%^mm%^C226%^e%^C220%^r%^CRST%^");
    set_obvious_short("A %^C220%^l%^C231%^o%^C226%^n%^C220%^g%^CRST%^ %^C220%^ha%^C226%^m%^C228%^m%^C231%^e%^C226%^r bin%^C231%^d%^C228%^i%^C226%^n%^C220%^g%^CRST%^ a %^C196%^b%^C202%^r%^C208%^i%^C214%^l%^C220%^l%^C226%^iant%^CRST%^ %^C220%^f%^C214%^l%^C208%^a%^C202%^m%^C196%^e%^CRST%^");
    set_long(" %^C220%^This %^C214%^h%^C226%^a%^C228%^mm%^C226%^e%^C214%^r%^C220%^ has been constructed wholly from pure %^C231%^g%^C228%^o%^C226%^l%^C220%^d, with the h%^C226%^a%^C228%^n%^C231%^d%^C229%^l%^C231%^e%^C220%^ and %^RESET%^%^C231%^s%^C228%^t%^C226%^a%^C228%^f%^C231%^f%^C220%^ of the hammer appearing to be made from a single, large piece of gold. The head of the hammer blunts out and %^C231%^pr%^C228%^ot%^C226%^ru%^C220%^de%^C214%^s%^C220%^ on one side while the other spikes out in a sharp and long %^RESET%^%^C231%^l%^C228%^e%^C226%^a%^C220%^f%^C214%^-bl%^C220%^a%^C226%^d%^C228%^e%^C231%^d%^C220%^ shape. Where both sides meet, small little %^C196%^f%^C202%^l%^C208%^am%^C202%^e%^C196%^s%^C220%^ %^C160%^g%^C196%^lo%^C160%^w%^RESET%^%^C220%^ in a dimly %^C160%^c%^C196%^r%^C160%^ims%^C196%^o%^C160%^n%^C220%^ colour among smaller engravings of %^C058%^r%^C094%^oot%^C136%^s%^C220%^ that %^C130%^g%^C094%^r%^C058%^o%^C022%^w%^C220%^ along the centre of the bladed side while towards the hammer, small %^C196%^f%^C160%^l%^C197%^or%^C160%^a%^C196%^l%^RESET%^%^C220%^ shapes bloom out. The pole of the hammer shows leather wrappings for a sturdier hold and little engravings of roots glide along it until growing out into a %^C160%^re%^C196%^d%^C220%^, %^C196%^p%^C202%^h%^C208%^o%^C214%^e%^C220%^n%^C226%^ix-like fe%^C220%^a%^C214%^t%^C208%^h%^C202%^e%^C196%^r%^RESET%^%^C220%^ at the bottom of the %^C226%^po%^C220%^l%^C226%^e.%^CRST%^");
    set_lore("");
    set_value(10000);
    set_weight(6);
    set_property("no_curse", 1);
    set_property("no steal", 1);
    set_property("lore difficulty", 40);
    set_property("id difficulty", 40);
    set_property("able to cast", 1);
    set_property("enchantment", 7);
    set_property("no alter", 1);
    set_flag_required("boss avatar");    
    set_item_bonus("positive energy resistance percent", -25);
    set_item_bonus("caster level", 5);
    set_item_bonus("fast healing", 3);
    
    set_wield( (: this_object(), "wield_func" :) );
    set_unwield( (: this_object(), "unwield_func" :) );
    set_hit( (: this_object(), "hit_func" :) );
}

void init()
{   
    ::init();
    
    holder = environment(this_object());
    
    if(!holder || !userp(holder))
        return;
    
    hit_count = 0;
    
    /*
    if(!strlen(owner) && !avatarp(holder) && !archp(holder))
        owner = holder->query_true_name();
    
    if(owner != holder->query_true_name() ||
    (holder->query_true_align() != 1 &&
    holder->query_true_align() != 4 &&
    holder->query_true_align() != 7) ||
    holder->query_character_level() < 40)
    {
        tell_object(holder, "The wooden handle rejects your hand and you drop it!");
        this_object()->move(environment(holder));
        return;
    }    
    */    
    tell_object(holder, "As you %^C220%^li%^C226%^f%^C220%^t%^CRST%^ the %^C220%^h%^C226%^a%^C220%^m%^C226%^m%^C220%^er%^CRST%^, you feel it %^C160%^en%^C208%^tw%^C202%^in%^C214%^e%^CRST%^ %^C160%^it%^C196%^se%^C160%^lf%^CRST%^ with %^C231%^yo%^C230%^u%^C229%^r l%^C227%^if%^C230%^e for%^C231%^ce%^CRST%^.");
}

int hit_func(object target)
{
    int dam;
    string ename, pname;
    
    if(!objectp(owner) || !objectp(target))
        return 0;
    
    hit_count++;
    
    if(hit_count < HIT_INTERVAL)
    {
        target->cause_typed_damage(target, "torso", roll_dice(1, 10), "divine");
        return 0;
    }
    
    hit_count = 0;
    
    pname = environment(this_object())->query_cap_name();
    ename = target->query_cap_name();
    
    tell_room(environment(owner), "%^C220%^The forces of %^C227%^r%^C226%^e%^C220%^n%^C214%^e%^C208%^w%^C202%^a%^C196%^l%^C220%^ begin to flame out of "+this_object()->query_short()+" %^C196%^e%^C202%^n%^C208%^c%^C214%^o%^C220%^m%^C226%^p%^C220%^as%^C214%^s%^C208%^i%^C202%^n%^C196%^g%^C220%^ "+pname+" and %^C226%^a%^C228%^l%^C231%^li%^C228%^e%^C226%^s%^C220%^ while %^C231%^re%^C228%^s%^C226%^t%^C220%^o%^C226%^r%^C228%^i%^C231%^ng%^C220%^ back to how it was and %^C166%^s%^C208%^h%^C231%^ou%^C214%^l%^C166%^d%^C208%^ %^C220%^be.%^CRST%^", owner);
    tell_object(owner, "%^C220%^The forces of %^C227%^r%^C226%^e%^C220%^n%^C214%^e%^C208%^w%^C202%^a%^C196%^l%^C220%^ begin to flame out of "+this_object()->query_short()+" %^C196%^e%^C202%^n%^C208%^c%^C214%^o%^C220%^m%^C226%^p%^C220%^as%^C214%^s%^C208%^i%^C202%^n%^C196%^g%^C220%^ you and %^C226%^a%^C228%^l%^C231%^li%^C228%^e%^C226%^s%^C220%^ while %^C231%^re%^C228%^s%^C226%^t%^C220%^o%^C226%^r%^C228%^i%^C231%^ng%^C220%^ back to how it was and %^C166%^s%^C208%^h%^C231%^ou%^C214%^l%^C166%^d%^C208%^ %^C220%^be.%^CRST%^");

    new("/cmds/spells/m/_mass_cure_light_wounds")->use_spell(owner, owner, 50, 100, "cleric");
    
    dam = roll_dice(6, 10) + 10;
    target->cause_typed_damage(target, target->query_target_limb(), dam, "divine");
}

int wield_func()
{   
    if(!holder)
        holder = this_player();
    
    if(!strlen(owner))
        owner = holder->query_true_name();

    if(owner != holder->query_true_name() ||
    (holder->query_true_align() == 3 ||
    holder->query_true_align() == 6 ||
    holder->query_true_align() == 9) ||
    holder->query_character_level() < 40)
    {
        tell_object(holder, "The wooden handle rejects your hand and you drop it!");
        this_object()->move(environment(holder));
        return 0;
    } 
    
    if(holder->query_true_name() != owner)
    {
        tell_object(holder, "The %^C221%^g%^C220%^old%^C226%^e%^C220%^n%^CRST%^ handle %^C196%^rejects%^CRST%^ your %^C124%^h%^C160%^an%^C124%^d%^CRST%^ and you %^C196%^drop%^CRST%^ it!!");
        return 0;
    }
    
    
    tell_object(holder, "As you wield it, the %^C220%^ha%^C226%^n%^C220%^d%^C226%^l%^C220%^e%^CRST%^ %^C220%^ali%^C226%^g%^C220%^ht%^C226%^s%^CRST%^ with %^C231%^r%^C229%^a%^C228%^d%^C226%^i%^C220%^ant fl%^C226%^a%^C228%^m%^C229%^e%^C231%^s%^CRST%^!");
    tell_room(environment(holder), color(holder->query_cap_name() + "'s hammer alights with radiant flames."), holder);
    return 1;
}

int unwield_func()
{
    owner && tell_object(owner, "You feel the %^C124%^w%^C160%^ar%^C196%^m%^C160%^t%^C124%^h%^CRST%^ of the %^C220%^ham%^C226%^m%^C220%^e%^C226%^r%^CRST%^ %^C220%^d%^C226%^i%^C227%^s%^C228%^s%^C229%^i%^C230%^p%^C231%^ate%^CRST%^ as you %^C231%^r%^C230%^e%^C229%^l%^C230%^ea%^C231%^se%^CRST%^ it.");
    return 1;
} 