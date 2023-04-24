#include <std.h>
#include <daemons.h>
inherit FEAT;

void create()
{
   ::create();
   feat_type("permanent");
   feat_category("PactInvocation");
   feat_name("vitriolic blast");
   feat_prereq("Warlock L21");
   feat_desc("This feat represents the ability to apply the 'vitriolic' modifier to your eldritch blast and eldritch strikes. Upon hitting the opponent, they will be splashed with a powerful acid that bypasses magical resistances and damages them over time. Applying this blasttype also changes the damage type of eldritch blast and eldritch strikes to acid. Only one blasttype can be applied at a time and they do not stack.\n\See Also: help blasttype");
   permanent(1);
   allow_blind(1);
   set_required_for(({}));
}

int allow_shifted() { return 1; }

int prerequisites(object ob)
{
    if(!objectp(ob)) { return 0; }

    if((int)ob->query_class_level("warlock") < 21) {
        dest_effect();
        return 0;
    }
    return ::prerequisites(ob);
}

void permanent_effects(object ob)
{
    ::permanent_effects(ob);
    dest_effect();
    return;
}

void reverse_permanent_effects(object ob)
{
    ::reverse_permanent_effects(ob);
    dest_effect();
    return;
}

void execute_feat()
{
   ::execute_feat();
   dest_effect();
}

void dest_effect()
{
   ::dest_effect();
   remove_feat(TO);
   return;
}
