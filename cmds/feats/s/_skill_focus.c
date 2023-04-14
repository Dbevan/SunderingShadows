#include <std.h>
#include <daemons.h>
#include <dirs.h>
#include <skills.h>

inherit FEAT;

int FLAG;

void create()
{
    ::create();
    feat_type("permanent");
    feat_category("NonCombat");
    feat_name("skill focus");
    feat_syntax("skill_focus");
    feat_desc("Skill Focus allows the selection of one non-class skill, which the character will then be skilled at as per a class skill.

%^BOLD%^N.B.%^RESET%^ If multiclassing in future causes the skill to be a class skill (skill on the class helpfile), this feat will have no effect. It must be abandoned and regained in order to select a different skill focus. Crafting skills cannot be taken by way of this feat.");
    permanent(1);
}

int allow_shifted() { return 1; }

void execute_feat()
{
    ::execute_feat();
    dest_effect();
}

int cmd_skill_focus(string args)
{
    if(this_player()->query("skill_focus"))
    {
        tell_object(this_player(), "%^CRST%^You already have chosen %^C255%^"+this_player()->query("skill_focus")+"%^CRST%^ to focus on.");
        dest_effect();
        return 1;
    }
    
    tell_object(this_player(), "%^YELLOW%^In order to gain the skill focus feat, you must select a skill that you wish to learn as a class-skill.  You can pick any skill that isn't already class-based.%^RESET%^");
    tell_object(this_player(), "Please type the name of the skill that you would like your skill focus feat to use.");
    input_to("skill_focus_setting", args, this_player());
    
    return 1;
}

int skill_focus_setting(string str, object ob)
{
    int myflag, i, amt;
    string* myclasses, * myclassskills, file;

    if (!objectp(ob)) {
        return 0;
    }

    if (!stringp(str)) {
        return 0;
    }

    if (member_array(str, CORE_SKILLS) == -1) {
        tell_object(ob, "%^YELLOW%^The %^BLUE%^" + str + " %^YELLOW%^ skill does not exist, or is not a valid choice for this feat.%^RESET%^");
        tell_object(ob, "Aborting...");

        return 1;
    }
    myclasses = ob->query_classes();
    for (i = 0; i < sizeof(myclasses); i++) {
        file = DIR_CLASSES + "/" + myclasses[i] + ".c";
        if (file_exists(file)) {
            myclassskills = (string*)file->class_skills(ob);
            if (member_array(str, myclassskills) != -1) {
                tell_object(ob, "%^RED%^Warning:%^YELLOW%^ you already have %^BLUE%^" + str + " %^YELLOW%^as a class skill from one or more of your existing classes.%^RESET%^");
            }
        }
    }
    
    tell_object(ob, "%^YELLOW%^Setting your Skill Focus skill to %^BLUE%^" + str + "%^YELLOW%^.%^RESET%^");
    ob->set("skill_focus", str);
    
    return 1;
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

void dest_effect()
{
    ::dest_effect();
    remove_feat(TO);
    return;
}
