#ifndef TDD_H
#define TDD_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class TDD
		@brief Asset which is a power-up. When a player holds this object their timer is frozen.
	 */
	class TDD: public Asset
	{
		friend Asset;
	public:
		TDD(int x, int y);
		void Update();
		~TDD();
		void setTheme(int pTheme){mEntity->setMaterialName("Assets/TDD");}
	private:
		Player *mPlayerOwner;
		std::string sSoundName;
		Ogre::ParticleSystem *mParticleSystem;
	protected:
	};
}

#endif