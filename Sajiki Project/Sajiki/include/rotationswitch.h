#ifndef ROTATIONSWITCH_H
#define ROTATIONSWITCH_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class RotationSwitch
		@brief Asset which is a button that when touched for a period causes the level to rotate.
	 */
	class RotationSwitch: public Asset
	{
		friend Asset;
	public:
		RotationSwitch(int x, int y);
		void setPosition(Ogre::Vector3 pos);
		void Update();
		void setTheme(int pTheme){mEntity->setMaterialName("Assets/RotationSwitch");}
	private:
		Ogre::SceneNode *sn, *snUp;
		Ogre::Timer mPushTimer, mEnableTimer;
		bool bPushed;

	protected:
	};
}

#endif