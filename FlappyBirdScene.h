//
//  FlappyBirdScene.h
//  hellococos2dx
//
//  Created by Macbook on 2014. 10. 26..
//
//

#ifndef __hellococos2dx__FlappyBirdScene__
#define __hellococos2dx__FlappyBirdScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CocosGUI.h"
#include "HttpClient.h"

//#include "HttpClient.cpp"

class FlappyBird : public cocos2d::Layer
{
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(FlappyBird);

private:
    // start
    cocos2d::Sprite *bird;
    cocos2d::Texture2D *pipeTextureUp;
    cocos2d::Texture2D *pipeTextureDown;
    cocos2d::Sequence *movePipesAndRemove;
    cocos2d::Node *moving;
    cocos2d::Node *pipes;
    cocos2d::Label *scoreLabelNode;
    cocos2d::RepeatForever *spawnThenDelayForever;
    
    cocos2d::Sprite *scoreboard;
    
    
    int birdCategory;
    int worldCategory;
    int pipeCategory;
    int scoreCategory;
    int blink;
    int birdspeed;
    int movingspeed;
    bool canRestart;
    int score;
    // function
    void resetScene();
    void setRestart();
    void Blink();
    void die();
    void removePipe(cocos2d::Node *pipe);
    void spawnPipes(float dt);
    void update(float delta);
    
    void Run();
    
    void goToMainMenuScene(float dt);

    // WebView Callbacks
    //we should check the validation of the URL and decide whether to continue or not
    void onHttpRequestCompleted(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response);
    
    
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    
    // Contact Listener
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    // Touches
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    // end
};

#endif // __HELLOWORLD_SCENE_H__
