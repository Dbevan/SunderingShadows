#include <std.h>
#include <magic.h>
#include <party.h>
#include <daemons.h>

inherit SPELL;


void create()
{
    ::create();
    set_spell_name("mass harm");
    set_spell_level(([ "cleric" : 9 ]));
    set_affixed_spell_level(6);
    set_spell_sphere("necromancy");
    set_syntax("cast CLASS mass harm on TARGET");
    set_damage_desc("negative energy");
    set_description("This is a stronger version of harm spell. It acts like mass heal, but channels negative energy.");
    set_verbal_comp();
    set_somatic_comp();
    set_target_required(1);
    set_helpful_spell(1);
    splash_spell(1);
}

int preSpell()
{
    if (!objectp(target)) {
        tell_object(caster, "You must specify a target for this spell.");
        return 0;
    }
    return 1;
}

string query_cast_string()
{
    tell_object(caster, "%^BOLD%^%^BLACK%^Your voice rings out as you begin to " +
                "chant a powerful prayer, gathering divine energy into your hands.");
    tell_room(place, "%^BOLD%^%^BLACK%^" + caster->QCN + "'s voice rings out as they" +
              " begin to chant a powerful prayer.  Cupping " + caster->QP + " hands, " +
              "" + caster->QS + " starts to gather energy.", caster);
    return "display";
}

void spell_effect(int prof)
{
    int i;
    object* party_members = ({}), * attackers = ({}), * targets = ({}), * followers = ({});

    set_helpful_spell(0);
    party_members = ob_party(caster);
    attackers = target_selector();
    followers = caster->query_followers();

    if (!objectp(target)) {
        target = caster;
    }

    if (target == caster ||
        member_array(target, party_members) != -1 ||
        member_array(target, followers) != -1) {
        targets = filter_array(distinct_array(party_members + (followers - attackers)) + ({ caster }),
            (: !!$1->query_property("negative energy affinity") || !!$1->query_property("heart of darkness") :));
        set_helpful_spell(1);
    }else if (member_array(target, attackers) != -1) {
        set_helpful_spell(0);
        targets = filter_array(attackers, (: !$1->query_property("negative energy affinity") :));
    }else {
        targets = all_living(place);
    }

    targets = distinct_array(targets);

    tell_room(place, "%^BOLD%^%^BLACK%^" + caster->QCN + " opens " +
              "" + caster->QP + " hands, releasing a fell wave of" +
              " energy as " + caster->QS + " shouts out the final words " +
              "of the prayer.", caster);

    if (sizeof(targets)) {
        int healamnt = calculate_healing(), wound;
        for (i = 0; i < sizeof(targets); i++) {
            if (!objectp(targets[i])) {
                continue;
            }
            if (!present(targets[i], place)) {
                continue;
            }
            if (!targets[i]->query_property("negative energy affinity") && !targets[i]->query_property("heart of darkness")) {
                if (targets[i] == caster) {
                    continue;
                    tell_object(targets[i], "You shouldn't do that to yourself.");
                }
                set_helpful_spell(0);
            }else {
                set_helpful_spell(1);
            }

            if (targets[i] == caster) {
                tell_object(targets[i], "%^BOLD%^%^BLACK%^A fell " +
                            "wave moves through you, carrying with it the essence of death.");
            }else {
                tell_room(place, "%^BOLD%^%^BLACK%^A fell wave moves through" +
                          " " + targets[i]->QCN + " carrying with it the essence of " +
                          "death, as " + caster->QCN + " voice rings out.", ({ targets[i], caster }));
                tell_object(caster, "%^BOLD%^%^BLACK%^A fell " +
                            "wave moves through " + targets[i]->QCN + ", carrying with it the essence of death.");
                tell_object(targets[i], "%^BOLD%^%^BLACK%^A fell " +
                            "wave moves through you, carrying with it the essence of death.");
            }
            wound = damage_targ(targets[i], targets[i]->return_target_limb(), healamnt, "negative energy");

            if(!help_or_harm && wound > 0 && spell_type == "oracle")
            {

                if(caster->query_mystery() == "lunar" && caster->query_class_level("oracle") >= 31)
                {
                    set_save("will");
                    if(!do_save(targets[i], 0))
                    {
                        tell_room(place, "%^BOLD%^The pull of the moon assaults " + targets[i]->query_cap_name() + "'s mind!", targets[i]);
                        tell_object(targets[i], "%^BOLD%^The pull of the moon assaults your mind!");
                        "/std/effect/status/confused"->apply_effect(targets[i],clevel/5+1,caster);
                    }
                    set_save();
                }
            }

            if (query_spell_name() == "mass harm") {
                if (member_array(targets[i], caster->query_attackers()) == -1) {
                    "/std/magic/cleanse"->cleanse(targets[i]);
                }
            }
        }
    }

    spell_successful();
    dest_effect();
    return;
}

int calculate_healing(object targ)
{
    return sdamage;
}

void dest_effect()
{
    ::dest_effect();
    if (objectp(TO)) {
        TO->remove();
    }
}
