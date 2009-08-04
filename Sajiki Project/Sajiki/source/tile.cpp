#include "stdafx.h"
#include "tile.h"



namespace Sajiki
{
	Tile::Tile(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/tile.mesh" );

		mBody = ARoot->getPhysicsScene()->createBody(str.str(), new NxOgre::CubeShape(1,1,3),Ogre::Vector3(x, y, 0));

		mBody->getNode()->attachObject(mEntity);
		mBody->raiseBodyFlag(NX_BF_FROZEN);

		iType=OT_TILE;
		iTheme=0;
		iRotation=0;
		sAssetName = "Tile";
	}
}
