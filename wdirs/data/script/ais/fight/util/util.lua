-- Some utility functions

require "luautil.lua"

function HasCharacterPlayed(character)
    return teal_fight_data.character.index > character.index
end

Character = Class()

function Character:MoveToward(character)
    self:MoveTo(character.x, character.y)
end

function Character:MoveTo(x, y)
    Teal_MoveCharacter(x, y)
end

function Character:TakeCover()
    Teal_TakeCover()
end

function Character:Attack(characterIndex, skillCodename)
    Teal_AttackCharacter(characterIndex, skillCodename or Teal_ChooseAttack(characterIndex))
end

function Character:ChooseTarget()
    local index = Teal_ChooseTarget()
    return teal_fight_data.characters[index]
end

function Character:AttacksThatCanBeUsed(characterIndex)
    Teal_AttacksThatCanBeUsed(characterIndex)
end
