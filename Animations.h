#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "ForwardDeclarations.h"


const float RING_START_RADIUS = 100.0f;
const float RING_SPEED = 3000.0f;

const Color RING_COLOR = Color(255, 100, 0);
const int MAX_RINGS = 100;

struct WaveRing
{
    CircleShape outer;
    float radius, speed, thickness;
    float maxAlpha, lifespan, currentLife;

    WaveRing(Vector2f center, float startRadius, float spd);
    bool update(float deltaTime, float maxRadius);
    void draw(RenderWindow &window);
};

class RingManager
{
    WaveRing *rings[MAX_RINGS];
    float maxRadius;

public:
    RingManager(float screenW, float screenH);
    ~RingManager();
    void addRing(Vector2f position);
    void update(float deltaTime);
    void draw(RenderWindow &window);
};

class Animations
{
    float timeAccumulator = 0.0f;
    // float dTime = 0;

    float randomFloat(float min, float max)
    {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    RingManager wave;

public:
    Animations() : wave(float(size_X), float(size_Y)) {}

    void createWaveRing(Vector2f position)
    {
        wave.addRing(position);
    }

    void update(float deltaTime)
    {
        wave.update(deltaTime);

        timeAccumulator += deltaTime;
        // dTime = deltaTime;
    }
    void draw(RenderWindow &window)
    {
        wave.draw(window);
    }

    void draw_defense_shield(RenderWindow &window, Vector2f origin, float ratio, float change = 0)
    {

        if (ratio <= 0.0f)
            return;

        origin.x += change;

        float radius = 100.0f;
        float pulse = 0.15f * sin(timeAccumulator * 3.0f) + 1.0f;
        float actualRadius = radius * pulse;

        CircleShape outerGlow(actualRadius * 1.2f);
        outerGlow.setOrigin(actualRadius * 1.2f, actualRadius * 1.2f);
        outerGlow.setPosition(origin);
        outerGlow.setFillColor(Color(30, 30, 255, static_cast<Uint8>(ratio * 80)));
        window.draw(outerGlow);

        CircleShape shield(actualRadius);
        shield.setOrigin(actualRadius, actualRadius);
        shield.setPosition(origin);
        shield.setFillColor(Color(0, 100, 255, static_cast<Uint8>(ratio * 120)));
        shield.setOutlineThickness(5.0f);
        shield.setOutlineColor(Color(100, 150, 255, static_cast<Uint8>(ratio * 200)));
        window.draw(shield);

        CircleShape innerPattern(actualRadius * 0.7f, 6);
        innerPattern.setOrigin(actualRadius * 0.7f, actualRadius * 0.7f);
        innerPattern.setPosition(origin);
        innerPattern.setFillColor(Color::Transparent);
        innerPattern.setOutlineThickness(2.0f);
        innerPattern.setOutlineColor(Color(150, 180, 255, static_cast<Uint8>(ratio * 180)));
        window.draw(innerPattern);

        for (int i = 0; i < 3; i++)
        {
            float rippleSize = actualRadius * (0.4f + 0.2f * i);
            CircleShape ripple(rippleSize);
            ripple.setOrigin(rippleSize, rippleSize);
            ripple.setPosition(origin);
            ripple.setFillColor(Color::Transparent);
            ripple.setOutlineThickness(1.5f);
            ripple.setOutlineColor(Color(50, 100, 255, static_cast<Uint8>(ratio * 100 * (1.0f - (float)i / 4))));
            window.draw(ripple);
        }

        if (ratio < 0.95f)
        {
            for (int i = 0; i < 8; i++)
            {
                float angle = randomFloat(0, 2 * 3.14159f);
                float distance = randomFloat(0.3f * radius, 0.9f * radius);
                CircleShape particle(randomFloat(1.0f, 3.0f));
                particle.setOrigin(particle.getRadius(), particle.getRadius());
                particle.setPosition(origin.x + cos(angle) * distance, origin.y + sin(angle) * distance);
                particle.setFillColor(Color(200, 220, 255, static_cast<Uint8>((1.0f - ratio) * 200)));
                window.draw(particle);
            }
        }
    }

    void draw_healing_shield(RenderWindow &window, Vector2f origin, float ratio, float change = 0)
    {

        if (ratio <= 0.0f)
            return;

        origin.x += change;

        float radius = 100.0f;

        // Create pulsing effect - more pronounced for healing
        float pulse = 0.2f * sin(timeAccumulator * 4.0f) + 1.0f;
        float actualRadius = radius * pulse;

        // Outer glow
        CircleShape outerGlow(actualRadius * 1.2f);
        outerGlow.setOrigin(actualRadius * 1.2f, actualRadius * 1.2f);
        outerGlow.setPosition(origin);
        outerGlow.setFillColor(Color(30, 255, 100, static_cast<Uint8>(ratio * 80)));
        window.draw(outerGlow);

        // Main shield
        CircleShape shield(actualRadius);
        shield.setOrigin(actualRadius, actualRadius);
        shield.setPosition(origin);
        shield.setFillColor(Color(0, 255, 100, static_cast<Uint8>(ratio * 120)));
        shield.setOutlineThickness(5.0f);
        shield.setOutlineColor(Color(100, 255, 200, static_cast<Uint8>(ratio * 200)));
        window.draw(shield);

        // Inner hexagon pattern
        CircleShape innerPattern(actualRadius * 0.7f, 6); // Hexagon
        innerPattern.setOrigin(actualRadius * 0.7f, actualRadius * 0.7f);
        innerPattern.setPosition(origin);
        innerPattern.setFillColor(Color::Transparent);
        innerPattern.setOutlineThickness(2.0f);
        innerPattern.setOutlineColor(Color(150, 255, 150, static_cast<Uint8>(ratio * 180)));
        window.draw(innerPattern);

        // Healing shield specific feature - cross pattern and healing particles
        // const int numRays = 4;
        // for (int i = 0; i < numRays; i++)
        // {
        //     float angle = (3.14159f / numRays) * i;

        //     // Draw cross rays
        //     RectangleShape ray(Vector2f(actualRadius * 1.4f, 5.0f));
        //     ray.setOrigin(0, 2.5f);
        //     ray.setPosition(origin);
        //     ray.setRotation(angle * 180.0f / 3.14159f);
        //     ray.setFillColor(Color(160, 255, 160, static_cast<Uint8>(ratio * 120)));
        //     window->draw(ray);
        // }

        // Healing particles - more small green particles that move outward
        for (int i = 0; i < 12; i++)
        {
            float angle = randomFloat(0, 2 * 3.14159f);
            float distance = randomFloat(0.2f * radius, 0.8f * radius);
            CircleShape particle(randomFloat(1.0f, 4.0f));
            particle.setOrigin(particle.getRadius(), particle.getRadius());
            particle.setPosition(origin.x + cos(angle) * distance, origin.y + sin(angle) * distance);
            particle.setFillColor(Color(180, 255, 180, static_cast<Uint8>(ratio * 160)));
            window.draw(particle);
        }

        // Dynamic white particles when healing is active
        if (ratio > 0.3f)
        {
            for (int i = 0; i < 5; i++)
            {
                float angle = randomFloat(0, 2 * 3.14159f);
                float distance = randomFloat(0.1f * radius, 0.6f * radius);
                CircleShape sparkle(randomFloat(1.5f, 3.5f));
                sparkle.setOrigin(sparkle.getRadius(), sparkle.getRadius());
                sparkle.setPosition(origin.x + cos(angle) * distance, origin.y + sin(angle) * distance);
                sparkle.setFillColor(Color(255, 255, 255, static_cast<Uint8>(ratio * 180)));
                window.draw(sparkle);
            }
        }
    }

    void draw_claw_strike(RenderWindow &window, Vector2f pos, float ratio, float scale = 1.0f)
    {
        static Texture clawTexture;
        static bool loaded = false;

        if (!loaded)
        {
            clawTexture.loadFromFile("Images/claw_strick.png");
            loaded = true;
        }

        if (ratio <= 0.0f)
            return;

        Sprite claw;
        claw.setTexture(clawTexture);
        claw.setOrigin(clawTexture.getSize().x / 2.f, clawTexture.getSize().y / 2.f);
        claw.setPosition(pos);
        claw.setScale(scale, scale);
        claw.setColor(Color(255, 255, 255, static_cast<Uint8>(ratio * 255)));

        window.draw(claw);
    }
};

#endif