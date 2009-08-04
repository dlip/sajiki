#include "stdafx.h"
#include "menumain.h"

namespace Sajiki
{

	MenuMain::MenuMain()
	{
		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(72,3,0));
		mEntity = ARoot->getScene()->createEntity("MainMenuMesh", "Main_menu.mesh");
		mEntity->setMaterialName("MainMenu");
		sn->attachObject(mEntity);
		sn->setScale(0.025, 0.025, 0.025);

		sn2=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(0,-100,0));
		mEntity2 = ARoot->getScene()->createEntity("MenuArrow", "Arrow_menu.mesh");
		mEntity2->setMaterialName("Menu/Arrow");
		sn2->attachObject(mEntity2);
		mAnimationState2 =  mEntity2->getAnimationState("Arrow_menu Animation");
		mAnimationState2->setEnabled(true);
		mAnimationState2->setLoop(true);

		switch(AInterface->iLastMenu)
		{
		case(MENU_NONE):
			mAnimationState = mEntity->getAnimationState("Init_menu");
			break;
		case(MENU_CHARSEL):
			mAnimationState = mEntity->getAnimationState("Menu_return");
			break;
		case(MENU_CREDITS):
			mAnimationState = mEntity->getAnimationState("Credits_2_menu");
			break;
		case(MENU_OPTIONS):
			mAnimationState = mEntity->getAnimationState("Options_2_menu");
			break;
		case(MENU_HELP):
			mAnimationState = mEntity->getAnimationState("Inst_2_menu");
			break;
		case(MENU_LEVELEDITOR):
			mAnimationState = mEntity->getAnimationState("Menu_return");
			break;
		case(MENU_LEVELSELECT):
			mAnimationState = mEntity->getAnimationState("Menu_return");
			break;
		};
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(false);
		
		sheet=0;
		mOption = AInterface->iLastOption;
		bStarted=bFinished=false;

		bEnableJoyX=false;

	}

	MenuMain::~MenuMain()
	{
		AInterface->iLastOption = mOption;
		ARoot->getScene()->destroyEntity(mEntity);
		ARoot->getScene()->destroyEntity(mEntity2);
	}

	void MenuMain::Update()
	{

		mAnimationState->addTime(ARoot->getTime());
		mAnimationState2->addTime(ARoot->getTime());

		if(!bStarted)
		{
			mEntity2->setVisible(false);
			if(mAnimationState->hasEnded())
			{
				mAnimationState->setEnabled(false);
				mAnimationState = mEntity->getAnimationState("Menu_loop");
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(true);
				bStarted=true;
				mEntity2->setVisible(true);
			}
			
			return;
		}

		if(bFinished)
		{
			mEntity2->setVisible(false);

			if(mOption==MENU_LEVELEDITOR || mOption==MENU_EXIT)
			{
				if(mAnimationState->getTimePosition()>1.2)
				{
					mEntity->setVisible(false);
					AInterface->showCodePlane(false);
				}
			}

			if(mOption==MENU_CHARSEL)
			{
				if(mAnimationState->getTimePosition()<3)
					mAnimationState->addTime(ARoot->getTime()*3);
				else
					mAnimationState->addTime(ARoot->getTime());
			}

			if(mAnimationState->hasEnded())
			{
				switch(mOption)
				{
				case(MENU_CHARSEL):
					AInterface->setMenu(MENU_CHARSEL);
					break;
				case(MENU_LEVELEDITOR):
					AInterface->setMenu(MENU_LEVELEDITOR);
					break;
				case(MENU_OPTIONS):
					AInterface->setMenu(MENU_OPTIONS);
					break;
				case(MENU_CREDITS):
					AInterface->setMenu(MENU_CREDITS);
					break;
				case(MENU_HELP):
					AInterface->setMenu(MENU_HELP);
					break;
				case(MENU_EXIT):
					ALevel->setLevelMode(LM_QUIT);
					break;
				};
			}
			return;
		}

		if(AInput->isKeySingle(OIS::KC_ESCAPE))
		{
			mOption=MENU_EXIT;
			bFinished=true;
			mAnimationState->setEnabled(false);
			mAnimationState = mEntity->getAnimationState("Menu_2_editor");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(false);
		}

		if(AInput->isKeySingle(OIS::KC_RETURN)||AInput->isKey(OIS::KC_JOY1B1))
		{
			bFinished=true;
			mAnimationState->setEnabled(false);
			switch(mOption)
			{
				case(MENU_CHARSEL):
					mAnimationState = mEntity->getAnimationState("Menu_2_charsel");
					break;
				case(MENU_LEVELEDITOR):
					mAnimationState = mEntity->getAnimationState("Menu_2_editor");
					break;
				case(MENU_OPTIONS):
					mAnimationState = mEntity->getAnimationState("Menu_2_options");
					break;
				case(MENU_CREDITS):
					mAnimationState = mEntity->getAnimationState("Menu_2_credits");
					break;
				case(MENU_HELP):
					mAnimationState = mEntity->getAnimationState("Menu_2_inst");
					break;
				case(MENU_EXIT):
					mAnimationState = mEntity->getAnimationState("Menu_2_editor");
					break;
			};

			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(false);
		}

		if(AInput->isKey(OIS::KC_JOY1RIGHT)&& bEnableJoyX)
			mOption++;
		else if(AInput->isKey(OIS::KC_JOY1LEFT)&& bEnableJoyX)
			mOption--;

		if(!AInput->isKey(OIS::KC_JOY1LEFT)&&!AInput->isKey(OIS::KC_JOY1RIGHT))
			bEnableJoyX=true;
		else
			bEnableJoyX=false;
		
		

		if(AInput->isKeySingle(OIS::KC_RIGHT))
			mOption++;
		if(AInput->isKeySingle(OIS::KC_LEFT))
			mOption--;

		if(mOption<MENU_CHARSEL)
			mOption= MENU_EXIT;
		else if(mOption>MENU_EXIT)
			mOption= MENU_CHARSEL;

		sn2->resetOrientation();
		switch(mOption)
		{
		case(MENU_CHARSEL):
			sn2->setPosition(8,13,0);
			sn2->roll(Ogre::Degree(115));
			break;
		case(MENU_LEVELEDITOR):
			sn2->setPosition(9,19,0);
			sn2->roll(Ogre::Degree(70));
			break;
		case(MENU_OPTIONS):
			sn2->setPosition(14,22,0);
			sn2->roll(Ogre::Degree(20));
			break;
		case(MENU_CREDITS):
			sn2->setPosition(19,21,0);
			sn2->roll(Ogre::Degree(-20));
			break;
		case(MENU_HELP):
			sn2->setPosition(23,18,0);
			sn2->roll(Ogre::Degree(-50));
			break;
		case(MENU_EXIT):
			sn2->setPosition(19,8,0);
			sn2->roll(Ogre::Degree(-160));
			break;
		};	
	}

}

