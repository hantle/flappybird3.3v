//
//  FlappyBirdScene.cpp
//  hellococos2dx
//
//  Created by Macbook on 2014. 10. 26..
//
//

#include "FlappyBirdScene.h"
#include "MainMenuScene.h"
//#include "AdmobAndroid/AdMobHelper.h"

#define SCALE 3.0
#define FORCE 200.0

USING_NS_CC;
Scene* FlappyBird::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    auto world = scene->getPhysicsWorld();
    
    // 'layer' is an autorelease object
    auto layer = FlappyBird::create();
    
//    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    world->setGravity(Point(0, -5.0*FORCE));
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    CCLOG("loaded FlappyBirdScene");
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FlappyBird::init()
{
    if ( !Layer::init() )
        return false;
    
    CCLOG("%f", this->getContentSize().width);
    CCLOG("%f", this->getContentSize().height);
    birdCategory = 1 << 0;
    worldCategory = 1 << 1;
    pipeCategory = 1 << 2;
    scoreCategory = 1 << 3;
    
    birdspeed = 1;
    movingspeed = 1;
    canRestart = false;
    
    moving = Node::create();
    moving->retain();
    this->addChild(moving);
    
    pipes = Node::create();
    pipes->retain();
    moving->addChild(pipes);
    
    scoreLabelNode = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 60);
//    scoreLabelNode = Label::createWithTTF("0", "Marker Felt", 60);
    scoreLabelNode->setPosition(Point(this->getContentSize().width/2.0, 3.0*this->getContentSize().height/4.0));
    scoreLabelNode->setPositionZ(100);
    this->addChild(scoreLabelNode);
    
    this->Run();

    scoreboard = Sprite::create("scoreboard.png");
    scoreboard->setPosition(this->getContentSize().width/2, 0 - scoreboard->getContentSize().height);
    scoreboard->setScale(1.5);
    this->addChild(scoreboard, 100);
    
    return true;
}

void FlappyBird::resetScene() {
    bird->setPosition(this->getContentSize().width/2.5, this->getContentSize().height/2.0);
    bird->getPhysicsBody()->setVelocity(Vect(0,0));
    bird->getPhysicsBody()->setCollisionBitmask(worldCategory | pipeCategory);
    
    scoreboard->stopAllActions();
    scoreboard->setPosition(this->getContentSize().width/2, 0 - scoreboard->getContentSize().height);
    
    birdspeed = 1;
    movingspeed = 1;
    canRestart = false;
    
    pipes->removeAllChildren();

    score = 0;
    scoreLabelNode->setString("0");
    
    // animate again
//    this->runAction(spawnThenDelayForever);
    for (auto child: moving->getChildren()) {
        child->resume();
    }
//    for (auto child: pipes->getChildren()) {
//        child->resume();
//    }
    this->schedule(schedule_selector(FlappyBird::spawnPipes), 3.0);
}

void FlappyBird::goToMainMenuScene(float dt) {
//    this->scheduleOnce(FlappyBird::goToMainMenuScene, 2);
//    auto backgroundSprite = Sprite::create("HelloWorld.png");
//    backgroundSprite->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
//    this->addChild(backgroundSprite);
    
    auto scene = MainMenuScene::createScene();
    
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void FlappyBird::Run() {
//    auto skyColor = Color3B(81.0, 192.0/255.0, 201.0/255.0);
    glClearColor(81.0/255.0, 192.0/255.0, 201.0/255.0, 1.0);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);
    
    auto groundTexture = new Texture2D();
    groundTexture = Director::getInstance()->getTextureCache()->addImage("land.png");
    for (int i = 0; i < 2.0 + this->getContentSize().width / (groundTexture->getContentSize().width * SCALE); i++) {
        auto moveGroundSprite = MoveBy::create(0.02*SCALE*groundTexture->getContentSize().width, Point(-SCALE*groundTexture->getContentSize().width, 0));
        auto resetGroundSprite = MoveBy::create(0, Point(SCALE*groundTexture->getContentSize().width, 0));
        auto moveGroundSequence = Sequence::createWithTwoActions(moveGroundSprite, resetGroundSprite);
        auto moveGroundSpriteForever = RepeatForever::create(moveGroundSequence);
        
        auto sprite = Sprite::createWithTexture(groundTexture);
        sprite->setScale(SCALE);
        sprite->setPosition(Point(i*SCALE*sprite->getContentSize().width+SCALE*sprite->getContentSize().width/2, SCALE*sprite->getContentSize().height / 2.0));
        sprite->setPositionZ(0);
        sprite->runAction(moveGroundSpriteForever);
        moving->addChild(sprite);
    }
    
    // skyline
    auto skyTexture = new Texture2D();
    skyTexture = Director::getInstance()->getTextureCache()->addImage("sky.png");
    for (int i = 0; i < 2.0 + this->getContentSize().width / (skyTexture->getContentSize().width * SCALE); i++) {
        auto moveSkySprite = MoveBy::create(0.1 * SCALE*skyTexture->getContentSize().width, Point(-SCALE*skyTexture->getContentSize().width, 0));
        auto resetSkySprite = MoveBy::create(0.0, Point(SCALE*skyTexture->getContentSize().width, 0));
        auto moveSkySpritesSequence = Sequence::createWithTwoActions(moveSkySprite, resetSkySprite);
        auto moveSkySpritesForever = RepeatForever::create(moveSkySpritesSequence);
        
        auto sprite = Sprite::createWithTexture(skyTexture);
        sprite->setScale(SCALE);
        sprite->setPositionZ(-20);
        sprite->setPosition(i*SCALE*sprite->getContentSize().width + SCALE*sprite->getContentSize().width/2.0, SCALE*sprite->getContentSize().height / 2.0 + SCALE*groundTexture->getContentSize().height);
        sprite->runAction(moveSkySpritesForever);
        moving->addChild(sprite);
    }
    
    // create the pipes textures
    pipeTextureUp = new Texture2D();
    pipeTextureDown = new Texture2D();
    pipeTextureUp = Director::getInstance()->getTextureCache()->addImage("PipeUp.png");
    pipeTextureDown = Director::getInstance()->getTextureCache()->addImage("PipeDown.png");
    
    // create the pipes movement actions
    // spawn the pipes
//    auto spawn = CallFunc::create(CC_CALLBACK_0(FlappyBird::spawnPipes, this));
//    auto delay = DelayTime::create(3.0);
//    auto spawnThenDelay = Sequence::create(spawn, delay, NULL);
//    spawnThenDelayForever = RepeatForever::create(spawnThenDelay);
//    spawnThenDelayForever->setTag(99);
//    spawnThenDelayForever->retain();
//    this->runAction(spawnThenDelayForever);
    this->schedule(schedule_selector(FlappyBird::spawnPipes), 3.0);
    
    // setup our bird
    auto birdTexture1 = new Texture2D();
    auto birdTexture2 = new Texture2D();
    auto birdTexture3 = new Texture2D();
    auto birdTexture4 = new Texture2D();
    birdTexture1 = Director::getInstance()->getTextureCache()->addImage("bird-01.png");
    birdTexture2 = Director::getInstance()->getTextureCache()->addImage("bird-02.png");
    birdTexture3 = Director::getInstance()->getTextureCache()->addImage("bird-03.png");
    birdTexture4 = Director::getInstance()->getTextureCache()->addImage("bird-04.png");
    
    Rect rect = Rect::ZERO;
    rect.size = birdTexture1->getContentSize();
    
    auto anim = Animation::create();
    anim->setDelayPerUnit(0.2);
    anim->addSpriteFrameWithTexture(birdTexture1, rect);
    anim->addSpriteFrameWithTexture(birdTexture2, rect);
    anim->addSpriteFrameWithTexture(birdTexture3, rect);
    anim->addSpriteFrameWithTexture(birdTexture4, rect);
    auto repeatedAnimation = RepeatForever::create(Animate::create(anim));
    
    bird = Sprite::createWithTexture(birdTexture1);
    bird->setScale(SCALE);
    bird->setPosition(this->getContentSize().width * 0.35, this->getContentSize().height * 0.6);
    bird->setPositionZ(10);
    bird->runAction(repeatedAnimation);
    
    bird->setPhysicsBody(PhysicsBody::createCircle(SCALE*bird->getContentSize().height/2.0));
    bird->getPhysicsBody()->setDynamic(true);
    bird->getPhysicsBody()->setRotationEnable(false);
    bird->getPhysicsBody()->setCategoryBitmask(birdCategory);
    bird->getPhysicsBody()->setCollisionBitmask(worldCategory | pipeCategory);
    bird->getPhysicsBody()->setContactTestBitmask(worldCategory | pipeCategory | scoreCategory);
    this->addChild(bird);
    
    auto ground = Node::create();
    ground->setPosition(SCALE*groundTexture->getContentSize().width/2.0, SCALE*groundTexture->getContentSize().height/2.0);
    ground->setPhysicsBody(PhysicsBody::createBox(Size(SCALE*this->getContentSize().width, SCALE*groundTexture->getContentSize().height * 1.0)));
    ground->getPhysicsBody()->setDynamic(false);
    ground->getPhysicsBody()->setCategoryBitmask(worldCategory);
    ground->getPhysicsBody()->setContactTestBitmask(birdCategory);
    this->addChild(ground);
    
    // Touch Events (Single)
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(FlappyBird::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FlappyBird::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    scheduleUpdate();
}

// function
bool FlappyBird::onContactBegin(PhysicsContact &contact) {
    if ((contact.getShapeA()->getCategoryBitmask() & scoreCategory) == scoreCategory || (contact.getShapeB()->getCategoryBitmask() & scoreCategory) == scoreCategory) {
        score++;
        scoreLabelNode->setString(__String::createWithFormat("%d", score)->getCString());
        scoreLabelNode->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.5), ScaleTo::create(0.1, 1.0)));
    } else if ((contact.getShapeA()->getCategoryBitmask() & worldCategory) == worldCategory || (contact.getShapeB()->getCategoryBitmask() & worldCategory) == worldCategory) {
        CCLOG("contact world category");
    } else if (movingspeed == 1) {
        movingspeed = 0;
        // stop all animations
//        AdMobHelper::showAd();
        auto moveToCenter = MoveTo::create(0.5, Point(this->getContentSize().width/2, this->getContentSize().height/2));
        scoreboard->runAction(moveToCenter);
        
        this->unschedule(schedule_selector(FlappyBird::spawnPipes));
        this->stopActionByTag(99); // stop spawning pipes
        for (auto child: moving->getChildren()) { // stop moving background
            child->pause(); }
        for (auto child: pipes->getChildren()) { // stop moving pipes
            child->pause(); }
        
        bird->getPhysicsBody()->setCollisionBitmask(worldCategory); // make the bird fall
        
        auto rotate = RotateBy::create(1, (M_PI)*bird->getPosition().y*0.01);
        auto callback = CallFunc::create(CC_CALLBACK_0(FlappyBird::die, this));
        bird->runAction(Sequence::createWithTwoActions(rotate, callback));
        
        // Flash background if contact is detected
        auto bgSequence = Sequence::createWithTwoActions(CallFunc::create(CC_CALLBACK_0(FlappyBird::Blink, this)), DelayTime::create(0.05));
        auto backgroundAnimtaion = Repeat::create(bgSequence, 4);
        auto restartAnimation = Sequence::createWithTwoActions(backgroundAnimtaion, CallFunc::create(CC_CALLBACK_0(FlappyBird::setRestart, this)));
        this->runAction(restartAnimation);
    }
    return true;
}
void FlappyBird::Blink() {
    if (blink) {
        glClearColor(81.0/255.0, 192.0/255.0, 201.0/255.0, 1.0);
        blink = 0;
    } else {
        glClearColor(1, 0, 0, 1);
        blink = 1;
    }
}
void FlappyBird::die() {
    CCLOG("died");
    birdspeed = 0;
    bird->setRotation(0);
    
//    network::HttpRequest *request = new network::HttpRequest();
//    request->setUrl("http://localhost:8000/ranks");
//    request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
//    request->setResponseCallback(CC_CALLBACK_2(FlappyBird::onHttpRequestCompleted, this));
//    request->setTag("GET ranks");
//    network::HttpClient::getInstance()->send(request);
//    request->release();
    
    
//    cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
//    request->setUrl("http://www.httpbin.org/post");
//    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
//    request->setResponseCallback( CC_CALLBACK_2(HttpClientTest::onHttpRequestCompleted, this) );
//
//    // write the post data
//    const char* postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
//    request->setRequestData(postData, strlen(postData));
//
//    request->setTag("POST test1");
//    cocos2d::network::HttpClient::getInstance()->send(request);
//    request->release();

    auto webView = cocos2d::experimental::ui::WebView::create();
    webView->setPosition(Point(this->getContentSize().width/2, this->getContentSize().height/2));
    webView->setContentSize(Size(this->getContentSize().width/2,  this->getContentSize().height/2));
    webView->loadURL("http://localhost:8000/ranks");
//    webView->setScalesPageToFit(true);
    webView->setOnShouldStartLoading(CC_CALLBACK_2(FlappyBird::onWebViewShouldStartLoading, this));
    webView->setOnDidFinishLoading(CC_CALLBACK_2(FlappyBird::onWebViewDidFinishLoading, this));
    webView->setOnDidFailLoading(CC_CALLBACK_2(FlappyBird::onWebViewDidFailLoading, this));
    this->addChild(webView, 101);
}

void FlappyBird::onHttpRequestCompleted(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response) {
    if (!response) {
        return;
    }
    // You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag())) {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    
    if (!response->isSucceed()) {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    for (unsigned int i = 0; i < buffer->size(); i++) {
        printf("%c", (*buffer)[i]);
    }
    printf("\n");
}
bool FlappyBird::onWebViewShouldStartLoading(experimental::ui::WebView *sender, const std::string &url) {
//    CCLOG("onWebViewShouldStartLoading, url is %s", url.c_str());
    return true;
}

void FlappyBird::onWebViewDidFinishLoading(experimental::ui::WebView *sender, const std::string &url) {
//    CCLOG("onWebViewDidFinishLoading, url is %s", url.c_str());
}

void FlappyBird::onWebViewDidFailLoading(experimental::ui::WebView *sender, const std::string &url) {
//    CCLOG("onWebViewDidFailLoading, url is %s", url.c_str());
}


void FlappyBird::setRestart() {
    canRestart = true;
}
void FlappyBird::removePipe(cocos2d::Node *pipe) {
    pipes->removeChild(pipe);
}
void FlappyBird::spawnPipes(float dt) {
    auto distanceToMove = (float)(this->getContentSize().width + SCALE*pipeTextureUp->getContentSize().width);
    auto movePipes = MoveBy::create(0.01 * distanceToMove, Point(-distanceToMove, 0));
    auto removePipes = CallFuncN::create(CC_CALLBACK_1(FlappyBird::removePipe, this));
    movePipesAndRemove = Sequence::create(movePipes, removePipes, NULL);
    movePipesAndRemove->setTag(99);
    
    int verticalPipeGap = 250.0;
    
    auto pipePair = Node::create();
    pipePair->setPosition(this->getContentSize().width + SCALE*pipeTextureUp->getContentSize().width, 0);
    pipePair->setPositionZ(-10);
    
    int height = this->getContentSize().height / 4.0;
    int y = arc4random() % height + height;
    
    auto pipeDown = Sprite::createWithTexture(pipeTextureDown);
    pipeDown->setScale(SCALE);
    pipeDown->setPosition(0.0, (float)y + SCALE*pipeDown->getContentSize().height + float(verticalPipeGap));
    pipeDown->setPositionZ(-10);
    
    PhysicsBody *pipeDownBody = PhysicsBody::createBox(Size(SCALE * pipeDown->getContentSize().width, SCALE * pipeDown->getContentSize().height));
    pipeDownBody->setDynamic(false);
    pipeDownBody->setCategoryBitmask(pipeCategory);
    pipeDownBody->setContactTestBitmask(birdCategory);
    pipeDown->setPhysicsBody(pipeDownBody);
    pipePair->addChild(pipeDown);
    
    auto pipeUp = Sprite::createWithTexture(pipeTextureUp);
    pipeUp->setScale(SCALE);
    pipeUp->setPosition(0.0, (float)y);
    pipeUp->setPositionZ(-10);
    
    PhysicsBody *pipeUpBody = PhysicsBody::createBox(Size(SCALE * pipeUp->getContentSize().width, SCALE * pipeUp->getContentSize().height));
    pipeUpBody->setDynamic(false);
    pipeUpBody->setCategoryBitmask(pipeCategory);
    pipeUpBody->setContactTestBitmask(birdCategory);
    pipeUp->setPhysicsBody(pipeUpBody);
    pipePair->addChild(pipeUp);
    
    auto contactNode = Sprite::create();
    contactNode->setPosition(pipeDown->getContentSize().width + bird->getContentSize().width/2, this->getContentSize().height/2);
    contactNode->setPhysicsBody(PhysicsBody::createBox(Size(pipeUp->getContentSize().width, this->getContentSize().height)));
    contactNode->getPhysicsBody()->setDynamic(false);
    contactNode->getPhysicsBody()->setCategoryBitmask(scoreCategory);
    contactNode->getPhysicsBody()->setContactTestBitmask(birdCategory);
    pipePair->addChild(contactNode);
    pipePair->runAction(movePipesAndRemove);

    pipes->addChild(pipePair);
}

float clamp(float min, float max, float value) {
    if (value > max) {
        return max;
    } else if (value < min) {
        return min;
    } else {
        return value;
    }
}
void FlappyBird::update(float delta) {
    bird->setRotation(clamp(-40, 40, -bird->getPhysicsBody()->getVelocity().y));
}// Touches
bool FlappyBird::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
    if (movingspeed > 0) {
        bird->getPhysicsBody()->setVelocity(Vect(0,0));
        bird->getPhysicsBody()->applyImpulse(Vect(0, 900*FORCE));
    } else if (canRestart) {
//    	AdMobHelper::hideAd();
        this->resetScene();
    }
    return true;
}
