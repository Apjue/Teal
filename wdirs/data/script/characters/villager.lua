teal_character =
{
    texture = ":/game/char/villager",
    size = { x = 113, y = 99 },
    offset = { x = -25, y = -66 },

    animations =
    {
        default = 0,
        "villager_walking"
    },

    maxhealth = 100,
    name = "Villager",

    attack =
    {
        { "neutral", 10 }
    },

    resistance =
    {
        { "neutral", 10 }
    },

    fight =
    {
        autoAttack = false,
        movementPoints = 3,
        actionPoints = 6
    }
}
