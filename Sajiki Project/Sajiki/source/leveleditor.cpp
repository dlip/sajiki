#include "stdafx.h"
#include "leveleditor.h"

#include "tile.h"
#include "brokentile.h"
#include "tdd.h"
#include "timeobj.h"
#include "spikes.h"
#include "jumppad.h"
#include "spikeball.h"
#include "spikes.h"
#include "slowmo.h"
#include "rotationswitch.h"
#include "playerspawn.h"
#include "pointlight.h"

namespace Sajiki
{
	LevelEditor::LevelEditor():
		bMouseInputEnabled(false),
		mAsset(0),
		iCurrentRot(0),
		iIsometricZoom(50)
	{
		sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"sajikileveleditor.layout"); 
	
		mBackground = ARoot->getScene()->createEntity( "Grid", "backgrounds/plane.mesh" );
		mBackground->setMaterialName("Grid");
		nBackground = ARoot->getScene()->getRootSceneNode()->createChildSceneNode("Grid");
		nBackground->attachObject(mBackground);
		nBackground->setPosition(Ogre::Vector3(0, 0, -1.5));


		AInterface->getSystem()->setGUISheet(sheet);
		ALevel->setCameraProjection(Ogre::PT_ORTHOGRAPHIC);

		//set up listeners for menus and buttons
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnNew")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::clearLevel, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/btnSave")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::saveLevel, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/btnOpen")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::loadLevel, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnFile")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::toggleFileMenu, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnQuit")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::quitLevel, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnPlay")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::playLevel, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnHelp")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::toggleHelp, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnRotateLeft")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::rotateLeft, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnRotateRight")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::rotateRight, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnResetRotation")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::resetRotation, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/lstOpenName")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::setSaveName, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/btnDelete")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::deleteFile, this));
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/btnSpawnErrorOk")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&LevelEditor::hideSpawnError, this));

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->hide();
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtHelp")->hide();
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtSpawnError")->hide();
	
		nCursor = ARoot->getScene()->getRootSceneNode()->createChildSceneNode("nCursor");
		setCurrentObj(OT_TILE);

		ALevel->getLevelList("Root/frmFile/lstOpenName");
		ALevel->loadLevel("temp");
		bTakeScreenshot = false;

		//set up preview plane for screenshots when loading levels
		snPreview = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(27,15,2));
		entPreview = ARoot->getScene()->createEntity("PreviewPlane", "levelpreview.mesh");
		snPreview->attachObject(entPreview);
		snPreview->setScale(0.6,0.6,0.6);
		entPreview->setMaterialName("Cake");
		entPreview->setVisible(false);
	}

	
	LevelEditor::~LevelEditor()
	{
		nCursor->detachAllObjects();
		ARoot->getScene()->destroySceneNode(nCursor->getName());
		sheet->destroy();
		ARoot->getScene()->destroyEntity(mBackground);
		ARoot->getScene()->destroySceneNode(nBackground->getName());
		ARoot->getScene()->destroyEntity(entPreview);
		delete mAsset;
	}

	bool LevelEditor::hideSpawnError(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtSpawnError")->hide();
		return true;
	}

	bool LevelEditor::resetRotation(const CEGUI::EventArgs& e)
	{
		iIsometricZoom = 50;
		ALevel->setCameraProjection(Ogre::PT_ORTHOGRAPHIC);
		return true;
	}

	bool LevelEditor::rotateLeft(const CEGUI::EventArgs& e)
	{
		ALevel->rotate(-90);
		return true;
	}

	bool LevelEditor::rotateRight(const CEGUI::EventArgs& e)
	{
		ALevel->rotate(90);
		return true;
	}

	void LevelEditor::Update()
	{

		if (bTakeScreenshot) //take a screenshot, then reshow window elements
		{
			CEGUI::Editbox* edt = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/edtSaveName");
			std::stringstream strJpg;
			strJpg << "./media/materials/textures/levels/" << edt->getText() << ".jpg";
			bTakeScreenshot = false;
			ARoot->getWindow()->writeContentsToFile(strJpg.str());
			sheet->show();
			CEGUI::MouseCursor::getSingleton().show();
			ALevel->setLightLevel(iLightLevel);

			//if there is no ogre material for screenshot, create it.
			Ogre::ResourcePtr res;
			res = Ogre::MaterialManager::getSingleton().getByName(edt->getText().c_str());
			if(!res.getPointer())
			{
				Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(edt->getText().c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
				std::stringstream strTex;
				strTex << "levels/"<<edt->getText().c_str()<<".jpg";
				Ogre::TextureUnitState* t = mat->getTechnique(0)->getPass(0)->createTextureUnitState(strTex.str());

			}
		}
		
		if(AInput->isKeySingle(OIS::KC_HOME))
		{
			ALevel->rotate(-90);
			return;
		}
		else if(AInput->isKeySingle(OIS::KC_END))
		{
			ALevel->rotate(90);
			return;
		}

		if (CEGUI::System::getSingleton().getWindowContainingMouse())
		{
			if(!CEGUI::System::getSingleton().getWindowContainingMouse()->getName().find("root"))
				bMouseInputEnabled=true;
			else 
				bMouseInputEnabled=false;
		}
		
		
		if(bMouseInputEnabled)
		{
			//project a ray from the mouse position to a plane to find point on grid
			mouseRay = ARoot->getCamera()->getCameraToViewportRay(CEGUI::MouseCursor::getSingleton().getPosition().d_x/AInterface->getRenderer()->getWidth(), CEGUI::MouseCursor::getSingleton().getPosition().d_y/AInterface->getRenderer()->getHeight());
			mouseRes = mouseRay.intersects(Ogre::Plane(Ogre::Vector3(0,0,1), Ogre::Vector3(0,0,0)));
			if(mouseRes.first)
				mousePoint=mouseRay.getPoint(mouseRes.second);
			if (mousePoint.x-int(mousePoint.x) >= 0.5)
				mousePoint.x++;
			if (mousePoint.y-int(mousePoint.y) >= 0.5)
				mousePoint.y++;
			mousePoint.x = int(mousePoint.x);
			mousePoint.y = int(mousePoint.y);
			
			if(mousePoint.x<0)
				mousePoint.x=0;
			else if(mousePoint.x>31)
				mousePoint.x=31;
			if(mousePoint.y<0)
				mousePoint.y=0;
			else if(mousePoint.y>31)
				mousePoint.y=31;

			mAsset->setPosition(mousePoint);
			nCursor->setPosition(mousePoint);

			if (AInput->isMouseButton(1))
			{
				ALevel->addObject(iCurrentObj, mousePoint.x, mousePoint.y, iCurrentRot);
			}
			else if (AInput->isMouseButton(2))
			{
				ALevel->removeObject(mousePoint.x, mousePoint.y);
			}

			
			if(AInput->getMouseWheel() > 0)
			{
				if (ARoot->getCamera()->getProjectionType()==Ogre::PT_ORTHOGRAPHIC)
				{
					iIsometricZoom-=3;
					if(iIsometricZoom<5)
						iIsometricZoom=5;

					ARoot->getCamera()->setNearClipDistance(iIsometricZoom);

				}
				else
					ARoot->getCamera()->moveRelative(Ogre::Vector3(0,0,-3));
			}

			if(AInput->getMouseWheel() < 0)
			{
				if (ARoot->getCamera()->getProjectionType()==Ogre::PT_ORTHOGRAPHIC)
				{
					iIsometricZoom+=3;
					ARoot->getCamera()->setNearClipDistance(iIsometricZoom);

				}
				else
					ARoot->getCamera()->moveRelative(Ogre::Vector3(0,0,3));
			}


			if (AInput->isKeySingle(OIS::KC_C))
			{
				if(ARoot->getCamera()->getProjectionType()==Ogre::PT_ORTHOGRAPHIC)
					ALevel->setCameraProjection(Ogre::PT_PERSPECTIVE);
				else
				{
					iIsometricZoom = 50;
					ALevel->setCameraProjection(Ogre::PT_ORTHOGRAPHIC);
				}

			}

			if (AInput->isKeySingle(OIS::KC_MINUS))
			{
				ALevel->setLightLevel(ALevel->getLightLevel()-25);
				iLightLevel = ALevel->getLightLevel();
			}
			else if (AInput->isKeySingle(OIS::KC_EQUALS))
			{
				ALevel->setLightLevel(ALevel->getLightLevel()+25);
				iLightLevel = ALevel->getLightLevel();
			}



			if (AInput->isMouseButton(3) && ARoot->getCamera()->getProjectionType() == Ogre::PT_PERSPECTIVE)
			{
				ARoot->getCamera()->yaw(Ogre::Radian(Ogre::Degree(-AInput->getMousePosRel().x/5)));
				ARoot->getCamera()->pitch(Ogre::Radian(Ogre::Degree(-AInput->getMousePosRel().y/5)));
			}
			

			if (AInput->isKeySingle(OIS::KC_Z))
			{
				iCurrentRot += 90;
				if (iCurrentRot == 360)
				{
					iCurrentRot = 0;
				}
				mAsset->setRotation(iCurrentRot);	
			}

			if (AInput->isKeySingle(OIS::KC_Q))
				setCurrentObj(iCurrentObj-1);

			if (AInput->isKeySingle(OIS::KC_E))
				setCurrentObj(iCurrentObj+1);
				

			if (AInput->isKey(OIS::KC_W))
			{
				ARoot->getCamera()->moveRelative(Ogre::Vector3(0,30* ARoot->getTime(),0 ));
			}
			if (AInput->isKey(OIS::KC_S))
			{
				ARoot->getCamera()->moveRelative(Ogre::Vector3(0,-30* ARoot->getTime(),0 ));
			}
			if (AInput->isKey(OIS::KC_A))
			{
				ARoot->getCamera()->moveRelative(Ogre::Vector3(-30 * ARoot->getTime(), 0 ,0));
			}
			if (AInput->isKey(OIS::KC_D))
			{
				ARoot->getCamera()->moveRelative(Ogre::Vector3(30 * ARoot->getTime(), 0 ,0));
			}
			if (AInput->isKeySingle(OIS::KC_G))
			{
				if(mBackground->isVisible())
					mBackground->setVisible(false);
				else
					mBackground->setVisible(true);
			}

			if (AInput->isKeySingle(OIS::KC_1))
				ALevel->setLevelTheme(1);
			else if (AInput->isKeySingle(OIS::KC_2))
				ALevel->setLevelTheme(2);
			else if (AInput->isKeySingle(OIS::KC_3))
				ALevel->setLevelTheme(3);
			else if (AInput->isKeySingle(OIS::KC_4))
				ALevel->setLevelTheme(4);
			else if (AInput->isKeySingle(OIS::KC_5))
				ALevel->setLevelTheme(5);
			else if (AInput->isKeySingle(OIS::KC_6))
				ALevel->setLevelTheme(6);

			mAsset->setTheme(ALevel->getLevelTheme());
		}
	}


	void LevelEditor::setCurrentObj(int oT)
	{
		if (oT == OT_LAST)
			oT = 0;
		else if (oT < 0)
			oT = OT_LAST-1;

		iCurrentObj = oT;

		if(mAsset)
			delete mAsset;

		if (oT == OT_TILE)
			mAsset = new Tile(-1,-1);
		else if (oT == OT_BROKENTILE)
			mAsset = new BrokenTile(-1,-1);
		else if (oT == OT_TDD)
			mAsset = new TDD(-1,-1);
		else if (oT == OT_TIME)
			mAsset = new TimeObj(-1,-1);
		else if (oT == OT_SPIKES)
			mAsset = new Spikes(-1,-1);
		else if (oT == OT_JUMPPAD)
			mAsset = new JumpPad(-1,-1);
		else if (oT == OT_SPIKEBALL)
			mAsset = new SpikeBall(-1,-1);
		else if (oT == OT_SLOWMO)
			mAsset = new SlowMo(-1,-1);
		else if (oT == OT_ROTATIONSWITCH)
			mAsset = new RotationSwitch(-1,-1);
		else if (oT==OT_PLAYERSPAWN1)
			mAsset = new PlayerSpawn(-1,-1, 1);
		else if (oT==OT_PLAYERSPAWN2)
			mAsset = new PlayerSpawn(-1,-1, 2);
		else if (oT==OT_PLAYERSPAWN3)
			mAsset = new PlayerSpawn(-1,-1, 3);
		else if (oT==OT_PLAYERSPAWN4)
			mAsset = new PlayerSpawn(-1,-1, 4);
		else if (oT==OT_POINTLIGHT1)
			mAsset = new PointLight(-1,-1, 1);
		else if (oT==OT_POINTLIGHT2)
			mAsset = new PointLight(-1,-1, 2);
		else if (oT==OT_POINTLIGHT3)
			mAsset = new PointLight(-1,-1, 3);
		else if (oT==OT_POINTLIGHT4)
			mAsset = new PointLight(-1,-1, 4);
		else if (oT==OT_POINTLIGHT5)
			mAsset = new PointLight(-1,-1, 5);



		mAsset->setRotation(iCurrentRot);
	}

	bool LevelEditor::toggleFileMenu(const CEGUI::EventArgs& e)
	{
		if(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->isVisible())
		{
			entPreview->setVisible(false);
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->hide();
		}
		else
		{
			entPreview->setVisible(true);
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->show();
		}
		
		return true;
	}

	bool LevelEditor::toggleHelp(const CEGUI::EventArgs& e)
	{
		if(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtHelp")->isVisible())
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtHelp")->hide();
		else
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtHelp")->show();
		
		return true;
	}
	bool LevelEditor::deleteFile(const CEGUI::EventArgs& e)
	{
		CEGUI::Editbox* edt = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/edtSaveName");
		CEGUI::String str = "media/levels/" + edt->getText();
		std::remove(str.c_str());
		CEGUI::String strjpg = "media/materials/textures/levels/" + edt->getText() + ".jpg";
		std::remove(strjpg.c_str());
		ALevel->getLevelList("Root/frmFile/lstOpenName");
		return true;
	}

	bool LevelEditor::setSaveName(const CEGUI::EventArgs& e)
	{
		CEGUI::Editbox* edt = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/edtSaveName");
		CEGUI::Listbox* mylst = (CEGUI::Listbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/lstOpenName");
		CEGUI::String str;
		if (!mylst->getFirstSelectedItem())
			return false;

		str = mylst->getFirstSelectedItem()->getText();
		edt->setText(str);
		entPreview->setMaterialName(str.c_str());
		return true;
	}

	bool LevelEditor::saveLevel(const CEGUI::EventArgs& e)
	{
		CEGUI::Editbox* edt = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/edtSaveName");

		if(edt->getText().compare("")==0)
			return false;
		if(ALevel->isPlayerSpawnListComplete())
		{
			ALevel->saveLevel(edt->getText().c_str());
			ALevel->getLevelList("Root/frmFile/lstOpenName");

			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->hide();
			entPreview->setVisible(false);
			bTakeScreenshot=true;
			sheet->hide();
			CEGUI::MouseCursor::getSingleton().hide();
			ALevel->setCameraProjection(Ogre::PT_PERSPECTIVE);
			mBackground->setVisible(false);
			mAsset->setPosition(Ogre::Vector3(-1,-1,-1));
			nCursor->setPosition(Ogre::Vector3(-1,-1,-1));
			iLightLevel = ALevel->getLightLevel();
			ALevel->setLightLevel(100);
		}
		else
		{
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->hide();
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtSpawnError")->show();
			entPreview->setVisible(false);
		}
		
		return true;
	} 

	bool LevelEditor::loadLevel(const CEGUI::EventArgs& e)
	{
		CEGUI::Editbox* edt = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile/edtSaveName");

		ALevel->loadLevel(edt->getText().c_str());

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/frmFile")->hide();
		entPreview->setVisible(false);
		iLightLevel = ALevel->getLightLevel();
		return true;
	}

	bool LevelEditor::clearLevel(const CEGUI::EventArgs& e)
	{
		ALevel->clearLevel();
		return true;
	}

	bool LevelEditor::quitLevel(const CEGUI::EventArgs& e){
		ALevel->setLevelMode(LM_MENU);
		return true;
	}

	bool LevelEditor::playLevel(const CEGUI::EventArgs& e)
	{
		if(ALevel->isPlayerSpawnListComplete())
		{
			ALevel->saveLevel("temp");
			ALevel->loadLevel("temp");
			ALevel->setLevelMode(LM_TEST);
		}
		else
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Root/txtSpawnError")->show();
		return true;
	}
}