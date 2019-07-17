local stone = 0
local grass = stone + 1
local grass2 = grass + 1
local grass3 = grass2 + 1

teal_tileset_core = -- Indexes for the normal map tileset, modify this if the tileset changes
{
    {
        name = "stone",
        index = stone
    },

    {
        name = "grass",
        index = grass
    },

    {
        name = "grass2",
        index = grass2
    },

    {
        name = "grass3",
        index = grass3
    },
}

teal_fight_tileset_core = -- Indexes for the fight tileset, internally used by the MapComponent
{                         -- This must not be modified
    walk = 0,
    empty = 1,
    red = 2,
    blue = 3
}