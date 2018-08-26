// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/DebugComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <limits>
#include "def/layerdef.hpp"
#include "util/assert.hpp"
#include "spellbarwidget.hpp"

SpellBarWidget::SpellBarWidget(Ndk::BaseWidget* parent)
    : Ndk::BaseWidget(parent), m_spellBar(CreateEntity(true)), m_spellBarSprite(Nz::Sprite::New(Nz::Material::New("Basic2D"))), m_spellBarFocus(CreateEntity(true)),
    m_spellBarFocusSprite(Nz::Sprite::New()), m_spellBarSemiFocus(CreateEntity(true)), m_spellBarSemiFocusSprite(Nz::Sprite::New()), m_upArrow(Add<Ndk::ButtonWidget>()),
    m_downArrow(Add<Ndk::ButtonWidget>())
{
    m_spellBar->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_spellBar->AddComponent<Ndk::GraphicsComponent>().Attach(m_spellBarSprite, Def::ButtonsLayer + 1);

    m_spellBarFocus->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_spellBarFocus->AddComponent<Ndk::GraphicsComponent>().Attach(m_spellBarFocusSprite, Def::ButtonsLayer + 1);

    m_spellBarSemiFocus->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_spellBarSemiFocus->AddComponent<Ndk::GraphicsComponent>().Attach(m_spellBarSemiFocusSprite, Def::ButtonsLayer + 1);
}


void SpellBarWidget::setBorderSize(Nz::Vector2ui borderSize)
{
    m_borderSize = borderSize;
    ResizeToContent();
}

void SpellBarWidget::setPadding(Nz::Vector2ui padding)
{
    m_padding = padding;
    ResizeToContent();
}

void SpellBarWidget::setBoxSize(Nz::Vector2ui boxSize)
{
    m_boxSize = boxSize;
    ResizeToContent();
}

void SpellBarWidget::setBoxNumber(Nz::Vector2ui boxNumber)
{
    m_boxNumber = boxNumber;
    ResizeToContent();
}

void SpellBarWidget::setDoubleClickInterval(Miliseconds interval)
{
    m_doubleClickMaxInterval = interval;
}

Miliseconds SpellBarWidget::getDoubleClickInterval() const
{
    return m_doubleClickMaxInterval;
}


void SpellBarWidget::setBarTexture(const Nz::TextureRef& texture, bool resizeSprite)
{
    m_spellBarSprite->SetTexture(texture, resizeSprite);
}

void SpellBarWidget::setBarSize(Nz::Vector2f size)
{
    m_spellBarSprite->SetSize(size);
}

const Nz::TextureRef& SpellBarWidget::getBarTexture() const
{
    return m_spellBarSprite->GetMaterial()->GetDiffuseMap();
}

Nz::Vector2f SpellBarWidget::getBarSize() const
{
    return m_spellBarSprite->GetSize();
}


Ndk::ButtonWidget* SpellBarWidget::getUpArrow()
{
    return m_upArrow;
}

Ndk::ButtonWidget* SpellBarWidget::getDownArrow()
{
    return m_downArrow;
}


Nz::Rectui SpellBarWidget::getBoxAABB(Nz::Vector2ui boxNumber) const
{
    TealAssert(boxNumber <= m_boxNumber, "Box number out of range");
    return { boxNumber * m_boxSize + boxNumber * m_padding, m_boxSize + m_borderSize * 2u };
}

Nz::Vector2ui SpellBarWidget::getBoxIndex(Nz::Vector2i mouseCoords) const
{
    for (unsigned x {}; x < m_boxNumber.x; ++x)
        for (unsigned y {}; y < m_boxNumber.y; ++y)
            if (getBoxAABB({ x, y }).Contains(Nz::Vector2ui { mouseCoords }))
                return { x, y };

    return s_invalidBox;
}

void SpellBarWidget::OnMouseEnter()
{
    m_drawSemiFocusSprite = true;
}

void SpellBarWidget::OnMouseExit()
{
    m_drawSemiFocusSprite = false;
    m_spellBarSemiFocus->Enable(false);
}
