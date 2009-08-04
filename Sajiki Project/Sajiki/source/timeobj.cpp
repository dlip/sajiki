#include "stdafx.h"
#include "timeobj.h"

namespace Sajiki
{
	TimeObj::TimeObj(int x, int y)
	{
		xPos = x; yPos=y;
		sn = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x,y,0));
		std::stringstream str;
		str << rand() << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/time.mesh" );
		sn->attachObject(mEntity);
		mBody = 0;
		
		mAnimationState = mEntity->getAnimationState("Time Animation");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);
		iType=OT_TIME;
		iTheme=0;
		iRotation=0;
		sAssetName = "Time";
		bCollected=false;
		bAddParticles=false;
	}

	TimeObj::~TimeObj()
	{
		//ARoot->getScene()->destroyParticleSystem(mParticleSystem);
	}

	
	void TimeObj::Update()
	{

		if(bCollected)
			return;
		/*if(!bAddParticles)
		{
			sn->attachObject(mParticleSystem);
			bAddParticles=true;
		}
		mParticleSystem->_update(ARoot->getTime());*/
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
				mPlayer->addTime();
				sn->setPosition(xPos+100, yPos+100, 0);
				bCollected=true;
				ASound->PlaySound("pickuptime.mp3");

				ARoot->getScene()->destroyEntity(mEntity);
				mEntity=0;
				return;
			}
		}

	}

	void TimeObj::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos);
	}
}
