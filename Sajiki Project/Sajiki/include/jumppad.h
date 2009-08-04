#ifndef JUMPPAD_H
#define JUMPPAD_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class JumpPad
		@brief Asset which is a spring that launches player when touched.
	 */
	class JumpPad: public Asset
	{
		friend Asset;
	public:
		JumpPad(int x, int y);
		void setPosition(Ogre::Vector3 pos);
		void setRotation(int iRot);
		void Update();
		void setTheme(int pTheme){mEntity->setMaterialName("Assets/JumpPad");}
	private:
		Ogre::SceneNode *sn, *sn2, *snUp1, *snUp2;
		Ogre::AnimationState *mAnimationState;
		std::string sSoundName;
		Ogre::Timer PlaySoundTimer;

	protected:
	};
}

#endif