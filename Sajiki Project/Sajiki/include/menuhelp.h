#ifndef MENUHELP_H
#define MENUHELP_H

#include "Menu.h"

namespace Sajiki{
	/**
		@class MenuHelp
		@brief Menu for help on playing the game.
	 */
	class MenuHelp: public Menu
	{
	public:
		MenuHelp();
		~MenuHelp();
		void Update();
	private:
		Ogre::SceneNode *sn,*sn2,*sn3;
		Ogre::Entity *mEntity, *mEntity2, *mEntity3;
		Ogre::AnimationState *mAnimationState, *mAnimationState2;
		int iCurrentPage;
		bool bEnableJoyX;

	protected:

	};
}
#endif