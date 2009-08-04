#include "stdafx.h"
#include "spikeball.h"



namespace Sajiki
{
	SpikeBall::SpikeBall(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset-" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/spikeball.mesh" );
		mBody = ARoot->getPhysicsScene()->createBody(str.str(), new NxOgre::SphereShape(0.75),Ogre::Vector3(x+0.5, y+0.5, 0), "Mass: 50");

		mBody->getNode()->attachObject(mEntity);

		iType=OT_SPIKEBALL;
		iTheme=0;
		iRotation=0;
		sAssetName = "SpikeBall";
		bPlaySound=false;
	}

	void SpikeBall::setPosition(Ogre::Vector3 pos)
	{
		mBody->setGlobalPosition(Ogre::Vector3(pos.x+0.5, pos.y+0.5, pos.z));
		mBody->render(0);
	}
	void SpikeBall::Update()
	{
		mBody->wakeUp();
		mBody->setGlobalPosition(Ogre::Vector3(mBody->getGlobalPosition().x, mBody->getGlobalPosition().y, 0));
		if(mBody->computeKineticEnergy() < 100 && bPlaySound)
		{
			bPlaySound=false;
			ASound->PlaySound("spikeball.mp3");
		}

		if(mBody->computeKineticEnergy() > 100)
		{
			bPlaySound=true;
		}

		for (int x=0; x<ALevel->getPlayerCount(); x++)
		{
			mPlayer = ALevel->getPlayer(x);

			bool bTouch=false;
			if(mPlayer->getPos()->distance(mBody->getNode()->getWorldPosition())<=1.5)
				bTouch=true;
			else if(mPlayer->getFootPos().distance(mBody->getNode()->getWorldPosition())<=1.1)
				bTouch=true;
			else if(mPlayer->getHeadPos().distance(mBody->getNode()->getWorldPosition())<=1.1)
				bTouch=true;

			if(bTouch)
			{
					mPlayer->Hurt();
			}
		}
		
	}
}
