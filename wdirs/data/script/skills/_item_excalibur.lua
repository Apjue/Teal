teal_skill =
{
    codename = "item_excalibur",
    display_name = "Excalibur",
    description = "Attack of the Excalibur's item",
    icon = "",

    attacks =
    {
        {
            type = "damage",
            target = "enemies",

            element = "fire",
            damage = 100
        },

        {
            type = "state",
            target = "enemies",
            state_type = "poison", -- type of state, also used for StateMetaDataStore

            turns = 3, -- number of turns the state lasts
            element = "fire",
            damage = 10
        }
    },

    movement_points = 0,
    action_points = 3,

    min_range = 1,
    max_range = 1,
    modifiable_range = false,
    view_through_walls = false, -- spoilers: does nothing if max range is 1

    area_type = "cross",
    area_min_range = 0,
    area_max_range = 0
}