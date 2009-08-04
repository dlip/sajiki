#ifndef MENUMAIN_H
#define MENUMAIN_H

#include "Menu.h"

namespace Sajiki{

	/**
		@class MenuMain
		@brief Menu on the initial screen.
	 */
	class MenuMain: public Menu
	{
	public:
		MenuMain();
		void Update();
		~MenuMain();
	private:
		Ogre::SceneNode *sn,*sn2;
		Ogre::Entity *mEntity, *mEntity2;
		int mOption;
		Ogre::AnimationState *mAnimationState, *mAnimationState2;
		bool bStarted, bFinished;
		bool bEnableJoyX;
	protected:

	};
}
#endif