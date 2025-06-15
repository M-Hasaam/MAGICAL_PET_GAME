#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <cstring>
using namespace sf;
using namespace std;

void Initial_Start_display(RenderWindow &window, const Texture &start_T, const Font &font, const Texture &backgroundTexture);
int Display_Login_Register_Menu(RenderWindow &window, const Texture &buttonTexture, const Font &font, const Texture &backgroundTexture);
string showLoginScreen(RenderWindow &window, const Font &font, const Texture &backgroundTexture, const Texture &button_Texture, string error_Message);
void setupButton(Sprite &button, Text &label, Texture *texture, Font &font, const string &textStr, Vector2f position, Vector2f size);

#endif
