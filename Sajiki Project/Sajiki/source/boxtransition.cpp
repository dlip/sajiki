#include "stdafx.h"
#include "boxtransition.h"

namespace Sajiki
{
	BoxTransition::BoxTransition()
	{
		snCentre = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15.5,15.5,19.51));
		snFront = snCentre->createChildSceneNode(Ogre::Vector3(0,0,-18));
		snBack = snCentre->createChildSceneNode(Ogre::Vector3(0,0,18));
		snBack->yaw(Ogre::Angle(180));
		snBottom = snCentre->createChildSceneNode(Ogre::Vector3(0,-18,0));
		snBottom->pitch(Ogre::Angle(-90));
		snBottom->roll(Ogre::Angle(-90));
		snTop = snCentre->createChildSceneNode(Ogre::Vector3(0,18,0));
		snTop->pitch(Ogre::Angle(90));
		snTop->roll(Ogre::Angle(90));

		snLeft = snCentre->createChildSceneNode(Ogre::Vector3(-18,0,0));
		snLeft->yaw(Ogre::Angle(90));
		snRight = snCentre->createChildSceneNode(Ogre::Vector3(18,0,0));
		snRight->yaw(Ogre::Angle(-90));

		entFront = ARoot->getScene()->createEntity("BoxFront", "levelpreview.mesh");
		entBack = ARoot->getScene()->createEntity("BoxBack", "levelpreview.mesh");
		entBottom = ARoot->getScene()->createEntity("BoxBottom", "levelpreview.mesh");
		entTop = ARoot->getScene()->createEntity("BoxTop", "levelpreview.mesh");
		entLeft = ARoot->getScene()->createEntity("BoxLeft", "levelpreview.mesh");
		entRight = ARoot->getScene()->createEntity("BoxRight", "levelpreview.mesh");
		
		std::string test = AInterface->getLevelQueueName(0);
		entFront->setMaterialName(AInterface->getLevelQueueName(0));
		entLeft->setMaterialName(AInterface->getLevelQueueName(1));
		entBack->setMaterialName(AInterface->getLevelQueueName(2));
		entRight->setMaterialName(AInterface->getLevelQueueName(3));
		entTop->setMaterialName(AInterface->getLevelQueueName(4));
		entBottom->setMaterialName(AInterface->getLevelQueueName(5));
		

		snFront->attachObject(entFront);
		snBack->attachObject(entBack);
		snBottom->attachObject(entBottom);
		snTop->attachObject(entTop);
		snLeft->attachObject(entLeft);
		snRight->attachObject(entRight);
		fCameraYaw=0;

		entFront->setVisible(false);
		
		bCompleteZoom=false;
		bCompleteTurn=false;
		fCameraZoom=0;
		bStartTransition=true;

	}

	BoxTransition::~BoxTransition()
	{
		snCentre->removeAndDestroyAllChildren();
		ARoot->getScene()->destroyEntity(entFront);
		ARoot->getScene()->destroyEntity(entLeft);
		ARoot->getScene()->destroyEntity(entRight);
		ARoot->getScene()->destroyEntity(entBack);
		ARoot->getScene()->destroyEntity(entTop);
		ARoot->getScene()->destroyEntity(entBottom);
	}

	bool BoxTransition::Update()
	{
		if(bStartTransition)
		{
			mZoomTimer.reset();
			bStartTransition=false;
			bCompleteZoom=false;
			bCompleteTurn=false;
			fCameraZoom=0;
			ASound->PlaySound("levelrotate.mp3");
		}

		
			if(!bCompleteZoom)
			{
				switch(ALevel->getCurrentRound())
				{
				case 1:
					entFront->setVisible(true);
					break;
				case 2:
					entLeft->setVisible(true);
					break;
				case 3:
					entBack->setVisible(true);
					break;
				case 4:
					entRight->setVisible(true);
					break;
				case 5:
					entTop->setVisible(true);
					break;
				}
				float fZoomTime= mZoomTimer.getMilliseconds()/1000.f;
				if (fZoomTime<1.5)
				{
					ARoot->getCamera()->moveRelative(Ogre::Vector3(0,0,fCameraZoom));
					fCameraZoom=15.5*(1-cosf(fZoomTime*2));
					ARoot->getCamera()->moveRelative(Ogre::Vector3(0,0,-fCameraZoom));
				}
				else
				{
					mTurnTimer.reset();
					bCompleteZoom=true;
					ASound->PlaySound("levelrotate.mp3");
				}
			}
			else
			{
				float fTurnTime= mTurnTimer.getMilliseconds()/1000.f;
				if (fTurnTime<1.5)
				{

					if(ALevel->getCurrentRound()<4)
					{
					ARoot->getCamera()->yaw(Ogre::Degree(-fCameraYaw));
					fCameraYaw=45*(1-cosf(fTurnTime*2));
					ARoot->getCamera()->yaw(Ogre::Degree(fCameraYaw));
					}
					else if(ALevel->getCurrentRound()==4)
					{
						ARoot->getCamera()->pitch(Ogre::Degree(-fCameraYaw));
						fCameraYaw=45*(1-cosf(fTurnTime*2));
						ARoot->getCamera()->pitch(Ogre::Degree(fCameraYaw));
					}
					else if(ALevel->getCurrentRound()==5)
					{
						ARoot->getCamera()->pitch(Ogre::Degree(-fCameraYaw));
						fCameraYaw=90*(1-cosf(fTurnTime*2));
						ARoot->getCamera()->pitch(Ogre::Degree(fCameraYaw));
					}
					mZoomTimer.reset();
					fCameraZoom=0;
				}
				else
				{
					if(!bCompleteTurn)
					{
						bCompleteTurn=true;
						ASound->PlaySound("levelrotate.mp3");
					}
					float fZoomTime= mZoomTimer.getMilliseconds()/1000.f;
					if (fZoomTime<1.5)
					{
						ARoot->getCamera()->moveRelative(Ogre::Vector3(0,0,-fCameraZoom));
						if(ALevel->getCurrentRound()==5)
							fCameraZoom=9*(1-cosf(fZoomTime*2));
						else
							fCameraZoom=15.5*(1-cosf(fZoomTime*2));
						ARoot->getCamera()->moveRelative(Ogre::Vector3(0,0,fCameraZoom));
					}
					else
					{
						switch(ALevel->getCurrentRound())
						{
						case 1:
							entLeft->setVisible(false);
							break;
						case 2:
							entBack->setVisible(false);
							break;
						case 3:
							entRight->setVisible(false);
							break;
						case 4:
							entTop->setVisible(false);
							break;
						case 5:
							entBottom->setVisible(false);
							break;
						}
						if(ALevel->getCurrentRound()<4)
							snCentre->yaw(Ogre::Degree(-90));
						else if(ALevel->getCurrentRound()==4)
							snCentre->roll(Ogre::Degree(-90));
						else if(ALevel->getCurrentRound()==5)
							snCentre->roll(Ogre::Degree(-180));
						
						fCameraYaw=0;
						bCompleteZoom=false;
						bStartTransition=true;
						return false;
					}
				}
			}


		return true;

	}
}

