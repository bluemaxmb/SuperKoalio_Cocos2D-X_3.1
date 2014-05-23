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

class Player
{
public:
    Player(const std::string& filename);
    ~Player();
    
    void update(float delta);
    
    //Accessors
    void setPosition(const Vec2& pos);
    Vec2 getPosition();
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
    
    Sprite* getPlayerSprite();
    
    Rect getCollisionBoundBox();
    
private:
    Vec2 m_vVelocity;
    Vec2 m_vDesiredPosition;
    Sprite* m_pPlayerSprite;
    bool m_isOnGround;
    bool m_isMoving;
    bool m_isJumping;
};

#endif
