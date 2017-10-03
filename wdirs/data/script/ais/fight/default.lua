-- Default Fight AI

fight_ai_type = "default"
fight_ai_monstername = "*"
fight_ai_name = "Default AI"

function execute()
    local data = teal_fight_data
    local character = data.character

    character:MoveAndAttack()
    character:TakeCover()
end
