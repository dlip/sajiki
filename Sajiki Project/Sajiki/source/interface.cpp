#include "stdafx.h"
#include "level.h"
#include "interface.h"

#include "menu.h"
#include "menucredits.h"
#include "menuhelp.h"
#include "menuoptions.h"
#include "menulevelselect.h"
#include "menucharacterselect.h"
#include "menumain.h"

namespace Sajiki
{
	void Interface::Init()
	{
		mCurrentMenu = 0;

		iPlayers=0;
		for (int x=0; x<4; x++)
			mPlayerSettings[x]=0;

		CreateLevelPreviews();
		iLastMenu=MENU_CHARSEL;

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-17,-10,-3));
		mEntity = ARoot->getScene()->createEntity("CodePlane", "plane.mesh");
		mEntity->setMaterialName("SajikiCode");
		sn->attachObject(mEntity);
		sn->setScale(2.3, 1.5, 1);
		iLastOption=1;

		//initialise CEGUI
		mRenderer = new CEGUI::OgreCEGUIRenderer(ARoot->getWindow(), Ogre::RENDER_QUEUE_OVERLAY, false, 3000, ARoot->getScene());
		mSystem = new CEGUI::System(mRenderer);
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
		mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
		CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());  
		CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(0.0, 0.0));
		
	}

	void Interface::showCodePlane(bool b)
	{
		mEntity->setVisible(b);
	}

	void Interface::setMenu(eMenu eMnu)
	{
		if(mCurrentMenu)
		{
			delete mCurrentMenu;
			mCurrentMenu=0;
		}


		switch(eMnu)
		{
		case(MENU_NONE):
			break;
		case(MENU_CHARSEL):
			mCurrentMenu = new MenuCharacterSelect();
			break;
		case(MENU_LEVELSELECT):
			mCurrentMenu = new MenuLevelSelect();
			break;
		case(MENU_CREDITS):
			mCurrentMenu = new MenuCredits();
			break;
		case(MENU_OPTIONS):
			mCurrentMenu = new MenuOptions();
			break;
		case(MENU_HELP):
			mCurrentMenu = new MenuHelp();
			break;
		case(MENU_LEVELEDITOR):
			ALevel->setLevelMode(LM_EDIT);
			break;
		case(MENU_MAINMENU):
			mCurrentMenu = new MenuMain();
			break;

		};

		iLastMenu = eMnu;
		
	}


	void Interface::Update()
	{
		if(mCurrentMenu)
			mCurrentMenu->Update();
	}


	void Interface::CreateLevelPreviews()
	{

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;

		// Find the first file in the directory.
		hFind = FindFirstFile( L"media/levels/*.*", &FindFileData );
		char cBuff[256];

		do
		{
			int i = 0;

			while ( ( (char)FindFileData.cFileName[i] != '\0' ) && ( (char)FindFileData.cFileName[i] != '.' ) )
			{
			  cBuff[i] = (char)FindFileData.cFileName[i];
			  i++;
			}

			cBuff[i] = '\0';
			if (cBuff[0]!='\0')
			{
				if(strcmp(cBuff,"temp")) //dont show temp level in list
				{
					Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(cBuff, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					std::stringstream strTex;
					strTex << "levels/"<<cBuff<<".jpg";
					Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState(strTex.str());
					std::string *strLevel = new std::string;
					*strLevel = cBuff;
					vecLevelList.push_back(strLevel);
				}
			}
		}
		while ( FindNextFile(hFind, &FindFileData) != 0 );
	}

	void Interface::destroyPlayerSettings()
	{
		iPlayers=0;
		for (int x=0; x<4; x++)
		{
			if (mPlayerSettings[x]!=0)
			{
				delete mPlayerSettings[x];
				mPlayerSettings[x]=0;
			}
		}
	}

	void Interface::setLevelQueue(std::string lvlqueue[6])
	{
		for (int x=0; x<6; x++)
			strLevelQueue[x]=lvlqueue[x];

	}

	bool Interface::addPlayerControls(ePlayerControls pc)
	{
		if (iPlayers==4)
			return false;

		bool bAdd=true;

		for (int x=0; x<4; x++)
		{
			if(mPlayerSettings[x]!=0)
				if (mPlayerSettings[x]->pc==pc)
					bAdd=false;
		}
		
		if(bAdd)
		{
			mPlayerSettings[iPlayers] = new sPlayerSettings();
			mPlayerSettings[iPlayers]->bIsMale=true;
			mPlayerSettings[iPlayers]->pc=pc;
			mPlayerSettings[iPlayers]->iColour=0;
			switch(pc)
			{
			case PC_KB1:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_RCONTROL;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_UP;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_DOWN;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_LEFT;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_RIGHT;
				break;
			case PC_KB2:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_LCONTROL;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_W;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_S;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_A;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_D;
				break;
			case PC_KB3:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_SPACE;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_I;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_K;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_J;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_L;
				break;
			case PC_KB4:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_NUMPADENTER;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_NUMPAD8;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_NUMPAD5;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_NUMPAD4;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_NUMPAD6;
				break;
			case PC_JOY1:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_JOY1B1;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_JOY1B2;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_JOY1DOWN;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_JOY1LEFT;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_JOY1RIGHT;
				break;
			case PC_JOY2:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_JOY2B1;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_JOY2B2;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_JOY2DOWN;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_JOY2LEFT;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_JOY2RIGHT;
				break;
			case PC_JOY3:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_JOY3B1;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_JOY3B2;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_JOY3DOWN;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_JOY3LEFT;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_JOY3RIGHT;
				break;
			case PC_JOY4:
				mPlayerSettings[iPlayers]->ctrl.kAttack = OIS::KC_JOY4B1;
				mPlayerSettings[iPlayers]->ctrl.kUp = OIS::KC_JOY4B2;
				mPlayerSettings[iPlayers]->ctrl.kDown = OIS::KC_JOY4DOWN;
				mPlayerSettings[iPlayers]->ctrl.kLeft = OIS::KC_JOY4LEFT;
				mPlayerSettings[iPlayers]->ctrl.kRight = OIS::KC_JOY4RIGHT;
				break;
			}
			iPlayers++;
		}

		return bAdd;
	}

}

