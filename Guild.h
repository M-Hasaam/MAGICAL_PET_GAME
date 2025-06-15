#ifndef GUILD_H
#define GUILD_H

#include "Pet.h"
#include "ForwardDeclarations.h"

// #include "UIhelper.h"
// extern Helper helper;

class Guild
{
public:
    string guildName;
    Pet *pets[4] = {0, 0, 0, 0};
    string names[4];
    string roles[4];
    int petCount = 0;

 

    RectangleShape nameBox;
    RectangleShape roleButtons[4];       // Array of buttons for editing roles
    RectangleShape roleOptionButtons[4]; // Buttons for the four role options
    bool editingName = false;
    int editingRole = -1; // -1 means not editing any role
    Clock inputClock;
    float inputDelay = 0.3f; //

public:
    Guild(string name = "Unnamed Guild") : guildName(name), petCount(0)
    {
        for (int i = 0; i < 4; ++i)
        {
            pets[i] = nullptr;
            roles[i] = "None";
        }
    }

    bool addPet(Pet *pet)
    {
        if (petCount >= 4 || pet == nullptr)
            return false;

        pets[petCount] = pet;
        roles[petCount] = pet->getRole();
        ++petCount;
        return true;
    }

    int No_of_Added_Pets() { return petCount; }

    bool removePet(const string &name)
    {
        for (int i = 0; i < petCount; ++i)
        {
            if (pets[i] && pets[i]->getName() == name)
            {
                for (int j = i; j < petCount - 1; ++j)
                {
                    pets[j] = pets[j + 1];
                    roles[j] = roles[j + 1];
                }
                pets[petCount - 1] = nullptr;
                roles[petCount - 1] = "None";
                --petCount;
                return true;
            }
        }
        return false;
    }

  

    friend ostream &operator<<(ostream &os, const Guild &g)
    {
        os << g.guildName << endl;
        os << g.petCount << endl;



        return os;
    }

    void Link_All_Pets(Pet *PP[4])
    {
        petCount = 0;
        for (int i = 0; i < 4; i++)
        {
            if (PP[i])
                cout << PP[i]->getName() << " - " << PP[i]->isAdded() << endl;

            if (PP[i] && PP[i]->isAdded())
                pets[petCount++] = PP[i];
        }
    }

    friend istream &operator>>(istream &is, Guild &g)
    {
        is >> g.guildName; // Read full line for guild name

        is >> g.petCount;
       
        return is;
    }
   

    void draw_hud(RenderWindow &window, Font *font, float offsetY = 250)
    {
        RectangleShape guildBox(Vector2f(380, 200));
        guildBox.setPosition(20, offsetY);
        guildBox.setFillColor(Color(0, 0, 0, 170));
        guildBox.setOutlineColor(Color::White);
        guildBox.setOutlineThickness(2);
        window.draw(guildBox);

        nameBox.setSize(Vector2f(310, 35));
        nameBox.setPosition(guildBox.getPosition().x + 20, guildBox.getPosition().y + 10);
        nameBox.setFillColor(editingName ? Color(50, 50, 50, 180) : Color(20, 20, 20, 180));
        nameBox.setOutlineColor(Color::White);
        nameBox.setOutlineThickness(1);
        window.draw(nameBox);

        Text gNameText;
        gNameText.setFont(*font);
        gNameText.setString("Guild: " + guildName + (editingName ? "|" : ""));
        gNameText.setCharacterSize(22);
        gNameText.setFillColor(Color::White);
        gNameText.setPosition(nameBox.getPosition().x + 10, nameBox.getPosition().y + 5);
        window.draw(gNameText);

        // Draw pets inside box
        float startY = guildBox.getPosition().y + 60;
        for (int i = 0; i < petCount && i < 4; i++)
        {
            if (!pets[i])
                continue;

            string petLine = pets[i]->getName() + " (Lv." + to_string(int(pets[i]->getLevel())) + ") - " + pets[i]->getRole();

            Text petText;
            petText.setFont(*font);
            petText.setCharacterSize(20);
            petText.setString(petLine);
            petText.setFillColor(Color::Cyan);
            petText.setPosition(guildBox.getPosition().x + 20, startY + i * 30);

            Text petShadow = petText;
            petShadow.setFillColor(Color(50, 50, 50, 200));
            petShadow.setPosition(petText.getPosition().x + 2, petText.getPosition().y + 2);

            window.draw(petShadow);
            window.draw(petText);
        }
    }
    void handleClick(RenderWindow &window, Event &event)
    {
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
        {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (nameBox.getGlobalBounds().contains(mousePos))
                editingName = true;
            else
                editingName = false;
        }
    }

    void handleInput(Event event)
    {
        if (!editingName)
            return;

     
        if (inputClock.getElapsedTime().asSeconds() < inputDelay)
            return;

        if (event.type == Event::TextEntered)
        {
            if (event.text.unicode == 8) // Backspace
            {
                if (!guildName.empty())
                    guildName.pop_back();
            }
            else if (event.text.unicode < 128 && isprint(event.text.unicode))
            {
                if (guildName.size() < 18)
                    guildName += static_cast<char>(event.text.unicode);
            }

            // Reset clock after valid key
            inputClock.restart();
        }
    }
};

#endif

