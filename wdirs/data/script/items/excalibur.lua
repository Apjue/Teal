teal_item =
{
    codename = "excalibur",
    name = "Excalibur",
    description = "Legendary Sword",
    level = 1,
    icon = ":/game/items/swords/broadsword_fire",

    components =
    {
        damage_modifier = 
        {
            attack =
            {
                fire = 150
            },

            resistance =
            {
                neutral = 6,
                fire = 18
            }
        },

        equippable =
        {
            bodypart = "Hands",
            side = "Both",
            skill_id = "item_excalibur"
        }
    }
}
