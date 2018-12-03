// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "components/items/itemcomponent.hpp"
#include "widgets/spellbarwidget.hpp"

const Nz::Vector2ui SpellBarWidget::s_invalidBox { std::numeric_limits<unsigned>::max(), std::numeric_limits<unsigned>::max() };
const float SpellBarWidget::s_buttonsPadding { 3.f };

void SpellBarWidget::Layout()
{
    TealAssert(m_spellBarSprite && m_upArrow && m_downArrow, "Null members");
    BaseWidget::Layout();

    resizeGraphicalEntities();
    Nz::Vector2f contentSize = GetSize();


    Nz::Vector2f arrowsPosition = Nz::Vector2f { m_spellBarSprite->GetSize().x + s_buttonsPadding, 0.f };
    m_upArrow->SetPosition(arrowsPosition.x, arrowsPosition.y);

    Nz::Vector2f downArrowPos { arrowsPosition.x, m_spellBarSprite->GetSize().y - m_downArrow->GetSize().y };
    m_downArrow->SetPosition(downArrowPos.x, downArrowPos.y);


    Nz::Boxf textBox = m_pageCounter->GetComponent<Ndk::GraphicsComponent>().GetAABB();
    Nz::Vector2f textPos { arrowsPosition.x + std::max(m_downArrow->GetSize().x, m_upArrow->GetSize().x) / 2.f - textBox.width / 2,
                           m_spellBarSprite->GetSize().y / 2.f - textBox.height / 2 };
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
        graphicalEntity->GetComponent<Ndk::NodeComponent>().SetPosition(Nz::Vector2f (aabb.GetPosition()));
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
    m_spellBarSemiFocus->GetComponent<Ndk::NodeComponent>().SetPosition(Nz::Vector2f(boxAABB.GetPosition()));
    m_spellBarSemiFocus->Enable();
}

void SpellBarWidget::OnMouseButtonRelease(int x, int y, Nz::Mouse::Button button)
{
    Nz::Recti spellBarBox { Nz::Vector2i(m_spellBarSprite->GetSize()) };

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

    m_spellBarFocus->GetComponent<Ndk::NodeComponent>().SetPosition(Nz::Vector2f(boxAABB.GetPosition()));
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

namespace Nz
{

inline unsigned int LuaImplQueryArg(const LuaState& state, int index, SpellBarWidget* spellBar, TypeTag<SpellBarWidget>)
{
    state.CheckType(index, Nz::LuaType_Table);

    spellBar->SetPosition(state.CheckField<Nz::Vector2f>("pos", index));
    spellBar->setBarTexture(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("texture", index)));
    spellBar->setBarSize(state.CheckField<Nz::Vector2f>("size", Nz::Vector2f(Nz::Vector2ui(spellBar->getBarTexture()->GetSize())), index));

    spellBar->setFocusTexture(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("focus_texture", index)));
    spellBar->setFocusColor(state.CheckField<Nz::Color>("focus_color", index));
    spellBar->setSemiFocusColor(state.CheckField<Nz::Color>("semifocus_color", index));

    spellBar->setBorderSize(state.CheckField<Nz::Vector2ui>("border_size", index));
    spellBar->setPadding(state.CheckField<Nz::Vector2ui>("padding", index));
    spellBar->setBoxSize(state.CheckField<Nz::Vector2ui>("box_size", index));
    spellBar->setBoxNumber(state.CheckField<Nz::Vector2ui>("box_number", index));

    spellBar->setPageCount(state.CheckField<unsigned>("page_count", spellBar->getPageCount(), index));
    spellBar->setPageCounterSize(state.CheckField<unsigned>("page_counter_size", spellBar->getPageCounterSize(), index));
    spellBar->setDoubleClickThresold(state.CheckField<Miliseconds>("double_click_thresold", spellBar->getDoubleClickThresold(), index));

    TealException(state.GetField("arrows", index) == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar.arrows isn't a table!");
    {
        TealException(state.GetField("down") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar.arrows.down isn't a table!");
        {
            Ndk::ButtonWidget* downArrow = spellBar->getDownArrow();
            Nz::String texture = state.CheckField<Nz::String>("texture");

            downArrow->SetTexture(Nz::TextureLibrary::Get(texture));
            downArrow->SetHoverTexture(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("hover_texture", texture, -1)));
            downArrow->SetPressTexture(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("press_texture", texture, -1)));

            downArrow->Resize(state.CheckField<Nz::Vector2f>("size", downArrow->GetSize()));
            downArrow->SetColor(Nz::Color::White, Nz::Color::White);
        }

        state.Pop();

        TealException(state.GetField("up") == Nz::LuaType_Table, "Lua: teal_ui_config.buttons.spell_bar.arrows.up isn't a table!");
        {
            Ndk::ButtonWidget* upArrow = spellBar->getUpArrow();
            Nz::String texture = state.CheckField<Nz::String>("texture");

            upArrow->SetTexture(Nz::TextureLibrary::Get(texture));
            upArrow->SetHoverTexture(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("hover_texture", texture, -1)));
            upArrow->SetPressTexture(Nz::TextureLibrary::Get(state.CheckField<Nz::String>("press_texture", texture, -1)));

            upArrow->Resize(state.CheckField<Nz::Vector2f>("size", upArrow->GetSize()));
            upArrow->SetColor(Nz::Color::White, Nz::Color::White);
        }

        state.Pop();
    }

    state.Pop();
    spellBar->updateSize();

    return 1;
}

}
