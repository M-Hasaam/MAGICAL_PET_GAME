#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "ForwardDeclarations.h"
#include "Object.h"

class FireBall : public Object
{
public:
    Pet *from_Pet = nullptr;
    float damage = 100;

    FireBall(Texture &Tex, Vector2f Ori, int direction, Pet *P) : Object("FireBall", Tex, 6, Vector2f(150, 150), Ori, Vector2f(1000 * direction, 0), direction), from_Pet(P)
    {
      
    }
};

class Rock : public Object
{
public:
    Pet *from_Pet = nullptr;
    float damage = 100;

    Rock(Texture &Tex, Vector2f Ori, Vector2f Vel = Vector2f(0, 500), Pet *P = nullptr) : Object("Rock", Tex, 16, Vector2f(100, 100), Ori, Vel, 1), from_Pet(P)
    {
        
    }
};

class AttckBall : public Object
{
public:
    Pet *from_Pet = nullptr;
    float damage = 50;

    AttckBall(Texture &Tex, Vector2f Ori, int direction, Pet *P = nullptr) : Object("Ball", Tex, 4, Vector2f(50, 50), Ori, Vector2f(500 * direction, 0), direction), from_Pet(P)
    {
      
    }
};


class ObjectProjectileManager
{
protected:
    Object *Objects[200];
    int maxFireballs;
    int activeCount;
    Vector2u winSize;
    const int winLimit = 300;
    Texture ObjectTextures[5];
    Texture CollisionTex[3];

   

    SoundBuffer SB;
    Sound sounds[30];

    int level = 1;

    Object *Collisions[100];
    int collisionCount = 0;
    int CollisionFrameCounts[3] = {64, 48, 5}; // frames for each sprite sheet

public:
    void setLevel(int lv) { level = lv; }

    ObjectProjectileManager(Vector2u size);
    ~ObjectProjectileManager();

    void addFireBall(Vector2f origin, int direction, Pet *source = nullptr);
    void addRock(Vector2f origin, Vector2f Vel = Vector2f(0, 200), Pet *source = nullptr);
    void addAttackBall(Vector2f origin, int direction, Pet *source = nullptr);
    void update(float dt);
    void draw(RenderWindow &window);
    void clearAll();
    int getFireBallCount() const;
    void Damage_Pet(Pet *P);

// collision
    void addCollisionEffect(Vector2f pos, int textureIdx)
    {
        if (collisionCount >= 100)
            return;

        Object *effect = new Object("CollisionEffect", CollisionTex[textureIdx], CollisionFrameCounts[textureIdx], Vector2f(100, 100), pos);

        effect->setFrameDuration(0.01f);

        Collisions[collisionCount++] = effect;
    }

    void updateCollisionEffects(float dt)
    {
        for (int i = 0; i < collisionCount;)
        {
            if (Collisions[i])
            {
                Collisions[i]->update(dt);

                if (Collisions[i]->Is_Animation_Restarted())
                {
                    delete Collisions[i];
                    Collisions[i] = Collisions[--collisionCount];
                    continue;
                }
            }
            ++i;
        }
    }

// collision withh each other
    void Destroy_Projectile_Collisions()
    {
        for (int i = 0; i < maxFireballs && i < 200; ++i)
        {
            if (!Objects[i])
                continue;

            if (Objects[i]->getName() != "FireBall" && Objects[i]->getName() != "Rock" && Objects[i]->getName() != "Ball")
                continue;

            for (int j = i + 1; j < maxFireballs && j < 200; ++j)
            {
                if (!Objects[j])
                    continue;

                if (Objects[j]->getName() != "FireBall" && Objects[j]->getName() != "Rock" && Objects[j]->getName() != "Ball")
                    continue;

                Object *A = Objects[i];
                Object *B = Objects[j];

                if (!A || !B || A == B)
                    continue;

                FloatRect rectA = A->getGlobalBounds();
                FloatRect rectB = B->getGlobalBounds();

                float dx = A->getCenter().x - B->getCenter().x;
                float dy = A->getCenter().y - B->getCenter().y;
                float distance = sqrt(dx * dx + dy * dy);
                float collisionRange = 30.f;

                if (distance <= collisionRange && rectA.intersects(rectB))
                {
                    addCollisionEffect(A->getCenter(), 0);
                    addCollisionEffect(B->getCenter(), 1);

                    delete Objects[i];
                    delete Objects[j];
                    Objects[i] = nullptr;
                    Objects[j] = nullptr;

                    sounds[i%30].play();
                    activeCount -= 2;
                    break; 
                }
            }
        }
    }
};



#endif