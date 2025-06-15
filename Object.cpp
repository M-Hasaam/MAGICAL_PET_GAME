

#include "Object.h"
#include "Helper.h"

Object::Object() {}

Object::Object(Texture &Tex)
{
    // texture = Tex;
    // sprite.setTexture(texture);
    // axis = texture.getSize().y;
    // length = texture.getSize().x;
    setTexture(Tex);
}

Object::Object(Texture &sharedTexture, Vector2f position, Vector2f sizeInPixels, const float frameDura)
    : frameDuration(frameDura), textureIndex(0), animationTimer(0)
{
    // texture = sharedTexture;
    // sprite.setTexture(texture);
    // axis = texture.getSize().y;
    // length = texture.getSize().x;

    // sprite.setTextureRect(IntRect(0, 0, axis, axis));
    setTexture(sharedTexture);

    set_sprite_size(sizeInPixels);

    setPosition(position);
    // sprite.setPosition(position);
}

Object::Object(string name, Texture &Tex, int AnimationPerSheet, Vector2f size, Vector2f Cen, Vector2f Vel, float Dir, float FrameDuration)
{
    this->name = name;
    setTexture(Tex);

    set_sprite_size(Vector2f(abs(size.x) * Dir / abs(Dir), size.y));

    change_axis(AnimationPerSheet);
    frameDuration = FrameDuration;

    if (Dir < 0)
        Invert_Right();
    else if (Dir > 0)
        Invert_Left();

    setCenter(Cen);
    setVelocity(Vel);
}

void Object::setTexture(Texture &T)
{
    texture = T;
    sprite.setTexture(texture);
    axis = texture.getSize().y;
    length = texture.getSize().x;
    if (!axis)
        axis = 1;
    if (!length)
        length = 1;
    sprite.setTextureRect(IntRect(0, 0, axis, axis));
}

void Object::set_sprite_size(Vector2f sizeInPixels)
{
    Size = sizeInPixels;
    Vector2f scale(sizeInPixels.x / axis, sizeInPixels.y / axis);

    if (sizeInPixels.x < 0)
        inverted = true;

    sprite.setScale(scale);
}

void Object::set_Position_By_Origin(Vector2f origin)
{
    FloatRect bounds = sprite.getGlobalBounds();
    Vector2f offset(bounds.width / 2.f, bounds.height / 2.f);

    if (inverted)
        offset.x = -offset.x + bounds.width;

    Vector2f newPos = origin - offset;
    setPosition(newPos);
}

void Object::Invert_Left()
{
    if (Size.x < 0)
    {
        Vector2f currentPos = sprite.getPosition();
        float width = abs(Size.x);
        Size.x = width;
        sprite.setScale(Size.x / axis, Size.y / axis);
        sprite.setPosition(currentPos.x - width, currentPos.y);
    }
    inverted = false;
}

void Object::Invert_Right()
{
    if (Size.x > 0)
    {
        Vector2f currentPos = sprite.getPosition();
        float width = abs(Size.x);
        Size.x = -width;
        sprite.setScale(Size.x / axis, Size.y / axis);
        sprite.setPosition(currentPos.x + width, currentPos.y);
    }
    inverted = true;
}

void Object::Stun_it()
{
    stunned_timer = 10;
}

Object &Object::operator=(const Object &O)
{
    if (this != &O)
    {
        this->texture = O.texture;
        sprite.setTexture(this->texture);
        axis = O.axis;
        length = O.length;
        textureIndex = O.textureIndex;
        animationTimer = O.animationTimer;
    }

    return *this;
}

void Object::draw(RenderWindow &window)
{
    window.draw(sprite);

    // helper.drawRectBounds(getGlobalBounds());
    // helper.drawDotAt(Vector2f(getLeft(), getTop()), Color::White, 10);
    // helper.drawDotAt(Vector2f(getLeft(), getBottom()), Color::Green, 10);
    // helper.drawDotAt(Vector2f(getRight(), getTop()), Color::Red, 10);
    // helper.drawDotAt(Vector2f(getRight(), getBottom()), Color::Yellow, 10);

    // helper.drawDotAt(Pos, Color::Black, 5);
    // helper.drawDotAt(getCenter(), Color::Blue, 10);
}

void Object::updateSpriteMovement(float deltaTime)
{
    stunned_timer = max(0.f, stunned_timer - deltaTime);

    if (stunned_timer > 0.f)
        return;

    animationTimer += deltaTime;

    if (animationTimer > frameDuration)
    {
        animationTimer = 0;

        int frameCount = length / axis;
        if (textureIndex + 1 == frameCount)
            one_loop_ended = true;
        textureIndex = (textureIndex + 1) % frameCount;

        sprite.setTextureRect(IntRect(textureIndex * axis, 0, axis, axis));
    }
}

void Object::updatePosition(float deltaTime)
{
    stunned_timer = max(0.f, stunned_timer);

    if (stunned_timer > 0.f)
        return;

    Pos += Vel * deltaTime;
    setPosition(Pos);
}

void Object::update(float deltaTime)
{
    updatePosition(deltaTime);
    updateSpriteMovement(deltaTime);
}

void Object::changePosition(Vector2f newPosition)
{
    sprite.setPosition(newPosition);
}

void Object::changeScale(Vector2f newScale)
{
    sprite.setScale(newScale);
}

bool Object::checkCollision(Vector2i mousePosition)
{
    return sprite.getGlobalBounds().contains(mousePosition.x, mousePosition.y);
}

FloatRect Object::getGlobalBounds() const
{
    return sprite.getGlobalBounds();
}

void Object::trackTarget_with_Position(Vector2f targetPos, float deltaTime)
{
    Vector2f currentPos = this->getPosition();
    Vector2f dir = targetPos - currentPos;
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

    if (dist > 5.f)
        dir /= dist;

    if (dir.x < 0 && this->inverted)
    {
        this->Invert_Left();
        this->inverted = false;
    }
    else if (dir.x > 0 && !this->inverted)
    {
        this->Invert_Right();
        this->inverted = true;
    }

    dir = Vector2f(dir.x * 100, dir.y * 100);
    this->moveObject(dir, deltaTime, Vector2f(size_X, size_Y));
}

void Object::moveObject(Vector2f velocity, float deltaTime, Vector2f windowSize, FloatRect boundW)
{
    if (boundW == FloatRect(0, 0, 0, 0))
        boundW = FloatRect(0, 0, windowSize.x, windowSize.y);

    stunned_timer = max(0.f, stunned_timer);

    if (stunned_timer > 0.f)
        return;

    if (velocity == Vector2f(0, 0))
        return;

    Vector2f displacement = velocity * deltaTime;
    sprite.move(displacement);

    Vector2f pos = sprite.getPosition();
    FloatRect bounds(sprite.getPosition().x, sprite.getPosition().y, Size.x, Size.y);

    if (pos.x < boundW.left)
        pos.x = boundW.left;
    else if (pos.x + max(bounds.width, 0.f) > (boundW.left + boundW.width))
        pos.x = (boundW.left + boundW.width) - max(bounds.width, 0.f);

    if (pos.y < boundW.top)
        pos.y = boundW.top;
    else if (pos.y + bounds.height > (boundW.top + boundW.height))
        pos.y = (boundW.top + boundW.height) - bounds.height;

    sprite.setPosition(pos);
    Pos = pos;
}

void Object::set_axis(int newAxis)
{
    axis = newAxis;
}

void Object::change_axis(int no_of_frames)
{
    length = texture.getSize().x;
    axis = length / no_of_frames;
}

void Object::setIntRect(float left, float top, float width, float height)
{
    set_sprite_size(Vector2f(width, height));
    sprite.setTextureRect(IntRect(left, top, width, height));
}

void Object::setIntRect_grid(int rows, int cols, int indexRow, int indexCol)
{
    int spriteWidth = texture.getSize().x / cols;
    int spriteHeight = texture.getSize().y / rows;

    int left = indexCol * spriteWidth;
    int top = indexRow * spriteHeight;

    set_sprite_size(Vector2f(spriteWidth, spriteHeight));

    sprite.setTextureRect(IntRect(left, top, spriteWidth, spriteHeight));
}

float Object::getHeight() const
{
    return sprite.getGlobalBounds().height;
}

float Object::getLeft() const
{
    return sprite.getGlobalBounds().left; //+ (Size.x < 0 ? -Size.x : 0);
}

float Object::getRight() const
{
    return sprite.getGlobalBounds().left + sprite.getGlobalBounds().width;
}

float Object::getTop() const
{
    return sprite.getGlobalBounds().top;
}

float Object::getBottom() const
{
    return sprite.getGlobalBounds().top + sprite.getGlobalBounds().height;
}

float Object::getWidth() const
{
    return sprite.getGlobalBounds().width;
}

void Object::setTopLeftCorner(Vector2f newPos)
{
    Pos = newPos;
    if (inverted)
        Pos.x += abs(Size.x);

    sprite.setPosition(Pos);
}

Vector2f Object::getCenter() const
{
    Vector2f center = sprite.getPosition() + Vector2f(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);

    if (inverted)
        center.x -= sprite.getGlobalBounds().width;

    return center;
}

void Object::setCenter(Vector2f Cen)
{
    // Pos += Cen - getCenter();
    Pos.y = Cen.y - Size.y / 2;
    if (!inverted)
    {
        Pos.x = Cen.x - (Size.x / 2.f);
    }
    else
    {
        Pos.x = Cen.x + (Size.x / 2.f);
    }

    sprite.setPosition(Pos);
    // Pos.x += Cen.x - (getLeft() + getRight()) / 2;
    // Pos.y += Cen.y - (getTop() + getBottom()) / 2;
}

Vector2f Object::getPosition() const
{
    if (inverted)
        return Vector2f(sprite.getPosition().x - Size.x, sprite.getPosition().y);

    return sprite.getPosition();
}

void Object::setPosition(Vector2f newPos)
{
    Pos = newPos;
    // if (inverted)
    //     newPos.x += abs(Size.x);
    sprite.setPosition(newPos);
}

Vector2f Object::getVelocity() const
{
    return Vel;
}

void Object::setVelocity(Vector2f newVel)
{
    Vel = newVel;
}

Vector2f Object::getSize() const
{
    return Size;
}

void Object::setSize(Vector2f newSize)
{
    Size = newSize;
    set_sprite_size(newSize);
}
