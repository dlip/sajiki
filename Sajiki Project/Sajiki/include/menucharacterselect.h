#ifndef MENUCHARACTERSELECT_H
#define MENUCHARACTERSELECT_H

#include "Menu.h"

namespace Sajiki{
	/**
		@class MenuCharacterSelect
		@brief Menu to select players for the game.
	 */
	class MenuCharacterSelect: public Menu
	{
	public:
		MenuCharacterSelect();
		~MenuCharacterSelect();
		void Update();
	private:
		Ogre::Entity* PlayerMeshM[4];
		Ogre::Entity* PlayerMeshF[4];
		Ogre::SceneNode* PlayerNode[4];
		Ogre::AnimationState *mAnimState;
		bool bEnableInput[4];
		bool bEnableInput2[4];
		bool bEnableInputbutton[4];
		bool bPlayerReady[4];
		bool bGameReady;
		int MenuSelect;
		std::string strLevelQueue[6];

		Ogre::SceneNode *sn,*sn2;
		Ogre::Entity *mEntity, *mEntity2;
		Ogre::AnimationState *mAnimationState;
	protected:

	};
}
#endif