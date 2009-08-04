#include "stdafx.h"
#include "menuhelp.h"

namespace Sajiki
{

	MenuHelp::MenuHelp()
	{

		sheet=0;

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(72,3,0));
		mEntity = ARoot->getScene()->createEntity("MainMenuMesh", "Main_menu.mesh");
		mEntity->setMaterialName("MainMenu");
		sn->attachObject(mEntity);
		sn->setScale(0.025, 0.025, 0.025);
		mAnimationState = mEntity->getAnimationState("Inst_loop");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);

		sn2=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(4,-1,0));
		sn2->roll(Ogre::Degree(160));
		mEntity2 = ARoot->getScene()->createEntity("HelpArrow", "Arrow_menu.mesh");
		mEntity2->setMaterialName("Menu/Arrow");
		sn2->attachObject(mEntity2);
		mAnimationState2 =  mEntity2->getAnimationState("Arrow_menu Animation");
		mAnimationState2->setEnabled(true);
		mAnimationState2->setLoop(true);
		mEntity2->setVisible(true);

		sn3=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-6,0,-2));
		mEntity3 = ARoot->getScene()->createEntity("HelpPlane", "plane.mesh");
		mEntity3->setMaterialName("Menu/Help1");
		sn3->attachObject(mEntity3);
		sn3->setScale(1.4, 1.1, 1);

		bEnableJoyX=false;
		iCurrentPage = 1;
	}

	MenuHelp::~MenuHelp()
	{
		ARoot->getScene()->destroyEntity(mEntity);
		ARoot->getScene()->destroyEntity(mEntity2);
		ARoot->getScene()->destroyEntity(mEntity3);
	}

	void MenuHelp::Update()
	{
		mAnimationState->addTime(ARoot->getTime());
		mAnimationState2->addTime(ARoot->getTime());

		if((AInput->isKey(OIS::KC_JOY1RIGHT)&& bEnableJoyX)||AInput->isKeySingle(OIS::KC_RIGHT))
			iCurrentPage++;
		else if((AInput->isKey(OIS::KC_JOY1LEFT)&& bEnableJoyX)||AInput->isKeySingle(OIS::KC_LEFT))
			iCurrentPage--;

		if(iCurrentPage>7)
			iCurrentPage=7;
		else if(iCurrentPage<1)
			iCurrentPage=1;

		std::stringstream strPageMat;
		strPageMat<<"Menu/Help" << iCurrentPage;
		mEntity3->setMaterialName(strPageMat.str());

		if(!AInput->isKey(OIS::KC_JOY1LEFT)&&!AInput->isKey(OIS::KC_JOY1RIGHT))
			bEnableJoyX=true;
		else
			bEnableJoyX=false;

		
		if(AInput->isKeySingle(OIS::KC_RETURN)||AInput->isKeySingle(OIS::KC_ESCAPE)||AInput->isKey(OIS::KC_JOY1B1))
			AInterface->setMenu(MENU_MAINMENU);
	}
}

