
#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "ForwardDeclarations.h"

using namespace sf;
using namespace std;

extern const int size_X;
extern const int size_Y;

class Object
{
    Vector2f Pos = Vector2f(0, 0), Vel = Vector2f(0, 0);

    Texture texture;
    Sprite sprite;
    Vector2f Size;

    bool one_loop_ended = false;

    bool inverted = false;

    int axis = 1, length = 1;

    float frameDuration = 0.05f;

    int textureIndex = 1;
    float animationTimer = 1;

    float stunned_timer = 0;
    string name = "null";

protected:
public:
    void setFrameDuration(float time) { frameDuration = time; }
    void setName(string n)
    {
        for (int i = 0; n[i] != '\0'; i++)
            if (n[i] == ' ')
                n[i] = '_';

        name = n;
    }
    string getName() const { return name; }
    bool isStuned(float deltaTime = 0)
    {
        if (stunned_timer > 0)
        {
            stunned_timer = max(0.f, stunned_timer - deltaTime);
            return true;
        }

        return false;
    }
    bool IsNotStunned() { return stunned_timer <= 0; }
    void setStunnedTimer(float t) { stunned_timer = t; }
    bool isInverted() { return inverted; }
    void setInverted(bool i) { inverted = i; }
    bool Is_Animation_Restarted(){return one_loop_ended;}

    Object();
    Object(Texture &Tex);
    Object(Texture &sharedTexture, Vector2f position, Vector2f sizeInPixels, const float frameDura);
    Object(string name, Texture &Tex, int AnimationPerSheet, Vector2f size, Vector2f Cen, Vector2f Vel = Vector2f(0, 0), float Dir = 1, float FrameDuration = 0.05f);

    void setTexture(Texture &T);
    void set_sprite_size(Vector2f sizeInPixels);
    // void set_Position(Vector2f newPos);
    void set_Position_By_Origin(Vector2f origin);
    void Invert_Left();
    void Invert_Right();
    void Stun_it();

    Object &operator=(const Object &O);

    void draw(RenderWindow &window);
    void updateSpriteMovement(float deltaTime);
    void updatePosition(float deltaTime);
    virtual void update(float deltaTime);

    void changePosition(Vector2f newPosition);
    void changeScale(Vector2f newScale);
    bool checkCollision(Vector2i mousePosition);
    FloatRect getGlobalBounds() const;

    void trackTarget_with_Position(Vector2f targetPos, float deltaTime);
    void moveObject(Vector2f velocity, float deltaTime, Vector2f windowSize, FloatRect bounds = FloatRect(0, 0, 0, 0));

    void set_axis(int newAxis);
    void change_axis(int no_of_frames);
    void setIntRect(float left, float top, float width, float height);
    void setIntRect_grid(int rows, int cols, int indexRow, int indexCol);

    float getHeight() const;
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    float getWidth() const;

    void setTopLeftCorner(Vector2f newPos);

    Vector2f getCenter() const;
    void setCenter(Vector2f Cen);
    Vector2f getPosition() const;
    void setPosition(Vector2f newPos);
    Vector2f getVelocity() const;
    void setVelocity(Vector2f newVel);
    Vector2f getSize() const;
    void setSize(Vector2f newSize);
};

#endif
