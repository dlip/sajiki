#ifndef SPIKES_H
#define SPIKES_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class Spikes
		@brief Asset which is static that hurts players that come in contact with it.
	 */
	class Spikes: public Asset
	{
		friend Asset;
	public:
		Spikes(int x, int y);
		void setPosition(Ogre::Vector3 pos);
		void setRotation(int iRot);
		void Update();
		void setTheme(int pTheme){mEntity->setMaterialName("Assets/Spikes");}
	private:
		Ogre::SceneNode *sn, *sn2;

	protected:
	};
}

#endif