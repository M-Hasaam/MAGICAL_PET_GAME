
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include "Pet.h"
#include "Shop.h"
#include "Player.h"
#include "Filling.h"
#include "Menu.h"
#include "Bot.h"
#include "Battle.h"
#include "Helper.h"
#include "Projectiles.h"
#include "Training.h"
// struct Vector2f{
//     float x , y;
// };

using namespace sf;
using namespace std;

const int size_X = 1600;
const int size_Y = 900;

Helper helper;

Texture Btn_T;

int main()
{
    srand(time(0));

    // arrays for different types of textures , musics
    Texture backgroundTextures[10];
    Texture buttonTextures[5];
    Texture petTextures[5];
    Texture ProjectileTexture[5];
    Font font;
    Music musics[5];

    // Loading files
    if (false ||
        !backgroundTextures[0].loadFromFile("Images/background1.jpg") ||
        // !backgroundTextures[1].loadFromFile("Images/background2.png") ||
        // !backgroundTextures[2].loadFromFile("Images/background3.jpeg") ||
        // !backgroundTextures[3].loadFromFile("Images/background4.jpg") ||
        // !backgroundTextures[4].loadFromFile("Images/background5.jpg") ||
        // !backgroundTextures[5].loadFromFile("Images/background6.jpg") ||
        !backgroundTextures[6].loadFromFile("Images/background7.jpg") ||
        // !backgroundTextures[7].loadFromFile("Images/background8.jpg") ||
        // !backgroundTextures[8].loadFromFile("Images/background9.jpg") ||
        !buttonTextures[0].loadFromFile("Images/button1.png") ||
        // !buttonTextures[1].loadFromFile("Images/button2.png") ||
        !buttonTextures[2].loadFromFile("Images/button3.png") ||
        !buttonTextures[3].loadFromFile("Images/button4.png") ||
        !petTextures[0].loadFromFile("Images/dragon1.png") ||
        !petTextures[1].loadFromFile("Images/dragon2.png") ||
        !petTextures[2].loadFromFile("Images/phoenix1.png") ||
        !petTextures[3].loadFromFile("Images/griffin1.png") ||
        !musics[0].openFromFile("Music/risk.ogg") ||
        // !ProjectileTexture[0].loadFromFile("Images/fireball1.png")||
        !font.loadFromFile("Font/Arial.ttf"))
    {
        cout << "Error loading assets!" << endl;
        return -1;
    }

    Btn_T = buttonTextures[3];

    RenderWindow window(VideoMode(size_X, size_Y), "Magical Pets Kingdom Game");
    window.setFramerateLimit(60);

    helper.setWindow(&window);
    helper.setFont(&font);

    PlayerIngredients player_Ingredients;

    Sprite background(backgroundTextures[6]);
    background.setScale(float(size_X) / float(background.getTexture()->getSize().x),
        float(size_Y) / float(background.getTexture()->getSize().y));

    // enemy pets
    Pet* Four_Pets[4] = {
        new Dragon(petTextures[0]),
        new Unicorn(petTextures[1]),
        new Phoenix(petTextures[2]),
        new Griffin(petTextures[3]) };

    // player
    Player player(petTextures[0], petTextures[2], petTextures[1], petTextures[3]);
    Shop shop;

    FileManager FILE;
    {
        string username, Error_Message;
        Initial_Start_display(window, buttonTextures[0], font, backgroundTextures[0]);
        if (!window.isOpen())
            return 0;

        int Login_1_Register_2 = 0;
        do
        {
            // login page
            Login_1_Register_2 = Display_Login_Register_Menu(window, buttonTextures[0], font, backgroundTextures[0]);
            if (!window.isOpen())
                return 0;

            if (Login_1_Register_2 == 3)
                FILE.drawLeaderboardWindow(window, font, backgroundTextures[0], buttonTextures[0]);

        } while (Login_1_Register_2 == 3);

        while (true)
        {

            username = showLoginScreen(window, font, backgroundTextures[0], buttonTextures[0], Error_Message);

            if (!window.isOpen())
                return 0;

            if (Login_1_Register_2 == 1)
            {
                if (FILE.isRegistered(username))
                {
                    break;
                }
                else
                {
                    Error_Message = "UserName Not Register";
                }
            }
            else if (Login_1_Register_2 == 2)
            {
                if (FILE.isRegistered(username))
                {
                    Error_Message = "UserName Already Register";
                }
                else
                {
                    FILE.addNewPlayer(username);
                    break;
                }
            }
        }

        FILE.getPlayer(username, player); //, petTextures[0], petTextures[2], petTextures[3], petTextures[1], poisonTextures[0]);
    }

    BattleArena BATTLE(&player);
    TrainingArena TRAINING;

    /////////////////////////////////////////////////////////////////////////
    const int menuGroups = 7;
    const int buttonCounts[menuGroups] = { 5, 3, 4, 1, 1 };

    Sprite buttons[menuGroups][6]; // Use max size in second dimension
    Text buttonLabels[menuGroups][6];
    string buttonStrings[menuGroups][6] = {
        {"Battle", "Guild Section", "Training", "Feed Pets", "Shop", ""},
        {"1 vs 1", "2 vs 2", "Guild vs Guild", "", ""},
        {"Dragon", "Phoenix", "Unicorn", "Griffin", "", ""},
        {"Start Battle", "", "", "", "", ""},
        {"Change Roles", "", "", "", "", ""},
        {"Tank", "Healer", "Guardian", "Striker", "", ""} };

    Vector2f buttonSize = Vector2f(300.f, 70.f);
    float spacing = 30.f;
    float startX = 65.f;

    for (int g = 0; g < menuGroups; ++g)
    {
        int count = buttonCounts[g];
        float totalHeight = count * buttonSize.y + (count - 1) * spacing;
        float startY = (size_Y - totalHeight) / 2.f;

        for (int i = 0; i < count; ++i)
        {

            Vector2f pos(startX, startY + i * (buttonSize.y + spacing));
            if (buttonCounts[g] >= 6)
                pos.y += 100;
            else if (buttonCounts[g] >= 5)
                pos.y += 50;

            setupButton(buttons[g][i], buttonLabels[g][i], &buttonTextures[0], font, buttonStrings[g][i], pos, buttonSize);
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////
    Sprite BackBtn;
    Text BackBtnLablel;
    setupButton(BackBtn, BackBtnLablel, &buttonTextures[0], font, "<--   Back", Vector2f(20.f, size_Y - 100), Vector2f(200.f, 70.f));

    Sprite TrainingBtn;
    Text TrainingLablel;
    setupButton(TrainingBtn, TrainingLablel, &buttonTextures[0], font, "Start Training", Vector2f(400.f, size_Y - 150), Vector2f(300.f, 70.f));

    double STATE = 0;
    int PET_STATE = 0;
    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear();
        window.draw(background);

        // STATE to controll differnt windows
        int Button_Index = -1;
        {
            if (STATE == 0)
                Button_Index = 0;
            else if (STATE == 1)
                Button_Index = 1;
            else if (STATE == 11)
                Button_Index = 3;
            else if (STATE == 12)
                Button_Index = 3;
            else if (STATE == 3)
                Button_Index = 2;
            else if (STATE == 4)
                Button_Index = 2;
        }

        if (Button_Index != -1)
        {
            static Clock shopClickClock;
            Time elapsed = shopClickClock.getElapsedTime();

            if (elapsed.asMilliseconds() > 200)
            {

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    for (int indx_btn = 0; indx_btn < buttonCounts[Button_Index]; ++indx_btn)
                    {
                        if (buttons[Button_Index][indx_btn].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                        {
                            cout << "Button : [" << buttonStrings[Button_Index][indx_btn] << "] [" << Button_Index << "," << indx_btn << "] clicked!" << endl;

                            if (Button_Index == 0)
                            {

                                STATE = indx_btn + 1;

                                if (indx_btn == 0)
                                {
                                }
                                else if (indx_btn == 1)
                                {
                                }
                                else if (indx_btn == 2)
                                {
                                    PET_STATE = 1;
                                    player.Change_Position(2);
                                }
                                else if (indx_btn == 3)
                                {
                                    PET_STATE = 1;
                                    player.Change_Position(1);
                                }
                            }
                            else if (Button_Index == 1)
                            {
                                if (indx_btn == 0)
                                {
                                    STATE = 11;
                                    // PET_STATE = 1;
                                    // player.Change_Position(1);
                                }
                                if (indx_btn == 1)
                                {
                                    STATE = 12;
                                    // PET_STATE = 1;
                                    // player.Change_Position(1);
                                }
                                if (indx_btn == 2)
                                {
                                    if (player.guild.No_of_Added_Pets() > 0)
                                    {
                                        STATE = 13;
                                        BATTLE.prepare_G_V_G(player, Four_Pets);
                                        musics[0].play();
                                    }
                                }
                            }
                            else if (Button_Index == 2)
                            {
                                PET_STATE = indx_btn + 1;
                            }
                            else if (Button_Index == 3)
                            {
                                if (STATE == 11)
                                {
                                    if (indx_btn == 0 && player.get_no_of_Pet_selected() == 1)
                                    {
                                        if (player.get_no_of_Pet_selected() == 1)
                                        {

                                            player.Change_Position(3);

                                            // BATTLE.prepare_1_V_1(&player.get_selected_pet(), Four_Pets[rand() % 4]);
                                            BATTLE.prepare_1_V_1(player, Four_Pets);
                                            // AI_Bot.set_Enemy(Four_Pets[rand() % 4]);
                                            // AI_Bot.set_Player(&player.get_selected_pet());
                                            STATE = 111;

                                            musics[0].play();
                                        }
                                    }
                                }
                                if (STATE == 12)
                                {
                                    if (indx_btn == 0 && player.get_no_of_Pet_selected() == 2)
                                    {
                                        cout << "if (STATE == 12){if (indx_btn == 0)\n ";
                                        //   if (player.get_no_of_Pet_selected() == 2)
                                        // {

                                        player.Change_Position(3);
                                        cout << "player.Change_Position(3);\n";

                                        BATTLE.prepare_2_V_2(player, Four_Pets);
                                        cout << "BATTLE.prepare_2_V_2(player, Four_Pets);XX\n";
                                        // AI_Bot.set_Enemy(Four_Pets[rand() % 4]);
                                        // AI_Bot.set_Player(&player.get_selected_pet());
                                        STATE = 121;

                                        musics[0].play();
                                        // }
                                    }
                                }
                            }
                        }
                    }
                }
                shopClickClock.restart();
            }

            for (int i = 0; i < buttonCounts[Button_Index]; ++i)
            {
                window.draw(buttons[Button_Index][i]);
                window.draw(buttonLabels[Button_Index][i]);
            }
        }

        if (STATE == 0)
        {
            player.update(deltaTime);
            player.draw(window, &font);
        }
        else if (STATE == 1)
        {
            player.update(deltaTime);
            player.draw(window, &font);
        }
        else if (STATE == 11)
        {
            player.update(deltaTime);
            player.draw_with_select_btn(window, &font);
        }
        else if (STATE == 12)
        {
            player.update(deltaTime);
            player.draw_with_select_btn(window, &font, 2);
        }
        else if (STATE == 111) // 1 V 1 Battle
        {

            player.draw_noPet(window, &font);

            BATTLE.update_controll(window, event, font, deltaTime);
        }
        else if (STATE == 121) // 2 V 2 Battle
        {

            player.draw_noPet(window, &font);

            BATTLE.update_controll(window, event, font, deltaTime);
        }
        else if (STATE == 13) // G V G Battle
        {
            player.draw_noPet(window, &font);

            BATTLE.update_controll(window, event, font, deltaTime);
        }
        else if (STATE == 2)
        {

            // player.draw_noPet(window, &font);
            player.draw_with_add_remove_btn(window, event, &font, 4);
            player.update(deltaTime);
            // helper.drawCenteredText("GUILD", 900, 100, 100);
        }
        else if (STATE == 3)
        {
            player.update(deltaTime);
            player.draw_noPet(window, &font);
            player.draw_1_PET(window, &font, PET_STATE);
        }
        else if (STATE == 31)
        {
            TRAINING.update_trainingCamp(window, event, font, deltaTime);
        }
        else if (STATE == 4) // FEED PET
        {

            player.update(deltaTime);
            player.draw_noPet(window, &font);
            player.draw_1_PET(window, &font, PET_STATE);

            int IngOption = player.handleInventoryItemClick(event, deltaTime);

            if (IngOption != 0)
            {

                if (IngOption == 1)
                {
                    cout << " Health \n";
                    player.useHealingPotion(PET_STATE);
                }
                if (IngOption == 2)
                {
                    cout << " Mana \n";
                    player.useManaPotion(PET_STATE);
                }
                if (IngOption == 3)
                {
                    cout << " BUff \n";
                }
                if (IngOption == 4)
                {
                    cout << " Sheild \n";
                }
            }
        }
        else if (STATE == 5) // SHOP
        {
            static Clock shopClickClock;
            Time elapsed = shopClickClock.getElapsedTime();

            player.draw_noPet(window, &font);

            const int C_cash[4] = { 50, 60, 75, 95 };
            int Item_Option = shop.draw(window, &buttonTextures[0], &event, &font);

            if (Item_Option != 0 && elapsed.asMilliseconds() > 300)
            {
                player.subtractCash(C_cash[Item_Option - 1]);

                if (Item_Option == 1)
                    player.addHealingPotion(1);
                if (Item_Option == 2)
                    player.addManaPotion(1);
                if (Item_Option == 3)
                    player.addBuffItem(1);
                if (Item_Option == 4)
                    player.addShield(1);

                shopClickClock.restart();
            }
        }
        else if (STATE == 6)
        {
            window.close();
        }

        if (STATE == 3)
        {
            window.draw(TrainingBtn);
            window.draw(TrainingLablel);

            // Training btn
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                static Clock shopClickClock;
                Time elapsed = shopClickClock.getElapsedTime();

                if (elapsed.asMilliseconds() > 1000)
                {
                    if (TrainingBtn.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                    {
                        cout << " training  clicked!" << endl;

                        STATE = 31;
                        musics[0].play();

                        TRAINING.prepare_Training(&player.getPet(PET_STATE));

                    }
                }
            }
        }

        if (STATE != 0) // Back Button
        {
            window.draw(BackBtn);
            window.draw(BackBtnLablel);

            static float backBtnTimer = 0.f;

            backBtnTimer += deltaTime;

            if (backBtnTimer >= 0.2f && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {

                if (BackBtn.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                {
                    backBtnTimer = 0.f;

                    cout << " Back Button  clicked!" << endl;

                    if (STATE == 31 && TRAINING.GAME)
                    {
                        TRAINING.GAME = false;



                        for (int i = 0; i < 4; i++)
                            player.Pets[i]->reset_factors();
                    }
                    else if ((STATE == 111 || STATE == 121 || STATE == 13) && BATTLE.GAME)
                    {
                        BATTLE.GAME = false;

                        for (int i = 0; i < 4; i++)
                            player.Pets[i]->reset_factors();
                    }
                    else
                    {
                        STATE = 0;
                        PET_STATE = 0;
                        BATTLE.reset();
                        TRAINING.reset();
                        player.Change_Position(0);
                    }

                    for (int i = 0; i < 1; i++)
                        musics[i].stop();

                }
            }
        }


        helper.update();
        helper.draw();
        window.display();
    }

    FILE.addPlayer(player);

    musics[0].stop();
    for (int i = 0; i < 4; ++i)
        delete Four_Pets[i];

    cout << "\n\n ==END==\n";

    return 0;
}







//#include <SFML/Graphics.hpp>

//using namespace sf;
//
//int main() {
//    RenderWindow window(VideoMode(800, 600), "SFML Sample");
//
//    CircleShape circle(50);
//    circle.setFillColor(Color::Red);
//    circle.setPosition(375, 275);
//
//    while (window.isOpen()) {
//        Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == Event::Closed)
//                window.close();
//        }
//
//        if (Keyboard::isKeyPressed(Keyboard::Left))  circle.move(-5, 0);
//        if (Keyboard::isKeyPressed(Keyboard::Right)) circle.move(5, 0);
//        if (Keyboard::isKeyPressed(Keyboard::Up))    circle.move(0, -5);
//        if (Keyboard::isKeyPressed(Keyboard::Down))  circle.move(0, 5);
//
//        window.clear(Color::Black);
//        window.draw(circle);
//        window.display();
//    }
//
//    return 0;
//}
