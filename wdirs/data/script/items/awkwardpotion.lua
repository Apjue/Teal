teal_item =
{
    codename = "awkwardpotion",
    name = "Awkward Potion",
    description = "",
    level = 1,
    icon = ":/game/items/consumables/potion",

    components =
    {
		consumable = {},

		states =
		{
            {
				metadata_id = "heal",
				turns = 3,
				
				effects =
				{
					{
		                state_type = "healed",

		                element = "fire",
		                health = 18
					},
				}
			}
		}
    }
}
