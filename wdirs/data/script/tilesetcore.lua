local concrete = 0
local grass = concrete + 1
local sand = grass + 1
local water = sand + 1

teal_tileset_core =
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

local walk = 0 -- 0 must be walkable/non-obstacle
local empty = walk + 1
-- local block = empty + 1
local red =  empty + 1
local blue = red + 1

teal_fight_tileset_core =
{
	{
		name = "walk",
		index = walk
	},

	{
		name = "empty",
		index = empty
	},

	{
		name = "red",
		index = red
	},

	{
		name = "blue",
		index = blue
	},
}