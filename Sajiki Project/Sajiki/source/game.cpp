#include "stdafx.h"
#include "game.h"
#include "level.h"


namespace Sajiki
{
	Game::Game()
	{
		bGameStarted=false;
		if(!ARoot->Init())
			return;
		
		AInput->Init(ARoot->getWindow());
		ASound->Init();
		ALevel->Init();
	
		bGameStarted=true;

		Ogre::WindowEventUtilities::addWindowEventListener(ARoot->getWindow(), this);
		ARoot->getRoot()->addFrameListener(this);
		ARoot->getRoot()->startRendering();
	}

	bool Game::frameStarted(const Ogre::FrameEvent &evt)
	{
		if(ARoot->getWindow()->getLastFPS()>300)
			Sleep(5);
		ARoot->Update();
		ALevel->Update();
		AInput->Update();
		ASound->Update();
		return true;
	}

	bool Game::frameEnded(const Ogre::FrameEvent &evt) 
	{
		if (ALevel->getLevelMode()==LM_QUIT)
			ARoot->getRoot()->queueEndRendering();
		return true;
	}

	void Game::windowClosed(Ogre::RenderWindow* rw)
	{
		ALevel->setLevelMode(LM_QUIT);
	}

	Game::~Game()
	{
		if(!bGameStarted)
			return;

		ASound->Destroy();
		AInput->Destroy();
		AInterface->Destroy();
		ALevel->Destroy();
		ARoot->Destroy();
	}
}