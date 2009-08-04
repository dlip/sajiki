#include "stdafx.h"
#include "background.h"

namespace Sajiki {

	Background::Background(int pTheme)
	{
		iTheme = pTheme;
		std::stringstream strEntity;
		strEntity << "backgrounds/background" << pTheme << ".mesh";
		mBackground = ARoot->getScene()->createEntity( strEntity.str(), strEntity.str() );
		std::stringstream strMaterial;
		strMaterial << "Backgrounds/Background" << pTheme;
		mBackground->setMaterialName(strMaterial.str());
		//mBackground->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
		mParticleSystem[0]=0;
		mBackground3=0;
		mAnimationState = mAnimationState2 = mAnimationState3 = 0;
		switch(iTheme)
		{
		case(1):
			nBackground2 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(17,15.5, -5));
			mBackground2 = ARoot->getScene()->createEntity( "sparkie", "sparkie.mesh" );
			mBackground2->setMaterialName(strMaterial.str());

			nBackground2->attachObject(mBackground2);
			mAnimationState = mBackground2->getAnimationState("Sparkie Animation");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);

			nBackground3 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			mBackground3 = ARoot->getScene()->createEntity( "goo", "background1_Goo.mesh" );
			mBackground3->setMaterialName("Backgrounds/Background1Goo");
			mBackground3->setRenderQueueGroup(Ogre::RENDER_QUEUE_1);
			nBackground3->attachObject(mBackground3);
			for(int x=0; x<15; x++)
			{
				std::stringstream strBone;
				strBone << "Bone" << x+1;
				mParticleSystem[x] = ARoot->getScene()->createParticleSystem(strBone.str(), "Background/Sparkie");
				mParticleSystem[x]->setVisible(false);
				mParticleSystem[x]->setSpeedFactor(0.01);
				mBackground2->attachObjectToBone(strBone.str(), mParticleSystem[x]);
			}
			nBackground = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			break;
		case(2):
			mBackground2 = ARoot->getScene()->createEntity( "fan1", "Fan1.mesh" );
			mBackground2->setMaterialName(strMaterial.str());

			mAnimationState = mBackground2->getAnimationState("Fan1 Animation");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);

			mBackground3 = ARoot->getScene()->createEntity( "fan2", "Fan2.mesh" );
			mBackground3->setMaterialName(strMaterial.str());
			mAnimationState2 = mBackground3->getAnimationState("Fan2 Animation");
			mAnimationState2->setLoop(true);
			mAnimationState2->setEnabled(true);

			nBackground2 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(32, 25.5, -4));
			nBackground3 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(32, 5.5, -4));
			nBackground2->attachObject(mBackground2);
			nBackground3->attachObject(mBackground3);
			nBackground = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			
			break;
		case(3):
			nBackground2 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15.5,15.5,-11));
			mBackground2 = ARoot->getScene()->createEntity( "zappers", "Zappers.mesh" );
			mBackground2->setMaterialName(strMaterial.str());

			nBackground2->attachObject(mBackground2);
			mAnimationState = mBackground2->getAnimationState("Zappers Animation");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
			nBackground = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			mBackground3=ARoot->getScene()->createEntity( "pipes", "Pipes.mesh" );
			mBackground3->setMaterialName("Backgrounds/Background3Pipes");
			nBackground->attachObject(mBackground3);

			mParticleNode[0]= ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(5,15.4,-12));//left
			mParticleNode[0]->roll(Ogre::Degree(-90));
			mParticleNode[1]= ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15.6,5,-12)); //bottom
			mParticleNode[2]= ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(25,15.4,-12));//right
			mParticleNode[2]->roll(Ogre::Degree(90));
			mParticleNode[3]= ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15.6,25,-12));//top
			mParticleNode[3]->roll(Ogre::Degree(180));
			mParticleNode[4]= ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15.5,15.5,-12));//top
			for (int x=0; x<4; x++)
			{
				mParticleSystem[x] = ARoot->getScene()->createParticleSystem("zapperpart" + x, "Background3/Zappers");
				mParticleNode[x]->attachObject(mParticleSystem[x]);
				mParticleSystem[x]->setSpeedFactor(0.01);
			}
			mParticleSystem[4] = ARoot->getScene()->createParticleSystem("zappercentre", "Background3/ZapperCentre");
			mParticleNode[4]->attachObject(mParticleSystem[4]);
			mParticleSystem[4]->setSpeedFactor(0.01);
			
			break;
		case(4):
			nBackground2 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15.5,15.5,-17));
			mBackground2 = ARoot->getScene()->createEntity( "b4mesh2", "Background4_object1.mesh" );
			mBackground2->setMaterialName(strMaterial.str());

			nBackground2->attachObject(mBackground2);
			mAnimationState = mBackground2->getAnimationState("Animation1");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
			nBackground = nBackground2->createChildSceneNode(Ogre::Vector3(-16,-16,18.5));
			break;
		case(5):
			mBackground2 = ARoot->getScene()->createEntity( "bottleline1", "Bottleline_01.mesh" );
			mBackground2->setMaterialName(strMaterial.str());

			mAnimationState = mBackground2->getAnimationState("Bottleline_01 Animation");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);

			mBackground3 = ARoot->getScene()->createEntity( "bottleline2", "Bottleline_02.mesh" );
			mBackground3->setMaterialName(strMaterial.str());
			mAnimationState2 = mBackground3->getAnimationState("Bottleline_02 Animation");
			mAnimationState2->setLoop(true);
			mAnimationState2->setEnabled(true);

			nBackground2 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(15, 14, -15));
			nBackground3 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(16, 36, -15));
			nBackground2->attachObject(mBackground2);
			nBackground3->attachObject(mBackground3);

			nBackground = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			break;
		case(6):

			mBackground2 = ARoot->getScene()->createEntity( "timer1", "Timer_1.mesh" );
			mBackground2->setMaterialName(strMaterial.str());

			mAnimationState = mBackground2->getAnimationState("Timer_1 Animation");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);

			mBackground3 = ARoot->getScene()->createEntity( "timer2", "Timer_2.mesh" );
			mBackground3->setMaterialName(strMaterial.str());
			mAnimationState2 = mBackground3->getAnimationState("Timer_2 Animation");
			mAnimationState2->setLoop(true);
			mAnimationState2->setEnabled(true);

			mBackground4 = ARoot->getScene()->createEntity( "timer3", "Timer_3.mesh" );
			mBackground4->setMaterialName(strMaterial.str());
			mAnimationState3 = mBackground4->getAnimationState("Timer_3 Animation");
			mAnimationState3->setLoop(true);
			mAnimationState3->setEnabled(true);

			nBackground2 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			nBackground3 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			nBackground4 = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			nBackground2->attachObject(mBackground2);
			nBackground3->attachObject(mBackground3);
			nBackground4->attachObject(mBackground4);

			nBackground = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-0.5, -0.5, 1.5));
			break;
		}

		nBackground->attachObject(mBackground);
	}

	void Background::Update()
	{
		if(mAnimationState)
			mAnimationState->addTime(ARoot->getTime());

		if(mAnimationState2)
			mAnimationState2->addTime(ARoot->getTime());

		if(mAnimationState3)
			mAnimationState3->addTime(ARoot->getTime());

		switch(iTheme)
		{
		case(1):
			for(int x=0; x<15; x++)
			{
				mParticleSystem[x]->setVisible(true);
				mParticleSystem[x]->_update(ARoot->getTime()*100);
			}
			break;
		case(2):
			break;
		case(3):
			for(int x=0; x<5; x++)
			{
				if(mAnimationState->getTimePosition()>3.33 && mAnimationState->getTimePosition()<5)
				{
					mParticleSystem[x]->getEmitter(0)->setEmissionRate(50);
				}
				else
				{
					mParticleSystem[x]->getEmitter(0)->setEmissionRate(0);
				}

				if(mAnimationState->getTimePosition()>4.5 && mAnimationState->getTimePosition()<7)
				{
					mParticleSystem[4]->getEmitter(0)->setEmissionRate(50);
				}
				else
				{
					mParticleSystem[4]->getEmitter(0)->setEmissionRate(0);
				}

				mParticleSystem[x]->_update(ARoot->getTime()*100);
			}
			break;
		case(4):
			break;
		case(5):
			break;
		case(6):
			break;
		}
	}

	Background::~Background()
	{

		switch(iTheme)
		{
		case(1):
			for(int x=0; x<15; x++)
			{
				ARoot->getScene()->destroyParticleSystem(mParticleSystem[x]);
			}
			ARoot->getScene()->destroyEntity(mBackground2);
			ARoot->getScene()->destroySceneNode(nBackground2->getName());
			ARoot->getScene()->destroyEntity(mBackground3);
			break;
		case(2):
			ARoot->getScene()->destroyEntity(mBackground2);
			ARoot->getScene()->destroyEntity(mBackground3);
			break;
		case(3):
			ARoot->getScene()->destroyEntity(mBackground3);
			for(int x=0; x<5; x++)
			{
				ARoot->getScene()->destroyParticleSystem(mParticleSystem[x]);
			}
			ARoot->getScene()->destroyEntity(mBackground2);
			ARoot->getScene()->destroySceneNode(nBackground2->getName());
			break;
		case(4):
			ARoot->getScene()->destroyEntity(mBackground2);
			ARoot->getScene()->destroySceneNode(nBackground2->getName());
			break;
		case(5):
			ARoot->getScene()->destroyEntity(mBackground2);
			ARoot->getScene()->destroyEntity(mBackground3);
			break;
		case(6):
			ARoot->getScene()->destroyEntity(mBackground2);
			ARoot->getScene()->destroyEntity(mBackground3);
			ARoot->getScene()->destroyEntity(mBackground4);
			break;
		}
		ARoot->getScene()->destroyEntity(mBackground);
		ARoot->getScene()->destroySceneNode(nBackground->getName());
	}
}