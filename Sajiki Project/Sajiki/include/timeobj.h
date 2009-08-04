#ifndef TIMEOBJ_H
#define TIMEOBJ_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class TimeObj
		@brief Asset which is a power-up. When player collects this their timer is replenished.
	 */
	class TimeObj: public Asset
	{
		friend Asset;
	public:
		TimeObj(int x, int y);
		~TimeObj();
		void setPosition(Ogre::Vector3 pos);
		void Update();
		void setRotation(int iRot){};
		void setTheme(int pTheme){mEntity->setMaterialName("Assets/Time");}
	private:
		Ogre::SceneNode *sn;
		Ogre::AnimationState *mAnimationState;
		Ogre::ParticleSystem *mParticleSystem;
		bool bAddParticles;
	protected:
	};
}

#endif