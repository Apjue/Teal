teal_character =
{
    texture = ":/game/char/villager",
    size = { x = 49, y = 94 },
    offset = { x = 5, y = -66 },

    animations =
    {
     -- Only one animation per type, syntax:
     -- { "type", "animation_codename" }
        { "walk", "villager_walking" },
     -- { "run", "villager_running" }
    },

    maxhealth = 100,
    name = "Villager",
    blocktile = true,

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
