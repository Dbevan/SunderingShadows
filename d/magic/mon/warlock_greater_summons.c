inherit MINION;

#include "summoned_monster.h"

object cast;
int mylevel, myheritage, iselemental;
void setup_beastie(object caster, int beastheritage);
string fname, castname;

void create(){
    ::create();
    set_name("outsider");
    set_id(({"outsider","greater summon"}));
    set("short","outsider");
    set("long","This is an outsider. If you can see this, your warlock heritage is borked. Please contact a wiz.");
    set_hd(4,1);
    set_hp(40);
    set_exp(1);
    set_size(3);
    set_stats("strength",20);
    set_stats("constitution",20);
    set_stats("dexterity",5);
    set_race("outsider");
    set_gender("neuter");
    set_funcs(({"my_special"}));
    set_func_chance(20);
    iselemental = 0;
}

int isMagic() {return 5;}

void setup_beastie(object caster, int beastheritage) {
    if(!objectp(caster)) { return; }
    cast = caster;
    myheritage = beastheritage;
    castname = cast->query_name();
    fname = "/d/save/summons/"+castname+"/"+query_name();
    "/daemon/yuck_d"->load_inventory(this_object(),fname);
    switch(myheritage) {
      case "celestial":
        set_id(({"shiradi","resplendent shiradi","celestial","outsider","summoned monster","greater summon"}));
        set_short("%^BOLD%^%^CYAN%^r%^WHITE%^e%^MAGENTA%^s%^WHITE%^p%^CYAN%^l%^WHITE%^e%^MAGENTA%^n%^WHITE%^d%^CYAN%^e%^WHITE%^n%^CYAN%^t s%^WHITE%^h%^CYAN%^i%^WHITE%^r%^MAGENTA%^a%^WHITE%^d%^CYAN%^i%^RESET%^");
        set_long("%^BOLD%^%^CYAN%^The %^MAGENTA%^regal %^CYAN%^figure before you bears some resemblance to an elf, bearing the delicate "
"facial features and long tapered ears common to that race, but its stature is anything but elven, well over eight feet tall and boasting "
"a %^GREEN%^powerful%^CYAN%^, heavily-muscled physique barely covered by its loose-fitting garb of %^WHITE%^s%^RESET%^i%^BOLD%^l%^RESET%^v"
"%^BOLD%^%^CYAN%^e%^RESET%^r%^BOLD%^y l%^RESET%^i%^BOLD%^n%^RESET%^e%^BOLD%^n%^CYAN%^. Unfettered, %^RESET%^%^ORANGE%^c%^BOLD%^%^BLACK%^h"
"%^RESET%^e%^BOLD%^%^BLACK%^s%^RESET%^%^ORANGE%^t%^BOLD%^%^BLACK%^n%^RESET%^u%^ORANGE%^t-b%^BOLD%^%^BLACK%^r%^RESET%^o%^BOLD%^%^BLACK%^w"
"%^RESET%^%^ORANGE%^n h%^BOLD%^%^BLACK%^a%^RESET%^i%^ORANGE%^r %^BOLD%^%^CYAN%^falls loosely to its shoulders, adding a slight contrast to "
"the large pair of %^ORANGE%^a%^RESET%^%^ORANGE%^n%^BOLD%^g%^RESET%^%^ORANGE%^e%^BOLD%^l%^RESET%^%^ORANGE%^'%^BOLD%^s w%^RESET%^%^ORANGE%^i"
"%^BOLD%^n%^RESET%^%^ORANGE%^g%^BOLD%^s %^CYAN%^that spread out from its shoulder blades, their %^ORANGE%^f%^RESET%^%^ORANGE%^e"
"%^BOLD%^%^WHITE%^a%^RESET%^%^ORANGE%^t%^BOLD%^h%^RESET%^%^ORANGE%^e%^BOLD%^rs %^CYAN%^the color of %^ORANGE%^g%^RESET%^%^ORANGE%^o"
"%^WHITE%^l%^YELLOW%^d%^CYAN%^.%^RESET%^");
        set_name("celestial");
        set_race("celestial");
        set_body_type("humanoid");
        set_attack_limbs(({"left hand","right hand"}));
        add_limb("wings","torso",0,0,0);
        set_base_damage_type("bludgeoning");
      break;
      case "fey":
        set_id(({"myconid","multicolored myconid","fey","outsider","summoned monster","greater summon"}));
        set_short("%^CYAN%^m%^BOLD%^%^RED%^u%^RESET%^%^CYAN%^l%^ORANGE%^t%^CYAN%^i%^YELLOW%^c%^RESET%^%^CYAN%^o%^GREEN%^l%^CYAN%^o%^BOLD%^%^GREEN%^r%^CYAN%^e%^RESET%^%^CYAN%^d m%^BOLD%^y%^WHITE%^c%^RESET%^%^CYAN%^o%^WHITE%^n%^BOLD%^%^RED%^i%^RESET%^%^CYAN%^d%^WHITE%^");
        set_long("%^CYAN%^This %^GREEN%^strange%^CYAN%^, %^GREEN%^mushroom-like %^CYAN%^creature stands at about eight feet tall, and looks "
"to be solidly built with a thick frame and long woody arms with a large knobby %^BOLD%^%^WHITE%^fist %^RESET%^%^CYAN%^on the end of each. "
"The creature's %^GREEN%^tough%^CYAN%^, r%^GREEN%^u%^CYAN%^b%^GREEN%^b%^CYAN%^e%^GREEN%^r%^CYAN%^y body is a %^BOLD%^p%^RESET%^%^CYAN%^a"
"%^BOLD%^le bl%^RESET%^%^CYAN%^u%^BOLD%^e %^RESET%^%^CYAN%^in color, and a fleshy %^BOLD%^crown %^RESET%^%^CYAN%^sits atop its head like a "
"wide-brimmed hat, bearing a %^BOLD%^%^WHITE%^dizzying %^RESET%^%^CYAN%^pattern of %^BOLD%^%^MAGENTA%^b%^CYAN%^r%^MAGENTA%^i%^CYAN%^g%^RED%^h"
"%^MAGENTA%^t c%^RED%^o%^CYAN%^l%^WHITE%^o%^CYAN%^r%^MAGENTA%^s%^RESET%^%^CYAN%^. Its thin, %^BOLD%^%^BLACK%^black eyes %^RESET%^%^CYAN%^can "
"just be seen from underneath the crown, and faint %^WHITE%^p%^BOLD%^uff%^RESET%^s %^CYAN%^of %^BOLD%^%^WHITE%^i%^RESET%^r%^BOLD%^%^CYAN%^i"
"%^RESET%^d%^BOLD%^%^GREEN%^e%^WHITE%^s%^RESET%^c%^BOLD%^%^MAGENTA%^e%^RESET%^n%^BOLD%^t s%^RESET%^p%^BOLD%^%^MAGENTA%^o%^WHITE%^r%^CYAN%^e"
"%^WHITE%^s %^RESET%^%^CYAN%^can be glimpsed in the air as it moves about.%^WHITE%^");
        set_name("myconid");
        set_race("myconid");
        set_body_type("humanoid");
        set_attack_limbs(({"left hand","right hand"}));
        set_base_damage_type("bludgeoning");
      break;
      case "abyssal":
        set_id(({"hezrou","abominable hezrou","demon","outsider","summoned monster","greater summon"}));
        set_short("%^ORANGE%^a%^BOLD%^%^BLACK%^b%^RESET%^%^GREEN%^o%^BOLD%^%^BLACK%^m%^RESET%^i%^ORANGE%^n%^BOLD%^%^BLACK%^a%^RESET%^%^GREEN%^b%^BOLD%^%^BLACK%^l%^RESET%^%^ORANGE%^e h%^BOLD%^%^BLACK%^e%^RESET%^%^ORANGE%^z%^GREEN%^r%^BOLD%^%^BLACK%^o%^RESET%^%^ORANGE%^u%^WHITE%^");
        set_long("%^ORANGE%^This %^BOLD%^%^BLACK%^hulking%^RESET%^%^ORANGE%^, toad-like demon is at least eight feet tall while "
"standing upright. Its %^RED%^powerful %^ORANGE%^frame boasts limbs as thick as some tree trunks, and its large w%^GREEN%^e%^ORANGE%^b"
"%^BOLD%^%^BLACK%^b%^RESET%^%^GREEN%^e%^ORANGE%^d hands look capable of %^BOLD%^%^BLACK%^crushing %^RESET%^%^ORANGE%^solid stone in "
"their grasp. Its face is as wide as its body, with small eyes and a squat nose dwarfed by a large mouth lined with rows of "
"%^BOLD%^%^WHITE%^r%^RESET%^a%^BOLD%^z%^RESET%^o%^BOLD%^r-s%^RESET%^h%^BOLD%^a%^RESET%^r%^BOLD%^p t%^RESET%^e%^BOLD%^e%^RESET%^t"
"%^BOLD%^h%^RESET%^%^ORANGE%^, and a series of long %^WHITE%^s%^BOLD%^%^BLACK%^p%^RESET%^ik%^BOLD%^%^BLACK%^e%^RESET%^s %^ORANGE%^runs "
"down its back. Yet the most noticeable trait of this creature is its %^GREEN%^stench%^ORANGE%^, an %^BOLD%^a%^WHITE%^c%^ORANGE%^r"
"%^WHITE%^i%^ORANGE%^d%^RESET%^%^ORANGE%^, %^BOLD%^%^GREEN%^s%^RED%^t%^GREEN%^om%^ORANGE%^a%^GREEN%^ch-t%^RESET%^%^GREEN%^u%^BOLD%^r"
"%^RESET%^%^RED%^n%^BOLD%^%^GREEN%^i%^ORANGE%^n%^GREEN%^g aroma %^RESET%^%^ORANGE%^that seems to seep from its leathery skin and "
"%^BOLD%^taint %^RESET%^%^ORANGE%^the air around it.%^WHITE%^");
        set_name("demon");
        set_race("demon");
        set_body_type("humanoid");
        set_attack_limbs(({"left hand","right hand"}));
        set_base_damage_type("bludgeoning");
      break;
      case "astral":
        set_id(({"shuggoth","ooze","outsider","summoned monster","greater summon"}));
        set_short("%^BOLD%^%^BLACK%^w%^WHITE%^r%^RESET%^%^ORANGE%^i%^BOLD%^%^BLACK%^t%^RESET%^%^ORANGE%^hin%^BOLD%^%^BLACK%^g %^RESET%^s%^BOLD%^%^BLACK%^h%^RESET%^%^ORANGE%^ugg%^BOLD%^%^BLACK%^o%^WHITE%^t%^BLACK%^h%^RESET%^");
        set_long("%^BOLD%^%^BLACK%^This writhing mass of %^RESET%^%^ORANGE%^flesh %^BOLD%^%^BLACK%^is constantly %^RESET%^%^MAGENTA%^"
"p%^RED%^ul%^MAGENTA%^s%^RED%^at%^MAGENTA%^i%^RED%^ng %^BOLD%^%^BLACK%^and %^RESET%^%^MAGENTA%^w%^RED%^ea%^MAGENTA%^v%^RED%^in%^MAGENTA%^"
"g%^BOLD%^%^BLACK%^, coated in a t%^WHITE%^h%^RESET%^i%^BOLD%^%^BLACK%^ck o%^RESET%^o%^BOLD%^%^BLACK%^z%^RESET%^i%^BOLD%^n%^BLACK%^g b"
"%^WHITE%^l%^RESET%^a%^BOLD%^%^BLACK%^ck%^WHITE%^n%^RESET%^e%^BOLD%^%^BLACK%^ss. All along the shifting surface, %^GREEN%^s%^ORANGE%^tr"
"%^GREEN%^an%^ORANGE%^g%^GREEN%^e e%^ORANGE%^ye%^GREEN%^s %^BLACK%^blink into existence with alien rectangular pupils, gazing at its "
"surroundings before winking back into obscurity. Vicious %^WHITE%^toothy maws %^BLACK%^open and close with %^RESET%^%^RED%^s%^BOLD%^l"
"%^RESET%^%^RED%^a%^BOLD%^ve%^RESET%^%^RED%^r%^BOLD%^i%^RESET%^%^RED%^n%^BOLD%^g %^BLACK%^prehensile %^RESET%^%^RED%^t%^BOLD%^on%^RESET%^"
"%^RED%^g%^BOLD%^u%^RESET%^%^RED%^e%^BOLD%^s %^BLACK%^along the twisted tendrils, flowing as if in a pool of animate liquid. It heaves "
"itself along, moving with a purpose and will of its own.%^RESET%^");
        set_name("shuggoth");
        set_race("shuggoth");
        set_body_type("ooze");
        add_limb("tentacle","torso",0,0,0);
        set_attack_limbs(({"tentacle","body"}));
        set_base_damage_type("bludgeoning");
      break;
      case "gloom":
        set_id(({"bodak","nightmarish bodak","outsider","summoned monster","greater summon"}));
        set_short("%^BOLD%^%^BLACK%^n%^WHITE%^i%^RESET%^g%^BOLD%^h%^RESET%^t%^BOLD%^%^BLACK%^m%^RESET%^a%^BOLD%^r%^RESET%^i%^BOLD%^s%^BLACK%^h b%^WHITE%^o%^BLACK%^d%^RESET%^a%^BOLD%^%^BLACK%^k%^RESET%^");
        set_long("%^BOLD%^%^BLACK%^This %^RESET%^ghastly %^BOLD%^%^BLACK%^creature has about the same size and build as a human, "
"but that is where the similarities end. %^RESET%^D%^BOLD%^e%^RESET%^a%^BOLD%^t%^RESET%^h%^BOLD%^l%^RESET%^y p%^BOLD%^a%^RESET%^le "
"s%^BOLD%^ki%^RESET%^n %^BOLD%^%^BLACK%^stretches over its lean, naked form, devoid of any characteristics that might hint at its "
"gender, and it is enveloped in an eerie aura of %^CYAN%^coldness %^BLACK%^that r%^RESET%^a%^BOLD%^%^BLACK%^d%^RESET%^i"
"%^BOLD%^%^BLACK%^at%^RESET%^e%^BOLD%^%^BLACK%^s out from it for several yards. Most disturbing of all, however, is its elongated, "
"ovular head, its face twisted into a visage of %^RESET%^%^RED%^m%^BOLD%^a%^RESET%^%^RED%^d%^BOLD%^n%^RESET%^%^RED%^e%^BOLD%^s"
"%^RESET%^%^RED%^s %^BOLD%^%^BLACK%^and %^WHITE%^horror %^BLACK%^with empty %^WHITE%^white eyes %^BLACK%^that stare out at the world "
"with a dreadful %^RESET%^gaze %^BOLD%^%^BLACK%^that seems to pierce one's very %^WHITE%^soul%^BLACK%^.%^RESET%^");
        set_name("bodak");
        set_race("bodak");
        set_body_type("humanoid");
        set_attack_limbs(({"left hand","right hand"}));
        set_base_damage_type("slashing");
      break;
      case "infernal":
        set_id(({"devil","barbed devil","menacing barbed devil","outsider","summoned monster","greater summon"}));
        set_short("%^BOLD%^%^BLACK%^m%^RED%^e%^RESET%^n%^BOLD%^%^RED%^a%^BLACK%^c%^RED%^i%^RESET%^%^RED%^n%^BOLD%^%^BLACK%^g b%^RESET%^%^RED%^a%^BOLD%^r%^BLACK%^b%^RED%^e%^BLACK%^d d%^RED%^e%^RESET%^v%^RED%^i%^BOLD%^%^BLACK%^l%^RESET%^");
        set_long("%^BOLD%^%^RED%^This fearsome %^BLACK%^devil %^RED%^stands at about seven feet tall, and its %^BLACK%^l%^RESET%^i"
"%^BOLD%^t%^RESET%^h%^BOLD%^%^BLACK%^e%^RED%^, %^WHITE%^s%^BLACK%^i%^WHITE%^n%^RESET%^e%^BOLD%^%^BLACK%^w%^WHITE%^y %^RED%^body is "
"covered almost entirely by long and %^RESET%^s%^BOLD%^ha%^RESET%^rp %^BOLD%^%^BLACK%^b%^RESET%^a%^BOLD%^r%^RESET%^b"
"%^BOLD%^%^BLACK%^s%^RED%^. Its slender, four-fingered hands are tipped with long, %^WHITE%^razor-sharp claws %^RED%^that look "
"particularly well suited to %^ORANGE%^ripping %^RED%^into flesh, and a long %^RESET%^s%^BOLD%^p%^RESET%^i%^BOLD%^%^BLACK%^k%^WHITE%^e"
"%^RESET%^d %^BOLD%^%^RED%^tail drags along the ground behind it. It walks with a hunched-over, %^BLACK%^p%^RESET%^r%^BOLD%^%^BLACK%^e"
"%^RESET%^d%^BOLD%^%^BLACK%^a%^RESET%^t%^BOLD%^%^BLACK%^o%^RESET%^r%^BOLD%^%^BLACK%^y %^RED%^manner, looking ready to leap upon some "
"unsuspecting %^RESET%^%^RED%^v%^BOLD%^%^BLACK%^i%^WHITE%^c%^RESET%^%^RED%^t%^BOLD%^%^BLACK%^i%^RESET%^%^RED%^m %^BOLD%^at any moment.%^RESET%^");
        set_name("devil");
        set_race("devil");
        set_body_type("humanoid");
        set_attack_limbs(({"left hand","right hand"}));
        add_limb("spiked tail","torso",0,0,0);
        set_base_damage_type("slashing");
      break;
    }
}

void my_special(object target) {
    if(!objectp(target)) return;

    switch(myheritage){
      case "celestial":
        tell_object(target,"%^BOLD%^%^CYAN%^With a powerful %^GREEN%^shout%^CYAN%^, the shiradi %^ORANGE%^transforms "
"%^CYAN%^into a %^ORANGE%^w%^WHITE%^h%^ORANGE%^i%^WHITE%^rl%^ORANGE%^i%^WHITE%^n%^ORANGE%^g c%^WHITE%^l%^ORANGE%^o"
"%^WHITE%^u%^ORANGE%^d %^CYAN%^of %^ORANGE%^g%^WHITE%^l%^ORANGE%^o%^WHITE%^w%^ORANGE%^i%^WHITE%^n%^ORANGE%^g s"
"%^WHITE%^h%^ORANGE%^ar%^WHITE%^d%^ORANGE%^s%^CYAN%^! It moves faster than you can react, trapping you momentarily "
"within the %^ORANGE%^razor-sharp storm %^CYAN%^and shredding your skin!%^RESET%^");
        tell_room(ETO,"%^BOLD%^%^CYAN%^With a powerful %^GREEN%^shout%^CYAN%^, the shiradi %^ORANGE%^transforms "
"%^CYAN%^into a %^ORANGE%^w%^WHITE%^h%^ORANGE%^i%^WHITE%^rl%^ORANGE%^i%^WHITE%^n%^ORANGE%^g c%^WHITE%^l%^ORANGE%^o"
"%^WHITE%^u%^ORANGE%^d %^CYAN%^of %^ORANGE%^g%^WHITE%^l%^ORANGE%^o%^WHITE%^w%^ORANGE%^i%^WHITE%^n%^ORANGE%^g s"
"%^WHITE%^h%^ORANGE%^ar%^WHITE%^d%^ORANGE%^s%^CYAN%^! "+capitalize(target->QCN)+" is caught within the %^ORANGE%^razor"
"-sharp storm%^CYAN%^, sustaining horrible wounds!%^RESET%^",target);
      break;
      case "fey":
        tell_object(target,"%^CYAN%^The %^BOLD%^myconid %^RESET%^%^CYAN%^releases a %^WHITE%^cloud %^CYAN%^of %^BOLD%^i"
"%^MAGENTA%^r%^CYAN%^i%^WHITE%^d%^CYAN%^e%^GREEN%^s%^CYAN%^c%^WHITE%^e%^CYAN%^nt s%^WHITE%^p%^MAGENTA%^o%^CYAN%^r"
"%^WHITE%^e%^CYAN%^s %^RESET%^%^CYAN%^in your face, and you find your thoughts quickly slipping away! You are "
"%^BOLD%^knocked %^RESET%^%^CYAN%^out of your reverie as the creature follows up with a %^BOLD%^%^WHITE%^crushing "
"blow %^RESET%^%^CYAN%^to your head!%^RESET%^");
        tell_room(ETO,"%^CYAN%^The %^BOLD%^myconid %^RESET%^%^CYAN%^releases a cloud of %^BOLD%^i%^MAGENTA%^r%^CYAN%^i"
"%^WHITE%^d%^CYAN%^e%^GREEN%^s%^CYAN%^c%^WHITE%^e%^CYAN%^nt s%^WHITE%^p%^MAGENTA%^o%^CYAN%^r%^WHITE%^e%^CYAN%^s "
"%^RESET%^%^CYAN%^in "+target->QCN+"'s face, %^BOLD%^%^WHITE%^dazing %^RESET%^%^CYAN%^"+target->QO+"! It follows up with "
"%^BOLD%^%^WHITE%^crushing blow %^RESET%^%^CYAN%^while "+target->QP+" guard is down!%^RESET%^",target);
      break;
      case "abyssal":
        tell_object(target,"%^ORANGE%^The hezrou grabs you in one of its %^BOLD%^%^GREEN%^massive hands %^RESET%^"
"%^ORANGE%^and drags you closer! Your stomach turns, and you can't help but %^BOLD%^c%^RED%^h%^ORANGE%^o%^RED%^k"
"%^ORANGE%^e %^RESET%^%^ORANGE%^on the demon's %^BOLD%^%^GREEN%^t%^RED%^o%^WHITE%^x%^ORANGE%^i%^GREEN%^c s"
"%^ORANGE%^t%^GREEN%^e%^RED%^n%^RESET%^%^ORANGE%^c%^BOLD%^%^GREEN%^h %^RESET%^%^ORANGE%^as you %^BOLD%^%^RED%^struggle "
"%^RESET%^%^ORANGE%^to free yourself!%^RESET%^");
        tell_room(ETO,"%^ORANGE%^The hezrou grabs "+target->QCN+" in one of its %^BOLD%^%^GREEN%^massive hands %^RESET%^"
"%^ORANGE%^and drags "+target->QO+" closer! "+capitalize(target->QCN)+" %^BOLD%^c%^RED%^h%^ORANGE%^o%^WHITE%^k%^RED%^e"
"%^ORANGE%^s %^RESET%^%^ORANGE%^on the demon's %^BOLD%^%^GREEN%^t%^RED%^o%^WHITE%^x%^ORANGE%^i%^GREEN%^c s%^ORANGE%^t"
"%^GREEN%^e%^RED%^n%^RESET%^%^ORANGE%^c%^BOLD%^%^GREEN%^h %^RESET%^%^ORANGE%^as "+target->QS+" %^BOLD%^%^RED%^struggles "
"%^RESET%^%^ORANGE%^to free "+target->QO+"self!%^RESET%^",target);
      break;
      case "astral":
        tell_object(target,"%^ORANGE%^With a great heave, the %^BOLD%^%^BLACK%^shuggoth %^RESET%^%^ORANGE%^rises up and "
"slams down into you, the %^BOLD%^%^BLACK%^t%^RESET%^e%^BOLD%^n%^BLACK%^ta%^WHITE%^c%^RESET%^l%^BOLD%^%^BLACK%^e%^RESET%^s "
"%^ORANGE%^and %^BOLD%^%^WHITE%^t%^RESET%^e%^BOLD%^e%^RESET%^t%^BOLD%^h %^RESET%^%^ORANGE%^writhing about your limbs in "
"sensations both %^RED%^slippery %^ORANGE%^and %^RED%^sinister%^ORANGE%^, %^BOLD%^%^RED%^wrenching %^RESET%^%^ORANGE%^and "
"%^BOLD%^%^RED%^tearing%^RESET%^%^ORANGE%^!%^WHITE%^");
        tell_room(ETO,"%^RESET%^%^ORANGE%^With a great heave, the %^BOLD%^%^BLACK%^shuggoth %^RESET%^%^ORANGE%^rises up "
"and slams down into %^WHITE%^"+target->QCN+"%^RESET%^%^ORANGE%^, the %^BOLD%^%^BLACK%^t%^RESET%^e%^BOLD%^n%^BLACK%^ta"
"%^WHITE%^c%^RESET%^l%^BOLD%^%^BLACK%^e%^RESET%^s %^ORANGE%^and %^BOLD%^%^WHITE%^t%^RESET%^e%^BOLD%^e%^RESET%^t%^BOLD%^h "
"%^RESET%^%^ORANGE%^writhing about %^WHITE%^"+target->QP+" %^RESET%^%^ORANGE%^limbs in sensations both %^RED%^slippery "
"%^ORANGE%^and %^RED%^sinister%^ORANGE%^, %^BOLD%^%^RED%^wrenching %^RESET%^%^ORANGE%^and %^BOLD%^%^RED%^tearing%^RESET%^"
"%^ORANGE%^!%^WHITE%^",target);
      break;
      case "gloom":
        tell_object(target,"%^BOLD%^The bodak stops abruptly and locks its %^BLACK%^g%^RESET%^a%^BOLD%^z%^BLACK%^e "
"%^WHITE%^with yours. Time seems to %^RESET%^%^CYAN%^freeze %^BOLD%^%^WHITE%^in place as you feel a terrible %^CYAN%^coldness "
"%^WHITE%^seep into your body and soul. You soon realize what is happening and tear your gaze away from the creature, your "
"whole body suddenly wracked with %^RED%^pain%^WHITE%^!%^RESET%^");
        tell_room(ETO,"%^BOLD%^The bodak abruptly stops and locks its %^BLACK%^g%^RESET%^a%^BOLD%^z%^BLACK%^e "
"%^WHITE%^with "+target->QCN+"'s. "+capitalize(target->QS)+" %^RESET%^%^CYAN%^freezes %^BOLD%^%^WHITE%^for the space of a "
"heartbeat, then %^RED%^screams %^WHITE%^in agony, tearing "+target->QP+" gaze away from the creature!%^RESET%^",target);
      break;
      case "infernal":
        tell_object(target,"%^BOLD%^%^RED%^The %^BLACK%^barbed devil %^RED%^bolts forward with a %^WHITE%^hiss%^RED%^, "
"grabbing you and drawing you into a %^WHITE%^crushing %^RED%^bear hug! You struggle to free yourself, but are %^ORANGE%^torn "
"%^RED%^by the devil's %^RESET%^s%^BOLD%^p%^RESET%^i%^BOLD%^%^BLACK%^k%^WHITE%^e%^RESET%^s %^BOLD%^%^RED%^in the process"
"!%^RESET%^");
        tell_room(ETO,"%^BOLD%^%^RED%^The %^BLACK%^barbed devil %^RED%^bolts forward with a %^WHITE%^hiss%^RED%^, "
"grabbing "+target->QCN+" and drawing "+target->QO+" into a %^WHITE%^crushing %^RED%^bear hug! "
+capitalize(target->QCN)+" is %^ORANGE%^torn %^RED%^by the devil's %^RESET%^s%^BOLD%^p%^RESET%^i%^BOLD%^%^BLACK%^k"
"%^WHITE%^e%^RESET%^s %^BOLD%^%^RED%^as "+target->QS+" struggles to free "+target->QO+"self!%^RESET%^",target);
      break;
      default: // error prompt
        tell_room(ETO,"%^BOLD%^%^RED%^Your beastie is broken, contact a wiz!%^RESET%^");
      break;
    }
    TO->set_property("magic",1);
    target->do_damage("torso",random(40)+(mylevel*2));
    TO->remove_property("magic");
}

void die(object obj) {
    if(objectp(cast)) cast->remove_property("has_elemental");
    tell_room(ETO,"%^BOLD%^%^RED%^Unable to maintain corporeal form, outsider return to their home plane.%^RESET%^");
    remove();
}

set_caster(object ob){
   if(objectp(ob)) cast = ob;
   else remove();
}

object query_caster(){return cast;}

void set_mylevel(int x){
   mylevel = x;
   set_hd(mylevel,10);
   set_mlevel("fighter",mylevel);
   set_max_hp(mylevel*20);
   set_hp((int)TO->query_max_hp());
   set_attacks_num((mylevel/13)+1);
   set_damage(1,mylevel);
   set_overall_ac(10-mylevel);
   set_stats("strength",20);
   set_stats("constitution",20);
   set_stats("dexterity",5);
   set_property("effective_enchantment",(mylevel/7)+1);
}

void defend(){
   int num,numfoes;
   object *tmp;

   tmp = ({});
   numfoes=sizeof(tmp = cast->query_attackers());
   for(num =0;num < numfoes;num++){
      if(!objectp(tmp[num])) continue;
      if(member_array(tmp[num], (object *)TO->query_attackers()) != -1) continue;
      if(!objectp(TO)) continue;
      TO->kill_ob(tmp[num],1);
      tell_room(ETO,"%^BOLD%^%^RED%^The extraplanar creature attacks "+tmp[num]->QCN+"!");
   }
   return;
}

int remove()
{
    object me;

    if (!objectp(me = this_object()))
        return 0;

    save_outsider();
    all_inventory(me)->remove();
    ::remove();

    return 1;
}

void heart_beat(){
   string party;
   ::heart_beat();
   if(!objectp(cast)) remove();
   if(!objectp(TO)) remove();
   if(!sizeof(cast->query_attackers())) return;
   if((object *)cast->query_attackers() != ({})) defend();
   TO->remove_attacker(0);

   if(query_hp() < query_max_hp()) add_hp(query_max_hp() / 25);
}

void save_outsider()
{
    if(!objectp(TO))
        return;
    if(!stringp(castname) ||
       castname == "")
        return;
    if(!userp(cast))
        return;
    seteuid(getuid());
    mkdir("/d/save/summons/"+castname);
    mkdir(fname);
    "/daemon/yuck_d"->save_inventory(this_object(),fname);
}

