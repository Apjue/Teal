-- Default Fight AI

fight_ai_type = "default"
fight_ai_name = "default"

function compute()
    local data = teal_fight_data
    local character = data.character

    character:MoveAndAttack(character:ChooseTarget())
    character:TakeCover()
end
