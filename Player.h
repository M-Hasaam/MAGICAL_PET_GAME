#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Pet.h"
#include "Shop.h"
#include "Helper.h"
#include "Guild.h"

// extern Helper helper;
extern Texture Btn_T;

using namespace std;
using namespace sf;

const Vector2f Const_Pet_size[4][4] =
    {
        {Vector2f(200, 200),  // Dragon [0]
         Vector2f(200, 200),  // Phoenix [1]
         Vector2f(200, 200),  // Unicorn [2]
         Vector2f(200, 200)}, // Griffin [3]

        {Vector2f(400, 400),
         Vector2f(400, 400),
         Vector2f(400, 400),
         Vector2f(400, 400)},

        {Vector2f(300, 300),
         Vector2f(300, 300),
         Vector2f(300, 300),
         Vector2f(300, 300)},

        {Vector2f(-200, 200),
         Vector2f(-200, 200),
         Vector2f(-200, 200),
         Vector2f(-200, 200)}};

const Vector2f Const_Pet_Pos[4][4] =
    {
        {Vector2f(600, 150),   // Dragon [0]
         Vector2f(1000, 150),  // Phoenix [1]
         Vector2f(600, 500),   // Unicorn [2]
         Vector2f(1000, 500)}, // Griffin [3]

        {Vector2f(600, 150),
         Vector2f(600, 150),
         Vector2f(600, 150),
         Vector2f(600, 150)},

        {Vector2f(400, 150),
         Vector2f(400, 150),
         Vector2f(400, 150),
         Vector2f(400, 150)},

        {Vector2f(300, 600),
         Vector2f(300, 600),
         Vector2f(300, 600),
         Vector2f(300, 600)}};

// player class having four Pets
class Player
{
private:
public:
    string username = "Null";
    int cash = 0;
    int score = 0;
    PlayerIngredients inventory;
    Guild guild;

    // Pets array with indices 0-Dragon, 1-Phoenix, 2-Unicorn, 3-Griffin
    Pet *Pets[4] = {nullptr, nullptr, nullptr, nullptr};

public:
    void addScores(int s) { score += s; }
    Player() {}

    Player(Texture &dT, Texture &pT, Texture &uT, Texture &gT)
        : inventory()
    {
        // Initialize Pets array
        Pets[0] = new Dragon("Dragon_P", dT, Const_Pet_Pos[0][0], Const_Pet_size[0][0]);
        Pets[1] = new Phoenix("Phoenix_P", pT, Const_Pet_Pos[0][1], Const_Pet_size[0][1]);
        Pets[2] = new Unicorn("Unicorn_P", uT, Const_Pet_Pos[0][2], Const_Pet_size[0][2]);
        Pets[3] = new Griffin("Griffin_P", gT, Const_Pet_Pos[0][3], Const_Pet_size[0][3]);

      
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->setPosition(Const_Pet_Pos[0][i]);
            Pets[i]->set_sprite_size(Const_Pet_size[0][i]);
        }
    }

 

    ~Player()
    {
        for (int i = 0; i < 4; i++)
        {
            delete Pets[i];
        }
    }

    void Change_Position(int t)
    {
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->set_sprite_size(Const_Pet_size[t][i]);
            Pets[i]->setPosition(Const_Pet_Pos[t][i]);
        }
    }

    void Change_Position_O(int t)
    {
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->set_sprite_size(Const_Pet_size[t][i]);
            Pets[i]->set_Position_By_Origin(Const_Pet_Pos[t][i]);
        }
    }

    // Accessor methods
    string getUsername() const { return username; }
    int getCash() const { return cash; }

    void addCash(int amount) { cash += amount; }
    void subtractCash(int amount)
    {
        if (amount <= cash)
            cash -= amount;
        else
            cout << "Not enough cash!" << endl;
    }


    Pet &getPet(int PET_No)
    {
        if (PET_No >= 1 && PET_No <= 4)
        {
            return *Pets[PET_No - 1];
        }
        return *Pets[0]; // default to Dragon
    }

    Pet &get_selected_pet()
    {
        for (int i = 0; i < 4; i++)
        {
            if (Pets[i] && Pets[i]->isSelected())
            {
                return *Pets[i];
            }
        }
        return *Pets[0]; // default to Dragon
    }

    void setDragonTexture(Texture &T) { Pets[0]->setTexture(T); }
    void setPhoenixTexture(Texture &T) { Pets[1]->setTexture(T); }
    void setUnicornTexture(Texture &T) { Pets[2]->setTexture(T); }
    void setGriffinTexture(Texture &T) { Pets[3]->setTexture(T); }

    void showPlayerInfo()
    {
        cout << "Username: " << username << "\n";
        cout << "Cash: " << cash << "\n";
        cout << "Pets: \n";
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->printStats();
        }
    }

    // Inventory 
    void addHealingPotion(int count) { inventory.addHealingPotion(count); }
    void addManaPotion(int count) { inventory.addManaPotion(count); }
    void addBuffItem(int count) { inventory.addBuffItem(count); }
    void addShield(int count) { inventory.addShield(count); }

    void useHealingPotion(int PET_No)
    {
        if (Pets[PET_No - 1]->refill_Health() && PET_No >= 1 && PET_No <= 4)
        {
            inventory.useHealingPotion();
        }
    }

    void useManaPotion(int PET_No)
    {
        if (Pets[PET_No - 1]->refill_AttackPower() && PET_No >= 1 && PET_No <= 4)
        {
            inventory.useManaPotion();
        }
    }

    void Increment_Level(int PET_No)
    {
        cash += 100;
        if (PET_No >= 1 && PET_No <= 4)
        {
            Pets[PET_No - 1]->Increment_Level();
        }
    }

    void useBuffItem() { inventory.useBuffItem(); }
    void useShield() { inventory.useShield(); }


    // Drawing methods
    void draw(RenderWindow &window, Font *font)
    {
        draw_noPet(window, font);
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->draw(window, font);
        }
    }

    void draw_with_select_btn(RenderWindow &window, Font *font, int Limit_Select = 1)
    {
        draw_noPet(window, font);

       
        bool wasSelected[4];
        for (int i = 0; i < 4; i++)
        {
            wasSelected[i] = Pets[i]->isSelected();
            Pets[i]->draw(window, font);
        }

        bool clicked[4];
        for (int i = 0; i < 4; i++)
        {
            clicked[i] = Pets[i]->draw_select_button(window, Btn_T, *font);
        }

   
        int selectedCount = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Pets[i] && Pets[i]->isSelected())
                selectedCount++;
        }

        if (selectedCount > Limit_Select)
        {
            for (int i = 0; i < 4; i++)
            {
                if (wasSelected[i] == clicked[i])
                {
                    Pets[i]->Unselect();
                }
            }
        }
    }

    void draw_with_add_remove_btn(RenderWindow &window, Event &event, Font *font, int Limit_Add = 3)
    {
        draw_noPet(window, font);

        guild.draw_hud(window, font);
        guild.handleInput(event);
        guild.handleClick(window, event);
   

        // Store 
        bool wasAdded[4];
        for (int i = 0; i < 4; i++)
        {
            wasAdded[i] = Pets[i]->isAdded();
        }

        // Draw buttons 
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->draw(window, font);
            Pets[i]->draw_add_remove_button(window, Btn_T, *font);
        }

        // count how many pets are added
        int addedCount = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Pets[i]->isAdded())
                addedCount++;
        }

      
        if (addedCount > Limit_Add)
        {
            for (int i = 0; i < 4; i++)
            {
                if (wasAdded[i] == false && Pets[i]->isAdded())
                {
                    // remove pet 
                    guild.removePet(Pets[i]->getName());

                    Pets[i]->Remove();
                }
            }
        }

        // Add pets 
        for (int i = 0; i < 4; i++)
        {
            if (Pets[i]->isAdded() && !wasAdded[i])
            {
                
                guild.addPet(Pets[i]);
            }
            else if (wasAdded[i] && !Pets[i]->isAdded())
            {
                guild.removePet(Pets[i]->getName());
            }
        }
    }

    int get_no_of_Pet_selected()
    {
        int count = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Pets[i]->isSelected())
                count++;
        }
        return count;
    }

    void draw_1_PET(RenderWindow &window, Font *font, int option)
    {
        if (option >= 1 && option <= 4)
        {
            Pets[option - 1]->draw(window, font);
            Pets[option - 1]->write_Pet_factor(window, *font);
        }
    }

   
    void draw_noPet(RenderWindow &window, Font *font)
    {
        static Texture hudBackgroundTexture;
        static bool loaded = false;

        if (!loaded)
        {
            if (!hudBackgroundTexture.loadFromFile("Images/Hud.png"))
            {
                cerr << "Failed to load Images/Hud.png" << endl;
                return;
            }
            loaded = true;
        }

        Sprite hudBackgroundSprite;
        hudBackgroundSprite.setTexture(hudBackgroundTexture);
        hudBackgroundSprite.setPosition(20, 10);
        hudBackgroundSprite.setScale(0.4f, 0.4f); 

        window.draw(hudBackgroundSprite);

   
        Text usernameText;
        usernameText.setFont(*font);
        usernameText.setString(username);
        usernameText.setCharacterSize(32);
        usernameText.setFillColor(Color::White);
        usernameText.setStyle(Text::Bold);
        usernameText.setPosition(140, 47); 
        window.draw(usernameText);

      
        Text cashText;
        cashText.setFont(*font);
        cashText.setString(to_string(cash));
        cashText.setCharacterSize(24);
        cashText.setFillColor(Color(255, 215, 0));
        cashText.setStyle(Text::Bold);
        cashText.setPosition(120, 120); 
        window.draw(cashText);

    
        Text scoreText;
        scoreText.setFont(*font);
        scoreText.setString(to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::Cyan);
        scoreText.setStyle(Text::Bold);
        scoreText.setPosition(120, 165); 
        window.draw(scoreText);
        inventory.draw(window, font);
    }

    int handleInventoryItemClick(Event &event, float deltaTime)
    {
        static float timer = 0;

        timer += deltaTime;

        if (timer > 0.2f)
        {

            timer = 0;

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f M_P = Vector2f(event.mouseButton.x, event.mouseButton.y);

                if (10 <= M_P.y && M_P.y <= 60)
                {
                    if (1200 <= M_P.x && M_P.x <= 1250)
                        return 1;
                    else if (1300 <= M_P.x && M_P.x <= 1350)
                        return 2;
                    else if (1400 <= M_P.x && M_P.x <= 1450)
                        return 3;
                    else if (1500 <= M_P.x && M_P.x <= 1550)
                        return 4;
                }
            }
        }

        return 0;
    }

    friend ostream &operator<<(ostream &os, const Player &p)
    {
        os << p.username << endl;
        os << p.cash << endl;
        os << p.score << endl
           << endl;
        for (int i = 0; i < 4; i++)
        {
            if (p.Pets[i])
                os << *p.Pets[i] << endl;
        }

        os << p.inventory << endl;

        os << p.guild << endl;

        return os;
    }

    friend istream &operator>>(istream &in, Player &p)
    {
        in >> p.username;
        in >> p.cash;
        in >> p.score;
        for (int i = 0; i < 4; i++)
        {
            if (p.Pets[i])
                in >> *p.Pets[i];
        }
        in >> p.inventory;
        in >> p.guild;

        p.guild.Link_All_Pets(p.Pets);

        return in;
    }

    void update(float deltaTime)
    {
        for (int i = 0; i < 4; i++)
        {
            Pets[i]->update(deltaTime);
        }
    }
};

#endif

