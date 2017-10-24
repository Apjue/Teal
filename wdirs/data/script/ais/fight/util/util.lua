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
    coroutine.yield()
end

function Character:TakeCover()
    Teal_TakeCover()
    coroutine.yield()
end

function Character:Attack(characterIndex, skillCodename)
    Teal_AttackCharacter(characterIndex, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function Character:MoveAndAttack(characterIndex, skillCodename)
    Teal_MoveAndAttackCharacter(characterIndex or self:ChooseTarget().index, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function Character:ChooseTarget()
    local index = Teal_ChooseTarget()
    return teal_fight_data.characters[index]
end

function Character:ChooseAttack(characterIndex)
    local index = Teal_ChooseAttack(characterIndex)
    return teal_fight_data.character.skills[index]
end

function Character:CanAttack(characterIndex)
    return Teal_CanAttack(characterIndex)
end
