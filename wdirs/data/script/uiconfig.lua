teal_ui_config =
{
    buttons =
    {
        inventory =
        {
            texture = ":/buttons/inv",
            hover_texture = ":/buttons/inv",
            press_texture = ":/buttons/inv_pressed",

            pos =  { x = 0.0, y = 0.0 },   -- Can be deleted
            size = { x = 60.0, y = 60.0 }, -- Can be deleted too

            colors =
            {
                default =
                {
                    { r = 255, g = 255, b = 255 }, -- color
                    { r = 255, g = 255, b = 255 }  -- corner color
                },

                hover_color =
                {
                    { r = 255, g = 255, b = 255 },
                    { r = 255, g = 255, b = 255 }
                },

                press_color =
                {
                    { r = 255, g = 255, b = 255 },
                    { r = 255, g = 255, b = 255 }
                },
            }
        },

        spell_bar =
        {
            pos = { x = 256.0, y = 0.0 },   -- Relative to the buttons bar
            texture = ":/buttons/spellbar",
            size = { x = 245.0, y = 60.0 }, -- Can be deleted to use texture's size

            border_size = { x = 2, y = 2 }, -- Size of box border
            padding = { x = 3, y = 2 },     -- Padding between boxes
            box_size = { x = 37, y = 37 },
            box_number = { x = 8, y = 2 },

            double_click_interval = 1000, -- miliseconds

            arrows =
            {
                down =
                {
                    texture = ":/buttons/spellbar/down_arrow",
                    --hover_texture = ":/buttons/spellbar/down_arrow",
                    --press_texture = ":/buttons/spellbar/down_arrow",
                    --size = { x = 20.0, y = 10.0 }
                },

                up =
                {
                    texture = ":/buttons/spellbar/up_arrow",
                    --hover_texture = ":/buttons/spellbar/up_arrow",
                    --press_texture = ":/buttons/spellbar/up_arrow",
                    --size = { x = 20.0, y = 10.0 }
                }
            }
        }
    }
}