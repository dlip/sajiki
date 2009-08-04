#ifndef ROOT_H
#define ROOT_H

#include "singleton.h"
#include "level.h"

namespace Sajiki
{
	/**
		@class Root
		@brief Gives central access to the main renderer, scenegraph, physics world and camera. Initialises resources, and performs physics updates.
	 */
	class Root: public Singleton<Root>
	{
		friend Singleton;
	public:
		bool Init();
		void Update();
		Ogre::RenderWindow* getWindow(){return mWindow;}
		Ogre::Root* getRoot(){ return mRoot;}
		Ogre::SceneManager* getScene(){ return mSceneManager; }
		Ogre::Camera* getCamera(){return mCamera;}
		NxOgre::World* getPhysicsWorld(){return mPhysicsWorld;}
		NxOgre::Scene* getPhysicsScene(){return mPhysicsScene;}
		float getTime(){return fTime;}
		float getTotalTime(){ return fTotalTime;}
	private:
		Ogre::RenderWindow* mWindow;
		Ogre::SceneManager *mSceneManager;
		Ogre::Root* mRoot;
		Ogre::Viewport* mViewport;
		Ogre::Camera *mCamera;
		NxOgre::World* mPhysicsWorld;
		NxOgre::Scene* mPhysicsScene;
		Ogre::Timer mTimer;
		float fTime;
		float fTotalTime;
	protected:
		Root(){}
		~Root();
	};

}

#define ARoot Root::Instance()

#endif