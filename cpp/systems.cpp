#include "systems.hpp"

void Systems::PosRefreshSystem::update()
{
    auto entities = getEntities();
    for (auto&& e: entities)
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
        auto const gInX = inX*Def::MAXPOSINTILE;
        auto const gInY = inY*Def::MAXPOSINTILE;

        auto const fX = gX+gInX; //We will move using this
        auto const fY = gY+gInY; //(so it's graphics pos)


        if (fX + defPos.first != gfx->pos().x() || fY + defPos.second != gfx->pos().y())
        {
            gfxcomp.setDefaultPos(); //logic pos: 0,0
            gfx->moveBy(fX, fY);
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
    unsigned x {e.x}, y {e.y}; //We don't need e anymore now

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
        y{ static_cast<int>(m_onClickPos->y) - static_cast<int>(tile.second)}; //Difference now, but reversed

    m_onClickMove->diffX = -x;
    m_onClickMove->diffY = -y;
}


void Systems::AISystem::update() //TODO: Change Direction.
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
        for (auto&& node: voidPath)
        {
            unsigned absX{}, absY{}; //Absolute position, not difference.
            Components::Map::NodeToXY(node, absX, absY);

            int x{ static_cast<int>(pos.x) - static_cast<int>(absX) },
                y{ static_cast<int>(pos.y) - static_cast<int>(absY) }; //Difference now, but reversed

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

            path.push(dir);
        }
        path.pop(); //First tile is actually the position (micropather's fault...)

        //All done. Now, erase the move and the inter-x if any.
        move.diffX = 0;
        move.diffY = 0;

        pos.inX = 0;
        pos.inY = 0;
    }
}


void Systems::MovementSystem::update()
{
    auto entities = getEntities();

    for (auto& e: entities)
    {
        auto& path = e.getComponent<Components::Path>().path;
        auto& pos = e.getComponent<Components::Position>();

        if (path.empty())
            continue; // No path, no move.

        auto dir = path.front();
        auto xy = DirToXY(dir); //crash

        int moveX { xy.first };
        int moveY { xy.second };

        pos.inX += moveX;
        pos.inY += moveY;

        if (pos.inX % Def::MAXPOSINTILE == 0) // Next tile reached.
        {
            pos.x += (pos.inX / Def::MAXPOSINTILE);
            pos.inX = 0;
        }

        if (pos.inY % Def::MAXPOSINTILE == 0)
        {
            pos.y += (pos.inY / Def::MAXPOSINTILE);
            pos.inY = 0;
        }

        if (pos.inX == 0 && pos.inY == 0)
            path.pop(); //To have next tile
    }
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
    const int tileW{Def::TILEXSIZE}; //pixels
    const int tileH{Def::TILEYSIZE};

    for (unsigned x{}; x < Def::MAPX; ++x)
        for (unsigned y{}; y < Def::MAPY; ++y) //on dessine avec le painter
        {
            const unsigned i{y*Def::MAPX+x}; //case dans la map
            const unsigned tileNumber{comp.map[i]}; //nombre de la tile
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
                               QPixmap(":/game/maptileset"), tileNumber*tileW, 0, tileW, tileH);
        }
}


void Systems::GraphicsRenderSystem::update()
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
