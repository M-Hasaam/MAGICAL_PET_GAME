#ifndef BOT_H
#define BOT_H

#include "Pet.h"
#include <cmath>

#include "Helper.h"
// extern Helper helper;

class Bot
{
    Pet *Player = nullptr;
    Pet *Enemy = nullptr;
    float battleTimer;

public:
    Bot() {}

    void set_Player(Pet *P)
    {
        Player = P;

        if (Enemy)
        {
            Player->pointEnemy(Enemy);
            Enemy->pointEnemy(Player);
        }
    }

    void set_Enemy(Pet *E)
    {
        Enemy = E;

        if (Player)
        {
            Enemy->pointEnemy(Player);
            Player->pointEnemy(Enemy);
        }
    }

    void reset()
    {
        reset_player();
        reset_enemy();
    }

    void reset_enemy()
    {
        if (Player)
        {
            Player->nullifyEnemy();
        }

        if (Enemy)
            Enemy->nullifyEnemy();

        Enemy = nullptr;
    }

    void reset_player()
    {
        if (Enemy)
            Enemy->nullifyEnemy();

        if (Player)
            Player->nullifyEnemy();

        Player = nullptr;
    }

    void Controll_Player(RenderWindow &window, Event &event, float deltaTime)
    {
        if (!Player)
            return;
        // Player->update(deltaTime);
        // Player->draw(window);
        // Player->updateFireballs(deltaTime, Vector2u(window.getSize()));
        Player->Controll_with_Keys(window, event, deltaTime);
        // Player->Controll_Pet_With_keys(window, event, deltaTime);
    }
    void Control_Enemy(RenderWindow &window, Event &E, float deltaTime)
    {
        if (Enemy && Player)
        {
            Vector2f originEnemy = Enemy->getCenter();
            Vector2f originPlayer = Player->getCenter();

            Vector2f dir = originPlayer - originEnemy;
            float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist > 5.f)
                dir /= dist;
            // dir*=100;

            battleTimer -= deltaTime;

            int h_diff = originEnemy.y - originPlayer.y;
            bool alignedY = (h_diff > 0 && abs(h_diff) < 50) || (h_diff <= 0 && abs(h_diff) < 10);

            // Handle face inversion based on horizontal direction
            if (dir.x < 0 && Enemy->isInverted()) // Moving left but facing right
            {
                Enemy->Invert_Left();
                Enemy->setInverted(false);
            }
            else if (dir.x > 0 && !Enemy->isInverted()) // Moving right but facing left
            {
                Enemy->Invert_Right();
                Enemy->setInverted(true);
            }

            if (alignedY)
            {
                Enemy->fire();
            }
            else
            {

                dir = Vector2f(dir.x * 100, dir.y * 100);
                Enemy->moveObject(dir, deltaTime, Vector2f(window.getSize())); ///////
            }
            // Enemy->moveObject(Vector2f(100,100), deltaTime, Vector2f(window.getSize()));

            checkHit();

            // Enemy->update(deltaTime);
            // Enemy->updateFireballs(deltaTime, Vector2u(window.getSize()));

            // Enemy->draw(window);
        }

        if (Enemy)
            helper.drawDotAt(Enemy->getCenter(), Color::Red);
        if (Player)
            helper.drawDotAt(Player->getCenter(), Color::Green);
        // CircleShape C1(4), C2(4);
        // C1.setFillColor(Color::Red);
        // C2.setFillColor(Color::Green);
        // C1.setPosition(originEnemy);
        // C2.setPosition(originPlayer);
        // window.draw(C1);
        // window.draw(C2);
    }

private:
    void checkHit()
    {
        if (!Player || !Enemy)
            return;

        Player->HitOther(Enemy);
        Enemy->HitOther(Player);
    }
};

#endif
