-- Default Fight AI

fight_ai_name = "Default AI"
fight_ai_monster_family = "*"
fight_ai_monster_name = "*"

function execute()
    local data = teal_fight_data
    local character = data.character

    character:MoveAndAttack()
    character:TakeCover()
end
