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
            pos = { x = 261.0, y = 0.0 },   -- Relative to the buttons bar
            texture = ":/buttons/spellbar",
            --size = { x = 200.0, y = 66.0 }, -- Can be deleted to use texture's size
            
            focus_texture = ":/buttons/spellbar/focus",
            focus_color = { r = 255, g = 140, b = 0, a = 255 },
            semifocus_color = { r = 255, g = 140, b = 0, a = 144 },

            border_size = { x = 2, y = 2 }, -- Size of box border
            padding =     { x = 1, y = 2 }, -- Padding between boxes
            box_size =    { x = 28, y = 28 },
            box_number =  { x = 6, y = 2 },

            page_count = 5,
            page_counter_size = 20,

            double_click_thresold = 500, -- miliseconds

            arrows =
            {
                down =
                {
                    texture = ":/buttons/spellbar/down_arrow",
                    --hover_texture = ":/buttons/spellbar/down_arrow",
                    --press_texture = ":/buttons/spellbar/down_arrow",
                    size = { x = 30.0, y = 17.0 }
                },

                up =
                {
                    texture = ":/buttons/spellbar/up_arrow",
                    --hover_texture = ":/buttons/spellbar/up_arrow",
                    --press_texture = ":/buttons/spellbar/up_arrow",
                    size = { x = 30.0, y = 17.0 }
                }
            }
        }
    }
}