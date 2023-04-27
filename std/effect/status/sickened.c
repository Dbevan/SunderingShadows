#include <std.h>
#include <magic.h>
#include <skills.h>
#include <daemons.h>

inherit STATUS;

int power;

void create()
{
    ::create();
    set_name("effect_sickened");
}
int status_effect()
{
    int i;

    if (target->query_property("effect_sickened")) {
        TO->remove();
        return;
    }
    
    if(PLAYER_D->immunity_check(target, "sickened"))
    {
        tell_object(target, "%^YELLOW%^You are immune to being sickened.%^RESET%^");
        this_object()->remove();
        return;
    }

    target->set_property("effect_sickened", 1);

    tell_object(target, "%^BLUE%^You feel sickened and disgusted.%^RESET%^");
    tell_room(ENV(target), "%^BLUE%^" + target->QCN + "'s face turns green, and " + target->QS + " look sickened.", target);

    power = target->query_level() / 12 + 1;

    for (i = 0; i < sizeof(CORE_SKILLS); i++) {
        target->add_skill_bonus(CORE_SKILLS[i], -power);
    }
    target->add_attack_bonus(-power);
    target->add_damage_bonus(-power);
    target->add_saving_bonus("all", -power);

    call_out("dest_effect", ROUND_LENGTH * duration, target);
    return 1;
}
int dest_effect(object ob)
{
    int i;
    if (objectp(ob)) {
        ob->set_property("effect_sickened", -1);

        if (ob->query_property("effect_sickened")) {
            tell_object(ob,"%^BLUE%^You feel less sickened.");
        } else {
            tell_object(ob, "%^BLUE%^You no longer feel sickened.%^RESET%^");
        }
        tell_room(ENV(ob), "%^BLUE%^" + ob->QCN + " no longer looks sickened.", ob);
        for (i = 0; i < sizeof(CORE_SKILLS); i++) {
            ob->add_skill_bonus(CORE_SKILLS[i], power);
        }
        ob->add_attack_bonus(power);
        ob->add_damage_bonus(power);
        ob->add_saving_bonus("all", power);

    }
    ::dest_effect();
    return 1;
}
