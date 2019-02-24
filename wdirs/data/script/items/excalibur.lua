teal_item =
{
    codename = "excalibur",
    name = "Excalibur",
    description = "Legendary Sword",
    level = 1,
    icon = ":/game/items/swords/broadsword_fire",
    map_offset = { x = 12, y = -3 },

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
            use_both_hands = true,
            skill_id = "item_excalibur"
        }
    }
}
