#include "stdafx.h"
#include "asset.h"

namespace Sajiki {
	
	Asset::~Asset()
	{

		if(mBody)
		{
			mBody->getNode()->detachAllObjects();
			ARoot->getPhysicsScene()->destroyBody(mBody->getName());
		}
		if(mEntity)
			ARoot->getScene()->destroyEntity(mEntity);
	}

	void Asset::setRotation(int iRot)
	{
		mBody->getNode()->resetOrientation();
		iRotation = iRot;
		mBody->getNode()->roll(Ogre::Radian(Ogre::Degree(iRotation)));
	}

	void Asset::setTheme(int pTheme)
	{
		if(pTheme!=iTheme)
		{
			iTheme = pTheme;
			std::stringstream str;
			str << "Assets/" << sAssetName << iTheme;
			mEntity->setMaterialName(str.str());
		}
	}
}