#include "stdafx.h"
#include "spikes.h"

namespace Sajiki
{
	Spikes::Spikes(int x, int y)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/spikes.mesh" );

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode();
		sn->setPosition(Ogre::Vector3(x, y, 0));
		sn2 = sn->createChildSceneNode(Ogre::Vector3(0,-0.5,0));

		mBody = ARoot->getPhysicsScene()->createBody(str.str(), new NxOgre::CubeShape(1,0.5,3),Ogre::Vector3(x, y, 0), "Mass: 20, Static: yes");
		
		upNode = sn->createChildSceneNode(Ogre::Vector3(0,0.5,0));
		sn->createChildSceneNode()->attachObject(mEntity);
		iType=OT_SPIKES;
		iTheme=0;
		iRotation=0;
		sAssetName = "Spikes";
	}

	void Spikes::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos.x, pos.y, pos.z);
	}

	void Spikes::setRotation(int iRot)
	{
		sn->resetOrientation();
		iRotation = iRot;
		sn->roll(Ogre::Radian(Ogre::Degree(iRotation)));
		sn2->roll(Ogre::Radian(Ogre::Degree(iRotation)));

		mBody->setGlobalOrientation(sn->_getDerivedOrientation()); 
		mBody->setGlobalPosition(sn2->getWorldPosition());
	}

	void Spikes::Update()
	{
		for (int x=0; x<ALevel->getPlayerCount(); x++)
		{
			mPlayer = ALevel->getPlayer(x);

			if(mPlayer->getPos()->distance(upNode->getWorldPosition())<0.6)
			{
				mPlayer->Hurt();
			}
		}
		
	}
}
