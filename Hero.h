//
//  Hero.h
//  flappybird
//
//  Created by Macbook on 2015. 1. 11..
//
//

#ifndef __BIRD_H__
#define __BIRD_H__

#include "cocos2d.h"

class Hero
{
public:
    Hero(cocos2d::Layer *layer);
    
    void Fall();
    void Fly() {isFalling = false;};
    void StopFlying(float dt) {isFalling = true;};
private:
    bool isFalling;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Sprite *flappyBird;
};



#endif
