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

            data =
            {
                metadata_id = "excalibur_hit", -- Used by StateMetaDataStore, for icon & description
                turns = 3, -- number of turns the state lasts

                effects =
                {
                    {
                        state_type = "poisonned",

                        element = "fire",
                        damage = 10
                    },

                    {
                        state_type = "weakness",

                        resistance =
                        {
                            neutral = 24,
                            air = 7,
                            fire = 28
                        }
                    }
                }
            }
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