#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "asset.h"

namespace Sajiki
{
	/**
		@class PointLight
		@brief Asset which is a point light with various colours.
	 */
	class PointLight: public Asset
	{
		friend Asset;
	public:
		PointLight(int x, int y, int no);
		~PointLight();
		void setTheme(int pTheme){};
		void setRotation(int iRot);
		void Update();
		void setPosition(Ogre::Vector3 pos);
	private:
		Ogre::Light *mLight;
		Ogre::SceneNode *sn;
		int iNo, iLightDepth;

	protected:
	};
}

#endif