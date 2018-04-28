-- Some utility functions

require "luautil.lua"

function HasCharacterPlayed(character)
    return teal_fight_data.character.index > character.index
end

CurrentCharacter = Class()

function CurrentCharacter:MoveToward(character)
    self:MoveTo(character.x, character.y)
end

function CurrentCharacter:MoveTo(x, y)
    Teal_MoveCharacter(x, y)
    coroutine.yield()
end

function CurrentCharacter:TakeCover()
    Teal_TakeCover()
end

function CurrentCharacter:Attack(characterIndex, skillCodename)
    Teal_AttackCharacter(characterIndex, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacter:MoveAndAttack(characterIndex, skillCodename)
    Teal_MoveAndAttackCharacter(characterIndex or self:ChooseTarget().index, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacter:ChooseTarget()
    local index = Teal_ChooseTarget()
    return teal_fight_data.characters[index]
end

function CurrentCharacter:ChooseAttack(characterIndex)
    local index = Teal_ChooseAttack(characterIndex)
    return teal_fight_data.character.skills[index]
end

function CurrentCharacter:CanAttack(characterIndex)
    return Teal_CanAttack(characterIndex)
end

function CurrentCharacter:CanAttackWith(characterIndex, skillIndex)
    return Teal_CanAttackWith(characterIndex, skillIndex)
end
