#include "stdafx.h"
#include "tdd.h"

namespace Sajiki
{
	TDD::TDD(int x, int y)  
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/tdd.mesh" );

		mBody = ARoot->getPhysicsScene()->createBody(str.str(), new NxOgre::CapsuleShape(0.25,0.5),Ogre::Vector3(x, y, 0), "Mass: 40");
		mBody->getNode()->attachObject(mEntity);


		iType=OT_TDD;
		iTheme=0;
		iRotation=0;
		sAssetName = "TDD";
		mPlayerOwner=0;
		mBody->raiseBodyFlag(NX_BF_KINEMATIC);

		std::stringstream str2;
		str2 << "AssetParticle" << xPos << "-"<< yPos;
		mParticleSystem = ARoot->getScene()->createParticleSystem(str2.str(), "TDD/Sparkle");
		mParticleSystem->setVisible(false);
		mBody->getNode()->createChildSceneNode()->attachObject(mParticleSystem);
		mParticleSystem->setSpeedFactor(0.01);


	}
	TDD::~TDD()
	{
		ARoot->getScene()->destroyParticleSystem(mParticleSystem);
	}

	void TDD::Update()
	{
		mParticleSystem->setVisible(true);
		mParticleSystem->_update(ARoot->getTime()*100);
		mBody->wakeUp();
		if(mPlayerOwner)
		{
			mBody->setGlobalPosition(mPlayerOwner->getTimerPos());
			mBody->render(0);
			
			if (!mPlayerOwner->bHasTDD)
			{
				mBody->setGlobalPosition(*mPlayerOwner->getPos());
				mBody->render(0);
				mPlayerOwner=0;
				mBody->clearBodyFlag(NX_BF_KINEMATIC);
			}
		}
		else
		{
			mEntity->setVisible(true);
			for (int x=0; x<ALevel->getPlayerCount(); x++)
			{
				mPlayer = ALevel->getPlayer(x);

				bool bTouch=false;
				if(mPlayer->getPos()->distance(mBody->getNode()->getWorldPosition())<=1.1)
					bTouch=true;
				else if(mPlayer->getFootPos().distance(mBody->getNode()->getWorldPosition())<=1.1)
					bTouch=true;
				else if(mPlayer->getHeadPos().distance(mBody->getNode()->getWorldPosition())<=1.1)
					bTouch=true;

				if(bTouch)
				{
					if (!mPlayer->isHurt()&& !mPlayer->isDead())
					{
						ASound->PlaySound("pickupTDD.mp3");
						mPlayerOwner=mPlayer;
						mPlayerOwner->bHasTDD = true;
						mBody->raiseBodyFlag(NX_BF_KINEMATIC);
					}
				}
			}
			mBody->setGlobalPosition(Ogre::Vector3(mBody->getGlobalPosition().x, mBody->getGlobalPosition().y, 0));
		}

		
	}
}
