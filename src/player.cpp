#include "player.h"

Player::Player(float x, float y) 
{
    if (!texture.loadFromFile("assets/stickman/stickman.png")) std::cerr << "Error: Failed to load stickman.png" << std::endl;
    if (!animation.loadFromFile("assets/stickman/stickman_ani.png")) std::cerr << "Error: Failed to load stickman_ani.png" << std::endl;

    runAnimation = Animation(&animation, 8, 0.15f, 122, 171, 3);
    idleAnimation = Animation(&animation, 8, 0.15f, 122, 171, 1);
    attackHitAnimation = Animation(&animation, 4, 0.05f, 157, 171, 0);
    attackKickAnimation = Animation(&animation, 2, 0.15f, 122, 171, 2);
    guardShieldAnimation = Animation(&animation, 2, 0.15f, 122, 171, 4);
    slideAnimation = Animation(&animation, 8, 0.0625f, 142, 171, 5);

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
    if (!window.hasFocus()) return;
    if (canAttack) isAttacking = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && canAttack)
    {
        guardShield();
        setAnimationState(AnimationState::guardShield);
        velocity.x = 0;
        return;
    }
    else if (curState == AnimationState::AttackKick)
    {
        if (isOnGround())
        {
            canAttack = true;
            isAttacking = false;
            velocity.x = 0;
        }
        else if (!canAttack) attackKick(entities, window);
    }
    else if (curState == AnimationState::Slide)
    {
        if (cooldownClock.getElapsedTime().asSeconds() > 0.5f) 
        {
            canAttack = true;
            velocity.x = 0.f;
            setInvincible(false, 0);
        }
        else 
        {
            slide();
        }
    } 
    else velocity.x = 0.f;

    
    if (curState == AnimationState::AttackHit && cooldownClock.getElapsedTime().asSeconds() > 0.2f) canAttack = true;
    
    if (!canAttack) return; 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        setAnimationState(AnimationState::Run);
        velocity.x = -speed;
        sprite.setScale(-1.f, 1.f);
        sprite.setOrigin(106, 0);
        facingRight = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && isOnGround())
        {
            setAnimationState(AnimationState::Slide);
            slide();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        setAnimationState(AnimationState::Run);
        velocity.x = speed;
        sprite.setScale(1.f, 1.f);
        sprite.setOrigin(0, 0);
        facingRight = true;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && isOnGround())
        {
            setAnimationState(AnimationState::Slide);
            slide();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isOnGround()) 
    {
        velocity.y = -jumpStrength;
        setOnGround(false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && canAttack && isOnGround() && !isInvincible())
    {
        attackHit(entities, window);
        setAnimationState(AnimationState::AttackHit);
        isAttacking = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && canAttack && !isOnGround() && !isInvincible())
    {
        attackKick(entities, window);
        setAnimationState(AnimationState::AttackKick);
        isAttacking = true;
    }
    if (abs(velocity.x) < 1.f && canAttack)
    {
        setAnimationState(AnimationState::Idle);
    }
}

void Player::update(float deltaTime, std::vector<Structure*>& structures, std::vector<Entity*>& entities, sf::RenderWindow& window) 
{
    animationTime += deltaTime;
    handleInput(entities, window);
    handleKb(deltaTime, structures);
    applyGravity(deltaTime, structures);

    // Temporary invincible after damage
    if (isInvincible())
    {
        setInvincibleTime(deltaTime + getInvincibleTime());
        if (getInvincibleTime() >= 2.f)
        {
            setInvincible(false, 0);
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

        case AnimationState::Slide:

            slideAnimation.update(deltaTime);
            slideAnimation.applyToSprite(sprite);
            break;

        case AnimationState::AttackHit:

            attackHitAnimation.update(deltaTime);
            attackHitAnimation.applyToSprite(sprite);
            break;
        
        case AnimationState::AttackKick:

            attackKickAnimation.update(deltaTime);
            attackKickAnimation.applyToSprite(sprite);
            break;
        
        case AnimationState::guardShield:

            guardShieldAnimation.update(deltaTime);
            guardShieldAnimation.applyToSprite(sprite);
            break;
    }
}

void Player::attackHit(std::vector<Entity*>& entities, sf::RenderWindow& window)
{
    // if (!canAttack || cooldownClock.getElapsedTime().asSeconds() < 0.1f) return;
    // std::cout << "HIT" << std::endl;
    canAttack = false;
    isAttacking = true;
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

void Player::attackKick(std::vector<Entity*>& entities, sf::RenderWindow& window)
{
    if (canAttack)
    {
        canAttack = false;
        isAttacking = true;
        velocity.x = facingRight ? 500.f : -500.f;
    }
    
    for (auto& e : entities)
    {
        if (this->getHitbox().intersects(e->getHitbox()) && !e->isInvincible())
        {
            std::cout << "D" << std::endl;
            sf::Vector2f kbVelocity = facingRight ? sf::Vector2f(200.f, 0.f) : sf::Vector2f(-200.f, 0.f);
            e->takeDamage(30, kbVelocity, 0.2f);
            e->setInvincible(true, 30);
            canAttack = true;
        }
    }
}

void Player::guardShield()
{
    setGuarding(true);
}

void Player::slide()
{
    if (canAttack) cooldownClock.restart();
    canAttack = false;
    velocity.x = facingRight ? 700.f : -700.f;
    setInvincible(true, 0);
}

void Player::setAnimationState(AnimationState state)
{
    if (curState == state) return;
    curState = state;
    animationTime = 0.f;

    switch(state)
    {
        case AnimationState::Idle: idleAnimation.reset(); break;
        case AnimationState::Run: runAnimation.reset(); break;
        case AnimationState::AttackHit: attackHitAnimation.reset(); cooldownClock.restart(); break;
    }
}

bool Player::getCanAttack()
{
    return canAttack;
}

void Player::setCanAttack(const bool option)
{
    canAttack = option;
}

std::string Player::getCurrentAnimation() const
{
    if (curState == AnimationState::Idle) return "Idle";
    else if (curState == AnimationState::Run) return "Run";
    else if (curState == AnimationState::Slide) return "Slide";
    else if (curState == AnimationState::AttackHit) return "AttackHit";
    else if (curState == AnimationState::AttackKick) return "AttackKick";
    else return "guardShield";
}
void Player::setCurrentAnimation(const std::string ani)
{
    if (ani == "Idle") curState = AnimationState::Idle;
    else if (ani == "Run") curState = AnimationState::Run;
    else if (ani == "Slide") curState = AnimationState::Slide;
    else if (ani == "AttackHit") curState = AnimationState::AttackHit;
    else if (ani == "AttackKick") curState = AnimationState::AttackKick;
    else if (ani == "guardShield") curState = AnimationState::guardShield;
}

float Player::getAnimationTime() const
{
    return animationTime;
}

bool Player::getIsAttacking() const
{
    return isAttacking;
}
void Player::setIsAttacking(const bool option)
{
    isAttacking = option;
}

float Player::getDamageCooldown() const
{
    return damageCooldown;
}

void Player::setDamageCooldown(const float val)
{
    damageCooldown = val;
}