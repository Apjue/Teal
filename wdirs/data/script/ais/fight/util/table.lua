-- Don't include this file. It is here just to provide an example of what Teal will do

require "util.lua"

teal_fight_data =
{
	character = -- Character the AI need to compute something for
	{
        index = 0, -- Timeline index

        x = 5,
        y = 3,

        level = 1,
        mp = 3, -- Movement Points
        ap = 6, -- Action Points

        hp = 100,
        maxhp = 100,

        attackModifier = -- can be nil
        {
            { "neutral", 150 },
            { "fire", -75 },
            { "air", 0 },
            { "water", 999 },
            { "earth", 0 }
        },

        resistanceModifier = -- can be nil too
        {
            { "neutral", 10 },
            { "fire", -5 },
            { "air", 0 },
            { "water", 0 },
            { "earth", 0 }
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

                minRange = 1,
                maxRange = 5,
                modifiableRange = true,

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

    characters = -- Does not contain character table
    {
        {
            ally = false,
            -- Same properties as character table
        },

        {
            -- ...
        }
    },

    objects = -- Traps, unanimated objects, ...
    { -- NOTE: Do not use this table, it will always be empty. It is not implemented
        {
            ally = false,
            owner = 1 -- Character index
        },

        {
            -- ...
        }
    }
}

setmetatable(teal_fight_data.character, Character)
