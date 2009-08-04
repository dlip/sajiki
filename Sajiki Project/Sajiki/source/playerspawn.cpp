#include "stdafx.h"
#include "playerspawn.h"



namespace Sajiki
{
	PlayerSpawn::PlayerSpawn(int x, int y, int no)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "Asset" << x << "-"<< y;

		std::stringstream str2;
		str2 << "assets/playerspawn" << no << ".mesh";

		mEntity = ARoot->getScene()->createEntity( str.str(), str2.str() );

		mBody=0;
		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x,y,0));
		sn->attachObject(mEntity);
		iType=OT_PLAYERSPAWN1-1+no;
		iTheme=no;
		iRotation=0;
		sAssetName = "Tile";
	}
		
	void PlayerSpawn::setTheme(int pTheme)
	{
		std::stringstream str;
		str << "Assets/PlayerSpawn" << iTheme;

		mEntity->setMaterialName(str.str());
	}

	void PlayerSpawn::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos);
	}

	void PlayerSpawn::Update()
	{

		if(ALevel->getLevelMode() == LM_TEST || ALevel->getLevelMode() == LM_PLAY)
			mEntity->setVisible(false);	
	}
}
