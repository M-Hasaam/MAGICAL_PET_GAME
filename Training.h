#ifndef TRAINING_H
#define TRAINING_H

#include "Pet.h"
#include "Bot.h"
#include "Player.h"
#include "Projectiles.h"
#include "ForwardDeclarations.h"
// #include <SFML/Graphics.hpp>

#include "Helper.h"

class TrainingArena
{
public:
    float TIME = 0;
    Pet *player_pet = nullptr;

    ObjectProjectileManager projectileManager;

    FloatRect winBound = FloatRect(-300.f, -300.f, size_X + 600.f, size_Y + 600.f);
    FloatRect petBound = FloatRect(2, 0.f, size_X - 200.f, size_Y);

    float Rock_timer = 1;
    float FireBall_timer = 2;
    float Ball_timer = 3;

    float timer = 0;

public:
    bool GAME = false;

    TrainingArena() : projectileManager(Vector2u(size_X, size_Y)) {}

    void prepare_Training(Pet *P)
    {
        reset();

        player_pet = P;

        if (player_pet)
        {
            player_pet->setCenter(Vector2f(500, size_Y / 2));
            player_pet->setSize(Vector2f(150, 150));
            player_pet->Invert_Right();

            player_pet->linkProjectiles(&projectileManager);
        }

        projectileManager.setLevel(player_pet->getLevel());

        // Save_Pre_Factors_of_Pets();
        // give_them_position(0);
    }

private:
    void update(float deltaTime)
    {
        if (player_pet)
        {
            player_pet->update_all(deltaTime);
        }

        projectileManager.update(deltaTime);
    }

    void draw(RenderWindow &window)
    {
        if (player_pet)
        {
            player_pet->draw_all(window);
        }
        projectileManager.draw(window);
    }

    // void Save_Pre_Factors_of_Pets()
    // {

    //     if (player_pet)
    //         player_pet->save_previous_factors();
    // }

    // void Reset_Factors_of_Pets()
    // {

    //     if (player_pet)
    //         player_pet->reset_factors();
    // }

public:
    void update_trainingCamp(RenderWindow &window, Event &event, Font &font, float deltaTime)
    {
        if (!player_pet)
            return;

        if (GAME)
        {

            TIME += deltaTime;
            // GAME = true;

            // cout << " Test -1\n";

            Rock_timer = max(0.f, Rock_timer - deltaTime);
            if (Rock_timer <= 0.f)
            {
                projectileManager.addRock(Vector2f(rand() % (size_X - 200) + 100, -100));
                Rock_timer = 1.f;
            }

            FireBall_timer = max(0.f, Rock_timer - deltaTime);
            if (FireBall_timer <= 0.f)
            {
                projectileManager.addFireBall(Vector2f(size_X + 100, rand() % (size_Y - 200) + 100), -1);
                FireBall_timer = 1.f;
            }

            Ball_timer = max(0.f, Rock_timer - deltaTime);
            if (Ball_timer <= 0.f)
            {
                projectileManager.addAttackBall(Vector2f(size_X + 100, rand() % (size_Y - 200) + 100), -1);
                Ball_timer = 1.f;
            }
        }

        player_pet->Controll_Pet_With_keys(window, event, deltaTime, petBound);

        player_pet->write_level_below_Pet(window, font);
        player_pet->write_Pet_factor(window, font);

        projectileManager.Damage_Pet(player_pet);

        draw(window);
        update(deltaTime);
        // cout << " Test -2\n";

        int minutes = TIME / 60;
        int seconds = int(TIME) % 60;
        helper.drawCenteredText("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds), size_X / 2, 100, 50);
        // trainingTimeText.setString("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds));
        // window.draw(trainingTimeText);

        if (player_pet->isHealthCritical())
        {

            if (GAME)
            {
                player_pet->trainCharacter(TIME);
                GAME = false;
            }
        }

        if (!GAME)
        {
            float healthRate = 5.0;
            float attackRate = 1.0;
            float speedRate = 0.5f;
            float defenseRate = 0.8f;

            helper.drawCenteredText("Training Ended", size_X / 2, size_Y / 2 - 100, 200);
            helper.drawCenteredText("Health Incresed by : " + to_string(int(TIME * healthRate)), size_X / 2, size_Y / 2 + 80, 40);
            helper.drawCenteredText("Attack Incresed by : " + to_string(int(TIME * attackRate)), size_X / 2, size_Y / 2 + 120, 40);
            helper.drawCenteredText("Speed Incresed by : " + to_string(int(TIME * speedRate)), size_X / 2, size_Y / 2 + 160, 40);
            helper.drawCenteredText("Defence Incresed by : " + to_string(int(TIME * defenseRate)), size_X / 2, size_Y / 2 + 200, 40);
        }
    }

public:
    void reset()
    {
        if (player_pet)
            player_pet->nullifyAllLinks();

        projectileManager.setLevel(1);
        projectileManager.clearAll();
        player_pet = nullptr;
        TIME = 0;
        GAME = true;
    }
};

#endif
