//
//  Player.cpp
//  SuperKoalio
//
//  Original by Jacob Gundersen on 6/4/12.
//  Copyright 2012 Interrobang Software LLC. All rights reserved.
//
//  Ported by Matthew Barney on 5/21/14.
//
//

#include "Player.h"

Sprite* Player::createSprite()
{
    // 'sprite' is an autorelease object
    auto sprite = Player::create();
    
    // return the layer
    return sprite;
}

// on "init" you need to initialize your instance
bool Player::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Sprite::initWithFile("koalio_stand.png") )
    {
        return false;
    }
    
    m_isJumping = false;
    m_isMoving = false;
    m_isOnGround = true;
    
    m_vDesiredPosition = Vec2::ZERO;
    m_vVelocity = Vec2::ZERO;
    
    return true;
}

void Player::update(float delta)
{
    Vec2 gravity = Vec2(0.0, -450.0);
    Vec2 gravityStep = gravity * delta;
    
    Vec2 forwardMove = Vec2(800.0, 0.0);
    Vec2 forwardStep = forwardMove * delta;
    
    m_vVelocity = m_vVelocity + gravityStep;
    m_vVelocity = Vec2(m_vVelocity.x * .90, m_vVelocity.y);
    
    Vec2 jumpForce = Vec2(0.0, 310.0);
    float jumpCutoff = 150.0;
    
    if (m_isJumping && m_isOnGround)
    {
        m_vVelocity = m_vVelocity + jumpForce;
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("jump.wav");
    }
    else if (!m_isJumping && m_vVelocity.y > jumpCutoff)
    {
        m_vVelocity = Vec2(m_vVelocity.x, jumpCutoff);
    }
    
    if (m_isMoving)
    {
        m_vVelocity = m_vVelocity + forwardStep;
    }
    
    Vec2 minMovement = Vec2(0.0, -450.0);
    Vec2 maxMovement = Vec2(120.0, 250.0);
    m_vVelocity = m_vVelocity.getClampPoint(minMovement, maxMovement); //4
    
    Vec2 stepVelocity = m_vVelocity * delta;
    Vec2 movementVector = getPosition() + stepVelocity;
    
    setDesiredPosition(movementVector);
}

//Accessors
void Player::setDesiredPosition(const Vec2& pos)
{
    m_vDesiredPosition = pos;
}

Vec2 Player::getDesiredPosition()
{
    return m_vDesiredPosition;
}

void Player::setVelocity(const Vec2& velocity)
{
    m_vVelocity = velocity;
}

Vec2 Player::getGelocity()
{
    return m_vVelocity;
}

void Player::setOnGroundFlag(const bool isOnGround)
{
    m_isOnGround = isOnGround;
}

bool Player::getOnGroundFlag()
{
    return m_isOnGround;
}

void Player::setIsMovingFlag(const bool isMoving)
{
    m_isMoving = isMoving;
}

bool Player::getIsMovingFlag()
{
    return m_isMoving;
}

void Player::setIsJumpingFlag(const bool isJumping)
{
    m_isJumping = isJumping;
}

bool Player::getIsJumpingFlag()
{
    return m_isJumping;
}

Rect Player::getCollisionBoundBox()
{
    Rect boundingBox = getBoundingBox();
    
    //Inset
    Rect collisionBox = Rect(getBoundingBox().origin.x + 3,
                             getBoundingBox().origin.y,
                             getBoundingBox().size.width - 6,
                             getBoundingBox().size.height);

    Vec2 diff = m_vDesiredPosition - getPosition();
    
    //Offset
    Rect returnBoundingBox = Rect(collisionBox.origin.x + diff.x,
                                  collisionBox.origin.y + diff.y,
                                  collisionBox.size.width,
                                  collisionBox.size.height);
    
    return returnBoundingBox;
}