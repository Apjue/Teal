// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <NDK/Components/GraphicsComponent.hpp>
#include <NDK/Components/DebugComponent.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <limits>
#include "components/util/renderablesstoragecomponent.hpp"
#include "def/layerdef.hpp"
#include "util/assert.hpp"
#include "entityfactory.hpp"
#include "spellbarwidget.hpp"

SpellBarWidget::SpellBarWidget(Ndk::BaseWidget* parent)
    : Ndk::BaseWidget(parent), m_spellBar(CreateEntity()), m_spellBarSprite(Nz::Sprite::New(Nz::Material::New("Basic2D"))), m_spellBarFocus(CreateEntity()),
    m_spellBarFocusSprite(Nz::Sprite::New(Nz::Material::New("Translucent2D"))), m_spellBarSemiFocus(CreateEntity()),
    m_spellBarSemiFocusSprite(Nz::Sprite::New(Nz::Material::New("Translucent2D"))), m_pageCounter(CreateEntity()), m_pageCounterSprite(Nz::TextSprite::New()),
    m_upArrow(Add<Ndk::ButtonWidget>()), m_downArrow(Add<Ndk::ButtonWidget>())
{
    m_spellBar->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_spellBar->AddComponent<Ndk::GraphicsComponent>().Attach(m_spellBarSprite, Def::ButtonsLayer + 1);

    m_spellBarFocus->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_spellBarFocus->AddComponent<Ndk::GraphicsComponent>().Attach(m_spellBarFocusSprite, Def::ButtonsLayer + 2);
    m_spellBarFocus->Enable(false);

    m_spellBarSemiFocus->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_spellBarSemiFocus->AddComponent<Ndk::GraphicsComponent>().Attach(m_spellBarSemiFocusSprite, Def::ButtonsLayer + 2);
    m_spellBarSemiFocus->Enable(false);

    m_pageCounter->AddComponent<Ndk::NodeComponent>().SetParent(this);
    m_pageCounter->AddComponent<Ndk::GraphicsComponent>().Attach(m_pageCounterSprite, Def::ButtonsLayer + 1);

    updateSpellBar();
    resizeEntities();
    updateSize();

    m_pageUpSlot.Connect(m_upArrow->OnButtonTrigger, [this] (const Ndk::ButtonWidget*)
    {
        if (m_currentPage > 1)
        {
            --m_currentPage;
            updateSpellBar();
        }
    });

    m_pageDownSlot.Connect(m_downArrow->OnButtonTrigger, [this] (const Ndk::ButtonWidget*)
    {
        if (m_currentPage < m_pageCount)
        {
            ++m_currentPage;
            updateSpellBar();
        }
    });
}


void SpellBarWidget::setBorderSize(Nz::Vector2ui borderSize)
{
    m_borderSize = borderSize;
    updateSize();
}

void SpellBarWidget::setPadding(Nz::Vector2ui padding)
{
    m_padding = padding;
    updateSize();
}

void SpellBarWidget::setBoxSize(Nz::Vector2ui boxSize)
{
    m_boxSize = boxSize;
    updateSize();
}

void SpellBarWidget::setBoxNumber(Nz::Vector2ui boxNumber)
{
    m_boxNumber = boxNumber;
    resizeEntities();
    updateSize();
}


void SpellBarWidget::setPageCount(unsigned count)
{
    m_pageCount = count;
}

unsigned SpellBarWidget::getPageCount() const
{
    return m_pageCount;
}

void SpellBarWidget::setPageCounterSize(unsigned size)
{
    m_pageCounterSize = size;
    updateSpellBar();
}

unsigned SpellBarWidget::getPageCounterSize() const
{
    return m_pageCounterSize;
}

void SpellBarWidget::setDoubleClickThresold(Miliseconds thresold)
{
    m_doubleClickThresold = thresold;
}

Miliseconds SpellBarWidget::getDoubleClickThresold() const
{
    return m_doubleClickThresold;
}


void SpellBarWidget::setBarTexture(const Nz::TextureRef& texture, bool resizeSprite)
{
    m_spellBarSprite->SetTexture(texture, resizeSprite);

    if (resizeSprite)
        Layout();
}

void SpellBarWidget::setBarSize(Nz::Vector2f size)
{
    m_spellBarSprite->SetSize(size);
    Layout();
}

const Nz::TextureRef& SpellBarWidget::getBarTexture() const
{
    return m_spellBarSprite->GetMaterial()->GetDiffuseMap();
}

Nz::Vector2f SpellBarWidget::getBarSize() const
{
    return m_spellBarSprite->GetSize();
}

void SpellBarWidget::updateSize()
{
    Nz::Vector2f size;
    size.Set(Nz::Vector2ui { m_boxNumber * m_boxSize + m_boxNumber * m_padding + m_boxNumber * m_borderSize * 2u });
    size.x += std::max(m_upArrow->GetSize().x, m_downArrow->GetSize().x) + s_buttonsPadding;

    resizeGraphicalEntities();

    SetMinimumSize(size);
    SetPreferredSize(size);
}

void SpellBarWidget::setFocusTexture(const Nz::TextureRef& texture)
{
    m_spellBarFocusSprite->SetTexture(texture, false);
    m_spellBarSemiFocusSprite->SetTexture(texture, false);
}

void SpellBarWidget::setFocusColor(Nz::Color color)
{
    m_spellBarFocusSprite->SetColor(color);
}

void SpellBarWidget::setSemiFocusColor(Nz::Color color)
{
    m_spellBarSemiFocusSprite->SetColor(color);
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

    Nz::Rectui rect {};
    rect.Translate(boxNumber * m_boxSize + boxNumber * m_padding + boxNumber * m_borderSize * 2u);

    Nz::Vector2ui size = m_boxSize + m_borderSize * 2u;
    rect.width = size.x;
    rect.height = size.y;

    return rect;
}

Nz::Vector2ui SpellBarWidget::getBoxIndex(Nz::Vector2i mouseCoords) const
{
    for (unsigned x {}; x < m_boxNumber.x; ++x)
        for (unsigned y {}; y < m_boxNumber.y; ++y)
            if (getBoxAABB({ x, y }).Contains(Nz::Vector2ui(mouseCoords)))
                return { x, y };

    return s_invalidBox;
}


bool SpellBarWidget::addEntity(Ndk::EntityHandle e)
{
    for (unsigned i {}; i < m_entities.size(); ++i)
    {
        Ndk::EntityHandle& entity = m_entities[i];

        if (!entity)
        {
            entity = e;

            auto graphicalEntity = makeGraphicalItem(CreateEntity(), { e, Nz::Vector2f(m_boxSize), LogicEntityIdComponent::InventoryItem });
            graphicalEntity->GetComponent<Ndk::NodeComponent>().SetParent(this);
            m_graphicalEntities.Insert(graphicalEntity);

            updateSpellBar();
            Layout();

            return true;
        }
    }

    return false;
}

void SpellBarWidget::removeEntity(Ndk::EntityHandle e)
{
    for (Ndk::EntityHandle& entity : m_entities)
    {
        if (entity.IsValid() && entity == e)
        {
            for (auto& graphicalEntity : m_graphicalEntities)
                if (graphicalEntity->GetComponent<LogicEntityIdComponent>().logicEntity == e)
                {
                    m_graphicalEntities.Remove(graphicalEntity);
                    DestroyEntity(graphicalEntity);
                }

            entity = nullptr;
        }
    }
}

void SpellBarWidget::emptyCell(Nz::Vector2ui boxIndex, unsigned page)
{
    if (page == std::numeric_limits<unsigned>::max())
        page = m_currentPage;

    m_entities[CellToIndex(boxIndex, page)] = nullptr;
}

bool SpellBarWidget::isCellEmpty(Nz::Vector2ui boxIndex, unsigned page)
{
    if (page == std::numeric_limits<unsigned>::max())
        page = m_currentPage;

    return !m_entities[CellToIndex(boxIndex, page)].IsValid();
}


void SpellBarWidget::OnMouseEnter()
{
    m_drawSemiFocusSprite = true;
    m_spellBarSemiFocus->Enable();
}

void SpellBarWidget::OnMouseExit()
{
    m_drawSemiFocusSprite = false;

    m_spellBarSemiFocus->Enable(false);
    m_spellBarSemiFocus->Enable(false);
}

void SpellBarWidget::updateSpellBar()
{
    m_pageCounterSprite->Update(Nz::SimpleTextDrawer::Draw(Nz::String::Number(m_currentPage), m_pageCounterSize));
    resizeEntities();

    for (auto& graphicalEntity : m_graphicalEntities)
    {
        auto& logicEntity = graphicalEntity->GetComponent<LogicEntityIdComponent>().logicEntity;
        bool found = false;

        for (std::size_t i {}; i < m_entities.size(); ++i)
            if (m_entities[i] == logicEntity)
            {
                graphicalEntity->Enable(m_currentPage == (IndexToCell(i).second + 1));
                found = true;

                break;
            }

        if (!found)
        {
            m_graphicalEntities.Remove(graphicalEntity);
            graphicalEntity->Kill();
        }
    }
}

void SpellBarWidget::resizeEntities()
{
    m_entities.resize(m_boxNumber.x * m_boxNumber.y * m_pageCount);
}

void SpellBarWidget::resizeGraphicalEntities()
{
    for (auto& graphicalEntity : m_graphicalEntities)
        for (auto& sprite : graphicalEntity->GetComponent<RenderablesStorageComponent>().sprites)
            sprite->SetSize(Nz::Vector2f (m_boxSize));
}

unsigned SpellBarWidget::CellToIndex(Nz::Vector2ui cell, unsigned page)
{
    TealAssert(page > 0 && cell < m_boxNumber, "Invalid arguments");
    return (page - 1) * m_boxNumber.x * m_boxNumber.y + cell.y * m_boxNumber.y + cell.x;
}

std::pair<Nz::Vector2ui /* cell */, unsigned /* page */> SpellBarWidget::IndexToCell(unsigned index)
{
    std::pair<Nz::Vector2ui, unsigned> returnValue;
    returnValue.second = index / (m_boxNumber.x * m_boxNumber.y);

    returnValue.first.x = index % m_boxNumber.x;
    returnValue.first.y = (index - returnValue.second * m_boxNumber.x * m_boxNumber.y) / m_boxNumber.x;

    return returnValue;
}
