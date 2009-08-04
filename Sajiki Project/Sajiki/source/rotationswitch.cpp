#include "stdafx.h"
#include "rotationswitch.h"



namespace Sajiki
{
	RotationSwitch::RotationSwitch(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/rotationswitch.mesh" );

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode();
		sn->setPosition(Ogre::Vector3(x, y, 0));

		mBody = ARoot->getPhysicsScene()->createBody(str.str(), new NxOgre::CubeShape(1,1,3),sn->getWorldPosition(), "Mass: 20, Static: yes");
		
		mBody->setNode(sn);
		mBody->getNode()->attachObject(mEntity);

		snUp = sn->createChildSceneNode(Ogre::Vector3(0,1,0));

		iType=OT_ROTATIONSWITCH;
		iTheme=0;
		iRotation=0;
		sAssetName = "RotationSwitch";
		bPushed=false;
	}

	void RotationSwitch::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos);
	}

	void RotationSwitch::Update()
	{
		if (mEnableTimer.getMilliseconds()<6000)
		{

			mEntity->setMaterialName("Assets/RotationSwitchNotActive");
			iTheme=0;
			mPushTimer.reset();
			bPushed=false;
			return;

		}
		else if (bPushed)
		{
			mEntity->setMaterialName("Assets/RotationSwitchActive");
			iTheme=0;
		}
		else
		{
			setTheme(1);
		}

		if(mPushTimer.getMilliseconds()>1000 && bPushed)
		{
			ALevel->randomRotate();
			mPushTimer.reset();
			mEnableTimer.reset();
		}

		bPushed=false;


		for (int x=0; x<ALevel->getPlayerCount(); x++)
		{
			mPlayer = ALevel->getPlayer(x);

			if(mPlayer->getPos()->distance(mBody->getNode()->getWorldPosition())<1.5)
			{
				if(!mPlayer->isDead())
					bPushed=true;
			}
		}
		
		if(!bPushed)
		{
			mPushTimer.reset();	
		}
		
	}
}
