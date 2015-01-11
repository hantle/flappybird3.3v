//
//  Hero.cpp
//  flappybird
//
//  Created by Macbook on 2015. 1. 11..
//
//

#include "Hero.h"

#define BIRD_COLLISION_BITMASK 0x000001
#define BIRD_FALLING_SPEED 0.0025

USING_NS_CC;

Hero::Hero(cocos2d::Layer *layer)
{
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    flappyBird = Sprite::create("bird-01.png");
    flappyBird->setPosition(visibleSize.width/2, visibleSize.height/2);
    
    auto flappyBody = PhysicsBody::createCircle(flappyBird->getContentSize().width/2);
    flappyBody->setCollisionBitmask(BIRD_COLLISION_BITMASK);
    flappyBody->setContactTestBitmask(true);
    flappyBird->setPhysicsBody(flappyBody);
    
    layer->addChild(flappyBird, 100);
    
    isFalling = true;
}

void Hero::Fall() {
    if (isFalling) {
        flappyBird->setPositionX(visibleSize.width/2);
        flappyBird->setPositionY(flappyBird->getPositionY() - (BIRD_FALLING_SPEED * visibleSize.height));
    } else {
        flappyBird->setPositionX(visibleSize.width/2);
        flappyBird->setPositionY(flappyBird->getPositionY() + (BIRD_FALLING_SPEED * visibleSize.height));
    }
}