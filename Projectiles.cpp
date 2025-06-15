#include "Projectiles.h"
#include "Pet.h"
#include "Object.h"

ObjectProjectileManager::ObjectProjectileManager(Vector2u size) : winSize(size), maxFireballs(200), activeCount(0)
{
    if (!ObjectTextures[0].loadFromFile("Images/fireball1.png"))
    {
        cout << "Failed to load fireball texture!" << endl;
        // exit(0);
    }
    if (!ObjectTextures[1].loadFromFile("Images/rock1.png"))
    {
        cout << "Failed to load fireball texture!" << endl;
        // exit(0);
    }
    if (!ObjectTextures[2].loadFromFile("Images/shoot2.png"))
    {
        cout << "Failed to load fireball texture!" << endl;
        // exit(0);
    }

    CollisionTex[0].loadFromFile("Images/collision1.png");
    CollisionTex[1].loadFromFile("Images/collision2.png");

    if(!SB.loadFromFile("Music/rock2.ogg")){
        cout<<" Rock Sound not loaded\n";
    }

    for (int i = 0; i < 30; i++){

        sounds[i].setBuffer(SB);
        sounds[i].setVolume(50.0f);
        sounds[i].setPitch(4);
        sounds[i].setPlayingOffset(seconds(2.f));
    }
    
    for (int i = 0; i < maxFireballs; ++i)
        Objects[i] = nullptr;
}

ObjectProjectileManager::~ObjectProjectileManager()
{
    for (int i = 0; i < 30; i++)
    {
        sounds[i].stop();
        sounds[i].resetBuffer();
    }
    for (int i = 0; i < maxFireballs; ++i)
    {
        delete Objects[i];
        Objects[i] = nullptr;
    }
}

void ObjectProjectileManager::addFireBall(Vector2f origin, int direction, Pet *source)
{
    for (int i = 0; i < maxFireballs; ++i)
    {
        if (!Objects[i])
        {
            Objects[i] = new FireBall(ObjectTextures[0], origin, direction, source);
            ++activeCount;
            break;
        }
    }
}

void ObjectProjectileManager::addAttackBall(Vector2f origin, int direction, Pet *source)
{
    for (int i = 0; i < maxFireballs; ++i)
    {
        if (!Objects[i])
        {
            Objects[i] = new AttckBall(ObjectTextures[2], origin, direction, source);
            ++activeCount;
            break;
        }
    }
}

void ObjectProjectileManager::addRock(Vector2f origin, Vector2f Vel, Pet *source)
{
    for (int i = 0; i < maxFireballs; ++i)
    {
        if (!Objects[i])
        {
            // if (source)
            Objects[i] = new Rock(ObjectTextures[1], origin, Vel, source);
            // else
            // Objects[i] = new Rock(ObjectTextures[1], origin, direction);

            ++activeCount;
            break;
        }
    }
}

// void ObjectProjectileManager::update(float dt)
// {
//     for (int i = 0; i < maxFireballs; ++i)
//     {
//         if (Objects[i])
//         {
//             Objects[i]->update(dt);
//             Vector2f pos = Objects[i]->getPosition();

//             if (pos.x < -winLimit || pos.x > winSize.x + winLimit || pos.y < -winLimit || pos.y > winSize.y + winLimit)
//             {
//                 delete Objects[i];
//                 Objects[i] = nullptr;
//                 --activeCount;
//             }
//         }
//     }
// }
void ObjectProjectileManager::update(float dt)
{
    for (int i = 0; i < maxFireballs; ++i)
    {
        if (Objects[i])
        {
            Objects[i]->update(dt);
            Vector2f pos = Objects[i]->getPosition();

            if (pos.x < -winLimit || pos.x > winSize.x + winLimit || pos.y < -winLimit || pos.y > winSize.y + winLimit)
            {
                addCollisionEffect(pos, 0); // use Collision[0] for out-of-bounds
                delete Objects[i];
                Objects[i] = nullptr;
                --activeCount;
            }
        }
    }

    Destroy_Projectile_Collisions();

    updateCollisionEffects(dt);
}

void ObjectProjectileManager::draw(RenderWindow &window)
{
    for (int i = 0; i < maxFireballs; ++i)
    {
        if (Objects[i])
        {
            Objects[i]->draw(window);
        }
    }

    for (int i = 0; i < collisionCount; ++i)
        if (Collisions[i])
            Collisions[i]->draw(window);
}

void ObjectProjectileManager::clearAll()
{
    for (int i = 0; i < maxFireballs; ++i)
    {
        delete Objects[i];
        Objects[i] = nullptr;
    }
    activeCount = 0;
}

int ObjectProjectileManager::getFireBallCount() const
{
    return activeCount;
}

void ObjectProjectileManager::Damage_Pet(Pet *P)
{
    if (!P)
        return;

    for (int i = 0; i < maxFireballs; ++i)
    {
        if (Objects[i] && Objects[i]->getName() == "FireBall")
        {

            FireBall *fb = (FireBall *)(Objects[i]);
            if (fb && fb->from_Pet != P)
            {
                float dx = fb->getCenter().x - P->getCenter().x;
                float dy = fb->getCenter().y - P->getCenter().y;
                float distance = sqrt(dx * dx + dy * dy);
                float maxRange = 70.f;

                if (distance <= maxRange &&
                    fb->getGlobalBounds().intersects(P->getGlobalBounds()))
                {
                    if (fb->from_Pet)
                        P->takeDamage(fb->from_Pet->getAttackPower() * 3);
                    else
                        P->takeDamage(fb->damage * level);

                    addCollisionEffect(Objects[i]->getCenter(), 0);
                    delete Objects[i];
                    Objects[i] = nullptr;
                    --activeCount;

                    sounds[i%30].play();
                }
            }

            
        }
        else if (Objects[i] && Objects[i]->getName() == "Rock")
        {
            Rock *rk = (Rock *)(Objects[i]);

            // cout << "Rock *rk = (Rock *)(Objects[i]);\n";
            // if (rk && !rk->from_Pet)
            // {
            //     P->takeDamage(rk->damage);
            //     cout << " P->takeDamage(rk->damage);\n";
            //     continue;
            // }

            // cout << "!continue\n";

            if (rk && rk->from_Pet != P)
            {
                float dx = rk->getCenter().x - P->getCenter().x;
                float dy = rk->getCenter().y - P->getCenter().y;
                float distance = sqrt(dx * dx + dy * dy);
                float maxRange = 70.f;

                if (distance <= maxRange &&
                    rk->getGlobalBounds().intersects(P->getGlobalBounds()))
                {
                    if (rk->from_Pet)
                        P->takeDamage(rk->from_Pet->getAttackPower());
                    else
                        P->takeDamage(rk->damage * level);
                    addCollisionEffect(Objects[i]->getCenter(), 1);
                    delete Objects[i];
                    Objects[i] = nullptr;
                    --activeCount;

                    sounds[i%30].play();
                }
            }
        }
        else if (Objects[i] && Objects[i]->getName() == "Ball")
        {
            AttckBall *ab = (AttckBall *)(Objects[i]);

            // cout << "Rock *rk = (Rock *)(Objects[i]);\n";
            // if (rk && !rk->from_Pet)
            // {
            //     P->takeDamage(rk->damage);
            //     cout << " P->takeDamage(rk->damage);\n";
            //     continue;
            // }

            // cout << "!continue\n";

            if (ab && ab->from_Pet != P)
            {
                float dx = ab->getCenter().x - P->getCenter().x;
                float dy = ab->getCenter().y - P->getCenter().y;
                float distance = sqrt(dx * dx + dy * dy);
                float maxRange = 70.f;

                if (distance <= maxRange &&
                    ab->getGlobalBounds().intersects(P->getGlobalBounds()))
                {
                    if (ab->from_Pet)
                        P->takeDamage(ab->from_Pet->getAttackPower());
                    else
                        P->takeDamage(ab->damage * level);

                    addCollisionEffect(Objects[i]->getCenter(), 1);
                    delete Objects[i];
                    Objects[i] = nullptr;
                    --activeCount;

                    sounds[i%30].play();
                }
            }
        }
    }
}
