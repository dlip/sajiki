#include "stdafx.h"
#include "pointlight.h"



namespace Sajiki
{
	PointLight::PointLight(int x, int y, int no)
	{
		xPos = x; yPos=y;
		std::stringstream str;
		str << "PointLight" << x << "-"<< y;
		mEntity = ARoot->getScene()->createEntity( str.str(), "assets/pointlight.mesh" );

		std::stringstream str2;
		str2 << "pointlight-" << xPos << "-" << yPos;
		iNo=no;
		mBody = 0;
		mLight = ARoot->getScene()->createLight(str2.str());

		sn=ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(x,y,-3));
		sn->attachObject(mEntity);
		sn->attachObject(mLight);

		

		std::stringstream strMaterialName;
		strMaterialName << "Assets/PointLight" << no;
		mEntity->setMaterialName(strMaterialName.str());
		switch(no)
		{
		case 1:
			mLight->setDiffuseColour(0.7,0,0); //red
			break;
		case 2:
			mLight->setDiffuseColour(0.8,0.79,0.3); //yellow
			break;
		case 3:
			mLight->setDiffuseColour(0,0.5,0.7); //aqua
			break;
		case 4:
			mLight->setDiffuseColour(0.6,0.6,0.6); //grey
			break;
		case 5:
			mLight->setDiffuseColour(0.4,0.8,0.29); //green
			break;
		}

		iType=OT_POINTLIGHT1-1+no;
		iTheme=0;
		iRotation=0;
		sAssetName = "PointLight";
		iLightDepth=0;
	}

	PointLight::~PointLight()
	{
		ARoot->getScene()->destroyLight(mLight);
	}

	void PointLight::setPosition(Ogre::Vector3 pos)
	{
		sn->setPosition(pos.x, pos.y, iLightDepth);
	}

	void PointLight::setRotation(int iRot)
	{

		iRotation = iRot;

		switch(iRotation/90)
		{
		case 0:
			iLightDepth=0;
			break;
		case 1:
			iLightDepth=4;
			break;
		case 2:
			iLightDepth=8;
			break;
		case 3:
			iLightDepth=-6;
			break;
		}
		
		sn->setPosition(sn->getWorldPosition().x, sn->getWorldPosition().y, iLightDepth);

		return;

		
	}

	void PointLight::Update()
	{

		if(ALevel->getLevelMode() == LM_TEST || ALevel->getLevelMode() == LM_PLAY)
			mEntity->setVisible(false);
		
	}
}
