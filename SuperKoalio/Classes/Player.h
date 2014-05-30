//
//  Player.h
//  SuperKoalio
//
//  Original by Jacob Gundersen on 6/4/12.
//  Copyright 2012 Interrobang Software LLC. All rights reserved.
//
//  Ported by Matthew Barney on 5/21/14.
//
//

#ifndef SuperKoalio_Player_h
#define SuperKoalio_Player_h

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Player : public cocos2d::Sprite
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Sprite* createSprite();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Player);
    
    //scheduled methods
    virtual void update(float dt);
    
    //Accessors
    void setDesiredPosition(const Vec2& pos);
    Vec2 getDesiredPosition();
    void setVelocity(const Vec2& velocity);
    Vec2 getGelocity();
    
    void setOnGroundFlag(const bool isOnGround);
    bool getOnGroundFlag();
    void setIsMovingFlag(const bool isMoving);
    bool getIsMovingFlag();
    void setIsJumpingFlag(const bool isJumping);
    bool getIsJumpingFlag();
    
    Rect getCollisionBoundBox();
    
private:
    Vec2 m_vVelocity;
    Vec2 m_vDesiredPosition;
    bool m_isOnGround;
    bool m_isMoving;
    bool m_isJumping;
};

#endif
