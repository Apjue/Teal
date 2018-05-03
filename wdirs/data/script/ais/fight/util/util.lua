-- Some utility functions

require "luautil.lua"

function HasCharacterPlayed(character)
    return teal_fight_data.character.index > character.index
end

CurrentCharacterClass = Class()

function CurrentCharacterClass:MoveToward(character)
    self:MoveTo(character.x, character.y)
end

function CurrentCharacterClass:MoveTo(x, y)
    CharacterBinding:MoveCharacter(x, y)
    coroutine.yield()
end

function CurrentCharacterClass:TakeCover()
    CharacterBinding:TakeCover()
end

function CurrentCharacterClass:Attack(characterIndex, skillCodename)
    CharacterBinding:AttackCharacter(characterIndex, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacterClass:MoveAndAttack(characterIndex, skillCodename)
    CharacterBinding:MoveAndAttackCharacter(characterIndex or self:ChooseTarget().index, skillCodename or self:ChooseAttack(characterIndex).codename)
    coroutine.yield()
end

function CurrentCharacterClass:ChooseTarget()
    local index = CharacterBinding:ChooseTarget()
    return teal_fight_data.characters[index]
end

function CurrentCharacterClass:ChooseAttack(characterIndex)
    local index = CharacterBinding:ChooseAttack(characterIndex)
    return teal_fight_data.character.skills[index]
end

function CurrentCharacterClass:CanCastSpell(characterIndex)
    return CharacterBinding:CanCastSpell(characterIndex)
end

function CurrentCharacterClass:CanUseSkill(characterIndex, skillIndex)
    return CharacterBinding:CanUseSkill(characterIndex, skillIndex)
end

Character = {}
setmetatable(Character, CurrentCharacterClass)
