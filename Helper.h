


#ifndef HELPER_H
#define HELPER_H

#include "ForwardDeclarations.h"



class Helper
{
    RenderWindow *window;
    Font *font;
    Clock internalClock;
    float lastRingTime;

public:


    Helper();
    Helper(RenderWindow *w);
    void setWindow(RenderWindow *w);
    void setFont(Font *f);

    RenderWindow &getWindow();
    Font &getFont();
    void update();
    void draw();
    void drawCenteredText(string text, float posX, float posY, int size = 27, Color color = Color::White);
    int getClickedItemIndex(Event &event, Sprite items[], int itemCount, Clock &clickClock, float debounceTime = 0.3f);
    void centerTextInButton(Text &text, const FloatRect &buttonRect);
    void drawDotAt(Vector2f position, Color color = Color::Blue, float radius = 4.f);

    void drawRectBounds(FloatRect rect, Color color = Color::Blue, float thickness = 2.f);
    void drawTextAt(const string textStr, Vector2f position = Vector2f(300, 300), unsigned int fontSize = 20, Color color = Color::White);
};
extern Helper helper;

#endif