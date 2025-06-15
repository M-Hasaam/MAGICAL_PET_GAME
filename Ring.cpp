#include "Animations.h"

WaveRing::WaveRing(Vector2f center, float startRadius, float spd)
{
    radius = startRadius;
    speed = spd;
    thickness = 30.0f;
    maxAlpha = 180.0f;
    lifespan = 4.0f;
    currentLife = 0;

    outer.setRadius(radius);
    outer.setOrigin(radius, radius);
    outer.setPosition(center);
    outer.setFillColor(Color::Transparent);
    outer.setOutlineThickness(-thickness);
    outer.setOutlineColor(Color(RING_COLOR.r, RING_COLOR.g, RING_COLOR.b, maxAlpha));
}

bool WaveRing::update(float deltaTime, float maxRadius)
{
    currentLife += deltaTime;
    if (currentLife >= 4.0f)
        return false;

    float progress = currentLife / 4.0f;
    float easedSpeed = speed * (1.0f - 0.5f * cos(progress * 3.14159f));
    radius += easedSpeed * deltaTime;
    if (radius > maxRadius)
        return false;

    float thicknessFactor = 1.0f - (radius / maxRadius) * 0.7f;
    float currentThickness = 30.0f * thicknessFactor;
    float alpha = 180.0f * (1.0f - pow(progress, 1.5f));

    outer.setRadius(radius);
    outer.setOrigin(radius, radius);
    outer.setOutlineThickness(-currentThickness);
    outer.setOutlineColor(Color(RING_COLOR.r, RING_COLOR.g, RING_COLOR.b, static_cast<Uint8>(alpha)));

    return true;
}

void WaveRing::draw(RenderWindow &window)
{
    window.draw(outer);
}

RingManager::RingManager(float screenW, float screenH)
{
    for (int i = 0; i < MAX_RINGS; i++)
        rings[i] = nullptr;
    maxRadius = sqrt(screenW * screenW + screenH * screenH);
}

RingManager::~RingManager()
{
    for (int i = 0; i < MAX_RINGS; i++)
    {
        delete rings[i];
        rings[i] = nullptr;
    }
}

void RingManager::addRing(Vector2f position)
{
    for (int i = 0; i < MAX_RINGS; i++)
    {
        if (rings[i] == nullptr)
        {
            rings[i] = new WaveRing(position, RING_START_RADIUS, RING_SPEED);
            break;
        }
    }
}

void RingManager::update(float deltaTime)
{
    for (int i = 0; i < MAX_RINGS; i++)
    {
        if (rings[i] && !rings[i]->update(deltaTime, maxRadius))
        {
            delete rings[i];
            rings[i] = nullptr;
        }
    }
}

void RingManager::draw(RenderWindow &window)
{
    for (int i = 0; i < MAX_RINGS; i++)
    {
        if (rings[i])
            rings[i]->draw(window);
    }
}

// Implementation for WaveRing methods
// WaveRing::WaveRing(Vector2f center, float startRadius, float spd, float thick)
// {
//     radius = startRadius;
//     speed = spd;
//     thickness = thick;
//     maxAlpha = RING_MAX_ALPHA;
//     lifespan = RING_LIFESPAN;
//     currentLife = 0;

//     outer.setRadius(radius);
//     outer.setOrigin(radius, radius);
//     outer.setPosition(center);
//     outer.setFillColor(Color::Transparent);
//     outer.setOutlineThickness(-thickness);
//     outer.setOutlineColor(Color(RING_COLOR.r, RING_COLOR.g, RING_COLOR.b, maxAlpha));
// }

// bool WaveRing::update(float deltaTime, float maxRadius)
// {
//     currentLife += deltaTime;
//     if (currentLife >= lifespan)
//         return false;

//     float progress = currentLife / lifespan;
//     float easedSpeed = speed * (1.0f - 0.5f * cos(progress * 3.14159f));
//     radius += easedSpeed * deltaTime;
//     if (radius > maxRadius)
//         return false;

//     float thicknessFactor = 1.0f - (radius / maxRadius) * RING_THICKNESS_DECAY;
//     float currentThickness = thickness * thicknessFactor;
//     float alpha = maxAlpha * (1.0f - pow(progress, 1.5f));

//     outer.setRadius(radius);
//     outer.setOrigin(radius, radius);
//     outer.setOutlineThickness(-currentThickness);
//     outer.setOutlineColor(Color(RING_COLOR.r, RING_COLOR.g, RING_COLOR.b, static_cast<Uint8>(alpha)));

//     return true;
// }

// void WaveRing::draw(RenderWindow &window)
// {
//     window.draw(outer);
// }

// // Implementation for RingManager methods
// RingManager::RingManager(float screenW, float screenH)
// {
//     for (int i = 0; i < MAX_RINGS; i++)
//         rings[i] = nullptr;
//     maxRadius = sqrt(screenW * screenW + screenH * screenH);
// }

// RingManager::~RingManager()
// {
//     for (int i = 0; i < MAX_RINGS; i++)
//     {
//         delete rings[i];
//         rings[i] = nullptr;
//     }
// }

// void RingManager::addRing(Vector2f position)
// {
//     float randRadius = RING_START_RADIUS + (rand() % int(RING_RANDOM_RADIUS_RANGE));
//     float randSpeed = RING_SPEED - RING_RANDOM_SPEED_RANGE / 2 + (rand() % int(RING_RANDOM_SPEED_RANGE));
//     float randThickness = RING_THICKNESS - RING_RANDOM_THICKNESS_RANGE / 2 + (rand() % int(RING_RANDOM_THICKNESS_RANGE));

//     for (int i = 0; i < MAX_RINGS; i++)
//     {
//         if (rings[i] == nullptr)
//         {
//             rings[i] = new WaveRing(position, randRadius, randSpeed, randThickness);
//             break;
//         }
//     }
// }

// void RingManager::update(float deltaTime)
// {
//     for (int i = 0; i < MAX_RINGS; i++)
//     {
//         if (rings[i] && !rings[i]->update(deltaTime, maxRadius))
//         {
//             delete rings[i];
//             rings[i] = nullptr;
//         }
//     }
// }

// void RingManager::draw(RenderWindow &window)
// {
//     for (int i = 0; i < MAX_RINGS; i++)
//     {
//         if (rings[i])
//             rings[i]->draw(window);
//     }
// }

// class Projectile
// {
// public:
//     CircleShape shape;
//     float speed;
//     Pet *from_Pet = nullptr;
//     int direction; // -1 for left, 1 for right
//     float damage_factor = 1;

//     Projectile(Vector2f position, int direction, float radius = 10.f, float speed = 800.0f, float damage_fact = 1);
//     void update(float deltaTime);
//     void draw(RenderWindow &window);
//     bool isOffScreen(Vector2u windowSize);
//     void setActive(bool active);
//     void setOrigin(Vector2f origin);
//     Vector2f getOrigin() const;
//     Vector2f getCenterOrigin() const;
// };

// ======================

// void draw_shield(Vector2f position, float ratio)
// {
//     if (ratio <= 0.0f)
//         return;

//     CircleShape shield(40);
//     shield.setOrigin(100, 100);
//     shield.setPosition(position);
//     shield.setFillColor(Color(0, 150, 255, static_cast<Uint8>(ratio * 255)));

//     window->draw(shield);
// }

// ============================================

// Implementation for Projectile methods
// Projectile::Projectile(Vector2f position, int direction, float radius, float speed, float damage_fact)
//     : direction(direction), speed(speed), damage_factor(damage_fact)
// {
//     shape.setRadius(radius);
//     shape.setFillColor(Color::Red);
//     shape.setOrigin(getCenterOrigin());
//     shape.setPosition(position);
// }

// void Projectile::update(float deltaTime)
// {
//     shape.move(direction * speed * deltaTime, 0.f);
// }

// void Projectile::draw(RenderWindow &window)
// {
//     window.draw(shape);
// }

// bool Projectile::isOffScreen(Vector2u windowSize)
// {
//     return (direction == -1 && shape.getPosition().x + shape.getRadius() * 2 < 0) ||
//            (direction == 1 && shape.getPosition().x - shape.getRadius() > windowSize.x);
// }

// void Projectile::setActive(bool active)
// {
//     if (!active)
//     {
//         shape.setPosition(-100.f, -100.f);
//     }
// }

// void Projectile::setOrigin(Vector2f origin)
// {
//     shape.setOrigin(origin);
// }

// Vector2f Projectile::getOrigin() const
// {
//     return Vector2f(shape.getPosition().x + shape.getRadius(), shape.getPosition().y + shape.getRadius());
// }

// Vector2f Projectile::getCenterOrigin() const
// {
//     float radius = shape.getRadius();
//     return Vector2f(radius, radius);
// }

// ================================================================

// class Projectile
// {
// public:
//     CircleShape shape;
//     Sprite sprite;
//     Texture *texture = nullptr;
//     bool useTexture = false;

//     float speed;
//     Pet *from_Pet = nullptr;
//     int direction;
//     float damage_factor = 1;

//     // Animation
//     int frameWidth, frameHeight;
//     int totalFrames = 1, currentFrame = 0;
//     float frameDuration = 0.1f, frameTime = 0;

//     Projectile(Vector2f position, int direction, float radius = 10.f, float speed = 800.0f, float damage_fact = 1);
//     void setTexture(Texture *tex, int frameW, int frameH, int totalF, float duration);
//     void update(float deltaTime);
//     void draw(RenderWindow &window);
//     bool isOffScreen(Vector2u windowSize);
//     void setActive(bool active);
//     void setOrigin(Vector2f origin);
//     Vector2f getOrigin() const;
//     Vector2f getCenterOrigin() const;
// };

// class ProjectileManager
// {
//     Projectile *projectiles[200];
//     int maxProjectiles;
//     int activeCount;
//     Vector2u winSize;

// public:
//     ProjectileManager(Vector2u size, int maxP = 200);
//     ~ProjectileManager();
//     void addProjectile(Vector2f pos, int dir, Pet *source, float radius = 10.f, float speed = 800, float damage_fact = 1);
//     void update(float dt);
//     void draw(RenderWindow &window);
//     void Damage_from_projectile(Pet *P);
//     int getProjectileCount() const;
// };

// // Animation configuration constants
// const float RING_START_RADIUS = 100.0f;
// const float RING_SPEED = 800.0f;
// const float RING_THICKNESS = 30.0f;
// const float RING_MAX_ALPHA = 180.0f;
// const float RING_LIFESPAN = 4.0f;
// const float RING_INTERVAL = 0.8f;
// const float RING_THICKNESS_DECAY = 0.7f;
// const float RING_RANDOM_RADIUS_RANGE = 20.0f;
// const float RING_RANDOM_SPEED_RANGE = 40.0f;
// const float RING_RANDOM_THICKNESS_RANGE = 15.0f;

// const Color RING_COLOR = Color(255, 100, 0);
// const int MAX_RINGS = 100;

// struct WaveRing
// {
//     CircleShape outer;
//     float radius, speed, thickness;
//     float maxAlpha, lifespan, currentLife;

//     WaveRing(Vector2f center, float startRadius, float spd, float thick);
//     bool update(float deltaTime, float maxRadius);
//     void draw(RenderWindow &window);
// };

// class RingManager
// {
//     WaveRing *rings[MAX_RINGS];
//     float maxRadius;

// public:
//     RingManager(float screenW, float screenH);
//     ~RingManager();
//     void addRing(Vector2f position);
//     void update(float deltaTime);
//     void draw(RenderWindow &window);
// };

// class Helper
// {
//     RenderWindow *window;
//     Font *font;
//     Clock internalClock;
//     float lastRingTime;

// public:
//     RingManager wave;
//     ProjectileManager projectile;

//     Helper();
//     Helper(RenderWindow *w);
//     void setWindow(RenderWindow *w);
//     void setFont(Font *f);
//     RenderWindow &getWindow();
//     Font &getFont();
//     void update();
//     void draw();
//     void drawCenteredText(string text, float posX, float posY, int size = 27, Color color = Color::White);
//     int getClickedItemIndex(Event &event, Sprite items[], int itemCount, Clock &clickClock, float debounceTime = 0.3f);
//     void centerTextInButton(Text &text, const FloatRect &buttonRect);
//     void drawDotAt(Vector2f position, Color color = Color::Blue, float radius = 4.f);
//     void drawTextAt(const string textStr, Vector2f position = Vector2f(300, 300), unsigned int fontSize = 20, Color color = Color::White);
//     void createWaveRing(Vector2f position);
//     void shootProjectile(Vector2f pos, int direction, Pet *P, float radius = 10.f, float speed = 800, float damage_fact = 1);

//     void draw_shield(Vector2f origin, float ratio)
//     {
//         if (ratio <= 0.0f)
//             return;

//         float radius = 100.0f;
//         CircleShape shield(radius);
//         shield.setOrigin(radius, radius);
//         shield.setPosition(origin);
//         shield.setFillColor(Color(0, 255, 0, static_cast<Uint8>(ratio * (255 - 100))));

//         window->draw(shield);
//     }

// };
