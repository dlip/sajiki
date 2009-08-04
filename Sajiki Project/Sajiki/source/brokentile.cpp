#include "stdafx.h"
#include "brokentile.h"

namespace Sajiki
{
	BrokenTile::BrokenTile(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/brokentile.mesh" );

		mBody = ARoot->getPhysicsScene()->createBody(str.str(), new NxOgre::CubeShape(1,1,2.8),Ogre::Vector3(x, y, 0), "Mass: 20");
		upNode = mBody->getNode()->createChildSceneNode(Ogre::Vector3(0,0.5,0));
		mBody->raiseBodyFlag(NX_BF_KINEMATIC);
		
		meshNode = mBody->getNode()->createChildSceneNode();
		meshNode->attachObject(mEntity);

		iType=OT_BROKENTILE;
		iTheme=0;
		iRotation=0;
		sAssetName = "BrokenTile";
		bTouched=false;
		bTimer=false;
	}

	void BrokenTile::Update()
	{
		if(!mBody)
			return;

		if(bTouched)
		{
			
			if(mFallTimer.getMilliseconds()>2000)
			{
				ARoot->getPhysicsScene()->destroyBody(mBody->getName());
				mBody=0;
			}
		}
		else
		{
			bTimer=false;
			for(int i=0; i<ALevel->getPlayerCount(); i++)
			{
				mPlayer = ALevel->getPlayer(i);
				if(mPlayer->getPos()->distance(mBody->getNode()->getWorldPosition())<1.5)
					bTimer=true;
			}

			if (bTimer)
			{
				meshNode->setPosition(Ogre::Vector3(sinf(ARoot->getTotalTime()*50)/6,0, 0));
				if(mFallTimer.getMilliseconds()>500)
				{
					meshNode->setPosition(Ogre::Vector3(0, 0, 0));
					mBody->clearBodyFlag(NX_BF_KINEMATIC);
					mBody->wakeUp();
					mFallTimer.reset();
					bTouched=true;
				}
			}
			else
			{
				meshNode->setPosition(Ogre::Vector3(0, 0, 0));
				mFallTimer.reset();
				mBody->raiseBodyFlag(NX_BF_KINEMATIC);
			}
		}
		
	}
}
