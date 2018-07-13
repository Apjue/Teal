// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "widgets/spellbarwidget.hpp"

const Nz::Vector2ui SpellBarWidget::s_invalidBox { std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max() };

void SpellBarWidget::ResizeToContent()
{
    Nz::Vector2f size;
    size.Set(Nz::Vector2ui { m_boxNumber * m_boxSize + m_boxNumber * m_padding + m_borderSize * 2u });
    size.x += std::max(m_upArrow->GetSize().x, m_downArrow->GetSize().x);

    SetContentSize(size);
    Layout();
}

void SpellBarWidget::Layout()
{
    TealAssert(m_spellBarSprite && m_upArrow && m_downArrow, "Null members");

    BaseWidget::Layout();

    Nz::Vector2f origin = GetContentOrigin();
    Nz::Vector2f contentSize = GetContentSize();
    Nz::Vector2f arrowsPosition = origin + m_spellBarSprite->GetSize();

    m_spellBar->GetComponent<Ndk::NodeComponent>().SetPosition(origin);
    
    m_upArrow->SetPosition(arrowsPosition.x, arrowsPosition.y);
    m_downArrow->SetPosition(arrowsPosition.x, arrowsPosition.y);
}


void SpellBarWidget::OnMouseMoved(int x, int y, int deltaX, int deltaY)
{

}

void SpellBarWidget::OnMouseButtonRelease(int x, int y, Nz::Mouse::Button button)
{

}
