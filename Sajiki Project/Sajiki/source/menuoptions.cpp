#include "stdafx.h"
#include "menuoptions.h"

namespace Sajiki
{

	MenuOptions::MenuOptions()
	{

		sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"sajikioptions.layout"); 
	
		AInterface->getSystem()->setGUISheet(sheet);

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(72,3,0));
		mEntity = ARoot->getScene()->createEntity("MainMenuMesh", "Main_menu.mesh");
		mEntity->setMaterialName("MainMenu");
		sn->attachObject(mEntity);
		sn->setScale(0.025, 0.025, 0.025);
		mAnimationState = mEntity->getAnimationState("Options_loop");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);

		sn2=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(28,25,0));
		sn2->roll(Ogre::Degree(160));
		mEntity2 = ARoot->getScene()->createEntity("CreditsArrow", "Arrow_menu.mesh");
		mEntity2->setMaterialName("Menu/Arrow");
		sn2->attachObject(mEntity2);
		mAnimationState2 =  mEntity2->getAnimationState("Arrow_menu Animation");
		mAnimationState2->setEnabled(true);
		mAnimationState2->setLoop(true);
		mEntity2->setVisible(true);


		bEnableSound = ASound->bSoundEnabled;
		bEnableJoyX=false;

	}

	MenuOptions::~MenuOptions()
	{
		ARoot->getScene()->destroyEntity(mEntity);
		ARoot->getScene()->destroyEntity(mEntity2);
	}
	
	void MenuOptions::Update()
	{

		if(bEnableSound)
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Options/txtOptions")->setText("Sound: Yes");
		else
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Options/txtOptions")->setText("Sound: No");

		mAnimationState->addTime(ARoot->getTime());
		mAnimationState2->addTime(ARoot->getTime());

		if((AInput->isKey(OIS::KC_JOY1RIGHT)&& bEnableJoyX)||(AInput->isKey(OIS::KC_JOY1LEFT)&& bEnableJoyX)||AInput->isKeySingle(OIS::KC_LEFT)||AInput->isKeySingle(OIS::KC_RIGHT))
			bEnableSound = bEnableSound^1;

		if(!AInput->isKey(OIS::KC_JOY1LEFT)&&!AInput->isKey(OIS::KC_JOY1RIGHT))
			bEnableJoyX=true;
		else
			bEnableJoyX=false;

		if(AInput->isKeySingle(OIS::KC_RETURN)||AInput->isKeySingle(OIS::KC_ESCAPE)||AInput->isKey(OIS::KC_JOY1B1))
		{

			if(bEnableSound != ASound->bSoundEnabled)
			{
				if (bEnableSound)
				{
					ASound->CacheSounds();
					ASound->bSoundEnabled=true;
					ASound->PlayMusic(6);
				}
				else
				{
					ASound->PlayMusic(0);
					ASound->bSoundEnabled=false;
				}
				ASound->SaveOptions();
			}

			AInterface->setMenu(MENU_MAINMENU);
		}


	/*	if(bEnableSound != ASound->bSoundEnabled)
		{
		}
		else
		AInterface->setMenu(MENU_MAINMENU);*/
	}

	/*bool MenuOptions::SoundToggle(const CEGUI::EventArgs& e)
	{
		if (CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Options/chkSound")->getProperty("Selected")=="True")
		{
			ASound->CacheSounds();
			ASound->bSoundEnabled=true;
			ASound->PlayMusic(6);
		}
		else
		{
			ASound->PlayMusic(0);
			ASound->bSoundEnabled=false;
		}
		return true;
	}

	bool MenuOptions::Ok(const CEGUI::EventArgs& e)
	{
		ASound->SaveOptions();
		AInterface->setMenu(MENU_MAINMENU);
		return true;
	}

	bool MenuOptions::Cancel(const CEGUI::EventArgs& e)
	{
		if (bEnableSound!=ASound->bSoundEnabled)
		{
			if(bEnableSound)
			{
				ASound->bSoundEnabled=true;
				ASound->PlayMusic(6);
			}
			else
			{
				ASound->bSoundEnabled=false;
				ASound->PlayMusic(0);
			}

		}

		AInterface->setMenu(MENU_MAINMENU);
		return true;
	}*/

}

