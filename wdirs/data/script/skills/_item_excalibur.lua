teal_skill =
{
    {
        {
            "damage", -- type of attack
            "enemies", -- target
            "fire", -- element
            100 -- damage
        },

        {
            "state",
            "enemies",
            3, -- number of turns the state lasts
            "poison", -- type of state

            "fire",
            10
        }
    },

    0, -- movement points
    3, -- action points
    "none", -- effect id

    1, -- min range
    1, -- max range
    false, -- modifiable range
    false, -- view through wall, does nothing if max range is 1

    "cross", -- area type
    0, -- area min range
    0, -- area max range

    "item_excalibur", -- codename
    "Excalibur", -- real name
    "Attack of the Excalibur's item", -- description
    "" -- icon
}