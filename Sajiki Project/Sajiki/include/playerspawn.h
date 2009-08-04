#ifndef PLAYERSPAWN_H
#define PLAYERSPAWN_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class PlayerSpawn
		@brief Asset which determines players starting position, not shown ingame.
	 */
	class PlayerSpawn: public Asset
	{
		friend Asset;
	public:
		PlayerSpawn(int x, int y, int no);
		void setTheme(int pTheme);
		void Update();
		void setRotation(int iRot){};
		void setPosition(Ogre::Vector3 pos);
	private:
		Ogre::SceneNode *sn;

	protected:
	};
}

#endif