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
obs = walk + 1
-- block = obs + 1
red =  obs + 1
blue = red + 1

teal_fighttilesetcore =
{
	{
		name = "walk",
		index = walk
	},

	{
		name = "obs",
		index = obs
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