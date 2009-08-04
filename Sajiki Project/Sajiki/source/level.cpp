#include "stdafx.h"
#include "level.h"
#include "interface.h"

#include "tile.h"
#include "brokentile.h"
#include "tdd.h"
#include "timeobj.h"
#include "spikes.h"
#include "jumppad.h"
#include "spikeball.h"
#include "leveleditor.h"
#include "slowmo.h"
#include "rotationswitch.h"
#include "playerspawn.h"
#include "pointlight.h"

namespace Sajiki
{
	void Level::Init()
	{
		//null all level object pointers
		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				oLevel[x][y] = 0;
			}
		}

		mBackground = 0;
		for (int x=0; x<4; x++)
			mPlayer[x]=0;
		iLevelTheme = 1;
		setCameraProjection(Ogre::PT_PERSPECTIVE);
		AInterface->Init();
		
		ARoot->getPhysicsScene()->createActor("FakeFloor", new NxOgre::CubeShape(100,1,100), Ogre::Vector3(0,-1,0), "static: yes"); 
		ARoot->getPhysicsScene()->createActor("FakeRoof", new NxOgre::CubeShape(100,1,100), Ogre::Vector3(0,32,0), "static: yes"); 
		ARoot->getPhysicsScene()->createActor("FakeLeft", new NxOgre::CubeShape(1,100,100), Ogre::Vector3(-1,0,0), "static: yes"); 
		ARoot->getPhysicsScene()->createActor("FakeRight", new NxOgre::CubeShape(1,100,100), Ogre::Vector3(32,0,0), "static: yes"); 
		//ARoot->getPhysicsScene()->createActor("FakeFront", new NxOgre::CubeShape(100,100,1), Ogre::Vector3(0,0,2), "static: yes"); 
		//ARoot->getPhysicsScene()->createActor("FakeBack", new NxOgre::CubeShape(100,100,1), Ogre::Vector3(0,0,-2), "static: yes"); 
		
		iLightLevel = 100;
		fCameraRoll = fCameraRollTarget = 0;
		iPlayerCount = 0;
		bSlowMo=false; 

		for (int x=0; x<4; x++)
			mPlayerSpawnPos[x]=Ogre::Vector2(-1,-1);
		
		ASound->PlayMusic(6);

		sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"sajikilevel.layout"); 
		
	
		bLevelTransition=false;
		lMode=LM_NONE;
		setLevelMode(LM_MENU);
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmCountDown")->hide();
	}


	void Level::setLightLevel(int lightlevel)
	{
		iLightLevel = lightlevel;
		if (iLightLevel<0)
			iLightLevel=100;
		else if (iLightLevel>100)
			iLightLevel=0;


		ARoot->getScene()->setAmbientLight(Ogre::ColourValue(iLightLevel/100.0f,iLightLevel/100.0f,iLightLevel/100.0f,1));
	}

	void Level::slowMo()
	{
		ASound->PlaySound("slowmo.mp3");
		mSlowMoTimer.reset();
		bSlowMo=true;


	}


	void Level::setCameraProjection(Ogre::ProjectionType pt)
	{
		fCameraRoll = 0;

		
		if(pt == Ogre::PT_ORTHOGRAPHIC)
		{
			ARoot->getCamera()->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
			ARoot->getCamera()->setPosition(Ogre::Vector3(16,16,500.0f));
			ARoot->getCamera()->lookAt(Ogre::Vector3(16,16,0.0f));
			ARoot->getCamera()->setNearClipDistance(50.0f);
			ARoot->getCamera()->setFarClipDistance(6000.0f);
		}
		else
		{
			ARoot->getCamera()->setProjectionType(Ogre::PT_PERSPECTIVE);
			ARoot->getCamera()->setPosition(Ogre::Vector3(15.5,15.5,50.0f));
			ARoot->getCamera()->lookAt(Ogre::Vector3(15.5,15.5,0.0f));
			ARoot->getCamera()->setNearClipDistance(1.0f);
			ARoot->getCamera()->setFarClipDistance(6000.0f);
		}
	}

	void Level::Update()
	{
		switch(lMode)
		{
		case LM_NONE:
			break;
		case LM_PLAY:
			playUpdate();
			break;
		case LM_TEST:
			testUpdate();
			break;
		case LM_EDIT:
			levelEditor->Update();
			break;
		case LM_ROTATE:
			rotateUpdate();
			break;
		case LM_MENU:
			AInterface->Update();
			break;
		case LM_QUIT:
			break;
		default:
			break;
		}
	}

	void Level::randomRotate()
	{

		if(lMode == LM_ROTATE)
			return;

		srand(mSlowMoTimer.getMilliseconds());
		int iRotateAmount;
		iRotateAmount=rand()%4;

		
		switch(iRotateAmount)
		{
		case(0): 
			rotate(90);
			break;
		case(1): 
			rotate(180);
			break;
		case(2): 
			rotate(-90);
			break;
		case(3): 
			rotate(180);
			break;
		}
	}

	void Level::rotate(int iDegrees)
	{
		if(lMode == LM_ROTATE)
			return;

		if(lMode != LM_EDIT)
			ASound->PlaySound("levelrotate.mp3");

		lModeOld = lMode;

		fCameraRollOriginal=fCameraRoll;
		fCameraRollTarget=iDegrees;
		mRotateTimer.reset();

		mCameraOrientation=ARoot->getCamera()->getOrientation();

		lMode = LM_ROTATE;
	}

	void Level::rotateUpdate()
	{
		float fRotateTime = mRotateTimer.getMilliseconds()/1000.f;
		
		ARoot->getCamera()->roll(Ogre::Degree(-fCameraRoll));

		if (fRotateTime>1.5)
		{
			lMode = lModeOld;
			fCameraRoll = fCameraRollOriginal+fCameraRollTarget;
		}
		else
			fCameraRoll = fCameraRollOriginal+(fCameraRollTarget*((1-cosf(fRotateTime*2))/2));

		ARoot->getCamera()->roll(Ogre::Degree(fCameraRoll));


		if (lModeOld==LM_TEST || lModeOld==LM_PLAY)
		{
			for (int x=0; x<iPlayerCount; x++)
				mPlayer[x]->Rotate();

			for (int y=0; y<32; y++)
			{
				for (int x=0; x<32; x++)
				{
					if (oLevel[x][y])
						oLevel[x][y]->Update();
				}
			}
			mBackground->Update();
		}

		if (lMode==LM_TEST || lMode==LM_PLAY)
		{
			if(fCameraRoll==-90)
				fCameraRoll=270;
			else if(fCameraRoll==450)
				fCameraRoll=90;
			else if(fCameraRoll==-180)
				fCameraRoll=180;
			else if(fCameraRoll==360)
				fCameraRoll=0;
			else
			{
				if(fCameraRollOriginal+fCameraRollTarget>=360 || fCameraRollOriginal+fCameraRollTarget<=-360)
				fCameraRoll=0;
			}

			for (int x=iPlayerCount-1; x>=0; x--)
				mPlayer[x]->createPlayer();
		}

	
	}

	void Level::playUpdate()
	{
		if(!bRoundStarted)
		{
			if(mTimerRoundStart.getMilliseconds()<3000)
			{
				std::stringstream strRoundStart;
				strRoundStart << "READY!...." << 3-(mTimerRoundStart.getMilliseconds()/1000);
				CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/txtCountDown")->setText(strRoundStart.str());
				CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmCountDown")->show();
				return;
			}
			else
			{
				for (int x=0; x<iPlayerCount; x++)
				mPlayer[x]=new Player(mPlayerSpawnPos[x].x, mPlayerSpawnPos[x].y, x);
				bRoundStarted=true;
				CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmCountDown")->hide();
			}
		}

			
		
		

		if(bLevelTransition)
		{

			if(!mBoxTransition->Update())
				StartNextRound();
			return;
		}

		int iPlayersAlive=0;
		for (int x=0; x<iPlayerCount; x++)
		{
			if (!mPlayer[x]->isDead())
				iPlayersAlive++;
			mPlayer[x]->Update();
		}

		if (bRoundOver)
		{
			std::stringstream strRoundInfo;

			if (iCurrentRound < 6)
			{
				strRoundInfo << "ROUND " << iCurrentRound << " OVER!\n\n";
				if (iRoundWinner==0)
					strRoundInfo << "Its a tie! Nobody wins the round!\n\n";
				else
					strRoundInfo << "Player " << iRoundWinner << " wins the round!\n\n";
			}
			else
			{
				strRoundInfo << "GAME OVER!\n\n";
				if(iGameWinner==0)
					strRoundInfo << "Its a tie! Nobody wins the game!\n\n";
				else
					strRoundInfo << "Player " << iGameWinner << " wins the game!\n\n";
			}
			
			if (mRoundOverTimer.getMilliseconds()/1000 < 3)
				strRoundInfo << "Get Ready..." << 3-(int)(mRoundOverTimer.getMilliseconds()/1000);
			else
			{
				strRoundInfo << "Press attack button to continue...";

				bool bButtonPressed=false;
				for (int x=0; x<iPlayerCount; x++)
				{
					if(AInput->isKey(AInterface->getPlayerSettings(x)->ctrl.kAttack))
						bButtonPressed=true;
				}
	
				if(bButtonPressed)
				{
					if(iCurrentRound==6)
					{
						setLevelMode(LM_MENU);
					}
					else
					{
						CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmRoundSummary")->hide();
						setCameraProjection(Ogre::PT_PERSPECTIVE);
						bLevelTransition=true;
						mRoundOverTimer.reset();
					}
				}
			}

			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/txtRoundInfo")->setText(strRoundInfo.str());
			
			std::stringstream strScoreSummary;
			strScoreSummary << "SCORE SUMMARY\n\n";
			for (int x=0; x<iPlayerCount; x++)
			{
				strScoreSummary << "Player " << x+1 << " : " << playerScores[x] << " wins\n";
			}
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/txtScoreSumaryInfo")->setText(strScoreSummary.str());
			
			return;
		}

		mBackground->Update();
		if(bSlowMo)
		{
			if(mSlowMoTimer.getMilliseconds() > 5000)
				bSlowMo=false;
		}



		if (iPlayersAlive<2)
		{
			std::stringstream str;

			
			if (iPlayersAlive==0)
				iRoundWinner=0;
			else
			{
				for (int x=0; x<iPlayerCount; x++)
				{
					if (!mPlayer[x]->isDead())
					{
						iRoundWinner=x+1;
						playerScores[x]+=1;
					}
				}
			}

			iGameWinner=-1;
			
			if(iCurrentRound==6)
			{
				//find highest score
				for (int x=0; x<iPlayerCount; x++)
				{
					bool bWinner=true;
					if(iGameWinner==-1)
					{
						for (int y=0; y<iPlayerCount; y++)
						{
							if(playerScores[y]>=playerScores[x] && x!=y)
								bWinner=false;
						}
						if(bWinner)
							iGameWinner=x;
					}
					
				}
				iGameWinner++;
			}
			

			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmRoundSummary")->show();
			bRoundOver=true;
			mRoundOverTimer.reset();
		}

		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				if (oLevel[x][y])
					oLevel[x][y]->Update();
			}
		}

		if(AInput->isKeySingle(OIS::KC_ESCAPE))
		{
			setLevelMode(LM_MENU);
		}
		
	}
	


	void Level::testUpdate()
	{
		bRoundStarted=true;
		if(bSlowMo)
		{
			if(mSlowMoTimer.getMilliseconds() > 5000)
				bSlowMo=false;
		}

		mBackground->Update();

		for (int x=0; x<iPlayerCount; x++)
			mPlayer[x]->Update();

		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				if (oLevel[x][y])
					oLevel[x][y]->Update();
			}
		}
		

		if(AInput->isKey(OIS::KC_HOME))
			rotate(-90);

		if(AInput->isKey(OIS::KC_END))
			rotate(90);

		if(AInput->isKeySingle(OIS::KC_ESCAPE))
		setLevelMode(LM_EDIT);
		
	}
	
	void Level::addObject(int oType, int x, int y, int rot)
	{
		if (!oLevel[x][y])
		{
			if (oType==-1)
				return;
			else if (oType==OT_TILE)
				oLevel[x][y] = new Tile(x, y);
			else if (oType==OT_BROKENTILE)
				oLevel[x][y] = new BrokenTile(x, y);
			else if (oType==OT_TDD)
			{
				oLevel[x][y] = new TDD(x, y);
				vecSpikeballList.push_back(oLevel[x][y]);
			}
			else if (oType==OT_TIME)
				oLevel[x][y] = new TimeObj(x, y);
			else if (oType==OT_SPIKES)
				oLevel[x][y] = new Spikes(x, y);
			else if (oType==OT_JUMPPAD)
				oLevel[x][y] = new JumpPad(x, y);
			else if (oType==OT_SPIKEBALL)
			{
				oLevel[x][y] = new SpikeBall(x, y);
				vecSpikeballList.push_back(oLevel[x][y]);
			}
			else if (oType==OT_SLOWMO)
				oLevel[x][y] = new SlowMo(x, y);
			else if (oType==OT_ROTATIONSWITCH)
				oLevel[x][y] = new RotationSwitch(x, y);
			else if (oType==OT_PLAYERSPAWN1)
			{
				if(mPlayerSpawnPos[0]!=Ogre::Vector2(-1,-1))
					return;

				oLevel[x][y] = new PlayerSpawn(x, y, 1);
				mPlayerSpawnPos[0]=Ogre::Vector2(x, y);	
			}
			else if (oType==OT_PLAYERSPAWN2)
			{
				if(mPlayerSpawnPos[1]!=Ogre::Vector2(-1,-1))
					return;

				oLevel[x][y] = new PlayerSpawn(x, y, 2);
				mPlayerSpawnPos[1]=Ogre::Vector2(x, y);	
			}
			else if (oType==OT_PLAYERSPAWN3)			
			{
				if(mPlayerSpawnPos[2]!=Ogre::Vector2(-1,-1))
					return;

				oLevel[x][y] = new PlayerSpawn(x, y, 3);
				mPlayerSpawnPos[2]=Ogre::Vector2(x, y);	
			}
			else if (oType==OT_PLAYERSPAWN4)
			{
				if(mPlayerSpawnPos[3]!=Ogre::Vector2(-1,-1))
					return;

				oLevel[x][y] = new PlayerSpawn(x, y, 4);
				mPlayerSpawnPos[3]=Ogre::Vector2(x, y);	
			}
			else if (oType==OT_POINTLIGHT1)
				oLevel[x][y] = new PointLight(x, y, 1);
			else if (oType==OT_POINTLIGHT2)
				oLevel[x][y] = new PointLight(x, y, 2);
			else if (oType==OT_POINTLIGHT3)
				oLevel[x][y] = new PointLight(x, y, 3);
			else if (oType==OT_POINTLIGHT4)
				oLevel[x][y] = new PointLight(x, y, 4);
			else if (oType==OT_POINTLIGHT5)
				oLevel[x][y] = new PointLight(x, y, 4);

			else
				return;
			if (rot >= 0)
				oLevel[x][y]->setRotation(rot);

			oLevel[x][y]->setTheme(iLevelTheme);
		}
	}

	bool Level::isPlayerSpawnListComplete()
	{
		for(int x=0; x<4; x++)
		{
			if (mPlayerSpawnPos[x]==Ogre::Vector2(-1,-1))
				return false;
		}
		return true;
	}

	void Level::removeObject(int x, int y)
	{
		if (oLevel[x][y])
		{
			if (oLevel[x][y]->getType()==OT_PLAYERSPAWN1)
				mPlayerSpawnPos[0]=Ogre::Vector2(-1,-1);
			else if (oLevel[x][y]->getType()==OT_PLAYERSPAWN2)
				mPlayerSpawnPos[1]=Ogre::Vector2(-1,-1);
			else if (oLevel[x][y]->getType()==OT_PLAYERSPAWN3)
				mPlayerSpawnPos[2]=Ogre::Vector2(-1,-1);
			else if (oLevel[x][y]->getType()==OT_PLAYERSPAWN4)
				mPlayerSpawnPos[3]=Ogre::Vector2(-1,-1);

			delete oLevel[x][y];
			oLevel[x][y] = 0;
		}
	}

	void Level::clearLevel()
	{
		vecSpikeballList.clear();
		for (int x=0; x<4; x++)
			mPlayerSpawnPos[x]=Ogre::Vector2(-1,-1);

		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				if (oLevel[x][y])
				{
					delete oLevel[x][y];
					oLevel[x][y] = 0;
				}
			}
		}
	}

	void Level::saveLevel( const char *filename )
	{
		std::stringstream str;
		str << "media/levels/" << filename;
		std::ofstream myFile (str.str().c_str(), std::ios::out);
		myFile << iLevelTheme << std::endl;
		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				if (oLevel[x][y])
				{
					myFile << oLevel[x][y]->getType() << std::endl;
					myFile << oLevel[x][y]->getRotation() << std::endl;
				}
				else
				{
					myFile << -1 << std::endl;
					myFile << -1 << std::endl;
				}
			}
		}

		myFile << iLightLevel;
		myFile.close();
	}

	void Level::loadLevel( const char *filename )
	{
		clearLevel();
		std::stringstream sstr;
		sstr << "media/levels/" << filename;

		std::ifstream myFile (sstr.str().c_str(), std::ios::in);
		
		std::string str;
		std::getline( myFile, str);
		if(str == "") 
			return;

		iLevelTheme = atoi(str.c_str());
		delete mBackground;
		mBackground = new Background(iLevelTheme);

		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				int iType;
				int iRotation;
				std::string str;
				std::getline( myFile, str);
				
				iType = atoi(str.c_str());
				std::getline( myFile, str);
				iRotation = atoi(str.c_str());
				
				if (iType!=-1)
					addObject(iType, x, y, iRotation);
			}
		}
		//get ambient light level
		std::getline( myFile, str);
		if(str == "")
			setLightLevel(100);
		else
			setLightLevel(atoi(str.c_str()));
		
		myFile.close();
	}

	void Level::setLevelTheme( int iT )
	{ 
		iLevelTheme = iT;
		if (mBackground)
			delete mBackground;
		mBackground = new Background(iT);

		for (int y=0; y<32; y++)
		{
			for (int x=0; x<32; x++)
			{
				if (oLevel[x][y])
				{
					oLevel[x][y]->setTheme(iLevelTheme);
				}
			}
		}
	}

	//fills a cegui list box with level list
	void Level::getLevelList(std::string ceguiListBoxName)
	{
		CEGUI::Listbox* mylst = (CEGUI::Listbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)ceguiListBoxName.c_str());

		//clear list
		while(mylst->getItemCount())
			mylst->removeItem(mylst->getListboxItemFromIndex(0));


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
					CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem (cBuff);
					mylst->addItem(listboxitem);
				}
			}
		}
		while ( FindNextFile(hFind, &FindFileData) != 0 );

	}

	void Level::setLevelMode(levMode l)
	{
		CEGUI::MouseCursor::getSingleton().hide();
		lModeOld=l;

		switch(lMode)
		{
		case LM_NONE:
			break;
		case LM_PLAY:
			delete mBoxTransition;
			ASound->PlayMusic(0);
			for (int x=0; x<iPlayerCount; x++)
				delete mPlayer[x];
			setCameraProjection(Ogre::PT_PERSPECTIVE);
			
			break;
		case LM_TEST:
			for (int x=0; x<4; x++)
					delete mPlayer[x];
			break;
		case LM_EDIT:
			setCameraProjection(Ogre::PT_PERSPECTIVE);
			delete levelEditor;
			break;
		case LM_MENU:
			AInterface->showCodePlane(false);
			AInterface->setMenu(MENU_NONE);
			break;
		default:
			break;
		}

		lMode = l;
		switch(l)
		{
		case LM_NONE:
			break;
		case LM_PLAY:
			mBoxTransition = new BoxTransition();
			bRoundStarted=false;
			iCurrentRound=0;
			StartNextRound();
			break;
		case LM_TEST:
		
			AInterface->destroyPlayerSettings();
			AInterface->addPlayerControls(PC_KB1);
			AInterface->addPlayerControls(PC_KB2);
			AInterface->addPlayerControls(PC_KB3);
			AInterface->addPlayerControls(PC_KB4);
			AInterface->getPlayerSettings(1)->bIsMale=false;
			AInterface->getPlayerSettings(3)->bIsMale=false;
			iPlayerCount=2;
			for (int x=0; x<4; x++)
			{
				mPlayer[x]=new Player(mPlayerSpawnPos[x].x, mPlayerSpawnPos[x].y, x);
			}
			iPlayerCount=AInterface->getPlayerCount();
			bRoundOver=false;
			break;
		case LM_EDIT:
			CEGUI::MouseCursor::getSingleton().show();
			ASound->PlayMusic(0);
			levelEditor = new LevelEditor;

			if(mBackground==0)
				setLevelTheme(1);
			break;
		case LM_MENU:
			setLightLevel(100);
			ASound->PlayMusic(6);
			clearLevel();
			delete mBackground;
			mBackground = 0;
			AInterface->setMenu(MENU_MAINMENU);
			AInterface->showCodePlane(true);
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmRoundSummary")->hide();
			break;
		case LM_QUIT:
			break;
		default:
			break;
		}
		
	}

	void Level::StartNextRound()
	{
		bRoundStarted=false;
		setCameraProjection(Ogre::PT_PERSPECTIVE);
		ASound->PlayMusic(iCurrentRound+1);
		AInterface->getSystem()->setGUISheet(sheet);
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Level/frmRoundSummary")->hide();
		iPlayerCount=AInterface->getPlayerCount();
		loadLevel(AInterface->getLevelQueueName(iCurrentRound).c_str());
		if(iCurrentRound)
		{
			for (int x=0; x<iPlayerCount; x++)
				delete mPlayer[x];
		}
		else
		{
			for(int x=0; x<4; x++)
				playerScores[x]=0;
		}
		bRoundOver=false;
		bLevelTransition=false;
		iCurrentRound++;
		mTimerRoundStart.reset();
	}

	bool Level::PressInfoOk(const CEGUI::EventArgs& e)
	{
		setLightLevel(100);
		
		if(iCurrentRound==6)
		{
			setLevelMode(LM_MENU);
		}
		else
		{
			setCameraProjection(Ogre::PT_PERSPECTIVE);
			bLevelTransition=true;
		}
		return true;
	}

	Player* Level::getPlayer(int no)
	{
		return mPlayer[no];
	}
}