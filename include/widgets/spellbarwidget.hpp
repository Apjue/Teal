// Copyright (C) 2018 Samy Bensaid
// This file is part of the Teal project
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef TEAL_SPELLBARWIDGET_HPP
#define TEAL_SPELLBARWIDGET_HPP

#include <NDK/Prerequisites.hpp>
#include <NDK/BaseWidget.hpp>
#include <NDK/EntityOwner.hpp>
#include <NDK/Entity.hpp>
#include <NDK/Components/NodeComponent.hpp>
#include <NDK/Widgets/ButtonWidget.hpp>
#include <Nazara/Renderer/Texture.hpp>
#include <Nazara/Graphics/Sprite.hpp>
#include <Nazara/Math/Vector2.hpp>
#include <Nazara/Math/Rect.hpp>
#include <Nazara/Core/Signal.hpp>
#include "util/chrono.hpp"

/*

spellbar:
hover case (vide ou pas): focus semi transparent
clic case (vide ou pas): focus full opacity (enter: action)
hover case pleine: infos + focus semi
double clic case pleine: action

action = équip/use/etc



*/

class SpellBarWidget : public Ndk::BaseWidget // todo: ce widget devra gérer les items hein | flèches à droite/gauche sinon, comme dans dofus
{
public:
    inline SpellBarWidget(Ndk::BaseWidget* parent);
    SpellBarWidget(const SpellBarWidget&) = delete;
    SpellBarWidget(SpellBarWidget&&) = default;
    ~SpellBarWidget() = default;


    inline void setBorderSize(Nz::Vector2ui borderSize);
    inline void setPadding(Nz::Vector2ui padding); // Padding between boxes
    inline void setBoxSize(Nz::Vector2ui boxSize); // starts at 0
    inline void setBoxNumber(Nz::Vector2ui boxNumber);

    inline void setDoubleClickInterval(Miliseconds interval);
    inline Miliseconds getDoubleClickInterval() const;

    inline void setBarTexture(const Nz::TextureRef& texture, bool resizeSprite = true);
    inline void setBarSize(Nz::Vector2f size);

    inline const Nz::TextureRef& getBarTexture() const;
    inline Nz::Vector2f getBarSize() const;


    inline Ndk::ButtonWidget* getUpArrow();
    inline Ndk::ButtonWidget* getDownArrow();

    inline Nz::Rectui getBoxAABB(Nz::Vector2ui boxNumber) const;
    inline Nz::Vector2ui getBoxIndex(Nz::Vector2i mouseCoords) const;


    void ResizeToContent() override;

    NazaraSignal(onItemUsed, Ndk::EntityHandle /*item*/);

private:
    void Layout() override;
    void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
    void OnMouseButtonRelease(int x, int y, Nz::Mouse::Button button) override;
    inline void OnMouseEnter() override;
    inline void OnMouseExit() override;


    // Settings
    Nz::Vector2ui m_borderSize;
    Nz::Vector2ui m_padding;
    Nz::Vector2ui m_boxSize;
    Nz::Vector2ui m_boxNumber; // starts at 1 (0 = no box)

    Chrono m_lastClick; // for double clicks
    Miliseconds m_doubleClickMaxInterval { 1000 };

    static const Nz::Vector2ui s_invalidBox;
    Nz::Vector2ui m_selectedBox { s_invalidBox }; // Coordinates of the selected box (if equal to invalid, nothing selected)
    unsigned m_currentPage {};

    // Graphics
    Ndk::EntityOwner m_spellBar;
    Nz::SpriteRef m_spellBarSprite;

    Ndk::EntityOwner m_spellBarFocus;
    Nz::SpriteRef m_spellBarFocusSprite;

    Ndk::EntityOwner m_spellBarSemiFocus;
    Nz::SpriteRef m_spellBarSemiFocusSprite;

    bool m_drawSemiFocusSprite { false };

    // Arrows buttons
    Ndk::ButtonWidget* m_upArrow {};
    Ndk::ButtonWidget* m_downArrow {}; // render order ?

    static const float s_buttonsPadding;
};

#include "spellbarwidget.inl"

#endif // TEAL_SPELLBARWIDGET_HPP
