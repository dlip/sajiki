#ifndef BROKENTILE_H
#define BROKENTILE_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class BrokenTile
		@brief Asset which is a block that shakes when player stands on top of it, then falls and dissapears.
	 */
	class BrokenTile: public Asset
	{
		friend Asset;
	public:
		BrokenTile(int x, int y);
		void Update();
	private:
		Ogre::SceneNode *meshNode;
		Ogre::Timer mFallTimer;
		bool bTimer,bFall;
		
	protected:
	};
}

#endif