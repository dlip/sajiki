#include "stdafx.h"
#include "slowmo.h"



namespace Sajiki
{
	SlowMo::SlowMo(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "a"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/slowmo.mesh" );
		mBody = 0;

		sn = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x,y,0));
		sn->attachObject(mEntity);

		mAnimationState = mEntity->getAnimationState("SlowMo Animation");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);

		iType=OT_SLOWMO;
		iTheme=0;
		iRotation=0;
		sAssetName = "SlowMo";
		bCollected=false;
	}

	void SlowMo::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos);
	}

	void SlowMo::Update()
	{
		if(bCollected)
		{
			return;
		}
		else
		{
			mAnimationState->addTime(ARoot->getTime());
			sn->setOrientation(ARoot->getCamera()->getOrientation());
			for (int x=0; x<ALevel->getPlayerCount(); x++)
			{
				mPlayer = ALevel->getPlayer(x);

				bool bTouch=false;
				if(mPlayer->getPos()->distance(sn->getWorldPosition())<=1.1)
					bTouch=true;
				else if(mPlayer->getFootPos().distance(sn->getWorldPosition())<=1.1)
					bTouch=true;
				else if(mPlayer->getHeadPos().distance(sn->getWorldPosition())<=1.1)
					bTouch=true;

				if(bTouch)
				{
					ALevel->slowMo();
					mPlayer->bHasSlowMo=true;
					bCollected=true;
					ARoot->getScene()->destroyEntity(mEntity);
					mEntity=0;
					return;
				}
			}
		}
		
	}
}
