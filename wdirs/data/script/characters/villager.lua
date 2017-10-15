teal_character =
{
    texture = ":/game/char/villager",
    size = { 113, 99 },
    defgfxpos = { -25, -66 },

    animations =
    {
        "villager_walking"
    },

    deflgcpos = { 1, 1 },
    maxhealth = 100,
    random = { false },
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
