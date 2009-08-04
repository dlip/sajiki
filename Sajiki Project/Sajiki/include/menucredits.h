#ifndef MENUCREDITS_H
#define MENUCREDITS_H

#include "Menu.h"

namespace Sajiki{
	/**
		@class MenuCredits
		@brief Menu to credit the team.
	 */
	class MenuCredits: public Menu
	{
	public:
		MenuCredits();
		~MenuCredits();
		void Update();
	private:
		Ogre::SceneNode *sn,*sn2;
		Ogre::Entity *mEntity, *mEntity2;
		Ogre::AnimationState *mAnimationState, *mAnimationState2;

	protected:

	};
}
#endif