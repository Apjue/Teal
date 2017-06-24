concrete = 0
grass = concrete + 1
sand = grass + 1
water = sand + 1

teal_tilesetcore =
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

walk = 0 -- 0 must be walkable/non-obstacle
empty = walk + 1
-- block = empty + 1
red =  empty + 1
blue = red + 1

teal_fighttilesetcore =
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