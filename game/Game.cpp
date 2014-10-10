//
//  Game.cpp
//  OgreGameKit
//
//  Created by Alex on 10/2/14.
//
//

#include "Game.h"

// scenes
#include "MainMenuScene.h"
#include "SinglePlayerScene.h"

using namespace Ogre;

//namespace Ogre
//{
//    template<> Game* Ogre::Singleton<Game>::msSingleton = 0;
//};

Game::Game() : OGKGame()
{
    
}

Game::~Game()
{
    
}

void Game::setup()
{
    MainMenuScene *firstScene = OGRE_NEW MainMenuScene("menu");
    mGameSceneManager->addScene(firstScene,"menu");

    SinglePlayerScene *singlePlayer = OGRE_NEW SinglePlayerScene("ingame");
    mGameSceneManager->addScene(singlePlayer,"ingame");
//
//    OGKInGameScene *thirdScene = OGRE_NEW OGKInGameScene("ingame");
//    mGameSceneManager->addScene(thirdScene,"ingame");

    mGameSceneManager->setActiveScene("ingame");
    
}