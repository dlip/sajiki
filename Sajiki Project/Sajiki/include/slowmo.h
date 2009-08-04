#ifndef SLOWMO_H
#define SLOWMO_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class SlowMo
		@brief Asset which is a power up that causes other players to move at a slower speed.
	 */
	class SlowMo: public Asset
	{
		friend Asset;
	public:
		SlowMo(int x, int y);
		void Update();
		void setPosition(Ogre::Vector3 pos);
		void setRotation(int iRot){};
		void setTheme(int pTheme){mEntity->setMaterialName("Assets/SlowMo");}
	private:
		bool bCollected;
		Ogre::AnimationState *mAnimationState;
		Ogre::SceneNode *sn;
	protected:
	};
}

#endif