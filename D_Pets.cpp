#include "Pet.h"
#include "Projectiles.h"

// ================= Dragon Class  ================

Dragon::Dragon()
{
    setName("Dragon");
  
}

Dragon::Dragon(Texture &Tex) : Pet(Tex)
{
    setName("Dragon");

}

Dragon::Dragon(Texture tex, Vector2f position, Vector2f size)
    : Pet("Dragon", "Tank", 300, 50, 30, 30, tex, position, size) {}

Dragon::Dragon(string n, Texture tex, Vector2f position, Vector2f size)
    : Pet(n, "Tank", 300, 50, 30, 30, tex, position, size) {}

void Dragon::useAbility_1(Pet *O)
{

    if (fireCooldownTimer2 <= 0)
    {
        cout << getName() << " uses Dragon's Fire Ball! \n";
        // helper.shootProjectile(getOrigin(), (inverted ? 1 : -1), this, 20, 800, 3);
        if (isProjectilesLinked())
            ProjectileManager()->addFireBall(getCenter(), (isInverted() ? 1 : -1), this);
        fireCooldownTimer2 = fireCooldown2;
    }

    // fireBalls(); - Commented in source
}

void Dragon::useAbility_2(Pet *O)
{

    if (isEnemyPointed() && Enemy()->IsNotStunned())
    {
        A.createWaveRing(getCenter());

        // projectileManager.addRock(getCenter(), 1);

        Enemy()->Stun_it();
        cout << getName() << " uses Dragon's Roar! Enemies stunned!\n";
    }
    else if (!isEnemyPointed())
        cout << " Enemy not pointed\n";
}

void Dragon::update_all(float deltaTime)
{
    fireCooldownTimer2 = max(0.f, fireCooldownTimer2 - deltaTime);

    update(deltaTime);
    // updateFire(deltaTime);
    // updateFireBalls(deltaTime);
}

void Dragon::draw_all(RenderWindow &window, Font *font)
{

    draw(window, font);
    // drawProjectilesFireBalls(window);

    // if (Enemy_pet_pointer)
    //     helper.drawDotAt(Enemy_pet_pointer->getCenter(), Color::White, 6);
    // else
    //     helper.drawDotAt(Vector2f(100, 100), Color::Black, 6);
}

void Dragon::HitOther(Pet *P)
{
    Take_projectile_damage();
    // DamageOther(P);
    // DamageOther2(P);
}

void Dragon::Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds)
{
   


    // if (Enemy_pet_pointer)
    //     Enemy_pet_pointer->write_Pet_factor(window, helper.getFont());

    Controll_Pet_With_keys(window, event, deltaTime, WBounds);
}

Dragon::~Dragon()
{
    
}

// ================= Phoenix Class  ================

Phoenix::Phoenix()
{
    setName("Phoenix");
}

Phoenix::Phoenix(Texture &Tex) : Pet(Tex)
{
    setName("Phoenix");

}

Phoenix::Phoenix(Texture tex, Vector2f position, Vector2f size)
    : Pet("Phoenix", "Healer", 300, 50, 30, 30, tex, position, size) {}

Phoenix::Phoenix(string n, Texture tex, Vector2f position, Vector2f size)
    : Pet(n, "Healer", 300, 50, 30, 30, tex, position, size) {}

void Phoenix::useAbility_1(Pet *O)
{
    if (healing_timer <= 0) // Only apply if not already active
    {
        heal(100);
        cout << getName() << " defense doubles!\n";
    }
}

void Phoenix::useAbility_2(Pet *O)
{
    if (!ability_2_used && health_Percent < 0.3)
    {
        refill_Health();
        ability_2_used = true;
    }
}

void Phoenix::update_all(float deltaTime)
{

    update(deltaTime);
}

void Phoenix::Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds)
{
   
   

    // if (Enemy_pet_pointer)
    //     Enemy_pet_pointer->write_Pet_factor(window, helper.getFont());

    Controll_Pet_With_keys(window, event, deltaTime, WBounds);
}

void Phoenix::draw_all(RenderWindow &window, Font *font)
{
    draw(window, font);

    // if (Enemy_pet_pointer)
    //     helper.drawDotAt(Enemy_pet_pointer->getCenter(), Color::White, 6);
    // else
    //     helper.drawDotAt(Vector2f(100, 100), Color::Black, 6);
}

void Phoenix::HitOther(Pet *P)
{
    Take_projectile_damage();
}

// ================= Unicorn Class  ================

Unicorn::Unicorn()
{
    setName("Unicorn");
    change_axis(6);
    flameDuration = 0.09f;
}

Unicorn::Unicorn(Texture &Tex) : Pet(Tex)
{
    setName("Unicorn");
    change_axis(6);
    flameDuration = 0.09f;
}

Unicorn::Unicorn(Texture tex, Vector2f position, Vector2f size)
    : Pet("Unicorn", "Guardian", 300, 50, 30, 30, tex, position, Vector2f(size.x, size.y))
{
    change_axis(6);
    flameDuration = 0.09f;
}

Unicorn::Unicorn(string n, Texture tex, Vector2f position, Vector2f size)
    : Pet(n, "Guardian", 300, 50, 30, 30, tex, position, Vector2f(size.x, size.y))
{
    change_axis(6);
    flameDuration = 0.09f;
}

void Unicorn::useAbility_1(Pet *O)
{
    // fire();
    double_speed();
    // speed_timer = 10;
}

void Unicorn::useAbility_2(Pet *O)
{
    double_defence();
}

void Unicorn::Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds)
{
    float temp = speed;
    if (speed_timer > 0)
        speed *= 2;


    Controll_Pet_With_keys(window, event, deltaTime, WBounds);

    if (speed_timer > 0)
    {
        speed = temp;
        speed_timer -= deltaTime;
    }
}

void Unicorn::update_all(float deltaTime)
{
    if (Abilities_timers[3] > 0)
    {
        // helper.draw_shield(getCenter(), defense_timer / 10, 10 * (inverted ? 1 : -1));
        // A.draw_defense_shield(getCenter(), defense_timer / 10, 10 * (inverted ? 1 : -1));
        Abilities_timers[3] -= deltaTime;
        if (Abilities_timers[3] <= 0)
        {
            defense = pre_defense;
            pre_defense = 0;
        }
    }

    update(deltaTime);
}

void Unicorn::draw_all(RenderWindow &window, Font *font)
{
    draw(window, font);

    // if (Enemy_pet_pointer)
    //     helper.drawDotAt(Enemy()->getCenter(), Color::White, 6);
    // else
    //     helper.drawDotAt(Vector2f(100, 100), Color::Black, 6);
}

void Unicorn::HitOther(Pet *P)
{
    Take_projectile_damage();
}

// ================= Griffin Class  ================

Griffin::Griffin()
{
    setName("Griffin");
    change_axis(11);
}

Griffin::Griffin(Texture Tex) : Pet(Tex)
{
    setName("Griffin");
    change_axis(11);
}

Griffin::Griffin(Texture tex, Vector2f position, Vector2f size)
    : Pet("Griffin", "Striker", 300, 50, 30, 30, tex, position, size)
{
    change_axis(11);
}

Griffin::Griffin(string n, Texture tex, Vector2f position, Vector2f size)
    : Pet(n, "Striker", 300, 50, 30, 30, tex, position, size)
{
    change_axis(11);
}

// void Griffin::useAbility_1(Pet *O)
// {
//     // fire();

//     if (strick_timer <= 0)
//     {
//         strick_timer = 2;
//     }
// }

void Griffin::useAbility_1(Pet *O)
{

    if (strick_timer <= 0 && isEnemyPointed())
    {
        Vector2f center1 = getCenter();
        Vector2f center2 = Enemy()->getCenter();
        float dx = center1.x - center2.x;
        float dy = center1.y - center2.y;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= 500)
        {
            Enemy()->takeDamage(getAttackPower() * 2);
            strick_timer = 5;
            strick_Ori = Vector2f((center1.x + center2.x) / 2, (center1.y + center2.y) / 2);
        }
    }
    else if (strick_timer <= 0)
    {
        strick_timer = 5;
        strick_Ori = getCenter();
    }
}

void Griffin::useAbility_2(Pet *O)
{
    if (!attack_target)
    {
        attack_target = true;

        preOrigin = this->getCenter();
        pre_direction = (isInverted() ? -1 : 1);

        cout << getName() << " uses Dive Attack !\n";
    }
}

void Griffin::update_all(float deltaTime)
{
    if (attack_target)
    {
        float temp_speed = speed;
        if (isEnemyPointed())
        {
            speed *= 50;

            trackTarget(Enemy()->getCenter(), deltaTime);

            Vector2f dir = Enemy()->getCenter() - this->getCenter();
            float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist < 10.f)
            {
                Enemy()->takeDamage(getAttackPower() * 3);
                Enemy()->Stun_it();

                attack_target = false;
            }
        }

        speed = temp_speed;
    }
    else if (preOrigin != Vector2f(0, 0))
    {
        float temp_speed = speed;

        {
            speed *= 50;

            trackTarget(preOrigin, deltaTime);

            Vector2f dir = preOrigin - this->getCenter();
            float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist < 10.f)
            {
                preOrigin = Vector2f(0, 0);

                if (pre_direction == 1)
                    Invert_Left();
                else if (pre_direction == -1)
                    Invert_Right();

                attack_target = false;
            }
        }
        speed = temp_speed;
    }

    strick_timer = max(0.f, strick_timer - deltaTime);

    update(deltaTime);
}

void Griffin::draw_all(RenderWindow &window, Font *font)
{
    draw(window, font);

    if (strick_timer > 0)
    {
        A.draw_claw_strike(window, strick_Ori, strick_timer / 5.f);
    }

}

void Griffin::Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds)
{
    
    Controll_Pet_With_keys(window, event, deltaTime, WBounds);
}

void Griffin::HitOther(Pet *P)
{
    Take_projectile_damage();
}