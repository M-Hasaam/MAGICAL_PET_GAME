// Filling.h
#ifndef FILLING_H
#define FILLING_H

#include <string>
#include "Player.h"
#include <SFML/Graphics.hpp>

using namespace std;

class FileManager
{
    const string MainFolder = "Data";

public:
    void addPlayer(Player &P);
    bool isRegistered(const string &username);
    void addNewPlayer(const string &username);
    bool getPlayer(const string &username, Player &P); //, Texture dT,Texture pT,Texture uT,Texture gT,Texture iT);
                                                       // bool getPlayer(const string username, Player &P);

    Player *loadLeaderBoard(int &outCount)
    {
        ifstream inFile("Data/LeaderBoard/leaderBoard.txt");
        if (!inFile)
        {
            outCount = 0;
            return nullptr;
        }

        int capacity = 10;
        outCount = 0;
        Player *players = new Player[capacity];

        while (inFile >> players[outCount].username >> players[outCount].score)
        {
            outCount++;
            if (outCount >= capacity)
            {
                capacity *= 2;
                Player *temp = new Player[capacity];
                for (int i = 0; i < outCount; i++)
                    temp[i] = players[i];
                delete[] players;
                players = temp;
            }
        }
        inFile.close();
        return players;
    }

    void saveLeaderBoard(Player &P)
    {
        int count = 0;
        Player *players = loadLeaderBoard(count);

        bool found = false;
        for (int i = 0; i < count; i++)
        {
            if (players[i].username == P.username)
            {
                players[i].score = P.score;
                found = true;
                break;
            }
        }

        if (!found)
        {
            Player *temp = new Player[count + 1];
            for (int i = 0; i < count; i++)
                temp[i] = players[i];
            temp[count].username = P.username;
            temp[count].score = P.score;
            delete[] players;
            players = temp;
            count++;
        }

        for (int i = 0; i < count - 1; i++)
        {
            for (int j = 0; j < count - i - 1; j++)
            {
                if (players[j].score < players[j + 1].score)
                {
                    Player temp = players[j];
                    players[j] = players[j + 1];
                    players[j + 1] = temp;
                }
            }
        }

        ofstream outFile("Data/LeaderBoard/leaderBoard.txt");
        for (int i = 0; i < count; i++)
        {
            outFile << players[i].username << " " << players[i].score << endl;
        }
        outFile.close();

        delete[] players;
    }

    void drawLeaderboardWindow(RenderWindow &window, Font &font, Texture &backgroundTexture, const Texture &button_Texture)
    {
        int count;
        Player *players = loadLeaderBoard(count);

        Sprite backgroundSprite(backgroundTexture);
        backgroundSprite.setScale(float(window.getSize().x) / backgroundTexture.getSize().x,
                                  float(window.getSize().y) / backgroundTexture.getSize().y);

        Sprite exitButton;
        exitButton.setTexture(button_Texture);
        exitButton.setPosition(20, window.getSize().y - 60);
        exitButton.setScale(120.f / button_Texture.getSize().x, 40.f / button_Texture.getSize().y);

        Text exitText("Exit", font, 30);
        exitText.setFillColor(Color::White);
        exitText.setStyle(Text::Bold);
        FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setOrigin(exitBounds.width / 2.f, exitBounds.height / 2.f);
        FloatRect btnBounds = exitButton.getGlobalBounds();
        exitText.setPosition(btnBounds.left + btnBounds.width / 2.f, btnBounds.top + btnBounds.height / 2.f - 8);

        RectangleShape leaderboardPanel;
        leaderboardPanel.setSize(Vector2f(400, 250));
        leaderboardPanel.setPosition(window.getSize().x / 2 - leaderboardPanel.getSize().x / 2, window.getSize().y / 2 - leaderboardPanel.getSize().y / 2);
        leaderboardPanel.setFillColor(Color(20, 20, 50, 220));
        leaderboardPanel.setOutlineThickness(3);
        leaderboardPanel.setOutlineColor(Color(180, 180, 255));

        Text titleText;
        titleText.setFont(font);
        titleText.setString("TOP SCORES");
        titleText.setCharacterSize(40);
        titleText.setStyle(Text::Bold);
        titleText.setFillColor(Color::White);
        titleText.setPosition(window.getSize().x / 2 - titleText.getLocalBounds().width / 2, window.getSize().y / 2 - leaderboardPanel.getSize().y / 2 - 40);

        Text rankHeader, nameHeader, scoreHeader;
        rankHeader.setFont(font);
        nameHeader.setFont(font);
        scoreHeader.setFont(font);

        rankHeader.setString("RANK");
        nameHeader.setString("PLAYER");
        scoreHeader.setString("SCORE");

        rankHeader.setCharacterSize(24);
        nameHeader.setCharacterSize(24);
        scoreHeader.setCharacterSize(24);

        rankHeader.setStyle(Text::Bold);
        nameHeader.setStyle(Text::Bold);
        scoreHeader.setStyle(Text::Bold);

        rankHeader.setFillColor(Color::White);
        nameHeader.setFillColor(Color::White);
        scoreHeader.setFillColor(Color::White);

        rankHeader.setPosition(leaderboardPanel.getPosition().x + 50, leaderboardPanel.getPosition().y + 20);
        nameHeader.setPosition(leaderboardPanel.getPosition().x + 150, leaderboardPanel.getPosition().y + 20);
        scoreHeader.setPosition(leaderboardPanel.getPosition().x + 300, leaderboardPanel.getPosition().y + 20);

        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                else if (event.type == Event::MouseButtonPressed)
                {
                    if (exitButton.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
                    {
                        delete[] players;
                        return;
                    }
                }
                else if (event.type == Event::KeyPressed)
                {
                    if (event.key.code == Keyboard::Escape)
                    {
                        delete[] players;
                        return;
                    }
                }
            }

            window.clear();
            window.draw(backgroundSprite);
            window.draw(leaderboardPanel);
            window.draw(titleText);
            window.draw(rankHeader);
            window.draw(nameHeader);
            window.draw(scoreHeader);

            for (int i = 0; i < 3 && i < count; i++)
            {
                float yPos = leaderboardPanel.getPosition().y + 50 + i * 50;

                Text rankText;
                rankText.setFont(font);
                rankText.setCharacterSize(24);
                rankText.setString(to_string(i + 1));
                rankText.setPosition(leaderboardPanel.getPosition().x + 50, yPos + 5);

                Text usernameText;
                usernameText.setFont(font);
                usernameText.setCharacterSize(24);
                usernameText.setFillColor(Color::White);
                usernameText.setString(players[i].username);
                usernameText.setPosition(leaderboardPanel.getPosition().x + 150, yPos + 5);

                Text scoreText;
                scoreText.setFont(font);
                scoreText.setCharacterSize(24);
                scoreText.setFillColor(Color::White);
                scoreText.setString(to_string(players[i].score));
                scoreText.setPosition(leaderboardPanel.getPosition().x + 300, yPos + 5);

                window.draw(rankText);
                window.draw(usernameText);
                window.draw(scoreText);
            }

            window.draw(exitButton);
            window.draw(exitText);

            window.display();
        }

        delete[] players;
    }
};

#endif
