-- Don't include this file. It is here just to provide an example of what Teal will do

require "util.lua"

teal_fight_data =
{
	map =
	{
		{
			obstacle = 0,
			spawn = "none"
		},

		{
			obstacle = 1,
			spawn = "none"
		},

		-- ...
	},

	character = -- Character the AI need to compute something for
	{
        index = 0, -- Timeline index

        x = 5,
        y = 3,

        level = 1,
        mp = 3, -- Movement Points
        ap = 6, -- Action Points

        attackmodifier =
        {
            { "neutral", 10 },
            { "fire", -5 },
            { "air", 0 }
        },

        resistancemodifier =
        {
            { "neutral", 10 },
            { "fire", -5 },
            { "air", 0 }
        },

        skills =
        {
            {
                attacks =
                {
                    {
                        type = "damage",
                        target = "enemies", -- possible values: enemies, allies, both

                        element = "earth",
                        damage = 100
                    },

                    {
                        type = "state",
                        target = "enemies",

                        state = "poisonned",
                        element = "fire",
                        damage = 100
                    },

                    {
                        type = "effect",
                        target = "enemies",

                        effect = "pull",
                        tiles = 5
                    },
                },

                mp = 0, -- Movement Points required
                ap = 3, -- Action Points Required

                minrange = 1,
                maxrange = 5,
                modifiablerange = true,

                areatype = "cross", -- possible values: cross, plus, circle, alignedcenterrl, alignedcenterdu, alignedright, alignedleft, aligneddown, alignedup
                areaMinRange = 0,
                areaMaxRange = 0,

                codename = "some_skill"
            },

            {
                -- Some other skill
            }
        }
	},

    characters =
    {
        {
            type = "enemy" -- or ally
            -- Same properties as character table
        },

        {
            -- ...
        }
    }
}

setmetatable(teal_fight_data.character, Character)
