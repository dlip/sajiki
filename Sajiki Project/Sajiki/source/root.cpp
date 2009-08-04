#include "stdafx.h"
#include "root.h"

namespace Sajiki
{
	bool Root::Init()
	{
		#ifdef _DEBUG
		mRoot = new Ogre::Root("plugins_d.cfg", "ogre_d.cfg", "ogre_d.log");
		#else
		mRoot = new Ogre::Root("plugins.cfg", "ogre.cfg", "ogre.log");
		#endif
		if(!mRoot->showConfigDialog())
			return false;

		mWindow = mRoot->initialise(true, "Sajiki");
		//set window icon
		HWND hWnd = FindWindow(L"OgreD3D9Wnd", L"Sajiki");
		if (!IsWindow(hWnd)) hWnd = FindWindow(L"OgreGLWindow", L"Sajiki");
		if (IsWindow(hWnd)) 
		{
			char buf[1024];
			GetModuleFileNameA(0, (LPCH)&buf, 1024);
			HINSTANCE instance = ::GetModuleHandleA(buf);

			SetWindowText(hWnd, L"Sajiki");
			HICON hIcon = LoadIcon(instance, (LPCTSTR)107);
			if (hIcon) 
			{
				SendMessage(hWnd, WM_SETICON, 1, (LPARAM)hIcon);
				SendMessage(hWnd, WM_SETICON, 0, (LPARAM)hIcon);
			}
		}

		//initialise resource groups
		mRoot->addResourceLocation("./media/models", "FileSystem");
		mRoot->addResourceLocation("./media/models/assets", "FileSystem");
		mRoot->addResourceLocation("./media/models/backgrounds", "FileSystem");
		mRoot->addResourceLocation("./media/models/menu", "FileSystem");
		mRoot->addResourceLocation("./media/materials/scripts","FileSystem");
		mRoot->addResourceLocation("./media/materials/textures","FileSystem");
		mRoot->addResourceLocation("./media/materials/textures/player","FileSystem");
		mRoot->addResourceLocation("./media/materials/textures/assets","FileSystem");
		mRoot->addResourceLocation("./media/materials/textures/menu","FileSystem");

		mRoot->addResourceLocation("./media/particles","FileSystem");
		mRoot->addResourceLocation("./media/gui","FileSystem");
		mRoot->addResourceLocation("./media/fonts","FileSystem");

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();


		//create cameras and scene
		mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
		mSceneManager->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );

		mCamera = mSceneManager->createCamera("Camera");
		mSceneManager->getRootSceneNode()->createChildSceneNode("CameraNode")->attachObject(mCamera);

		mViewport = mWindow->addViewport(mCamera);
		mViewport->setBackgroundColour(Ogre::ColourValue(0.0f,0.0f,0.0f));

		mPhysicsWorld = new NxOgre::World("FrameListener: no");
		mPhysicsScene = mPhysicsWorld->createScene("Main", mSceneManager, "gravity: yes, time-step-method: variable");
		mPhysicsScene->setGravity(Ogre::Vector3(0,-9.8,0));
		mTimer.reset();
		fTime = fTotalTime = 0;


		return true;
	}

	void Root::Update()
	{
		

		fTime = mTimer.getMilliseconds() / 1000.f;
		mTimer.reset();
		fTotalTime+=fTime;
		if(ALevel->getOldLevelMode()==LM_EDIT)
		{
			mPhysicsWorld->simulate(0);
			mPhysicsWorld->render(0);
			return;
		}
		if (ALevel->getLevelMode()== LM_ROTATE || ALevel->getLevelMode()== LM_TEST || ALevel->getLevelMode()== LM_PLAY)
		{
			if(!ALevel->isRoundStarted())
			{
				mPhysicsWorld->simulate(0);
				mPhysicsWorld->render(0);
				return;
			}
			if(ALevel->isSlowMo())
			{
				mPhysicsWorld->simulate(getTime()*0.5f);
				mPhysicsWorld->render(getTime()*0.5f);
			}
			else
			{
				mPhysicsWorld->simulate(getTime());
				mPhysicsWorld->render(getTime());
			}
		}
		mPhysicsScene->setGravity(mCamera->getUp()*-9.8);
	}

	Root::~Root()
	{
		mPhysicsWorld->destroyScene(mPhysicsScene->getName());
		delete mPhysicsWorld;
		mPhysicsWorld = 0;
		mSceneManager->destroyAllCameras();
		mSceneManager->destroyAllEntities();
		mSceneManager->clearScene();
		mRoot->destroySceneManager(mSceneManager);

		mWindow->removeAllViewports(); 

	}
}