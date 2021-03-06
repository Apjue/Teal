// Copyright (C) 2019 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_SPELLBARWIDGET_HPP
#define TEAL_SPELLBARWIDGET_HPP

#include <NDK/Prerequisites.hpp>
#include <NDK/BaseWidget.hpp>
#include <NDK/EntityOwner.hpp>
#include <NDK/Entity.hpp>
#include <NDK/EntityList.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Widgets/ButtonWidget.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Utility/SimpleTextDrawer.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Graphics/TextSprite.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Core/Signal.hpp>
#include <Nazara/Core/Color.hpp>
#include <vector>
#include <limits>
#include "util/chrono.hpp"

class SpellBarWidget : public Ndk::BaseWidget
{
public:
    inline SpellBarWidget(Ndk::BaseWidget* parent);
    SpellBarWidget(SpellBarWidget&&) = default;
    ~SpellBarWidget() = default;


    // Spell Bar config
    inline void setBorderSize(Nz::Vector2ui borderSize);
    inline void setPadding(Nz::Vector2ui padding); // Padding between boxes
    inline void setBoxSize(Nz::Vector2ui boxSize); // starts at 0
    inline void setBoxNumber(Nz::Vector2ui boxNumber);

    // Config, again
    inline void setPageCount(unsigned count);
    inline unsigned getPageCount() const;
    inline void setPageCounterSize(unsigned size);
    inline unsigned getPageCounterSize() const;

    inline void setDoubleClickThresold(Miliseconds thresold);
    inline Miliseconds getDoubleClickThresold() const;

    // Spell Bar appearance
    inline void setBarTexture(const Nz::TextureRef& texture, bool resizeSprite = true);
    inline void setBarSize(Nz::Vector2f size);

    inline const Nz::TextureRef& getBarTexture() const;
    inline Nz::Vector2f getBarSize() const;

    inline void updateSize();

    // Focus appearance
    inline void setFocusTexture(const Nz::TextureRef& texture);
    inline void setFocusColor(Nz::Color color);
    inline void setSemiFocusColor(Nz::Color color);

    // Arrows
    inline Ndk::ButtonWidget* getUpArrow();
    inline Ndk::ButtonWidget* getDownArrow();

    // Utility functions
    inline Nz::Rectui getBoxAABB(Nz::Vector2ui boxNumber) const;
    inline Nz::Vector2ui getBoxIndex(Nz::Vector2i mouseCoords) const;

    // Items
    inline bool addEntity(Ndk::EntityHandle e);
    inline void removeEntity(Ndk::EntityHandle e);
    inline void emptyCell(Nz::Vector2ui boxIndex, unsigned page = std::numeric_limits<unsigned>::max());
    inline bool isCellEmpty(Nz::Vector2ui boxIndex, unsigned page = std::numeric_limits<unsigned>::max());

    NazaraSignal(onItemUsed, Ndk::EntityHandle /*item*/);
    //NazaraSignal(onSpellSelected, Ndk::EntityHandle /*spell*/); // todo: will spell be entities? or maybe something like SpellData, or an id (using DoubleStores)

private:
    void Layout() override;
    void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
    void OnMouseButtonRelease(int x, int y, Nz::Mouse::Button button) override;
    inline void OnMouseEnter() override;
    inline void OnMouseExit() override;

    // Refresh
    inline void updateSpellBar();
    inline void resizeEntities();
    inline void resizeGraphicalEntities();

    // Utility
    inline unsigned CellToIndex(Nz::Vector2ui cell, unsigned page);
    inline std::pair<Nz::Vector2ui /* cell */, unsigned /* page */> IndexToCell(unsigned index);

    // Settings
    Nz::Vector2ui m_borderSize {};
    Nz::Vector2ui m_padding {};
    Nz::Vector2ui m_boxSize {};
    Nz::Vector2ui m_boxNumber {}; // starts at 1 (0 = no box)

    Chrono m_lastClick; // for double clicks
    Miliseconds m_doubleClickThresold { 1000 };

    static const Nz::Vector2ui s_invalidBox;
    Nz::Vector2ui m_selectedBox { s_invalidBox }; // Coordinates of the selected box (if equal to invalid, nothing selected)

    unsigned m_currentPage { 1 };
    unsigned m_pageCount { 5 };
    unsigned m_pageCounterSize { 20 };

    std::vector<Ndk::EntityHandle> m_entities;
    Ndk::EntityList m_graphicalEntities;

    // Graphics
    Ndk::EntityOwner m_spellBar;
    Nz::SpriteRef m_spellBarSprite;

    Ndk::EntityOwner m_spellBarFocus;
    Nz::SpriteRef m_spellBarFocusSprite;

    Ndk::EntityOwner m_spellBarSemiFocus;
    Nz::SpriteRef m_spellBarSemiFocusSprite;

    Ndk::EntityOwner m_pageCounter;
    Nz::TextSpriteRef m_pageCounterSprite;

    bool m_drawSemiFocusSprite { false };

    // Arrows buttons
    Ndk::ButtonWidget* m_upArrow {};
    Ndk::ButtonWidget* m_downArrow {}; // render order ?

    NazaraSlot(Ndk::ButtonWidget, OnButtonTrigger, m_pageUpSlot);
    NazaraSlot(Ndk::ButtonWidget, OnButtonTrigger, m_pageDownSlot);

    static const float s_buttonsPadding;
};

#include <Nazara/Lua/LuaState.hpp>
#include <NDK/LuaAPI.hpp>

namespace Nz
{

extern unsigned int LuaImplQueryArg(const LuaState& state, int index, SpellBarWidget* spellBar, TypeTag<SpellBarWidget>);

}

#include "spellbarwidget.inl"

#endif // TEAL_SPELLBARWIDGET_HPP
