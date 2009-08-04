#ifndef MENUOPTIONS_H
#define MENUOPTIONS_H

#include "Menu.h"

namespace Sajiki{
	/**
		@class MenuOptions
		@brief Menu to change options.
	 */
	class MenuOptions: public Menu
	{
	public:
		MenuOptions();
		~MenuOptions();
		void Update();
		bool bEnableSound;
	private:
		bool bEnableJoyX;
		Ogre::SceneNode *sn,*sn2;
		Ogre::Entity *mEntity, *mEntity2;
		Ogre::AnimationState *mAnimationState, *mAnimationState2;

	protected:

	};
}
#endif