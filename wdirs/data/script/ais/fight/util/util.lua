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
    Teal:MoveCharacter(x, y)
    coroutine.yield()
end

function CurrentCharacter:TakeCover()
    Teal:TakeCover()
end

function CurrentCharacter:Attack(characterIndex, skillCodename)
    Teal:AttackCharacter(characterIndex, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacter:MoveAndAttack(characterIndex, skillCodename)
    Teal:MoveAndAttackCharacter(characterIndex or self:ChooseTarget().index, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacter:ChooseTarget()
    local index = Teal:ChooseTarget()
    return teal_fight_data.characters[index]
end

function CurrentCharacter:ChooseAttack(characterIndex)
    local index = Teal:ChooseAttack(characterIndex)
    return teal_fight_data.character.skills[index]
end

function CurrentCharacter:CanAttack(characterIndex)
    return Teal:CanAttack(characterIndex)
end

function CurrentCharacter:CanAttackWith(characterIndex, skillIndex)
    return Teal:CanAttackWith(characterIndex, skillIndex)
end
