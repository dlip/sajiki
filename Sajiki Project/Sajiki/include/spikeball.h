#ifndef SPIKEBALL_H
#define SPIKEBALL_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class SpikeBall
		@brief Asset that rolls around the level and hurts players that come in contact with it.
	 */
	class SpikeBall: public Asset
	{
		friend Asset;
	public:
		SpikeBall(int x, int y);
		void Update();
		bool bPlaySound;
		void setRotation(int iRot){}
		void setPosition(Ogre::Vector3 pos);
	private:
		std::string sSoundName;
	protected:
	};
}

#endif