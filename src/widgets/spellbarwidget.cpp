// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "widgets/spellbarwidget.hpp"

const Nz::Vector2ui SpellBarWidget::s_invalidBox { std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max() };
const float SpellBarWidget::s_buttonsPadding { 3.f };

void SpellBarWidget::ResizeToContent()
{
    Nz::Vector2f size;
    size.Set(Nz::Vector2ui { m_boxNumber * m_boxSize + m_boxNumber * m_padding + m_borderSize * 2u });
    size.x += std::max(m_upArrow->GetSize().x, m_downArrow->GetSize().x) + s_buttonsPadding;

    SetContentSize(size);
}

void SpellBarWidget::Layout()
{
    TealAssert(m_spellBarSprite && m_upArrow && m_downArrow, "Null members");
    BaseWidget::Layout();

    Nz::Vector2f origin = GetContentOrigin();
    Nz::Vector2f contentSize = GetContentSize();

    m_spellBar->GetComponent<Ndk::NodeComponent>().SetPosition(origin);


    Nz::Vector2f arrowsPosition = origin + Nz::Vector2f { m_spellBarSprite->GetSize().x + s_buttonsPadding, 0.f };
    m_upArrow->SetPosition(arrowsPosition.x, arrowsPosition.y);

    Nz::Vector2f downArrowPos { arrowsPosition.x, downArrowPos.y = origin.y + m_spellBarSprite->GetSize().y - m_downArrow->GetSize().y };
    m_downArrow->SetPosition(downArrowPos.x, downArrowPos.y);
}


void SpellBarWidget::OnMouseMoved(int x, int y, int deltaX, int deltaY)
{
    Nz::Vector2ui boxIndex = getBoxIndex({ x + deltaX, y + deltaY });

    if (boxIndex == s_invalidBox || !m_drawSemiFocusSprite)
    {
        m_spellBarSemiFocus->Enable(false);
        return;
    }

    Nz::Rectui boxAABB = getBoxAABB(boxIndex);
    m_spellBarSemiFocus->GetComponent<Ndk::NodeComponent>().SetPosition(float(boxAABB.x), float(boxAABB.y));
    m_spellBarSemiFocus->Enable();
}

void SpellBarWidget::OnMouseButtonRelease(int x, int y, Nz::Mouse::Button button)
{
    Nz::Vector2ui boxIndex = getBoxIndex({ x, y }); // todo: et pour les arrows ?
    m_selectedBox = boxIndex; // + selected changé même si != left

    if (button != Nz::Mouse::Left || boxIndex == s_invalidBox)
    {
        m_spellBarFocus->Enable(false);
        return;
    }

    Nz::Rectui boxAABB = getBoxAABB(boxIndex);
    m_spellBarFocus->GetComponent<Ndk::NodeComponent>().SetPosition(float(boxAABB.x), float(boxAABB.y));
    m_spellBarFocus->Enable();

    if (m_lastClick.getElapsedTime().asMiliseconds() < m_doubleClickMaxInterval)
    {
        m_lastClick.restart();
        //onItemUsed(item);
    }
}
