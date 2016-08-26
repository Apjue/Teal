// Copyright (C) 2016 Samy Bensaid
// This file is part of the Teal game.
// For conditions of distribution and use, see copyright notice in LICENSE

#include "systems.hpp"

//Static things
Ndk::SystemIndex Systems::AISystem::systemIndex;
Ndk::SystemIndex Systems::AnimationSystem::systemIndex;
Ndk::SystemIndex Systems::MovementSystem::systemIndex;
Ndk::SystemIndex Systems::PosRefreshSystem::systemIndex;


void Systems::AnimationSystem::OnUpdate(float elapsed)
{
    for(auto& e: GetEntities())
    {
        auto& anim = e->GetComponent<Components::Animation>();
        auto& moving = e->GetComponent<Components::Position>().moving;
        auto& sprite = e->GetComponent<Ndk::GraphicsComponent>();
        auto& dir = e->GetComponent<Components::CDirection>().dir;
        int const intDir = static_cast<int>(dir);

        Nz::Sprite* gfx = getSpriteFromComponent(sprite);

        if (!gfx)
            continue; //No sprite has been found

        int const startX = intDir * anim.size.x; //Get the x and the y
        int const startY = anim.frame * anim.size.y;

        switch (anim.animationState)
        {
        case Components::Animation::Undefined:
            UndefinedStateAnimation();
            break;

        case Components::Animation::Moving:
            MovingStateAnimation(startX, startY, gfx, anim, moving);
        	break;
        }
    }
}

void Systems::AnimationSystem::UndefinedStateAnimation()
{
    Nz::StringStream errLog;
    errLog << "UndefinedStateAnimation has been called";

    NazaraWarning(errLog.ToString());
}

void Systems::AnimationSystem::MovingStateAnimation(unsigned startX, unsigned startY, Nz::SpriteRef gfx, 
                                                    Components::Animation& anim, bool moving)
{
    if (!moving) //No animation if not moving
    {
        if (anim.frame != 0)
        {
            anim.frame = 0;
            gfx->SetTextureRect({ startX, 0u, anim.size.x, anim.size.y });
        }
        return;
    }

    if (anim.maxframe == 0) //Only change the direction, no animation
    {
        gfx->SetTextureRect({ startX, 0, anim.size.x, anim.size.y });
    }
    else //Animation !
    {
        gfx->SetTextureRect({ startX, startY, anim.size.x, anim.size.y });

        ++anim.frame;
        if (anim.frame > anim.maxframe)
            anim.frame = 0;
    }
}


void Systems::PosRefreshSystem::OnUpdate(float elapsed)
{
    NazaraUnused(elapsed);

    for (auto& e: GetEntities())
    {
        auto& pos = e->GetComponent<Components::Position>();
        auto& gfxcomp = e->GetComponent<Ndk::GraphicsComponent>();
        auto& gfxpos = e->GetComponent<Ndk::NodeComponent>();
        auto& dpos = e->GetComponent<Components::DefaultGraphicsPos>();

        Nz::Sprite* gfx = getSpriteFromComponent(gfxcomp);

        if (!gfx)
            continue;

        Nz::Vector2ui defPos { dpos.x, dpos.y };

        auto const x = pos.x; //logic pos
        auto const y = pos.y;
        auto const inX = pos.inX;
        auto const inY = pos.inY;


        auto const gX = x * Def::TILEGXSIZE; //graphics pos
        auto const gY = y * Def::TILEGYSIZE;
        auto const gInX = inX * Def::MAXGXPOSINTILE;
        auto const gInY = inY * Def::MAXGYPOSINTILE;

        int const finalX = gX + gInX; //We will move using this
        int const finalY = gY + gInY; //(so it's graphics pos)


        if (finalX + defPos.x != gfxpos.GetPosition().x  //if the entity is already at that position
         || finalY + defPos.y != gfxpos.GetPosition().y) //no need to move it
        {
            gfxpos.SetPosition(dpos.x, dpos.y);
            gfxpos.Move(finalX, finalY);
        }
    }
}


void Systems::AISystem::OnUpdate(float elapsed)
{
    assert(m_pather && "Pather is null !");
    NazaraUnused(elapsed);

    for (auto& e: GetEntities())
    {
        auto& path = e->GetComponent<Components::Path>().path;
        auto& pos = e->GetComponent<Components::Position>();
        auto& move = e->GetComponent<Components::MoveTo>();

        if (move.diffX == 0 && move.diffY == 0)
            continue; //This entity doesn't want to move.

        if (pos.moving)
            continue; //It is already moving !

        //Ok, let's do the path.

        //First, make sure to erase the previous path (if any)
        while (!path.empty())
            path.pop();

        //Now, compute the path with the position and the move component.
        std::vector<void*> voidPath;
        float totalCost{}; //In case of debugging

        int endX{ static_cast<int>(pos.x) + move.diffX },
            endY{ static_cast<int>(pos.y) + move.diffY };

        m_pather->Solve(Components::Map::XYToNode(pos.x, pos.y),
                       Components::Map::XYToNode(endX, endY),
                       &voidPath, &totalCost); //returns the absolute position, not difference.

        //Path done... in void*... let's add it to the entity's path. Not in void*.

        int oldX{};
        int oldY{};

        for (std::size_t i{} ; i < voidPath.size() ; ++i)
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

            Direction::Dir dir = XYToDir(DiffTile{x, y});

            bool reexec = false; // [WORKAROUND 1]
            if (dir == Direction::Right
             || dir == Direction::Down
             || dir == Direction::Left
             || dir == Direction::Up)
                reexec = true;

            path.push(std::make_pair(dir, reexec)); // [WORKAROUND 1]

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


void Systems::MovementSystem::OnUpdate(float elapsed)
{
    for (auto& e: GetEntities())
    {
        auto& path = e->GetComponent<Components::Path>().path;
        auto& pos = e->GetComponent<Components::Position>();

        if (path.empty())
            continue; // No path, no move.

        pos.moving = true;

        auto& cdir = e->GetComponent<Components::CDirection>().dir;
        auto& dir = path.front();

        cdir = DirToOrien(dir.first); // [WORKAROUND 1]
        auto xy = DirToXY(dir.first); // [WORKAROUND 1]

        bool walkMode = (path.size() == 1); //We finished our path, let's stop running.

        int moveX { xy.x };
        int moveY { xy.y };

        if (walkMode)
        {
            moveX = (moveX == 2 || moveX == -2) ? moveX / 2 : moveX;
            moveY = (moveY == 2 || moveY == -2) ? moveY / 2 : moveY;
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
                dir.second = false; // [WORKAROUND 1]
        }

        if (path.empty())
            pos.moving = false;
    }
}
