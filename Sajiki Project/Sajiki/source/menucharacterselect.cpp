#include "stdafx.h"
#include "menucharacterselect.h"

namespace Sajiki
{

	MenuCharacterSelect::MenuCharacterSelect()
	{

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(72,3,0));
		mEntity = ARoot->getScene()->createEntity("MainMenuMesh", "Main_menu.mesh");
		mEntity->setMaterialName("MainMenu");
		sn->attachObject(mEntity);
		sn->setScale(0.025, 0.025, 0.025);
		mAnimationState = mEntity->getAnimationState("Charsel_loop");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);

		ASound->PlayMusic(5);

		sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"sajikicharacterselection.layout"); 
	
		AInterface->getSystem()->setGUISheet(sheet);

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Charsel/frmReady")->hide();

		PlayerNode[0] = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(12,16,0));
		PlayerNode[1] = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(30,16,0));
		PlayerNode[2] = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(12,0,0));
		PlayerNode[3] = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(30,0,0));

		for(int x=0; x<4; x++)
		{
			PlayerMeshM[x]=0;
			PlayerMeshF[x]=0;
			PlayerNode[x]->setScale(5.5,5.5,5.5);
			bEnableInput[x] = true;
		}
		AInterface->destroyPlayerSettings();

		for(int x=0; x<4; x++)
		{
			bPlayerReady[x]=false;
		}
		
		MenuSelect=0;
		bGameReady=false;
	}

	MenuCharacterSelect::~MenuCharacterSelect()
	{
		for(int x=0; x<4; x++)
		{
			if(PlayerMeshM[x]!=0)
			{
				PlayerNode[x]->detachAllObjects();
				ARoot->getScene()->destroyEntity(PlayerMeshM[x]);
				ARoot->getScene()->destroyEntity(PlayerMeshF[x]);
				PlayerNode[x]->removeAndDestroyAllChildren();
			}
		}
		ARoot->getScene()->destroyEntity(mEntity);
	}
	void MenuCharacterSelect::Update()
	{
		mAnimationState->addTime(ARoot->getTime());

		if(AInput->isKey(OIS::KC_ESCAPE))
		{
			ASound->PlayMusic(6);
			AInterface->setMenu(MENU_MAINMENU);
			return;
		}

		bool bAllReady=true;
		for(int x=0; x<4; x++)
		{
			std::stringstream strPlayerReady;
			strPlayerReady << "Charsel/txtPlayerReady" << x+1;
			if(bPlayerReady[x])
				CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)strPlayerReady.str().c_str())->show();
			else
			{
				CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)strPlayerReady.str().c_str())->hide();
				
				if(AInterface->getPlayerCount()>1)
				{
					if(x<AInterface->getPlayerCount())
						bAllReady=false;
				}
				else
					bAllReady=false;
			}

		}
		if(bAllReady)
		{
			if(bEnableInput2[0])
			{
				if(AInput->isKey(AInterface->getPlayerSettings(0)->ctrl.kDown))
				{
					MenuSelect++;
					if(MenuSelect>3)
						MenuSelect=0;
					bEnableInput2[0]=false;
				}
				else if(AInput->isKey(AInterface->getPlayerSettings(0)->ctrl.kUp))
				{
					MenuSelect--;
					if(MenuSelect<0)
						MenuSelect=3;
					bEnableInput2[0]=false;
				}
			}
			else
			{
				if (!(AInput->isKey(AInterface->getPlayerSettings(0)->ctrl.kUp) || AInput->isKey(AInterface->getPlayerSettings(0)->ctrl.kDown)))
				bEnableInput2[0]=true;
			}

			if(bEnableInputbutton[0])
			{
				if (AInput->isKey(AInterface->getPlayerSettings(0)->ctrl.kAttack))
				{
					bEnableInputbutton[0]=false;
					switch(MenuSelect)
					{
					case(0):
						for(int x=0; x<6; x++)
						{
							bool bOriginalLevel=true;
							do
							{
								srand(rand());
								int irand=rand()%(AInterface->getvecLevelList()->size());
								strLevelQueue[x]=AInterface->getvecLevelList()->at(irand)->c_str();
								
								std::string mystr = strLevelQueue[x];

								bOriginalLevel=true;
								for(int y=0; y<x; y++)
								{
									if(x!=y)
									{
										std::string mystr2 = strLevelQueue[y];
										if(strLevelQueue[x] == strLevelQueue[y])
											bOriginalLevel=false;
									}

								}

							} while (!bOriginalLevel);
						}
						AInterface->setLevelQueue(strLevelQueue);
						ALevel->setLevelMode(LM_PLAY);
						return;
						break;
					case(1):
						AInterface->setMenu(MENU_LEVELSELECT);
						return;
						break;
					case(2):
						for(int x=0; x<4; x++)
						{
							bPlayerReady[x]=false;
						}
						MenuSelect=0;
						break;
					case(3):
						ASound->PlayMusic(6);
						AInterface->setMenu(MENU_MAINMENU);
						return;
						break;
					}
				}
			}
			else
			{
				if (!(AInput->isKey(AInterface->getPlayerSettings(0)->ctrl.kAttack)))
					bEnableInputbutton[0]=true;
			}



			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Charsel/frmReady")->show();
			std::stringstream strReady;
			strReady<<"PLEASE SELECT\n\n";
			switch(MenuSelect)
			{
			case(0):
				strReady<<">INSTANT ACTION<\nLevel Select\nPlayer Select\nQuit";
				break;
			case(1):
				strReady<<"Instant Action\n>LEVEL SELECT<\nPlayer Select\nQuit";
				break;
			case(2):
				strReady<<"Instant Action\nLevel Select\n>PLAYER SELECT<\nQuit";
				break;
			case(3):
				strReady<<"Instant Action\nLevel Select\nPlayer Select\n>QUIT<";
				break;
			}
			
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Charsel/txtReady")->setText(strReady.str().c_str());
			return;
		}
		else
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Charsel/frmReady")->hide();
		for (int x=0; x<AInterface->getPlayerCount(); x++)
		{
			PlayerNode[x]->yaw(Ogre::Degree(ARoot->getTime() *50));

			if(bEnableInputbutton[x])
			{
				if (AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kAttack))
				{
					bPlayerReady[x]=bPlayerReady[x]^1;
					bEnableInputbutton[x]=false;
				}
			}
			else
			{
				if (!(AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kAttack)))
					bEnableInputbutton[x]=true;
			}

			if(bPlayerReady[x])
				continue;

			if(bEnableInput[x])
			{
				if (AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kLeft) || AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kRight))
				{
					AInterface->getPlayerSettings(x)->bIsMale = AInterface->getPlayerSettings(x)->bIsMale^1;
					bEnableInput[x]=false;
				}
			}
			else
			{
				if (!(AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kLeft) || AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kRight)))
				bEnableInput[x]=true;
			}

			if(bEnableInput2[x])
			{
				if(AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kUp))
				{
					AInterface->getPlayerSettings(x)->iColour++;
					bEnableInput2[x]=false;
				}
				else
				if(AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kDown))
				{
					AInterface->getPlayerSettings(x)->iColour--;
					bEnableInput2[x]=false;
				}

				if(AInterface->getPlayerSettings(x)->iColour > MAX_PLAYER_THEMES)
					AInterface->getPlayerSettings(x)->iColour=0;
				else if(AInterface->getPlayerSettings(x)->iColour < 0)
					AInterface->getPlayerSettings(x)->iColour=MAX_PLAYER_THEMES;
			}
			else
			{
				if (!(AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kUp) || AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kDown)))
				bEnableInput2[x]=true;
			}


			if(AInterface->getPlayerSettings(x)->bIsMale)
			{
				PlayerMeshM[x]->setVisible(true);
				PlayerMeshF[x]->setVisible(false);
				mAnimState=PlayerMeshM[x]->getAnimationState("Run");
				mAnimState->addTime(ARoot->getTime() / 2);
				std::stringstream strMaterialName;
				strMaterialName << "Player/PlayerM" << AInterface->getPlayerSettings(x)->iColour;
				PlayerMeshM[x]->setMaterialName(strMaterialName.str());
			}
			else
			{
				PlayerMeshM[x]->setVisible(false);
				PlayerMeshF[x]->setVisible(true);
				mAnimState=PlayerMeshF[x]->getAnimationState("Run");
				mAnimState->addTime(ARoot->getTime() / 2);

				std::stringstream strMaterialName;
				strMaterialName << "Player/PlayerF" << AInterface->getPlayerSettings(x)->iColour;
				PlayerMeshF[x]->setMaterialName(strMaterialName.str());
			}
		}

		bool bPlayerAdded = false;
		if(AInput->isKey(OIS::KC_RCONTROL))
			bPlayerAdded = AInterface->addPlayerControls(PC_KB1);
		else if(AInput->isKey(OIS::KC_LCONTROL))
			bPlayerAdded = AInterface->addPlayerControls(PC_KB2);
		else if(AInput->isKey(OIS::KC_SPACE))
			bPlayerAdded = AInterface->addPlayerControls(PC_KB3);
		else if(AInput->isKey(OIS::KC_NUMPADENTER))
			bPlayerAdded = AInterface->addPlayerControls(PC_KB4);
		else if(AInput->isKey(OIS::KC_JOY1B1)||AInput->isKey(OIS::KC_JOY1B2))
			bPlayerAdded = AInterface->addPlayerControls(PC_JOY1);
		else if(AInput->isKey(OIS::KC_JOY2B1)||AInput->isKey(OIS::KC_JOY2B2))
			bPlayerAdded = AInterface->addPlayerControls(PC_JOY2);
		else if(AInput->isKey(OIS::KC_JOY3B1)||AInput->isKey(OIS::KC_JOY3B2))
			bPlayerAdded = AInterface->addPlayerControls(PC_JOY3);
		else if(AInput->isKey(OIS::KC_JOY4B1)||AInput->isKey(OIS::KC_JOY4B2))
			bPlayerAdded = AInterface->addPlayerControls(PC_JOY4);

		if (bPlayerAdded)
		{
			std::stringstream str;
			str << "playerpreview" << AInterface->getPlayerCount()-1;
			
			PlayerMeshM[AInterface->getPlayerCount()-1] = ARoot->getScene()->createEntity(str.str(), "Character_M.mesh");
			
			mAnimState=PlayerMeshM[AInterface->getPlayerCount()-1]->getAnimationState("Run");
			mAnimState->setEnabled(true);
			mAnimState->setLoop(true);
			str << "f";
			PlayerMeshF[AInterface->getPlayerCount()-1] = ARoot->getScene()->createEntity(str.str(), "Character_F.mesh");
			
			PlayerNode[AInterface->getPlayerCount()-1]->attachObject(PlayerMeshM[AInterface->getPlayerCount()-1]);
			PlayerNode[AInterface->getPlayerCount()-1]->attachObject(PlayerMeshF[AInterface->getPlayerCount()-1]);
			mAnimState=PlayerMeshF[AInterface->getPlayerCount()-1]->getAnimationState("Run");
			mAnimState->setEnabled(true);
			mAnimState->setLoop(true);
			bEnableInputbutton[AInterface->getPlayerCount()-1]=false;
		}
	}


}

