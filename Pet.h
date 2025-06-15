#ifndef PET_H
#define PET_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include "Object.h"
#include "ForwardDeclarations.h"
#include "Helper.h"
#include "Animations.h"
#include "Projectiles.h"

using namespace std;
using namespace sf;

class Pet : public Object
{
protected:
    Animations A; // controlling animations

private:
    ObjectProjectileManager *projectileManager_pointer = nullptr;
    Pet *Enemy_pet_pointer = nullptr;

public:
    void pointEnemy(Pet *E) { Enemy_pet_pointer = E; }
    void linkProjectiles(ObjectProjectileManager *pManager) { projectileManager_pointer = pManager; }
    void nullifyEnemy() { Enemy_pet_pointer = nullptr; }
    bool isEnemyPointed() { return Enemy_pet_pointer; }
    bool isProjectilesLinked() { return projectileManager_pointer; }
    void nullifyAllLinks()
    {
        projectileManager_pointer = nullptr;
        Enemy_pet_pointer = nullptr;
    }
    Pet *Enemy() { return Enemy_pet_pointer; }
    ObjectProjectileManager *ProjectileManager() const { return projectileManager_pointer; }

private:
    float level = 1;

    const float fireCooldown = 0.5f; 
    float fireCooldownTimer = 0.0f;  

    const float Abilities_time_limit[4] = {10, 10, 10, 10};

    float Pre_factors[4] = {0, 0, 0, 0};

    string role = "null";

    int maxHealth = 300;
    int maxAttackPower = 50;

    bool selected = false, added = false;
    bool wasPressedGlobal = false;

public:
    int getLevel() { return level; }
    string getRole() const { return role; }
    void setRole(string s) { role = s; }
    bool isSelected() const { return selected; }
    void Select() { selected = true; }
    void Unselect() { selected = false; }

    bool isAdded() const { return added; }
    void Add() { added = true; }
    void Remove() { added = false; }

protected:
    float Abilities_timers[4] = {0, 0, 0, 0};

    float speed = 15;
    float defense = 30;
    int health = 300;
    int attackPower = 50;
    float health_Percent = 1.f;
    float attackPower_Percent = 1.f;
    float speedhealth_Percent = 1.f;
    float defensehealth_Percent = 1.f;

public:
    void reset_Health_Attack() { health = maxHealth, attackPower = maxAttackPower; }

public:
    Pet() {}
    Pet(const Pet &O) : Object(O) {}
    Pet(Texture Tex) : Object(Tex)
    {
        set_sprite_size(Vector2f(200, 200));
        setPosition(Vector2f(1300, 600));
    }
    Pet(string n, string r, int health, int attackPower, int speed, int defense, Texture tex, Vector2f position, Vector2f size)
        : health(health), attackPower(attackPower), speed(speed), defense(defense),
          Object(tex, position, size, 0.07f)
    {
        setName(n);
        role = r;
    }
    virtual ~Pet() {}

    void save_previous_factors()
    {
        if (Pre_factors[2] == 0)
        {
            Pre_factors[0] = health;
            Pre_factors[1] = attackPower;
            Pre_factors[2] = speed;
            Pre_factors[3] = defense;
        }
    }
    void reset_factors()
    {
      
        if (Pre_factors[2] != 0)
        {
            speed = Pre_factors[2];
            defense = Pre_factors[3];
        }
    }
    int calculateLevel()
    {
        float baseHealth = 300.0;
        float baseAttack = 50.0;
        float baseSpeed = 15.0;
        float baseDefense = 30.0;

        float healthScore = maxHealth / baseHealth;
        float attackScore = maxAttackPower / baseAttack;
        float speedScore = speed / baseSpeed;
        float defenseScore = defense / baseDefense;

        float rawLevel = (healthScore + attackScore + speedScore + defenseScore) / 4.0;
        level = int(rawLevel + 0.5);
        return level; // round to nearest int
    }

    void applyLevelToStats(int level)
    {
        this->level = level;

        float baseHealth = 300.0;
        float baseAttack = 50.0;
        float baseSpeed = 15.0;
        float baseDefense = 30.0;

        float factor = float(level);

        maxHealth = health = baseHealth * factor;
        maxAttackPower = attackPower = baseAttack * factor;
        speed = baseSpeed * factor;
        defense = baseDefense * factor;
    }

    void trainCharacter(int trainingTime)
    {
        float healthRate = 5.0;   // +5 health per time unit
        float attackRate = 1.0;   // +1 attack per time unit
        float speedRate = 0.5f;   // +0.5 speed per time unit
        float defenseRate = 0.8f; // +0.8 defense per time unit

        Pre_factors[0] += trainingTime * healthRate;
        Pre_factors[1] += trainingTime * attackRate;
        Pre_factors[2] += trainingTime * speedRate;
        Pre_factors[3] += trainingTime * defenseRate;

        maxHealth += trainingTime * healthRate;
        maxAttackPower += trainingTime * attackRate;

        level = calculateLevel();

        updatePercentages();
    }

    bool usePotion(int idx)
    {
        if (idx == 0)
        {
            return refill_Health();
        }
        else if (idx == 1)
        {
            return refill_AttackPower();
        }
        else if (idx == 2)
        {
            return double_defence();
        }
        else if (idx == 3)
        {
            return double_speed();
        }
        return false;
    }

    bool double_defence()
    {
        if (Abilities_timers[3] <= 0)
        {
            Abilities_timers[3] = Abilities_time_limit[3];

            defense *= 2;
            cout << "Defence doubled\n";
            return true;
        }
        cout << "Defence Not doubled\n";
        return false;
    }

    bool double_speed()
    {
        if (Abilities_timers[2] <= 0)
        {
            Abilities_timers[2] = Abilities_time_limit[2];

            speed *= 2;

            return true;
        }
        return false;
    }

    virtual void useAbility_1(Pet *O = nullptr) = 0;
    virtual void useAbility_2(Pet *O = nullptr) = 0;
    virtual void update_all(float deltaTime) = 0;
    virtual void draw_all(RenderWindow &window, Font *font = nullptr) = 0;
    virtual void HitOther(Pet *P) = 0;
    virtual void Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds = FloatRect(0, 0, 0, 0)) = 0;

    void updatePercentages();
    void update(float deltaTime) override;
    bool refill_Health();
    bool refill_AttackPower();
    void Controll_Pet_With_keys(RenderWindow &window, Event &E, float deltaTime, FloatRect Bounds = FloatRect(0, 0, 0, 0));
    void fire();
    void Take_projectile_damage();
    void trackTarget(Vector2f targetOrigin, float deltaTime, int invert_type = 0, float inc_speed = 1);
    void Increment_Level();
    void takeDamage(int damage);
    bool isHealthCritical();
    void heal(int amount);
    void Stun_it() { setStunnedTimer(3.f); } //  stun the pet

    Pet &operator=(const Pet &O);

    void printStats() const;
    int getHealth() const;
    int getAttackPower() const;
    int getSpeed() const;
    int getDefense() const;
    void write_level_below_Pet(RenderWindow &window, Font &font);
    void write_Pet_factor(RenderWindow &window, Font &font);
    bool draw_select_button(RenderWindow &window, Texture &Btn_T, Font &font);
    bool draw_add_remove_button(RenderWindow &window, Texture &Btn_T, Font &font);
    void draw_health_bar_of_Pet(RenderWindow &window);
    Color getHealthBarColor(float percent);
    void draw(RenderWindow &window, Font *font = nullptr);

    friend ostream &operator<<(ostream &os, Pet &p);
    friend istream &operator>>(istream &in, Pet &p);
};

class Dragon : public Pet
{
public:
    float fireCooldown2 = 1.0f;      // cooldown 
    float fireCooldownTimer2 = 0.0f; // timer for cooldown

    Dragon();
    Dragon(Texture &Tex);
    Dragon(Texture tex, Vector2f position, Vector2f size);
    Dragon(string n, Texture tex, Vector2f position, Vector2f size);

    void useAbility_1(Pet *O = nullptr) override;
    void useAbility_2(Pet *O = nullptr) override;
    void update_all(float deltaTime) override;
    void draw_all(RenderWindow &window, Font *font = nullptr) override;
    void HitOther(Pet *P) override;
    void Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds = FloatRect(0, 0, 0, 0)) override;

    ~Dragon();
};

class Phoenix : public Pet
{
public:
    float healing_timer = 0;
    bool ability_2_used = false;

    Phoenix();
    Phoenix(Texture &Tex);
    Phoenix(Texture tex, Vector2f position, Vector2f size);
    Phoenix(string n, Texture tex, Vector2f position, Vector2f size);

    void useAbility_1(Pet *O = nullptr) override;
    void useAbility_2(Pet *O = nullptr) override;
    void update_all(float deltaTime) override;
    void Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds = FloatRect(0, 0, 0, 0)) override;
    void draw_all(RenderWindow &window, Font *font = nullptr) override;
    void HitOther(Pet *P) override;
};

class Unicorn : public Pet
{
public:
    float speed_timer = 0;
    float defense_timer = 0;
    float pre_defense = 0;
    float flameDuration; // Added from the code

    Unicorn();
    Unicorn(Texture &Tex);
    Unicorn(Texture tex, Vector2f position, Vector2f size);
    Unicorn(string n, Texture tex, Vector2f position, Vector2f size);

    void useAbility_1(Pet *O = nullptr) override;
    void useAbility_2(Pet *O = nullptr) override;
    void Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds = FloatRect(0, 0, 0, 0)) override;
    void update_all(float deltaTime) override;
    void draw_all(RenderWindow &window, Font *font = nullptr) override;
    void HitOther(Pet *P) override;
};

class Griffin : public Pet
{
public:
    bool attack_target = false;
    // float preSpeed = 0;
    Vector2f preOrigin = Vector2f(0, 0);
    int pre_direction = 0;
    float strick_timer = 0;
    Vector2f strick_Ori = Vector2f(0, 0);

    Griffin();
    Griffin(Texture Tex);
    Griffin(Texture tex, Vector2f position, Vector2f size);
    Griffin(string n, Texture tex, Vector2f position, Vector2f size);

    void useAbility_1(Pet *O = nullptr) override;
    void useAbility_2(Pet *O = nullptr) override;
    void update_all(float deltaTime) override;
    void draw_all(RenderWindow &window, Font *font = nullptr) override;
    void Controll_with_Keys(RenderWindow &window, Event &event, float deltaTime, FloatRect WBounds = FloatRect(0, 0, 0, 0)) override;
    void HitOther(Pet *P) override;
};

#endif // PET_H
