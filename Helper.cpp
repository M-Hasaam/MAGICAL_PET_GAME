
#include "Helper.h"

Helper::Helper()
    : window(nullptr), font(nullptr), lastRingTime(0.0f) {}

Helper::Helper(RenderWindow *w)
    : window(w), font(nullptr), lastRingTime(0.0f) {}

void Helper::setWindow(RenderWindow *w)
{
    window = w;
    // projectile = ProjectileManager(w->getSize());
    // fireballs = ObjectProjectileManager(w->getSize());
}

void Helper::setFont(Font *f) { font = f; }

RenderWindow &Helper::getWindow() { return *window; }
Font &Helper::getFont() { return *font; }

void Helper::update()
{
    float dt = internalClock.restart().asSeconds();
    // wave.update(dt);
    // projectile.update(dt);
    // fireballs.update(dt);
}

void Helper::draw()
{
    // wave.draw(*window);
    // projectile.draw(*window);
    // fireballs.draw(*window);
}

void Helper::drawCenteredText(string text, float posX, float posY, int size, Color color)
{
    Text t;
    t.setFont(*font);
    t.setString(text);
    t.setStyle(Text::Bold);
    t.setCharacterSize(size);
    t.setFillColor(color);

    FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2);
    t.setPosition(posX, posY);

    window->draw(t);
}

int Helper::getClickedItemIndex(Event &event, Sprite items[], int itemCount, Clock &clickClock, float debounceTime)
{
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
    {
        if (clickClock.getElapsedTime().asSeconds() < debounceTime)
            return -1;

        Vector2f mousePos = window->mapPixelToCoords(Mouse::getPosition(*window));

        for (int i = 0; i < itemCount; ++i)
        {
            FloatRect bounds = items[i].getGlobalBounds();

            IntRect texRect = items[i].getTextureRect();
            float textureRatioX = static_cast<float>(texRect.width) / items[i].getTexture()->getSize().x;
            float textureRatioY = static_cast<float>(texRect.height) / items[i].getTexture()->getSize().y;

            bounds.width *= textureRatioX;
            bounds.height *= textureRatioY;

            if (bounds.contains(mousePos))
            {
                clickClock.restart();
                return i;
            }
        }
    }
    return -1;
}

void Helper::centerTextInButton(Text &text, const FloatRect &buttonRect)
{
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);

    float centerX = buttonRect.left + buttonRect.width / 2.f;
    float centerY = buttonRect.top + buttonRect.height / 2.f;

    text.setPosition(centerX, centerY);
}

void Helper::drawDotAt(Vector2f position, Color color, float radius)
{
    CircleShape dot(radius);
    dot.setFillColor(color);
    dot.setOrigin(radius, radius);
    dot.setPosition(position);
    window->draw(dot);
}

void Helper::drawRectBounds(FloatRect rect, Color color, float thickness)
{
    RectangleShape outline;
    outline.setPosition(rect.left, rect.top);
    outline.setSize(Vector2f(rect.width, rect.height));
    outline.setFillColor(Color::Transparent);
    outline.setOutlineColor(color);
    outline.setOutlineThickness(thickness);
    window->draw(outline);
}

void Helper::drawTextAt(const string textStr, Vector2f position, unsigned int fontSize, Color color)
{
    Text text;
    text.setFont(*font);
    text.setString(textStr);
    text.setCharacterSize(fontSize);
    text.setFillColor(color);
    text.setPosition(position);
    window->draw(text);
}

// void Helper::shootProjectile(Vector2f pos, int direction, Pet *P, float radius, float speed, float damage_fact)
// {
//     projectile.addProjectile(pos, direction, P, radius, speed, damage_fact);
// }

// the global helper variable
extern Helper helper;
