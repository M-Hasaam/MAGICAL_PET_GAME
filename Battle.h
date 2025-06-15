#ifndef BATTLE_H
#define BATTLE_H

#include "Pet.h"
#include "Bot.h"
#include "Player.h"
#include "Projectiles.h"
#include <SFML/Graphics.hpp>

#include "Helper.h"

extern const int size_X;
extern const int size_Y;

const Vector2f P_Positions[2][4] = {
    {Vector2f(20, 250),
     Vector2f(170, 250),
     Vector2f(320, 250),
     Vector2f(470, 250)},

    {Vector2f(100, 400),
     Vector2f(100, 400),
     Vector2f(100, 400),
     Vector2f(100, 400)}};

const Vector2f E_Positions[2][4] = {
    {Vector2f(size_X - 170, size_Y - 150),  //- 250-150),
     Vector2f(size_X - 320, size_Y - 150),  //- 250-150),
     Vector2f(size_X - 470, size_Y - 150),  //- 250-150),
     Vector2f(size_X - 620, size_Y - 150)}, //- 250-150)},

    {Vector2f(100, 400),
     Vector2f(100, 400),
     Vector2f(100, 400),
     Vector2f(100, 400)}};

const Vector2f Size[2][4] = {
    {Vector2f(150, 150),
     Vector2f(150, 150),
     Vector2f(150, 150),
     Vector2f(150, 150)}};

class BattleArena // for COntrolling battels
{
    float TIME = 0;
    int LEVEL = 0;
    bool score_added = false;

    ObjectProjectileManager projectileManager;

    Pet *player_pet[4] = {0, 0, 0, 0};
    int P_count = 0;
    int P_indx = 0;
    Pet *enemy_pet[4] = {0, 0, 0, 0};

    Player *player = nullptr;
    int E_count = 0;
    int E_indx = 0;
    Bot AI_Bot;

    float Rock_timer = 0;


    float timer = 0;

public:
    bool GAME = true;
    BattleArena(Player *P) : projectileManager(Vector2u(size_X, size_Y)), player(P) {}

    ~BattleArena()
    {
        reset();
    }

// 1 V 1
    void prepare_1_V_1(Player &player, Pet *E_pets[4])
    {
        reset();

        player_pet[P_count++] = &player.get_selected_pet();
        enemy_pet[E_count++] = E_pets[rand() % 4];

        P_indx = E_indx = 0;

        for (int i = P_count; i < 4; i++)
        {
            player_pet[i] = nullptr;
            enemy_pet[i] = nullptr;
        }

       

        AI_Bot.set_Enemy(enemy_pet[0]);
        AI_Bot.set_Player(player_pet[0]);

        give_them_position(0);
        give_them_level();

        
    }

    
// 2 V 2 

    void prepare_2_V_2(Player &player, Pet *E_pets[4])
    {
        cout << " prepare_2_V_2(Player &player, Pet *E_pets[4])\n";

        reset();

        cout << "  reset();\n";

        P_count = 0;
        for (int i = 0; i < 4; i++)
        {

            if (player.Pets[i] && player.Pets[i]->isSelected())
            {
                cout << " selected i = " << i << endl;
                player_pet[P_count++] = player.Pets[i];
            }
            else
            {
                cout << " unselected i = " << i << endl;
            }
        }
       
        int r1 = rand() % 4;
        int r2;

        do
        {
            r2 = rand() % 4;
        } while (r1 == r2);

        enemy_pet[E_count++] = E_pets[r1];
        enemy_pet[E_count++] = E_pets[r2];

        for (int i = P_count; i < 4; i++)
        {
            player_pet[i] = nullptr;
            enemy_pet[i] = nullptr;
        }

        if (enemy_pet[0] && player_pet[0])
        {
            AI_Bot.set_Enemy(enemy_pet[0]);
            AI_Bot.set_Player(player_pet[0]);
        }

        // Save_Pre_Factors_of_Pets();

        give_them_position(0);
        give_them_level();

        cout << " prepare_2_V_2(Player &player, Pet *E_pets[4]) XXX\n";
    }

// G V G

    void prepare_G_V_G(Player &player, Pet *E_pets[4])
    {
        cout << " prepare_G_V_G(Player &player, Pet *E_pets[4])\n";

        reset();

        cout << "  reset();\n";

        int Pets_C = player.guild.petCount;
        if (Pets_C > 4)
            Pets_C = 4; // Safety cap

        // Add up to Pets_C guild pets
        P_count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (player.guild.pets[i])
            {

                player_pet[P_count++] = player.guild.pets[i];
            }
        }

        // Generate unique random indices for enemy pets
        bool selected[4] = {false, false, false, false};
        int count = 0;
        while (count < Pets_C)
        {
            int r = rand() % 4;
            if (!selected[r] && E_pets[r])
            {
                selected[r] = true;
                enemy_pet[E_count++] = E_pets[r];
                count++;
            }
        }

        for (int i = P_count; i < 4; i++)
        {
            player_pet[i] = nullptr;
            enemy_pet[i] = nullptr;
        }

        if (enemy_pet[0] && player_pet[0])
        {
            AI_Bot.set_Enemy(enemy_pet[0]);
            AI_Bot.set_Player(player_pet[0]);
        }

        give_them_position(0);
        give_them_level();

        // Save_Pre_Factors_of_Pets();

        cout << " prepare_G_V_G(Player &player, Pet *E_pets[4]) XXX\n";
    }

private:
    void give_them_position(int I)
    {
        int C = 0;
        for (int i = 0; i < 4; i++)
        {
            if (player_pet[i])
            {
                player_pet[i]->setSize(Size[I][C]);
                player_pet[i]->Invert_Right();
                player_pet[i]->setPosition(P_Positions[I][i] + Vector2f(150, 0));
                player_pet[i]->linkProjectiles(&projectileManager);
                // player_pet[i]->setCenter(P_Positions[I][i]);
            }

            if (enemy_pet[i])
            {
                enemy_pet[i]->setSize(Size[I][i]);
                enemy_pet[i]->Invert_Left();
                enemy_pet[i]->setTopLeftCorner(E_Positions[I][i]);
                enemy_pet[i]->linkProjectiles(&projectileManager);
            }
        }
    }

    void give_them_level()
    {
        int C = 0;
        float avg_level = 0;
        for (int i = 0; i < 4; i++)
        {
            if (player_pet[i])
            {
                avg_level += player_pet[i]->calculateLevel();
                C++;
            }
        }

        avg_level /= C;

        LEVEL = avg_level + 0.5;

        for (int i = 0; i < 4; i++)
        {
            if (enemy_pet[i])
            {
                enemy_pet[i]->applyLevelToStats(LEVEL);
            }
        }

        projectileManager.setLevel(LEVEL);
    }

    void update(float deltaTime)
    {
        for (int i = 0; i < 4; i++)
        {
            if (player_pet[i])
            {
                player_pet[i]->update_all(deltaTime);
            }

            if (enemy_pet[i])
            {
                enemy_pet[i]->update_all(deltaTime);
            }
        }
    }

    void update_position(float deltaTime)
    {
        for (int i = 0; i < 4; i++)
        {
            if (i != P_indx)
            {
                if (player_pet[i])
                {

                    Vector2f currentPos = player_pet[i]->getPosition();
                    Vector2f targetPos = P_Positions[0][i];
                    Vector2f diff = targetPos - currentPos;

                    player_pet[i]->trackTarget(Vector2f(targetPos.x + 150 / 2, targetPos.y + 150 / 2), deltaTime, -1, 3);
                 
                }
            }
        }

        for (int i = 0; i < 4; i++)
        {
            if (i != E_indx)
            {
                if (enemy_pet[i])
                {

                    Vector2f currentPos = enemy_pet[i]->getPosition();
                    Vector2f targetPos = E_Positions[0][i];
                    Vector2f diff = targetPos - currentPos;

                    // if (sqrt((diff.x*diff.x) + (diff.y*diff.y)) > 5.f)
                    // {
                    enemy_pet[i]->trackTarget(Vector2f(targetPos.x + 150 / 2, targetPos.y + 150 / 2), deltaTime, 1, 6);
                    // }
                 
                }
            }
        }
    }

    void draw(RenderWindow &window)
    {
        for (int i = 0; i < 4; i++)
        {
            if (player_pet[i])
            {
                player_pet[i]->draw_all(window);
            }

            if (enemy_pet[i])
            {
                enemy_pet[i]->draw_all(window);
            }
        }
    }

public:
    void update_controll(RenderWindow &window, Event &event, Font &font, float deltaTime)
    {
        if (!(player_pet[0] && enemy_pet[0]))
            return;

        if (TIME > 30 * P_count)
            GAME = false;

        // cout << " Test -1\n";
        timer += deltaTime;

        Rock_timer = max(0.f, Rock_timer - deltaTime);
        if (Rock_timer <= 0)
        {
            projectileManager.addRock(Vector2f(rand() % (size_X - 200) + 100, -100));
            Rock_timer = 2;
        }

        draw(window);
        update(deltaTime);

        // cout << " Test 1\n";

        if (GAME)
        {

            TIME += deltaTime;

            update_position(deltaTime);

            Update_Potion_use(window, event, deltaTime);

            if (P_count > 0 && E_count > 0)
            {
                AI_Bot.Controll_Player(window, event, deltaTime);

                AI_Bot.Control_Enemy(window, event, deltaTime);
            }

            if (player_pet[P_indx])
            {
                player_pet[P_indx]->write_level_below_Pet(window, font);
                player_pet[P_indx]->write_Pet_factor(window, font);
            }
            if (enemy_pet[E_indx])
            {
                enemy_pet[E_indx]->write_level_below_Pet(window, font);
                enemy_pet[E_indx]->write_Pet_factor(window, font);
            }

            if (0 <= E_indx && E_indx < 4 && enemy_pet[E_indx] && enemy_pet[E_indx]->isHealthCritical())
            {
                for (int i = 1; i <= 3; i++)
                {

                    if (enemy_pet[(E_indx + i) % 4] && !enemy_pet[(E_indx + i) % 4]->isHealthCritical())
                    {
                        AI_Bot.set_Enemy(enemy_pet[(E_indx + i) % 4]);
                        E_indx = (E_indx + i) % 4;
                        break;
                    }

                    if (i == 3)
                    {
                        AI_Bot.reset_enemy();
                        E_indx = -1;
                        GAME = false;
                    }
                }
            }

            if (0 <= P_indx && P_indx < 4 && player_pet[P_indx] && player_pet[P_indx]->isHealthCritical())
            {
                for (int i = 1; i <= 3; i++)
                {

                    if (player_pet[(P_indx + i) % 4] && !player_pet[(P_indx + i) % 4]->isHealthCritical())
                    {
                        AI_Bot.set_Player(player_pet[(P_indx + i) % 4]);
                        P_indx = (P_indx + i) % 4;
                        break;
                    }

                    if (i == 3)
                    {
                        AI_Bot.reset_player();
                        P_indx = -1;
                        GAME = false;
                    }
                }
            }
        }

        if (timer > 0.2)
        {
            if (Keyboard::isKeyPressed(Keyboard::LShift)) //&& fireCooldownTimer <= 0.0f)
            {

                for (int i = 1; i <= 3; i++)
                {

                    if (player_pet[(P_indx + i) % 4])
                    {
                        AI_Bot.set_Player(player_pet[(P_indx + i) % 4]);
                        P_indx = (P_indx + i) % 4;
                        break;
                    }
                }
            }
            timer = 0;
        }

        int minutes = TIME / 60;
        int seconds = int(TIME) % 60;

        helper.drawCenteredText("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds) + " / " + to_string(P_count / 2) + ":" + to_string(P_count % 2 * 30), size_X / 2, 120, 50);
        helper.drawCenteredText("LEVEL : " + to_string(LEVEL), size_X / 2, 50, 50);

        if (!GAME)
        {
            helper.drawCenteredText("Battle Ended", size_X / 2, size_Y / 2 - 100, 200);

            int score = calculateScore(LEVEL, P_count, TIME);

            if (P_indx == -1 && E_indx == -1)
            {
                score /= 2;
                helper.drawCenteredText("It\'s a Draw", size_X / 2, size_Y / 2 + 100, 100);
            }
            else if (E_indx == -1)
            {

                helper.drawCenteredText("You Won", size_X / 2, size_Y / 2 + 100, 100);
            }
            else if (P_indx == -1)
            {
                score /= 10;
                helper.drawCenteredText("You Lose", size_X / 2, size_Y / 2 + 100, 100);
            }
            else
            {
                score = 0;
            }

            helper.drawCenteredText(" Score Added = " + to_string(score), size_X / 2, size_Y / 2 + 200, 50);

            if (!score_added)
            {
                player->addScores(score);
                score_added = true;
            }
        }

        projectileManager.update(deltaTime);
        projectileManager.draw(window);
    }

    void clearAll()
    {
        projectileManager.clearAll();
    }

public:
    void reset()
    {
        // player->addScores(calculate)
        projectileManager.clearAll();

        for (int i = 0; i < 4; i++)
        {
            if (enemy_pet[i])
            {
                enemy_pet[i]->nullifyAllLinks(); // player_pet[0]->Enemy_pet_pointer = enemy_pet[0];
                enemy_pet[i]->reset_Health_Attack();
            }

            if (player_pet[i])
            {
                player_pet[i]->nullifyAllLinks();
            }
        }

        for (int i = 0; i < 4; i++)
        {
            player_pet[i] = nullptr;
            enemy_pet[i] = nullptr;
        }

        cout << "-> AI_Bot.reset();\n";
        AI_Bot.reset();
        cout << "XX AI_Bot.reset();\n";

        E_count = E_indx = P_count = P_indx = 0;

        projectileManager.setLevel(1);
        score_added = false;
        GAME = true;
        TIME = 0;
        LEVEL = 0;
    }

    void Update_Potion_use(RenderWindow &window, Event &event, float deltaTime)
    {

        int idx = player->handleInventoryItemClick(event, deltaTime);

        if (idx != 0)
        {
            cout << " if (idx != 0)\n";
            if (player_pet[P_indx]->usePotion(idx - 1))
            {
                cout << "if (player_pet[P_indx]->usePotion(idx))\n";

                cout << " idx = " << idx << "\n";
                player->inventory.usePotion(idx - 1);
            }
        }
    }

    int calculateScore(int level, int petsFought, float timeTaken)
    {
        float baseScore = 100.0f;

        float levelFactor = level * 20.0f;                 
        float petPenalty = 1.0f - (petsFought - 1) * 0.1f; 
        if (petPenalty < 0.6f)
            petPenalty = 0.6f;

        float timeFactor = 60.0f / (timeTaken + 1.0f); 
        if (timeFactor > 3.0f)
            timeFactor = 3.0f;

        float rawScore = baseScore + (levelFactor * petPenalty * timeFactor);
        return static_cast<int>(rawScore + 0.5f);
    }
};

#endif
