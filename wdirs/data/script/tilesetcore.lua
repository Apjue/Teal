local concrete = 0
local grass = concrete + 1
local sand = grass + 1
local water = sand + 1

teal_tileset_core = -- Indexes for the normal map tileset, modify this if the tileset changes
{
    {
        name = "cncrt",
        index = concrete
    },

    {
        name = "grass",
        index = grass
    },

    {
        name = "sandy",
        index = sand
    },

    {
        name = "water",
        index = water
    },
}

teal_fight_tileset_core = -- Indexes for the fight tileset, internally used by the MapComponent
{                         -- This must not be modified
    walk = 0,
    empty = 1,
    red = 2,
    blue = 3
}