// Commonalities for all standard herbs
// Reworked to fit the new alchemy system by Spade, 27 March 2023
// Originally coded by ~Circe~ 4/19/08
// Descriptions and actions by Lurue

inherit "/std/alchemy/herb_inherit.c";


void create()
{
    set_name("a paper packet of " + alchemical_id);
    set_id(({ "herb", "herbs", alchemical_id, "packet", "packet of " + alchemical_id }));
    set_short("A paper packet of " + alchemical_id);
    set_weight(1);
}

void init()
{
   ::init();

   if (!living(this_player()))
       return;

   add_action("use_em","use");
}

int use_em(string str)
{
    object player = this_player();

    if(!objectp(this_object()) || !id(str))
        return 0;

    switch(random(100))
    {
    case 0..74:
        tell_object(player, "You use the %^GREEN%^" + this_object()->query_short() + " %^RESET%^and soon feel it affecting you in a helpful way.");
        tell_room(environment(player), "" + player->query_cap_name() + " uses some %^GREEN%^" + this_object()->query_short() + "%^RESET%^.", player);
        remove();
        break;
    case 75..94:
        tell_object(player, "You use the %^GREEN%^" + herb_name + " %^RESET%^and wait, but feel no effect.");
        tell_room(environment(player), "" + player->query_cap_name() + " uses some %^GREEN%^" + this_object()->query_short() + "%^RESET%^.", player);
        remove();
        break;
    default:
        tell_object(player, "You use the %^GREEN%^"+herb_name+" %^RESET%^and soon begin to feel nauseous and ill!");
        tell_room(environment(player), "" + player->query_cap_name() + " uses some %^GREEN%^" + this_object()->query_cap_name() + "%^RESET%^.", player);
        remove();
        break;
   }

   return 1;
}
