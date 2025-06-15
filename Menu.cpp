
#include <SFML/Audio.hpp>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

using namespace sf;

#include "Menu.h"

#include "Helper.h"
// extern Helper helper;

extern const int size_X;
extern const int size_Y;

void Initial_Start_display(RenderWindow& window, const Texture& start_T, const Font& font, const Texture& backgroundTexture)
{
	Sprite start_S(start_T);
	start_S.setColor(Color(80, 0, 200));
	start_S.setPosition((size_X - start_S.getGlobalBounds().width) / 2.0,
		(size_Y - start_S.getGlobalBounds().height) / 2.0);

	Sprite background(backgroundTexture);
	background.setScale(float(size_X) / background.getTexture()->getSize().x,
		float(size_Y) / background.getTexture()->getSize().y);

	Text text;
	text.setFont(font);
	text.setString("START");
	text.setStyle(Text::Bold);
	text.setCharacterSize(start_S.getGlobalBounds().height * 0.7f);
	text.setFillColor(Color::White);

	FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
	text.setPosition(start_S.getPosition().x + start_S.getGlobalBounds().width / 2.f,
		start_S.getPosition().y + start_S.getGlobalBounds().height / 2.f);

	window.clear();
	window.draw(background);
	window.draw(start_S);
	window.draw(text);
	window.display();

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
			{
				if (start_S.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
				{
					cout << "Start Selected \n";
					return;
				}
			}
		}

		window.clear();
		window.draw(background);
		window.draw(start_S);
		window.draw(text);
		window.display();
	}
}

int Display_Login_Register_Menu(RenderWindow& window, const Texture& buttonTexture, const Font& font, const Texture& backgroundTexture)
{
	const int buttonCount = 4;
	const string labels[buttonCount] = { "Login", "Register","LeaderBoard" ,"EXIT" };
	const Vector2f buttonSize(300.f, 80.f);
	const float spacing = 30.f;

	//  background texture 
	Sprite background(backgroundTexture);
	background.setScale(float(size_X) / float(background.getTexture()->getSize().x),
		float(size_Y) / float(background.getTexture()->getSize().y));

	Sprite buttons[buttonCount];
	Text texts[buttonCount];

	Vector2f textureSize(buttonTexture.getSize());
	Vector2f scale(buttonSize.x / textureSize.x, buttonSize.y / textureSize.y);
	float totalHeight = buttonCount * buttonSize.y + (buttonCount - 1) * spacing;
	float startX = (size_X - buttonSize.x) / 2.f;
	float startY = (size_Y - totalHeight) / 2.f;

	// buttons and text
	for (int i = 0; i < buttonCount; ++i)
	{
		buttons[i].setTexture(buttonTexture); //  button texture for all buttons
		buttons[i].setScale(scale);
		buttons[i].setColor(Color(80, 0, 200));
		buttons[i].setPosition(startX, startY + i * (buttonSize.y + spacing));

		texts[i].setFont(font);
		texts[i].setString(labels[i]);
		texts[i].setStyle(Text::Bold);
		texts[i].setCharacterSize(buttonSize.y * 0.5f);
		texts[i].setFillColor(Color::White);

		FloatRect textBounds = texts[i].getLocalBounds();
		texts[i].setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
		texts[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2.f,
			buttons[i].getPosition().y + buttonSize.y / 2.f);
	}

	while (window.isOpen())
	{
		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
			{
				for (int i = 0; i < buttonCount; ++i)
				{
					if (buttons[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
					{
						cout << labels[i] << " Selected \n";

						if (i == 3)
						{
							window.close();
							return 0;
						}
						else
							return i + 1;
					}
				}
			}
		}

		window.clear();
		window.draw(background);
		for (int i = 0; i < buttonCount; ++i)
		{
			window.draw(buttons[i]);
			window.draw(texts[i]);
		}
		window.display();
	}

	return -1;
}

string showLoginScreen(RenderWindow& window, const Font& font, const Texture& backgroundTexture, const Texture& button_Texture, string error_Message)
{
	string username;

	Vector2u size = window.getSize();

	Sprite background(backgroundTexture);
	background.setScale(float(size.x) / background.getTexture()->getSize().x,
		float(size.y) / background.getTexture()->getSize().y);

	RectangleShape inputBox(Vector2f(400, 50));
	inputBox.setFillColor(Color(20, 20, 20, 180));
	inputBox.setOutlineThickness(2);
	inputBox.setOutlineColor(Color::White);
	inputBox.setPosition((size.x - inputBox.getSize().x) / 2.f, (size.y - inputBox.getSize().y) / 2.f);

	Text title("Enter Username", font, 36);
	title.setFillColor(Color::White);
	FloatRect titleBounds = title.getLocalBounds();
	title.setOrigin(titleBounds.width / 2.f, 0);
	title.setPosition(size.x / 2.f, inputBox.getPosition().y - 60);

	Text inputText("", font, 30);
	inputText.setFillColor(Color::Cyan);

	Text errorMessage(error_Message, font, 30);
	errorMessage.setFillColor(Color::Red);
	FloatRect errorBounds = errorMessage.getLocalBounds();
	errorMessage.setOrigin(errorBounds.width / 2.f, 0);
	errorMessage.setPosition(size.x / 2.f, inputBox.getPosition().y + inputBox.getSize().y + 10);

	Sprite exitButton;
	exitButton.setTexture(button_Texture);
	exitButton.setPosition(20, size.y - 60);
	exitButton.setColor(Color(80, 0, 200));
	exitButton.setScale(120.f / button_Texture.getSize().x, 40.f / button_Texture.getSize().y);

	Text exitText("Exit", font, 30);
	exitText.setFillColor(Color::White);
	exitText.setStyle(Text::Bold);
	FloatRect exitBounds = exitText.getLocalBounds();
	exitText.setOrigin(exitBounds.width / 2.f, exitBounds.height / 2.f);
	FloatRect btnBounds = exitButton.getGlobalBounds();
	exitText.setPosition(btnBounds.left + btnBounds.width / 2.f,
		btnBounds.top + btnBounds.height / 2.f - 8);

	bool enterPressed = false;

	while (window.isOpen() && !enterPressed)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::TextEntered)
			{
				if (event.text.unicode == '\b')
				{
					if (!username.empty())
						username.pop_back();
				}
				else if (event.text.unicode == '\r' || event.text.unicode == '\n')
				{
					if (!username.empty())
						enterPressed = true;
				}
				else if (event.text.unicode < 128 && isprint(event.text.unicode))
				{
					char ch = static_cast<char>(event.text.unicode);
					string invalidChars = "\\/:*?\"<>|. ";

					bool isValidChar = true;
					for (char c : invalidChars)
					{
						if (ch == c)
						{
							isValidChar = false;
							break;
						}
					}

					if (username.size() < 20 && isValidChar)
						username += ch;
				}
			}

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
				if (exitButton.getGlobalBounds().contains(mousePos))
				{
					window.close();
				}
			}
		}

		inputText.setString(username);
		FloatRect textBounds = inputText.getLocalBounds();
		inputText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
		inputText.setPosition(inputBox.getPosition().x + inputBox.getSize().x / 2.f,
			inputBox.getPosition().y + inputBox.getSize().y / 2.f);

		window.clear();
		window.draw(background);
		window.draw(title);
		window.draw(inputBox);
		window.draw(inputText);
		if (!error_Message.empty())
			window.draw(errorMessage);
		window.draw(exitButton);
		window.draw(exitText);
		window.display();
	}

	return username;
}


void setupButton(Sprite& button, Text& label, Texture* texture, Font& font, const string& textStr, Vector2f position, Vector2f size)
{
	button.setTexture(*texture);
	button.setScale(size.x / texture->getSize().x, size.y / texture->getSize().y);
	button.setPosition(position);

	label.setFont(font);
	label.setString(textStr);
	label.setStyle(Text::Bold);
	label.setCharacterSize(size.y * 0.5f);
	label.setFillColor(Color::White);

	FloatRect textBounds = label.getLocalBounds();
	label.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
	label.setPosition(position.x + size.x / 2.f, position.y + size.y / 2.f);
}