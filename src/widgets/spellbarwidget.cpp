// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/items/itemcomponent.hpp"
#include "widgets/spellbarwidget.hpp"

const Nz::Vector2ui SpellBarWidget::s_invalidBox { std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max() };
const float SpellBarWidget::s_buttonsPadding { 3.f };

void SpellBarWidget::ResizeToContent()
{
    Nz::Vector2f size;
    size.Set(Nz::Vector2ui { m_boxNumber * m_boxSize + m_boxNumber * m_padding + m_boxNumber * m_borderSize * 2u });
    size.x += std::max(m_upArrow->GetSize().x, m_downArrow->GetSize().x) + s_buttonsPadding;

    resizeGraphicalEntities();
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

    Nz::Vector2f downArrowPos { arrowsPosition.x, origin.y + m_spellBarSprite->GetSize().y - m_downArrow->GetSize().y };
    m_downArrow->SetPosition(downArrowPos.x, downArrowPos.y);


    Nz::Boxf textBox = m_pageCounter->GetComponent<Ndk::GraphicsComponent>().GetBoundingVolume().aabb;
    Nz::Vector2f textPos { arrowsPosition.x + std::max(m_downArrow->GetSize().x, m_upArrow->GetSize().x) / 2.f - textBox.width / 2,
                           origin.y + m_spellBarSprite->GetSize().y / 2.f - textBox.height / 2 };
    m_pageCounter->GetComponent<Ndk::NodeComponent>().SetPosition(textPos);


    m_spellBarFocusSprite->SetSize(Nz::Vector2f(m_boxSize + m_borderSize * 2u));
    m_spellBarSemiFocusSprite->SetSize(Nz::Vector2f(m_boxSize + m_borderSize * 2u));


    for (auto& graphicalEntity : m_graphicalEntities)
    {
        auto& logicEntity = graphicalEntity->GetComponent<LogicEntityIdComponent>().logicEntity;
        Nz::Vector2ui box = s_invalidBox;

        for (std::size_t i {}; i < m_entities.size(); ++i)
            if (m_entities[i] == logicEntity)
            {
                box = IndexToCell(i).first;
                break;
            }

        TealAssert(box != s_invalidBox, "Item doesn't exist anymore!");

        Nz::Rectui aabb = getBoxAABB(box);
        graphicalEntity->GetComponent<Ndk::NodeComponent>().SetPosition(origin + Nz::Vector2f (aabb.GetPosition()));
    }
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
    m_spellBarSemiFocus->GetComponent<Ndk::NodeComponent>().SetPosition(GetContentOrigin() + Nz::Vector2f(boxAABB.GetPosition()));
    m_spellBarSemiFocus->Enable();
}

void SpellBarWidget::OnMouseButtonRelease(int x, int y, Nz::Mouse::Button button)
{
    Nz::Recti spellBarBox { Nz::Vector2i (GetContentOrigin()), Nz::Vector2i(m_spellBarSprite->GetSize()) };

    if (!spellBarBox.Contains(x, y))
        return;


    Nz::Vector2ui boxIndex = getBoxIndex({ x, y });

    if (button != Nz::Mouse::Left || boxIndex == s_invalidBox)
    {
        m_spellBarFocus->Enable(false);
        m_selectedBox = s_invalidBox;

        return;
    }


    Nz::Rectui boxAABB = getBoxAABB(boxIndex);

    m_spellBarFocus->GetComponent<Ndk::NodeComponent>().SetPosition(GetContentOrigin() + Nz::Vector2f(boxAABB.GetPosition()));
    m_spellBarFocus->Enable();

    if (m_lastClick.getElapsedTime().asMiliseconds() < m_doubleClickThresold && m_selectedBox == boxIndex)
    {
        Ndk::EntityHandle& entity = m_entities[CellToIndex(m_selectedBox, m_currentPage)];

        if (entity && entity->HasComponent<ItemComponent>())
            onItemUsed(entity);
    }

    else
        m_lastClick.restart();

    m_selectedBox = boxIndex;
}
