#ifndef SHOP_H
#define SHOP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Object.h"

#include "Helper.h"
// extern Helper helper;
// #include "Player.h"

using namespace std;
using namespace sf;

class Item : public Object
{
public:
    string name;
    int cost;

    Item() : name("null"), cost(0), Object() {}

    Item(string name, int cost, Texture tex, Vector2f position, Vector2f size)
        : Object(tex, position, size, 0.07f), name(name), cost(cost) {}

    virtual ~Item() {}

    string getName() { return name; }
    int getCost() { return cost; }

    virtual void use() = 0;

    void draw(RenderWindow &window)
    {
        Object::draw(window);
    }

    void update(float deltaTime)
    {
        updateSpriteMovement(deltaTime);
    }
};

class HealingPotion : public Item
{
public:
    HealingPotion() {}

    HealingPotion(Texture tex, Vector2f position, Vector2f size)
        : Item("Healing Potion", 30, tex, position, size)
    {
        // setIntRect_grid(3, 3, 2, 2);
    }

    void use() override
    {
        cout << name << " used: Pet healed!\n";
    }
};

class ManaPotion : public Item
{
public:
    ManaPotion() {}

    ManaPotion(Texture tex, Vector2f position, Vector2f size)
        : Item("Mana Potion", 25, tex, position, size)
    {
        // setIntRect_grid(3, 3, 0, 0);
    }

    void use() override
    {
        cout << name << " used: Ability points restored!\n";
    }
};

class BuffItem : public Item
{
public:
    BuffItem() {}

    BuffItem(Texture tex, Vector2f position, Vector2f size)
        : Item("Buff Item", 40, tex, position, size)
    {
        // setIntRect_grid(3, 3, 2, 1);
    }

    void use() override
    {
        cout << name << " used: Attributes boosted!\n";
    }
};

class Shield : public Item
{
public:
    Shield() {}

    Shield(Texture tex, Vector2f position, Vector2f size)
        : Item("Shield", 50, tex, position, size)
    {
        // setIntRect_grid(3, 3, 2, 1);
    }

    void use() override
    {
        cout << name << " used: Damage reduced!\n";
    }
};

class Shop
{
protected:
    Item *Potions[4] = {nullptr};
    Texture Tex[4];
    const int C_cash[4] = {50, 60, 75, 95};

public:
    Shop()
    {
        if (!Tex[0].loadFromFile("Images/potion1.png") ||
            !Tex[1].loadFromFile("Images/potion2.png") ||
            !Tex[2].loadFromFile("Images/potion3.png") ||
            !Tex[3].loadFromFile("Images/sheild1.png"))
        {
            cout << "Potion Textures not fully loaded\n";
        }

        Potions[0] = new HealingPotion(Tex[0], Vector2f(800 - 250 - 125, 400), Vector2f(400 / 3, 400 / 3));
        Potions[1] = new ManaPotion(Tex[1], Vector2f(800 - 125, 400), Vector2f(400 / 3, 400 / 3));
        Potions[2] = new BuffItem(Tex[2], Vector2f(800 + 125, 400), Vector2f(400 / 3, 400 / 3));
        Potions[3] = new Shield(Tex[3], Vector2f(800 + 250 + 125, 400), Vector2f(400 / 3, 400 / 3));
    }

    void draw(RenderWindow &window)
    {
        for (int i = 0; i < 4; i++)
            if (Potions[i])
                Potions[i]->draw(window);
    }

    int draw(RenderWindow &window, Texture *button_Texture, Event *event, Font *font)
    {
        draw(window);
        const int Btn_height = 550;

        if (button_Texture && font)
        {
            Sprite buttons[4];
            for (int i = 0; i < 4; ++i)
            {
                buttons[i].setTexture(*button_Texture);
                buttons[i].setScale(0.5f, 0.5f);
            }

            buttons[0].setPosition(800 - 250 - 125 - 20, Btn_height); // healing
            buttons[1].setPosition(800 - 125 - 5, Btn_height);        // mana
            buttons[2].setPosition(800 + 125, Btn_height);            // buff
            buttons[3].setPosition(800 + 250 + 125 + 2, Btn_height);  // shield

            for (int i = 0; i < 4; ++i)
                window.draw(buttons[i]);

            Text price;
            price.setFont(*font);
            price.setCharacterSize(30);
            price.setFillColor(Color::White);

            for (int i = 0; i < 4; ++i)
            {
                price.setString(to_string(C_cash[i]) + "$");
                price.setOrigin(price.getLocalBounds().width / 2, price.getLocalBounds().height / 2);
                price.setPosition(buttons[i].getPosition().x + buttons[i].getGlobalBounds().width / 2,
                                  buttons[i].getPosition().y + buttons[i].getGlobalBounds().height / 2 - 5);
                window.draw(price);
            }

            if (event->type == Event::MouseButtonPressed && event->mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
                for (int i = 0; i < 4; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos))
                    {
                        cout << "Item Purchased: " << i << endl;
                        return i + 1;
                    }
                }
            }
        }

        return 0;
    }

    Item *getItem(int index)
    {
        if (index >= 0 && index < 4)
            return Potions[index];
        return nullptr;
    }

    ~Shop()
    {
        for (int i = 0; i < 4; ++i)
            delete Potions[i];
    }
};

class PlayerIngredients
{
public:
    int PotionCount[4] = {0, 0, 0, 0};

    Item *Potions[4] = {nullptr};
    Texture Tex[4];

   
    PlayerIngredients()
    {

        if (!Tex[0].loadFromFile("Images/potion1.png") ||
            !Tex[1].loadFromFile("Images/potion2.png") ||
            !Tex[2].loadFromFile("Images/potion3.png") ||
            !Tex[3].loadFromFile("Images/sheild1.png"))
        {
            cout << "Potion Textures not fully loaded\n";
        }

        Potions[0] = new HealingPotion(Tex[0], Vector2f(1200, 10), Vector2f(50, 50));
        Potions[1] = new ManaPotion(Tex[1], Vector2f(1300, 10), Vector2f(50, 50));
        Potions[2] = new BuffItem(Tex[2], Vector2f(1400, 10), Vector2f(50, 50));
        Potions[3] = new Shield(Tex[3], Vector2f(1500 - 3, 10), Vector2f(50, 50));
    }

    void set_Ingredient_count(int h, int m, int b, int s)
    {
        PotionCount[0] = h;
        PotionCount[1] = m;
        PotionCount[2] = b;
        PotionCount[3] = s;
    }

    void addHealingPotion(int count) { PotionCount[0] += count; }
    void addManaPotion(int count) { PotionCount[1] += count; }
    void addBuffItem(int count) { PotionCount[2] += count; }
    void addShield(int count) { PotionCount[3] += count; }

    bool usePotion(int idx)
    {
        if (PotionCount[idx] > 0)
        {
            PotionCount[idx]--;
            cout << Potions[idx]->name << "-used\n";
            // Potions[0].use();
            return true;
        }
        else
            cout << "No  Potions left!\n";
        return false;
    }
    bool useHealingPotion()
    {
        return usePotion(0);
    }

    bool useManaPotion()
    {
        return usePotion(1);
    }

    bool useBuffItem()
    {
        return usePotion(2);
    }

    bool useShield()
    {
        return usePotion(3);
    }

  

    void draw(RenderWindow &window, Font *font)
    {

        for (int i = 0; i < 4; i++)
        {
            if (Potions[i])
                Potions[i]->draw(window);
        }

 
        if (font)
        {
            Text CountText;
            CountText.setFont(*font);
            CountText.setCharacterSize(20);
            CountText.setFillColor(Color::White);

            CountText.setString(to_string(PotionCount[0]));
            CountText.setPosition(1215, 60);
            window.draw(CountText);

            CountText.setString(to_string(PotionCount[1]));
            CountText.setPosition(1315, 60);
            window.draw(CountText);

            CountText.setString(to_string(PotionCount[2]));
            CountText.setPosition(1417, 60);
            window.draw(CountText);

            CountText.setString(to_string(PotionCount[3]));
            CountText.setPosition(1518, 60);
            window.draw(CountText);
        }
    }

    friend ostream &operator<<(ostream &os, const PlayerIngredients &p)
    {
        os << p.PotionCount[0] << endl;
        os << p.PotionCount[1] << endl;
        os << p.PotionCount[2] << endl;
        os << p.PotionCount[3] << endl;
        return os;
    }

    friend istream &operator>>(istream &in, PlayerIngredients &p)
    {
        in >> p.PotionCount[0];
        in >> p.PotionCount[1];
        in >> p.PotionCount[2];
        in >> p.PotionCount[3];
        return in;
    }

    int getHealingCount() const { return PotionCount[0]; }
    int getManaCount() const { return PotionCount[1]; }
    int getBuffCount() const { return PotionCount[2]; }
    int getShieldCount() const { return PotionCount[3]; }

 
};

#endif
