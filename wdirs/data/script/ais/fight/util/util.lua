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
    CharacterBinding:MoveCharacter(x, y)
    coroutine.yield()
end

function CurrentCharacter:TakeCover()
    CharacterBinding:TakeCover()
end

function CurrentCharacter:Attack(characterIndex, skillCodename)
    CharacterBinding:AttackCharacter(characterIndex, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacter:MoveAndAttack(characterIndex, skillCodename)
    CharacterBinding:MoveAndAttackCharacter(characterIndex or self:ChooseTarget().index, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacter:ChooseTarget()
    local index = CharacterBinding:CharacterBinding()
    return teal_fight_data.characters[index]
end

function CurrentCharacter:ChooseAttack(characterIndex)
    local index = CharacterBinding:ChooseAttack(characterIndex)
    return teal_fight_data.character.skills[index]
end

function CurrentCharacter:CanAttack(characterIndex)
    return CharacterBinding:CanAttack(characterIndex)
end

function CurrentCharacter:CanAttackWith(characterIndex, skillIndex)
    return CharacterBinding:CanAttackWith(characterIndex, skillIndex)
end
