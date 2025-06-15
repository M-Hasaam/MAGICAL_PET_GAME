#include "Pet.h"
#include "Projectiles.h"

// ================= Pet Base Class ================

void Pet::updatePercentages()
{
    health_Percent = float(health) / maxHealth;
    attackPower_Percent = float(attackPower) / maxAttackPower;
}

bool Pet::refill_Health()
{
    if (Abilities_timers[0] <= 0)
    {
        if (health < maxHealth)
        {
            Abilities_timers[0] = Abilities_time_limit[1];

            health = maxHealth;
            updatePercentages();

            return true;
        }
    }

    return false;
}

void Pet::update(float deltaTime)
{
    Object::update(deltaTime);

    A.update(deltaTime);

    fireCooldownTimer = max(0.f, fireCooldownTimer - deltaTime);

    if (Abilities_timers[3] > 0)
    {

        Abilities_timers[3] -= deltaTime;

        if (Abilities_timers[3] <= 0)
        {
            defense = Pre_factors[3];
            Abilities_timers[3] = 0;
        }
    }

    if (Abilities_timers[2] > 0)
    {

        Abilities_timers[2] -= deltaTime;

        if (Abilities_timers[2] <= 0)
        {
            speed = Pre_factors[2];
            Abilities_timers[2] = 0;
        }
    }

    Abilities_timers[0] = max(0.f, Abilities_timers[0] - deltaTime);
    Abilities_timers[1] = max(0.f, Abilities_timers[1] - deltaTime);
}

bool Pet::refill_AttackPower()
{
    if (Abilities_timers[2] <= 0)
    {
        if (health < maxHealth)
        {
            Abilities_timers[2] = Abilities_time_limit[2];

            attackPower = maxAttackPower;
            updatePercentages();

            return true;
        }
    }

    return false;
}

void Pet::Controll_Pet_With_keys(RenderWindow &window, Event &E, float deltaTime, FloatRect Bounds)
{
    // Skip if stunned
    if (isStuned(deltaTime))
        return;

    Vector2f movement(0.0f, 0.0f);

    if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
        movement.y = -1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
        movement.y = 1.0f;
    if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
    {
        movement.x = -1.0f;
        Invert_Left();
    }
    if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
    {
        movement.x = 1.0f;
        Invert_Right();
    }

    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        fire();
    }

    if (Keyboard::isKeyPressed(Keyboard::M))
    {
        useAbility_1();
    }

    if (Keyboard::isKeyPressed(Keyboard::N))
    {
        useAbility_2();
    }

    // Normalize diagonal movement
    if (movement.x != 0.0f && movement.y != 0.0f)
    {
        float length = sqrt(movement.x * movement.x + movement.y * movement.y);
        movement /= length;
    }

    movement *= 1000 * speed * deltaTime;

    Vector2f windowSize(window.getSize().x, window.getSize().y);
    Object::moveObject(movement, deltaTime, windowSize, Bounds);
}

void Pet::fire()
{
    // cout << name << " Tried to fired ";
    if (isStuned())
        return;

    // cout << " Further problem \n";

    if (fireCooldownTimer <= 0.0f)
    {
        // cout << " Further problem 2 \n";

        // Vector2f fireballPos(sprite.getPosition().x + Size.x / 2.f + (inverted ? 60 : -60), sprite.getPosition().y + Size.y / 2.f - 40);
        if (projectileManager_pointer)
            projectileManager_pointer->addAttackBall(getCenter(), (isInverted() ? 1 : -1), this);
        else
            cout << "projectileManager_pointer not Linked\n";

        fireCooldownTimer = fireCooldown;
    }
    
}

void Pet::Take_projectile_damage()
{
    if (projectileManager_pointer)
        projectileManager_pointer->Damage_Pet(this);
}

void Pet::trackTarget(Vector2f targetOrigin, float deltaTime, int invert_type, float inc_speed)
{
    Vector2f originPet = this->getCenter();
    Vector2f dir = targetOrigin - originPet;
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y);

    // float temp_speed = speed;

    if (dist > 5.f)
        dir /= dist;

    if (invert_type && dist < (10.f))
    {
        if (invert_type == 1)
            this->Invert_Left();
        else if (invert_type == -1)
            this->Invert_Right();

        // speed = 50;
    }
    else
    {
        if (dir.x < 0 && isInverted())
        {
            this->Invert_Left();
            setInverted(false);
        }
        else if (dir.x > 0 && !this->isInverted())
        {
            this->Invert_Right();
            setInverted(true);
        }
    }

    if (dist < (10.f))
        dir = Vector2f(dir.x * 5 * inc_speed, dir.y * 5 * inc_speed); // use speed here
    else
        dir = Vector2f(dir.x * 30 * inc_speed, dir.y * 30 * inc_speed); // use speed here

    this->moveObject(dir, deltaTime, Vector2f(size_X, size_Y));

    // speed = temp_speed;
}

void Pet::Increment_Level()
{
    health += 1;
    attackPower += 1;
    speed += 1;
    defense += 1;

    level += 0.1;

    maxHealth += 1;
    maxAttackPower += 1;
}

void Pet::takeDamage(int damage)
{
    int damageTaken = damage - defense;
    cout << " d = " << damageTaken << " = " << damage << " - " << defense << " ;\n";
    if (damageTaken < 1)
        damageTaken = 1;
    health -= damageTaken;
    if (health < 0.05 * maxHealth)
        health = 0.05 * maxHealth;

    attackPower -= damageTaken * 0.2;
    if (attackPower < 0.05 * maxAttackPower)
        attackPower = 0.05 * maxAttackPower;

    updatePercentages();
}

bool Pet::isHealthCritical()
{
    return health <= 0.05 * maxHealth;
}

void Pet::heal(int amount)
{
    if (Abilities_time_limit[0] <= 0)
    {
        if (health < maxHealth)
        {
            Abilities_timers[0] = Abilities_time_limit[0];

            health = min(maxHealth, health + amount);
            updatePercentages();
        }
    }
}

Pet &Pet::operator=(const Pet &O)
{
    if (this != &O)
    {
        setName(O.getName());
        health = O.health;
        attackPower = O.attackPower;
        speed = O.speed;
        defense = O.defense;
        level = O.level;
        maxHealth = O.maxHealth;
        maxAttackPower = O.maxAttackPower;

        Object::operator=(O);
        updatePercentages();
    }
    return *this;
}

void Pet::printStats() const
{
    cout << getName() << " Stats: \n";
    cout << "Health: " << health << "\n";
    cout << "Attack Power: " << attackPower << "\n";
    cout << "Speed: " << speed << "\n";
    cout << "Defense: " << defense << "\n";
}

int Pet::getHealth() const
{
    return health;
}

int Pet::getAttackPower() const
{
    return attackPower;
}

int Pet::getSpeed() const
{
    return speed;
}

int Pet::getDefense() const
{
    return defense;
}

void Pet::write_level_below_Pet(RenderWindow &window, Font &font)
{
    Text levelText;
    levelText.setFont(font);
    levelText.setString(getName() + " (Level " + to_string((int)level) + ")");
    levelText.setCharacterSize(30);
    levelText.setFillColor(Color::White);

    FloatRect textBounds = levelText.getLocalBounds();
    levelText.setOrigin(textBounds.width / 2.f, 0);
    levelText.setPosition(getLeft() + abs(getSize().x) / 2.f, getTop() + getSize().y + 50);

    window.draw(levelText);
}

void Pet::write_Pet_factor(RenderWindow &window, Font &font)
{
    Text factorText;
    factorText.setFont(font);
    factorText.setCharacterSize(27);
    factorText.setFillColor(Color::White);

    FloatRect petBounds = getGlobalBounds();
    float x = petBounds.left + petBounds.width / 2.f;
    float y = petBounds.top + petBounds.height + 100;
    int spacing = 40;

    // Health
    factorText.setString("Health: " + to_string(health) + " / " + to_string(maxHealth));
    FloatRect bounds1 = factorText.getLocalBounds();
    factorText.setOrigin(bounds1.width / 2.f, 0);
    factorText.setPosition(x, y);
    window.draw(factorText);

    // Attack
    factorText.setString("Attack: " + to_string(attackPower) + " / " + to_string(maxAttackPower));
    FloatRect bounds2 = factorText.getLocalBounds();
    factorText.setOrigin(bounds2.width / 2.f, 0);
    factorText.setPosition(x, y + spacing);
    window.draw(factorText);

    // Speed
    factorText.setString("Speed: " + to_string(speed));
    FloatRect bounds3 = factorText.getLocalBounds();
    factorText.setOrigin(bounds3.width / 2.f, 0);
    factorText.setPosition(x, y + 2 * spacing);
    window.draw(factorText);

    // Defense
    factorText.setString("Defense: " + to_string(defense));
    FloatRect bounds4 = factorText.getLocalBounds();
    factorText.setOrigin(bounds4.width / 2.f, 0);
    factorText.setPosition(x, y + 3 * spacing);
    window.draw(factorText);
}

bool Pet::draw_select_button(RenderWindow &window, Texture &Btn_T, Font &font)
{
    Sprite Button;
    Button.setTexture(Btn_T);
    Button.setScale(0.7f, 0.5f);

    float xCenter = getLeft() + getSize().x / 2.f;
    float yTop = getTop() + getSize().y + 90;

    Vector2f rawSize = (Vector2f)Btn_T.getSize();
    Button.setOrigin(rawSize.x / 2.f, 0.f);
    Button.setPosition(xCenter, yTop);

    if (selected)
        Button.setColor(Color(50, 150, 255));
    else
        Button.setColor(Color::White);

    window.draw(Button);

    FloatRect bounds = Button.getGlobalBounds();

    Text btnText;
    btnText.setFont(font);
    btnText.setString("Select");
    btnText.setCharacterSize(30);
    btnText.setFillColor(Color::White);
    btnText.setStyle(Text::Bold);

    helper.centerTextInButton(btnText, bounds);
    window.draw(btnText);

    Vector2i mousePos = Mouse::getPosition(window);
    bool nowPressed = Mouse::isButtonPressed(Mouse::Left);

    if (bounds.contains((float)mousePos.x, (float)mousePos.y))
    {
        if (nowPressed && !wasPressedGlobal)
        {
            selected = !selected;
        }
    }

    wasPressedGlobal = nowPressed;
    return selected;
}

bool Pet::draw_add_remove_button(RenderWindow &window, Texture &Btn_T, Font &font)
{
    Sprite button;
    button.setTexture(Btn_T);
    button.setScale(0.7f, 0.5f);

    float xCenter = getLeft() + getSize().x / 2.f;
    float yTop = getTop() + getSize().y + 90;

    Vector2f rawSize = (Vector2f)Btn_T.getSize();
    button.setOrigin(rawSize.x / 2.f, 0.f);
    button.setPosition(xCenter, yTop);

    if (added)
        button.setColor(Color(255, 30, 30)); // Bright Red for Remove
    else
        button.setColor(Color(30, 255, 30)); // Bright Green for Add

    // if (added)
    //     button.setColor(Color(200, 50, 50)); // Red for Remove
    // else
    //     button.setColor(Color(50, 200, 50)); // Green for Add

    window.draw(button);

    FloatRect bounds = button.getGlobalBounds();

    Text btnText;
    btnText.setFont(font);
    btnText.setCharacterSize(26);
    btnText.setFillColor(Color::White);
    btnText.setStyle(Text::Bold);

    if (added)
        btnText.setString("Remove");
    else
        btnText.setString("Add");

    helper.centerTextInButton(btnText, bounds);
    window.draw(btnText);

    Vector2i mousePos = Mouse::getPosition(window);
    bool nowPressed = Mouse::isButtonPressed(Mouse::Left);

    if (bounds.contains((float)mousePos.x, (float)mousePos.y))
    {
        if (nowPressed && !wasPressedGlobal)
        {
            added = !added;
        }
    }

    wasPressedGlobal = nowPressed;
    return added;
}

void Pet::draw_health_bar_of_Pet(RenderWindow &window)
{
    float barWidth = abs(getSize().x);
    float barHeight = 6.f;

    float barLeft = getLeft(); // fixed left edge regardless of sprite flip
    float barTop = getTop() - barHeight - 4.f;

    // if (Size.x < 0)
    // {
    //     barLeft += Size.x;
    //     barWidth = abs(Size.x);
    // }

    RectangleShape healthBarBack(Vector2f(barWidth, barHeight));
    healthBarBack.setFillColor(Color(50, 50, 50));
    healthBarBack.setPosition(barLeft, barTop);

    Color dynamicColor = getHealthBarColor(health_Percent);

    RectangleShape healthBarFront(Vector2f(barWidth * health_Percent, barHeight));
    healthBarFront.setFillColor(dynamicColor);
    healthBarFront.setPosition(barLeft, barTop); // always draw left to right

    window.draw(healthBarBack);
    window.draw(healthBarFront);
}

Color Pet::getHealthBarColor(float percent)
{
    percent = std::clamp(percent, 0.f, 1.f);

    Uint8 red, green;

    if (percent > 0.5f)
    {
        // From Green to Yellow
        red = static_cast<Uint8>(2 * (1.0f - percent) * 255);
        green = 255;
    }
    else
    {
        // From Yellow to Red
        red = 255;
        green = static_cast<Uint8>(2 * percent * 255);
    }

    return Color(red, green, 0);
}

void Pet::draw(RenderWindow &window, Font *font)
{
    Object::draw(window); // Draw the pet sprite
    A.draw(window);

    if (Abilities_timers[3] > 0)
    {
        A.draw_defense_shield(window, getCenter(), Abilities_timers[3] / Abilities_time_limit[3]);
    }

    if (Abilities_timers[2] > 0)
    {
        A.draw_defense_shield(window, getCenter(), Abilities_timers[2] / Abilities_time_limit[2]);
    }

    if (Abilities_timers[0] > 0)
    {
        A.draw_healing_shield(window, getCenter(), Abilities_timers[0] / Abilities_time_limit[0]);
    }

    draw_health_bar_of_Pet(window);

    if (font)
    {
        write_level_below_Pet(window, *font);
    }
}

ostream &operator<<(ostream &os, Pet &p)
{
    p.reset_factors();
    p.calculateLevel();

    os << p.getName() << endl;
    os << p.health << " " << p.maxHealth << " " << p.health_Percent << endl;
    os << p.attackPower << " " << p.maxAttackPower << " " << p.attackPower_Percent << endl;
    os << p.speed << endl;
    os << p.defense << endl;
    os << p.level << endl;
    os << p.added << endl;
    os << p.role << endl;

    return os;
}

istream &operator>>(istream &in, Pet &p)
{
    string name;
    in >> name;
    p.setName(name);
    in >> p.health >> p.maxHealth >> p.health_Percent;
    in >> p.attackPower >> p.maxAttackPower >> p.attackPower_Percent;
    in >> p.speed;
    in >> p.defense;
    in >> p.level;
    in >> p.added;
    in >> p.role;

    p.calculateLevel();
    p.save_previous_factors();
    p.updatePercentages();
    return in;
}
