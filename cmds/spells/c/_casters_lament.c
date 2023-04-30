#include <std.h>
#include <magic.h>
inherit SPELL;

int wasbuff, lower;

void create() {
    ::create();
    set_author("nienne");
    set_spell_name("casters lament");
    set_spell_level(([ "warlock" : 4 ]));
    set_spell_sphere("abjuration");
    set_bonus_type("resistance");
    set_damage_desc("+6 to all saves OR shatters magic on cursed item");
    set_syntax("cast CLASS casters lament (on <object> [at <player>])");
    set_description("This invocation harnesses the greatest of a warlock's natural aptitude for manipulating magical energies and items. For the first, if cast with no target, it will imbue the warlock with a considerable resistance to magical attacks. If directed at an item, either in the caster's own inventory or in that of another player, it can be used to shatter the magic upon a cursed item.");
    set_verbal_comp();
    set_somatic_comp();
    set_arg_needed();
    set_helpful_spell(1);
}

string query_casting_string() {
    tell_object(caster, "%^RESET%^%^CRST%^%^C091%^You voice the words to the incantation, and your fingertips %^C099%^g%^C105%^l%^C111%^e%^C105%^a%^C099%^m %^RESET%^%^C091%^with %^C097%^e%^C103%^ne%^C109%^r%^C097%^g%^C103%^y%^C093%^.%^CRST%^");
    tell_room(place, "%^RESET%^%^CRST%^%^C091%^"+caster->query_cap_name()+"%^RESET%^%^CRST%^%^C091%^ voices an invocation, and "+caster->query_possessive()+" fingertips %^C099%^g%^C105%^l%^C111%^e%^C105%^a%^C099%^m %^RESET%^%^C091%^with %^C097%^e%^C103%^ne%^C109%^r%^C097%^g%^C103%^y%^C093%^.%^CRST%^", caster);
    return "display";
}

void spell_effect(int prof) {
    string what,who;
    object ob, ob2;
    int ench;

    if(!arg) { // if no targetting, raise saves of the caster - same formula as prot from spells/etc
      /*
      if(caster->query_property("protection from spells")){
        tell_object(caster,"%^BOLD%^Your invocation conflicts with existing magic and is repelled forcibly.");
        dest_effect();
        return;
      }
      */
      tell_object(caster, "%^RESET%^%^CRST%^%^C091%^You press your hands together and the %^C097%^e%^C103%^ne%^C109%^r%^C097%^g%^C103%^y %^RESET%^%^C099%^r%^C105%^i%^C111%^p%^C099%^p%^C105%^l%^C111%^e%^C099%^s %^RESET%^%^C091%^and flows back over itself, down your arms and over your body to encase you in %^C103%^protective spellcraft%^C091%^!%^CRST%^");
      tell_room(place, "%^RESET%^%^CRST%^%^C091%^"+caster->query_cap_name()+"%^RESET%^%^CRST%^%^C091%^ presses "+caster->query_possessive()+" hands together and the %^C097%^e%^C103%^ne%^C109%^r%^C097%^g%^C103%^y %^RESET%^%^C099%^r%^C105%^i%^C111%^p%^C099%^p%^C105%^l%^C111%^e%^C099%^s %^RESET%^%^C091%^and flows back over itself, down "+caster->query_possessive()+" arms and over "+caster->query_possessive()+" body!%^CRST%^", caster);
      wasbuff = 1;
      lower = 6;
      //lower = clevel / 6 + 2;
      //lower = lower > 8 ? 8 : lower;
      caster->add_saving_bonus("all",lower);
      //caster->set_property("protection from spells", 1);
      addSpellToCaster();
      spell_successful();
      spell_duration = (clevel + roll_dice(1, 20)) * ROUND_LENGTH;
      set_end_time();
      call_out("dest_effect",spell_duration);
      return;
    }
    if (sscanf(arg,"%s at %s", what, who) != 2) {
        what = arg;
        who = 0;
    }
    if(!who) ob = present(what,caster);
    else {
        if((string)caster->realName(who) != "") who = (string)caster->realName(who);
        if(!ob2 = present(who, place)) {
            tell_object(caster, "%^RESET%^%^CRST%^%^C059%^That person isn't here.%^CRST%^");
            dest_effect();
            return;
        }
        ob = present(what,ob2);
    }
    if (!objectp(ob)) {
        tell_object(caster, "%^RESET%^%^CRST%^%^C059%^That object is not present.%^CRST%^");
        dest_effect();
        return;
    }
    ench = ob->query_property("enchantment");
    if (ench > -1) {
        tell_object(caster, "%^RESET%^%^CRST%^%^C059%^That object is not cursed.%^CRST%^");
        dest_effect();
        return;
    }
    if(ob->query_property("no remove") || ob->query_property("no remove curse")) {
        tell_object(caster, "%^RESET%^%^CRST%^%^C059%^That curse is too powerful even for you to break.%^CRST%^");
        dest_effect();
        return;
    }
    ob->remove_property("enchantment");
    tell_object(caster, "%^RESET%^%^CRST%^%^C091%^You touch your hands to the "+what+"%^RESET%^%^CRST%^%^C091%^ and the %^C097%^e%^C103%^ne%^C109%^r%^C097%^g%^C103%^y %^RESET%^%^C091%^feeds into it, before %^C099%^s%^C105%^h%^C111%^a%^C099%^t%^C105%^t%^C111%^e%^C099%^r%^C105%^i%^C111%^n%^C099%^g %^RESET%^%^C091%^in a %^C111%^b%^C117%^r%^C123%^ig%^C117%^h%^C111%^t b%^C117%^u%^C123%^r%^C117%^s%^C111%^t%^C255%^!%^CRST%^");
    tell_room(place, "%^RESET%^%^CRST%^%^C091%^"+caster->query_cap_name()+"%^RESET%^%^CRST%^%^C091%^ touches "+caster->query_possessive()+" hands to the "+what+"%^RESET%^%^CRST%^%^C091%^ and the %^C097%^e%^C103%^ne%^C109%^r%^C097%^g%^C103%^y %^RESET%^%^C091%^feeds into it, before %^C099%^s%^C105%^h%^C111%^a%^C099%^t%^C105%^t%^C111%^e%^C099%^r%^C105%^i%^C111%^n%^C099%^g %^RESET%^%^C091%^in a %^C111%^b%^C117%^r%^C123%^ig%^C117%^h%^C111%^t b%^C117%^u%^C123%^r%^C117%^s%^C111%^t%^C255%^!%^CRST%^", caster);
    spell_successful();
    dest_effect();
}

void dest_effect(){
    if(wasbuff) {
      if (objectp(caster)) {
        caster->add_saving_bonus("all",-lower);
        caster->remove_property("protection from spells");
        tell_room(environment(caster), "%^RESET%^%^CRST%^%^C097%^E%^C103%^ne%^C109%^r%^C097%^g%^C103%^y %^C099%^f%^C105%^l%^C111%^i%^C099%^c%^C105%^k%^C111%^e%^C099%^r%^C105%^s %^RESET%^%^C091%^briefly around "+caster->query_cap_name()+"%^RESET%^%^CRST%^%^C091%^ before waning.%^CRST%^", caster);
        tell_object(caster, "%^RESET%^%^CRST%^%^C091%^A breath of cool air reaches your skin as the %^C103%^protective invocation %^C099%^f%^C105%^l%^C111%^i%^C099%^c%^C105%^k%^C111%^e%^C099%^r%^C105%^s %^RESET%^%^C091%^and wanes!%^CRST%^");
      }
    }
    ::dest_effect();
    if(objectp(TO)) TO->remove();
}

