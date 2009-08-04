#include "stdafx.h"
#include "jumppad.h"



namespace Sajiki
{
	JumpPad::JumpPad(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/jumppad.mesh" );
		
		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode();
		sn->setPosition(Ogre::Vector3(x, y, 0));
		
		mBody = 0;
		sn->attachObject(mEntity);
		
		snUp1 = sn->createChildSceneNode(Ogre::Vector3(0,-0.25,0));
		snUp2 = sn->createChildSceneNode(Ogre::Vector3(1,-0.25,0));

		mAnimationState = mEntity->getAnimationState("Animation");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(false);

		iType=OT_JUMPPAD;
		iTheme=0;
		iRotation=0;
		sAssetName = "JumpPad";
	}

	void JumpPad::setRotation(int iRot)
	{
		sn->resetOrientation();
		iRotation = iRot;
		sn->roll(Ogre::Radian(Ogre::Degree(iRotation)));
	}

	void JumpPad::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos);
	}

	void JumpPad::Update()
	{
		mAnimationState->addTime(ARoot->getTime());

		for (int x=0; x<ALevel->getPlayerCount(); x++)
		{
			mPlayer = ALevel->getPlayer(x);

			bool bPlayerHit=false;

			if(mPlayer->getFootPos().distance(snUp1->getWorldPosition())<0.6 || mPlayer->getFootPos().distance(snUp2->getWorldPosition())<0.6)
				bPlayerHit=true;
			else if(mPlayer->getHeadPos().distance(snUp1->getWorldPosition())<0.6 || mPlayer->getHeadPos().distance(snUp2->getWorldPosition())<0.6)
				bPlayerHit=true;
			else if(mPlayer->getPos()->distance(snUp1->getWorldPosition())<0.6 || mPlayer->getPos()->distance(snUp2->getWorldPosition())<0.6)
				bPlayerHit=true;

			if(bPlayerHit)
			{
				if(iRotation==ALevel->getCameraRoll())
					mPlayer->Bounce();
				else if(iRotation==(int)(ALevel->getCameraRoll()+90)%360)
					mPlayer->Bounce(2);
				else if(iRotation+ALevel->getCameraRoll()==270 || iRotation==(int)(ALevel->getCameraRoll()-90)%360)
					mPlayer->Bounce(1);
				else if(iRotation==(int)(ALevel->getCameraRoll()+180)%360)
					mPlayer->Bounce(3);
				else
					return;

			}

			bool bSpikeBallHit=false;
			std::vector<Asset *> *vecSpikeballList = ALevel->getSpikeballList();
			std::vector<Asset *>::iterator vecSpikeballListIter;
			NxOgre::Actor* SpikeBallHit;
			for(vecSpikeballListIter= vecSpikeballList->begin(); vecSpikeballListIter != vecSpikeballList->end(); vecSpikeballListIter++)
			{
				SpikeBallHit = (*vecSpikeballListIter)->getBody();
				if(SpikeBallHit->getGlobalPosition().distance(snUp1->getWorldPosition())<0.7 || SpikeBallHit->getGlobalPosition().distance(snUp2->getWorldPosition())<0.7)
				{
					bSpikeBallHit =true;

					if(iRotation==ALevel->getCameraRoll())
						SpikeBallHit->setLinearVelocity(ARoot->getCamera()->getUp()*20);
					else if(iRotation==(int)(ALevel->getCameraRoll()+90)%360)
						SpikeBallHit->setLinearVelocity(ARoot->getCamera()->getRight()*-20);
					else if(iRotation+ALevel->getCameraRoll()==270 || iRotation==(int)(ALevel->getCameraRoll()-90)%360)
						SpikeBallHit->setLinearVelocity(ARoot->getCamera()->getRight()*20);
					else if(iRotation==(int)(ALevel->getCameraRoll()+180)%360)
						SpikeBallHit->setLinearVelocity(ARoot->getCamera()->getUp()*-20);
					else
						bSpikeBallHit=false;

					
				}
				
			}
			
			if(bSpikeBallHit || bPlayerHit)
			{
				if(mAnimationState->hasEnded())
				{
					ASound->PlaySound("jumppad.mp3");
					mAnimationState->setTimePosition(0);
				}
			}

		}
		
	}
	
}
