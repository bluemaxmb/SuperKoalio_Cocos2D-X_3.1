//
//  GameLevelLayer.cpp
//  SuperKoalio
//
//  Original by Jacob Gundersen on 6/4/12.
//  Copyright 2012 Interrobang Software LLC. All rights reserved.
//
//  Ported by Matthew Barney on 5/21/14.
//
//

#include "GameLevelLayer.h"

Scene* GameLevelLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLevelLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLevelLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    LayerColor* blueSky = LayerColor::create(Color4B(100, 100, 255, 255));
    this->addChild(blueSky, 0);
    
    m_pMap = TMXTiledMap::create("level1.tmx");
    this->addChild(m_pMap, 0);
    
    m_pPlayer = new Player("koalio_stand.png");
    m_pPlayer->setPosition(Vec2(100, 50));
    m_pMap->addChild(m_pPlayer->getPlayerSprite(), 15);
    
    m_pWallLayer = m_pMap->getLayer("walls");
    m_pHazardLayer = m_pMap->getLayer("hazards");
    
    setTouchEnabled(true);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("level1.mp3");
    
    this->scheduleUpdate();
    
    return true;
}

void GameLevelLayer::update(float delta)
{
    if (m_isGameOver)
    {
        return;
    }
    
    if (m_pPlayer)
    {
        m_pPlayer->update(delta);
        handleHazardCollisions(m_pPlayer);
        checkForWin();
        checkForAndResolveCollisions(m_pPlayer);
        setViewpointCenter(m_pPlayer->getPosition());
    }
}

Vec2 GameLevelLayer::tileCoordForPosition(Vec2 position)
{
    float x = floor(position.x / m_pMap->getTileSize().width);
    float levelHeightInPixels = m_pMap->getMapSize().height * m_pMap->getTileSize().height;
    float y = floor((levelHeightInPixels - position.y) / m_pMap->getTileSize().height);
    return Vec2(x, y);
}

Rect GameLevelLayer::tileRectFromTileCoords(Vec2 tileCoords)
{
    float levelHeightInPixels = m_pMap->getMapSize().height * m_pMap->getTileSize().height;
    Vec2 origin = Vec2(tileCoords.x * m_pMap->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * m_pMap->getTileSize().height));
    return Rect(origin.x, origin.y, m_pMap->getTileSize().width, m_pMap->getTileSize().height);
}

std::vector<tileInfo> GameLevelLayer::getSurroundingTilesAtPosition(Vec2 position, TMXLayer* layer)
{
    Vec2 plPos = tileCoordForPosition(position); //1
    
    std::vector<tileInfo> gids; //2
    
    for (int i = 0; i < 9; i++)
    { //3
        int c = i % 3;
        int r = (int)(i / 3);
        Vec2 tilePos = Vec2(plPos.x + (c - 1), plPos.y + (r - 1));
        
        if (tilePos.y > (m_pMap->getMapSize().height - 1))
        {
            //fallen in a hole
            gameOver(false);
            return gids;
        }
        
        int tgid = layer->getTileGIDAt(tilePos); //4
        
        Rect tileRect = tileRectFromTileCoords(tilePos); //5
        
        tileInfo tileDict;
        tileDict.gid = tgid;
        tileDict.x = tileRect.origin.x;
        tileDict.y = tileRect.origin.y;
        tileDict.tilePos = tilePos;
        
        gids.push_back(tileDict); //6
    }
    
    gids.erase(gids.begin()+4);
    gids.insert(gids.begin()+6, gids[2]);
    gids.erase(gids.begin()+2);
    
    tileInfo temp;
    temp = gids[6];
    gids[6] = gids[4];
    gids[4] = temp;
    
    temp = gids[0];
    gids[0] = gids[4];
    gids[4] = temp;
    
/*
 for (int i=0;i<gids.size();++i)
    {
        printf("i: %d\n", i);
        printf("gid: %d\n", gids[i].gid);
        printf("tilePos.x: %f\n", gids[i].tilePos.x);
        printf("tilePos.y: %f\n", gids[i].tilePos.y);
        printf("x: %f\n", gids[i].x);
        printf("y: %f\n\n", gids[i].y);
    }
 */
    
    return gids;
}

void GameLevelLayer::checkForAndResolveCollisions(Player *pThePlayer)
{
    std::vector<tileInfo> tiles = getSurroundingTilesAtPosition(pThePlayer->getPosition(), m_pWallLayer); //1
    
    if (m_isGameOver)
    {
        return;
    }
    
    pThePlayer->setOnGroundFlag(false);
    
    for (int i=0; i<tiles.size();++i)
    {
        Rect pRect = pThePlayer->getCollisionBoundBox(); //2
        
        int gid = tiles[i].gid; //3
        
        if (gid)
        {
            Rect tileRect = Rect(tiles[i].x,
                                 tiles[i].y,
                                 m_pMap->getTileSize().width,
                                 m_pMap->getTileSize().height);
            
            if (pRect.intersectsRect(tileRect))
            {
                // 5
                // This is replicating CGRectIntersection
                Rect intersection = Rect(std::max(pRect.getMinX(),tileRect.getMinX()),
                                    std::max(pRect.getMinY(),tileRect.getMinY()),
                                    0,
                                    0);
                
                intersection.size.width = std::min(pRect.getMaxX(),
                                                   tileRect.getMaxX()) - intersection.getMinX();
                intersection.size.height = std::min(pRect.getMaxY(),
                                                    tileRect.getMaxY()) - intersection.getMinY();
                
                int tileIndx = i; //6
                
                if (tileIndx == 0)
                {
                    //tile is directly below Koala
                    pThePlayer->setDesiredPosition(Vec2(pThePlayer->getDesiredPosition().x,
                                                        pThePlayer->getDesiredPosition().y + intersection.size.height));

                    pThePlayer->setVelocity(Vec2(pThePlayer->getGelocity().x, 0.0));
                    pThePlayer->setOnGroundFlag(true);
                }
                else if (tileIndx == 1)
                {
                    //tile is directly above Koala
                    pThePlayer->setDesiredPosition(Vec2(pThePlayer->getDesiredPosition().x, pThePlayer->getDesiredPosition().y - intersection.size.height));
                   pThePlayer->setVelocity(Vec2(pThePlayer->getGelocity().x, 0.0));
                }
                else if (tileIndx == 2)
                {
                    //tile is left of Koala
                    pThePlayer->setDesiredPosition(Vec2(pThePlayer->getDesiredPosition().x + intersection.size.width, pThePlayer->getDesiredPosition().y));
                }
                else if (tileIndx == 3)
                {
                    //tile is right of Koala
                    pThePlayer->setDesiredPosition(Vec2(pThePlayer->getDesiredPosition().x - intersection.size.width, pThePlayer->getDesiredPosition().y));
                }
                else
                {
                    if (intersection.size.width > intersection.size.height)
                    { //7
                        //tile is diagonal, but resolving collision vertically
                        pThePlayer->setVelocity(Vec2(pThePlayer->getGelocity().x, 0.0));
                        
                        float resolutionHeight;
                        if (tileIndx > 5)
                        {
                            resolutionHeight = intersection.size.height;
                            pThePlayer->setOnGroundFlag(true);
                        }
                        else
                        {
                            resolutionHeight = -intersection.size.height;
                        }
                        pThePlayer->setDesiredPosition(Vec2(pThePlayer->getDesiredPosition().x, pThePlayer->getDesiredPosition().y + intersection.size.height ));
                    }
                    else
                    {
                        //tile is diagonal, but resolving horizontally
                        float resolutionWidth;
                        
                        if (tileIndx == 6 || tileIndx == 4)
                        {
                            resolutionWidth = intersection.size.width;
                        }
                        else
                        {
                            resolutionWidth = -intersection.size.width;
                        }
                        pThePlayer->setDesiredPosition(Vec2(pThePlayer->getDesiredPosition().x, pThePlayer->getDesiredPosition().y + resolutionWidth));
                    } 
                } 
            }
        } 
    }
    pThePlayer->setPosition(pThePlayer->getDesiredPosition()); //7
}

void GameLevelLayer::handleHazardCollisions(Player* pThePlayer)
{
    std::vector<tileInfo> tiles = getSurroundingTilesAtPosition(pThePlayer->getPosition(), m_pHazardLayer);
    
    for (int i=0; i<tiles.size();++i)
    {
        Rect tileRect = Rect(tiles[i].x,
                             tiles[i].y,
                             m_pMap->getTileSize().width,
                             m_pMap->getTileSize().height);
        
        Rect pRect = pThePlayer->getCollisionBoundBox();
        
        if (tiles[i].gid && pRect.intersectsRect(tileRect))
        {
            gameOver(false);
        }
    }
}

// Camera
void GameLevelLayer::setViewpointCenter(Vec2 position)
{
    Size winSize = Director::getInstance()->getWinSize();
    
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (m_pMap->getMapSize().width * m_pMap->getTileSize().width)
            - winSize.width / 2);
    y = MIN(y, (m_pMap->getMapSize().height * m_pMap->getTileSize().height)
            - winSize.height/2);
    Vec2 actualPosition = Vec2(x, y);
    
    Vec2 centerOfView = Vec2(winSize.width/2, winSize.height/2);
    Vec2 viewPoint = centerOfView - actualPosition;
    m_pMap->setPosition(viewPoint);
}

void GameLevelLayer::checkForWin()
{
    if (m_pPlayer->getPosition().x > 3130.0)
    {
        gameOver(true);
    }
}

// Touches
void GameLevelLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    for (int i=0;i<touches.size();++i)
    {
        Vec2 touchLocation = convertTouchToNodeSpace(touches[i]);
        if (touchLocation.x > 240)
        {
            m_pPlayer->setIsJumpingFlag(true);
        }
        else
        {
            m_pPlayer->setIsMovingFlag(true);
        }
    }
}

void GameLevelLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    for (int i=0;i<touches.size();++i)
    {
        Vec2 touchLocation = convertTouchToNodeSpace(touches[i]);
        
        //get previous touch and convert it to node space
        Vec2 previousTouchLocation = touches[i]->getPreviousLocationInView();
        Size screenSize = Director::getInstance()->getWinSize();
        previousTouchLocation = Vec2(previousTouchLocation.x, screenSize.height - previousTouchLocation.y);
        
        if (touchLocation.x > 240 && previousTouchLocation.x <= 240)
        {
            m_pPlayer->setIsMovingFlag(false);
            m_pPlayer->setIsJumpingFlag(true);
        }
        else if (previousTouchLocation.x > 240 && touchLocation.x <=240)
        {
            m_pPlayer->setIsMovingFlag(true);
            m_pPlayer->setIsJumpingFlag(false);
        }
    }
}

void GameLevelLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    for (int i=0;i<touches.size();++i)
    {
        Vec2 touchLocation = convertTouchToNodeSpace(touches[i]);
        if (touchLocation.x < 240)
        {
            m_pPlayer->setIsMovingFlag(false);
        }
        else
        {
            m_pPlayer->setIsJumpingFlag(false);
        }
    }
}

// Menus
void GameLevelLayer::gameOver(bool playerDidWin)
{
	m_isGameOver = true;
    std::string gameText;
    
	if (playerDidWin)
    {
		gameText = "You Won!";
	}
    else
    {
		gameText = "You have Died!";
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("hurt.wav");
	}
    
    Menu* pMenu = CCMenu::create();
    pMenu->setPosition(Vec2(240, -100));
    
    MenuItemFont *diedLabel = MenuItemFont::create(gameText);
    diedLabel->setFontName("Marker Felt");
    diedLabel->setFontSize(24);
    diedLabel->setPosition(Vec2(240, 200));
    pMenu->addChild(diedLabel);
    
    MoveBy *slideIn = MoveBy::create(1.0, Vec2(0,250));
    
    MenuItemImage *replay = MenuItemImage::create("replay.png", "replay.png", "replay.png");
    replay->setPosition(Point::ZERO);
    replay->setCallback(CC_CALLBACK_1(GameLevelLayer::replayButtonCallback, this));
    pMenu->addChild(replay);
    
    this->addChild(pMenu, 1);
    
    pMenu->runAction(slideIn);
}

//Menu Callbacks
void GameLevelLayer::replayButtonCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameLevelLayer::createScene());
}