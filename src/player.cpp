#include "player.h"

Player::Player(float x, float y) 
{
    if (!texture.loadFromFile("assets/stickman/stickman.png"))
    {
        std::cerr << "Error: Failed to load stickman.png" << std::endl;
    }
    if (!runTexture.loadFromFile("assets/stickman/stickman_run.png"))
    {
        std::cerr << "Error: Failed to load stickman_run.png" << std::endl;
    }
    if (!idleTexture.loadFromFile("assets/stickman/stickman_idle.png"))
    {
        std::cerr << "Error: Failed to load stickman_idle.png" << std::endl;
    }
    if (!hitTexture.loadFromFile("assets/stickman/stickman_hit.png"))
    {
        std::cerr << "Error: Failed to load stickman_hit.png" << std::endl;
    }

    runAnimation = Animation(&runTexture, 8, 0.15f, 122, 171);
    idleAnimation = Animation(&idleTexture, 8, 0.15f, 122, 171);
    attackHitAnimation = Animation(&hitTexture, 4, 0.05f, 157, 171);

    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(1.f, 1.f);

    this->setFrameWidth(sprite.getTexture()->getSize().x);
    this->setFrameHeight(sprite.getTexture()->getSize().y);

    facingRight = true;
    canAttack = true;

    speed = 150.f;
    jumpStrength = 700.f;

    maxHealth = 100;
    health = 100;
    healthBar.setSize(sf::Vector2f(50.f, 5.f));
    healthBar.setFillColor(sf::Color::Green);
}

void Player::handleInput(std::vector<Entity*>& entities, sf::RenderWindow& window) 
{
    velocity.x = 0.f;
    // std::cout << cooldownClock.getElapsedTime().asSeconds() << std::endl;
    if (curState == AnimationState::AttackHit && cooldownClock.getElapsedTime().asSeconds() > 0.2f) canAttack = true;

    if (!canAttack) return; 
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        setAnimationState(AnimationState::Run);
        velocity.x = -speed;
        sprite.setScale(-1.f, 1.f);
        sprite.setOrigin(106, 0);
        facingRight = false;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        setAnimationState(AnimationState::Run);
        velocity.x = speed;
        sprite.setScale(1.f, 1.f);
        sprite.setOrigin(0, 0);
        facingRight = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->isOnGround()) 
    {
        velocity.y = -jumpStrength;
        this->setOnGround(false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && canAttack)
    {
        this->attackHit(entities, window);
        setAnimationState(AnimationState::AttackHit);
    }
    if (abs(velocity.x) < 1.f && canAttack)
    {
        setAnimationState(AnimationState::Idle);
    }
}

void Player::update(float deltaTime, std::vector<Structure*>& structures, std::vector<Entity*>& entities, sf::RenderWindow& window) 
{
    handleInput(entities, window);
    handleKb(deltaTime, structures);
    applyGravity(deltaTime, structures);

    // Temporary invincible after damage
    if (isInvincible())
    {
        setInvincibleTime(deltaTime + getInvincibleTime());
        if (getInvincibleTime() >= 2.f)
        {
            setInvincible(false);
            setInvincibleTime(0.f);
        }
    }
    
    switch(curState) 
    {
        case AnimationState::Idle:
            
            idleAnimation.update(deltaTime);
            idleAnimation.applyToSprite(sprite);
            break;

        case AnimationState::Run:
            
            runAnimation.update(deltaTime);
            runAnimation.applyToSprite(sprite);
            break;
        
        case AnimationState::AttackHit:

            attackHitAnimation.update(deltaTime);
            attackHitAnimation.applyToSprite(sprite);
            break;
    
    }
}

void Player::attackHit(std::vector<Entity*>& entities, sf::RenderWindow& window)
{
    // if (!canAttack || cooldownClock.getElapsedTime().asSeconds() < 0.1f) return;
    // std::cout << "HIT" << std::endl;
    canAttack = false;
    cooldownClock.restart();
    
    sf::FloatRect attackZone;
    sf::Vector2f pos = sprite.getPosition();

    float attackWidth = 50.f;
    float attackHeight = sprite.getGlobalBounds().height;

    if (facingRight) attackZone = sf::FloatRect(pos.x + 110, pos.y, attackWidth, attackHeight);
    else attackZone = sf::FloatRect(pos.x - 50, pos.y, attackWidth, attackHeight);
    
    for (auto& e : entities)
    {
        if (attackZone.intersects(e->getHitbox()))
        {
            sf::Vector2f kbVelocity = facingRight ? sf::Vector2f(200.f, 0.f) : sf::Vector2f(-200.f, 0.f);
            e->takeDamage(20, kbVelocity, 0.2f);
        }
    }
}

void Player::setAnimationState(AnimationState state)
{
    if (curState == state) return;
    curState = state;

    switch(state)
    {
        case AnimationState::Idle: idleAnimation.reset(); break;
        case AnimationState::Run: runAnimation.reset(); break;
        case AnimationState::AttackHit: attackHitAnimation.reset(); cooldownClock.restart(); break;
    }
}