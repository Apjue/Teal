// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems.hpp"

void Systems::PosRefreshSystem::update(Miliseconds)
{
    auto entities = getEntities();
    for (auto& e: entities)
    {
        auto& pos = e.getComponent<Components::Position>();
        auto& gfxcomp = e.getComponent<Components::GraphicsItem>();

        auto gfx = gfxcomp.item();
        auto defPos = gfxcomp.defaultPos();

        auto const x = pos.x; //logic pos
        auto const y = pos.y;
        auto const inX = pos.inX;
        auto const inY = pos.inY;


        auto const gX = x*Def::TILEGXSIZE; //graphics pos
        auto const gY = y*Def::TILEGYSIZE;
        auto const gInX = inX*Def::MAXGXPOSINTILE;
        auto const gInY = inY*Def::MAXGYPOSINTILE;

        int const finalX = gX + gInX; //We will move using this
        int const finalY = gY + gInY; //(so it's graphics pos)


        if (finalX + defPos.first != gfx->pos().x()   //if the entity is already at that position
         || finalY + defPos.second != gfx->pos().y()) //no need to move it
        {
            gfxcomp.setDefaultPos(); //logic pos of the graphics item: 0,0
            gfx->moveBy(finalX, finalY);
        }
    }
}


void Systems::InputSystem::notify(const Event& e)
{
    switch (e.type())
    {
    case Event::ClickEvent:
        if (m_onClickMove && m_onClickPos)
            handleEvent(e.clickEvent);
        break;

    default:
        break;
    }
}

AbsTile Systems::InputSystem::getTileFromClick(const MouseClickEvent& e) const
{
    unsigned x { e.x }, y { e.y }; //We don't need e anymore now

    unsigned rectX { (x / Def::TILEXSIZE) * 2}; // Rectangle where we clicked
    unsigned rectY { (y / Def::TILEYSIZE) * 2}; // We need losange

    int iRectX { static_cast<int>(rectX) };
    int iRectY { static_cast<int>(rectY) };

    unsigned rectClickX { x % Def::TILEXSIZE }; //We need the click to see
    unsigned rectClickY { y % Def::TILEYSIZE }; //where in the rectangle we clicked

    QColor color = m_scheme.pixel(rectClickX, rectClickY);

    int losangeX{};
    int losangeY{};

    //Blue == up left
    if (color == Qt::blue)
    {
        losangeX = iRectX - 1;
        losangeY = iRectY - 1;
    }

    //Red == up right
    if (color == Qt::red)
    {
        losangeX = iRectX + 1;
        losangeY = iRectY - 1;
    }

    //Yellow == down left
    if (color == Qt::yellow)
    {
        losangeX = iRectX - 1;
        losangeY = iRectY + 1;
    }

    //Green == down right
    if (color == Qt::green)
    {
        losangeX = iRectX + 1;
        losangeY = iRectY + 1;
    }

    //White == The tile.
    if (color == Qt::white)
    {
        losangeX = iRectX;
        losangeY = iRectY;
    }

    //If the tile is negative:
    losangeX = (losangeX < 0) ? 0 : losangeX;
    losangeY = (losangeY < 0) ? 0 : losangeY;

    unsigned fLosangeX { static_cast<unsigned>(losangeX) };
    unsigned fLosangeY { static_cast<unsigned>(losangeY) };

    //If tile is out the map:
    fLosangeX = (fLosangeX > Def::MAPXMAP) ? Def::MAPXMAP : fLosangeX; //BUG ?
    fLosangeY = (fLosangeY > Def::MAPYMAP) ? Def::MAPYMAP : fLosangeY;

    return std::make_pair(fLosangeX, fLosangeY);

    //TODO: Add the z axis ?
}

void Systems::InputSystem::handleEvent(const MouseClickEvent& e)
{
    assert(m_onClickMove && m_onClickPos && "MoveTo or Position nullptr !");
    AbsTile tile = getTileFromClick(e);

    int x{ static_cast<int>(m_onClickPos->x) - static_cast<int>(tile.first) },
        y{ static_cast<int>(m_onClickPos->y) - static_cast<int>(tile.second)}; //Difference, but reversed

    m_onClickMove->diffX = -x;
    m_onClickMove->diffY = -y;
}


void Systems::AISystem::update(Miliseconds) //TODO: Change Direction.
{
    assert(m_pather && "Pather is null !");
    auto entities = getEntities();

    for (auto& e: entities)
    {
        auto& path = e.getComponent<Components::Path>().path;
        auto& pos = e.getComponent<Components::Position>();
        auto& move = e.getComponent<Components::MoveTo>();

        if (move.diffX == 0 && move.diffY == 0)
            continue; //This entity doesn't want to move.

        //Ok, let's do the path.

        //First, make sure to erase the previous path (if any)
        while (!path.empty())
            path.pop();

        //Now, compute the path with the position and the move component.
        std::vector<void*> voidPath;
        float totalCost{}; //We don't need this...

        int endX{ static_cast<int>(pos.x) + move.diffX },
            endY{ static_cast<int>(pos.y) + move.diffY };

        m_pather->Solve(Components::Map::XYToNode(pos.x, pos.y),
                       Components::Map::XYToNode(endX, endY),
                       &voidPath, &totalCost); //returns the absolute position, not difference.

        //Path done... in void*... let's add it to the entity's path. Not in void*.

        int oldX{};
        int oldY{};

        for (unsigned i{} ; i < voidPath.size() ; ++i)
        {
            if (i == 0) //First tile is actually the position (micropather's fault...)
                continue;

            auto&& node = voidPath[i];

            unsigned absX{}, absY{}; //Absolute position, not difference.
            Components::Map::NodeToXY(node, absX, absY);

            int startX { static_cast<int>(pos.x) };
            int startY { static_cast<int>(pos.y) };

            if (i > 1) // If i == 1 we use the initial position
            {          // Else we use the position micropather generated before
                startX = oldX;
                startY = oldY;
            }

            int x{ startX - static_cast<int>(absX) },
                y{ startY - static_cast<int>(absY) }; //Difference now, but reversed

            x = -x; //Ok
            y = -y;

            Direction::Dir dir = Direction::Up; //Must put a default value

            if (x > 0)
                dir |= Direction::Right;
            else if (x < 0)
                dir |= Direction::Left;

            if (y > 0)
                dir |= Direction::Down;
            if (y >= 0)
                dir &= ~Direction::Up;

            bool reexec = false; // [WORKAROUND]
            if (dir == Direction::Right
             || dir == Direction::Down
             || dir == Direction::Left
             || dir == Direction::Up)
                reexec = true;

            path.push(std::make_pair(dir, reexec)); // [WORKAROUND]

            oldX = static_cast<int>(absX);
            oldY = static_cast<int>(absY);
        }

        //All done. Now, erase the move and the inter-x if any.
        move.diffX = 0;
        move.diffY = 0;

        pos.inX = 0;
        pos.inY = 0;
    }
}


void Systems::MovementSystem::update(Miliseconds elapsed)
{
    if (!updateFps(elapsed))
        return;

    auto entities = getEntities();

    for (auto& e: entities)
    {
        auto& path = e.getComponent<Components::Path>().path;
        auto& pos = e.getComponent<Components::Position>();

        if (path.empty())
            continue; // No path, no move.

        auto& dir = path.front();
        auto xy = DirToXY(dir.first); // [WORKAROUND]

        bool walkMode = (path.size() == 1); //We finished our path, let's stop running.

        int moveX { xy.first };
        int moveY { xy.second };

        if (walkMode)
        {
            moveX = (moveX == 2 || moveX == -2) ? moveX/2 : moveX;
            moveY = (moveY == 2 || moveY == -2) ? moveY/2 : moveY;
        }

        pos.inX += moveX;
        pos.inY += moveY;
		
        if (pos.inX % Def::MAXPOSINTILE == 0) // Next tile reached.
        {
            if (pos.inX > 0)
                pos.x += (pos.inX / Def::MAXPOSINTILE);
            else
                pos.x -= (-pos.inX / Def::MAXPOSINTILE);
            pos.inX = 0;
        }

        if (pos.inY % Def::MAXPOSINTILE == 0)
        {
            if (pos.inY > 0)
                pos.y += (pos.inY / Def::MAXPOSINTILE);
            else
                pos.y -= (-pos.inY / Def::MAXPOSINTILE);
            pos.inY = 0;
        }

        if (pos.inX == 0 && pos.inY == 0) //dir.second == reexec
        {
            if (!dir.second)
                path.pop(); //To have next tile
            else
                dir.second = false; // [WORKAROUND]
        }
    }
}

bool Systems::MovementSystem::updateFps(Miliseconds fpsToAdd)
{
    m_fpsCount += fpsToAdd;
    if (m_fpsCount >= Def::MAXFPS)
    {
        m_fpsCount = 0;
        return true;
    }
    return false;
}


void Systems::MapRenderSystem::update()
{
    assert(m_mapTab && "Map Tab may not be nullptr");

    QPixmap newMap(Def::MAPXSIZE, Def::MAPYSIZE);
    newMap.fill();
    QPainter painter(&newMap);

    renderMap(*m_mapTab, painter);
    m_map->setPixmap(newMap);
}

void Systems::MapRenderSystem::renderMap(const Components::Map& comp, QPainter& painter)
//TODO: Render in Fight Mode + map border
{
    const int tileW { Def::TILEXSIZE }; //pixels
    const int tileH { Def::TILEYSIZE };

    for (unsigned x{}; x < Def::MAPX; ++x)
        for (unsigned y{}; y < Def::MAPY; ++y) //on dessine avec le painter
        {
            unsigned const tile { y * Def::MAPX + x }; // Absolute tile
            unsigned const tileNumber { comp.map[tile] }; // Used for texture
            // == texture selon le nombre

            int xDraw{};
            int yDraw{};

            if(x % 2 == 0)
            {
                xDraw = (x == 0) ? x*tileW : (x/2)*tileW;
                yDraw = y*tileH;
            }
            else
            {
                xDraw = x*tileW-x*tileH;
                yDraw = y*tileH+16;
            }

            painter.drawPixmap(xDraw, yDraw, tileW, tileH,
                               QPixmap(":/game/maptileset"), tileNumber * tileW, 0, tileW, tileH);
        }
}


void Systems::GraphicsRenderSystem::update(Miliseconds)
//TOMAYBEDO: Include Pos Component and render with it
{
    auto& entities = getEntities();
    auto&& list = getScene().items();

    for (auto&& e: entities)
    {
        auto& item = e.getComponent<Components::GraphicsItem>();
        auto gfx = item.item();

        //Verifying if the GraphicsItem is activated, and if it's in the scene.
        //At the end of the function, the GraphicsItem must be in the scene, visible or not.

        if (item.isVisible())
        {
            if (list.contains(gfx))
            {
                if (!gfx->isVisible())
                    gfx->show();
            }
            else
                getScene().addItem(gfx);
        }
        else
            if (list.contains(gfx))
            {
                if (gfx->isVisible())
                    gfx->hide();
            }
            else
            {
                getScene().addItem(gfx);
                gfx->hide(); //do not forget to activate !
            }
    }
}
