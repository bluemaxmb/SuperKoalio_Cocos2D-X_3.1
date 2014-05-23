//
//  GameLevelLayer.h
//  SuperKoalio
//
//  Original by Jacob Gundersen on 6/4/12.
//  Copyright 2012 Interrobang Software LLC. All rights reserved.
//
//  Ported by Matthew Barney on 5/21/14.
//
//

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

struct tileInfo
{
    int gid;
    float x;
    float y;
    Vec2 tilePos;
};

class GameLevelLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void update(float delta);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLevelLayer);
    
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    
private:
    //Collisions
    Vec2 tileCoordForPosition(Vec2 position);
    Rect tileRectFromTileCoords(Vec2 tileCoords);
    std::vector<tileInfo> getSurroundingTilesAtPosition(Vec2 position, TMXLayer* layer);
    void checkForAndResolveCollisions(Player *pThePlayer);
    void handleHazardCollisions(Player* pThePlayer);
    void checkForWin();
    
    // Camera
    void setViewpointCenter(Vec2 position);
    
    // Menu
    void gameOver(bool playerDidWin);
    
    //Menu Callbacks
    void replayButtonCallback(Ref* pSender);
    
    //data
    TMXTiledMap* m_pMap;
    TMXLayer *m_pWallLayer;
    TMXLayer *m_pHazardLayer;
    
    Player* m_pPlayer;
    
    bool m_isGameOver;

};

#endif // __HELLOWORLD_SCENE_H__
