#include "stdafx.h"
#include "menucredits.h"

namespace Sajiki
{

	MenuCredits::MenuCredits()
	{
		sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"sajikicredits.layout"); 
		AInterface->getSystem()->setGUISheet(sheet);

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(72,3,0));
		mEntity = ARoot->getScene()->createEntity("MainMenuMesh", "Main_menu.mesh");
		mEntity->setMaterialName("MainMenu");
		sn->attachObject(mEntity);
		sn->setScale(0.025, 0.025, 0.025);
		mAnimationState = mEntity->getAnimationState("Credits_loop");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);

		sn2=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(32,0,0));
		sn2->roll(Ogre::Degree(10));
		mEntity2 = ARoot->getScene()->createEntity("CreditsArrow", "Arrow_menu.mesh");
		mEntity2->setMaterialName("Menu/Arrow");
		sn2->attachObject(mEntity2);
		mAnimationState2 =  mEntity2->getAnimationState("Arrow_menu Animation");
		mAnimationState2->setEnabled(true);
		mAnimationState2->setLoop(true);
		mEntity2->setVisible(true);
	}

	MenuCredits::~MenuCredits()
	{
		ARoot->getScene()->destroyEntity(mEntity);
		ARoot->getScene()->destroyEntity(mEntity2);
	}

	void MenuCredits::Update()
	{
		mAnimationState->addTime(ARoot->getTime());
		mAnimationState2->addTime(ARoot->getTime());

		if(AInput->isKeySingle(OIS::KC_ESCAPE)||AInput->isKeySingle(OIS::KC_RETURN)||AInput->isKey(OIS::KC_JOY1B1)||AInput->isKey(OIS::KC_JOY1B2))
			AInterface->setMenu(MENU_MAINMENU);
	}

}

