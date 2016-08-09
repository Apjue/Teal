// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#ifndef GRAPHICSSCENE_HPP_INL
#define GRAPHICSSCENE_HPP_INL

const anax::World& GraphicsScene::getWorld() const noexcept
{
    return m_world;
}
anax::Entity& GraphicsScene::getPerso() noexcept
{
    return m_charac;
}
anax::Entity& GraphicsScene::getMap() noexcept
{
    return m_map;
}
void GraphicsScene::updateMap()
{
    m_maprenderSys.update();
}

void GraphicsScene::writeFpsToOutput()
{
    if (m_outputFps)
        m_outputFps->setText(QString::number(m_fps));
}

#endif // GRAPHICSSCENE_HPP_INL
