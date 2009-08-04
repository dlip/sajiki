#include "stdafx.h"
#include "player.h"
namespace Sajiki
{

	Player::Player(int x, int y, int no):
		mCharacter(0),
		mAnimationState(0),
		fMaxSpeed(10),
		fAccelSpeed(20)
	{
		iPlayerNo = no;
		xPos = x; yPos = y;
		kLeft = AInterface->getPlayerSettings(iPlayerNo)->ctrl.kLeft; 
		kRight = AInterface->getPlayerSettings(iPlayerNo)->ctrl.kRight;
		kUp = AInterface->getPlayerSettings(iPlayerNo)->ctrl.kUp;
		kDown = AInterface->getPlayerSettings(iPlayerNo)->ctrl.kDown;
		kAttack = AInterface->getPlayerSettings(iPlayerNo)->ctrl.kAttack;
		iTimeToLive = 70;
		bIsMale = AInterface->getPlayerSettings(iPlayerNo)->bIsMale;
		bIsDead=false;
		iColour= AInterface->getPlayerSettings(iPlayerNo)->iColour;
		mPos.x = x;
		mPos.y = y;
		mPos.z = 0;
		mPos+=ARoot->getCamera()->getUp()*0.4;
		std::stringstream strpname;
		strpname << "player" << no;
		mpName = strpname.str();


		std::stringstream strMat;
		if(bIsMale)
			strMat<< "Player/PlayerM" << iColour;
		else
			strMat<< "Player/PlayerF" << iColour;

		strMaterialName=strMat.str();
		strMaterialNameWhite = strMat.str() + "White";

		mParticleSystem = ARoot->getScene()->createParticleSystem(mpName+"pdie", "Player/Explode");
		mParticleSystem->setVisible(true);
		bIsOnGround=false;
		mTimer = ARoot->getRoot()->getTimer();
		mTimer->reset();
		bAddSound =true;
		fElapsedTime=0;
		fMoveSpeed=0;
		bIsHurt=false;
		moveVec = moveVec.ZERO;
		mRayCaster = new NxOgre::RayCaster(Ogre::Vector3(0,0,0), Ogre::Vector3(0,0,0), 1, NxOgre::RayCaster::RCT_ALL, ARoot->getPhysicsScene());
		bHasTDD = bHasSlowMo = false;
		bLookRight = true;
		//load sounds
		strSoundJump = mpName + "jump";
		if(bIsMale)
		{
			strSoundJump = "playermalejump.mp3";
			strSoundDie = "playermaledie.mp3";
			strSoundHurt = "playermalepain.mp3";
			strSoundAttack = "playermaleattack.mp3";
		}
		else
		{
			strSoundJump = "playerfemalejump.mp3";
			strSoundDie = "playerfemaledie.mp3";
			strSoundHurt = "playerfemalepain.mp3";
			strSoundAttack = "playerfemaleattack.mp3";
		}

		strSoundLand ="playerland.mp3";
		bEnableJump=false;
		bEnableDoubleJump=true;

		mCharacter=0;
		createPlayer();
		bJumpKick=false;
		iFramesPassed=0;
	}
	Player::~Player()
	{
		ARoot->getScene()->destroyEntity(mEntity);
		ARoot->getScene()->destroyEntity(TimerMesh);
		if(mCharacter)
			ARoot->getPhysicsScene()->destroyCharacter(mCharacter->getName());
		ARoot->getScene()->destroyParticleSystem(mParticleSystem);
	}

	void Player::createPlayer()
	{
		bool bFrozen=false;
		
		if (mCharacter)
		{
			if (mPos.distance(Ogre::Vector3(xPos,yPos+0.4,0))< 0.5)//mCharacter->getGlobalPosition().x == xPos && mCharacter->getGlobalPosition().y == yPos+0.4)
				bFrozen=true;
			else
				mPos = mCharacter->getNode()->getPosition()+(ARoot->getCamera()->getUp()*0.4);

			mAnimationState->setEnabled(false);
			
			mCharacter->getNode()->detachAllObjects();
			ARoot->getScene()->destroyEntity(mEntity);
			ARoot->getScene()->destroyEntity(TimerMesh);
			ARoot->getPhysicsScene()->destroyCharacter(mCharacter->getName());	
		}
		NxOgre::CharacterParams cp; 
		cp.setToDefault(); 
		cp.mType = NxOgre::CharacterParams::CT_Box;
		if(ALevel->getCameraRoll()==180 || ALevel->getCameraRoll()==0)
			cp.mDimensions.set(0.5,1.3,2);
		else if(ALevel->getCameraRoll()==90 || ALevel->getCameraRoll()==270)
			cp.mDimensions.set(1.3,0.5,2);
		srand(rand());
		std::stringstream strname;
		strname << iPlayerNo << "-" << rand();
		if(bFrozen)
			ARoot->getPhysicsScene()->createCharacter(strname.str()+"frozen", Ogre::Vector3(-100,-100,0), cp);

		if (bIsDead)
			mCharacter = ARoot->getPhysicsScene()->createCharacter(strname.str(), Ogre::Vector3(0,-100,0), cp);
		else
			mCharacter = ARoot->getPhysicsScene()->createCharacter(strname.str(), mPos, cp);
		
		mCharacter->createNode();

		TimerMeshNode = mCharacter->getNode()->createChildSceneNode(Ogre::Vector3(0,2,2));
		TimerMesh = ARoot->getScene()->createEntity( mpName + "Timer", "Hp_Timer.mesh" );
		TimerMesh->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
		TimerMeshNode->attachObject(TimerMesh);
		
		TimerMeshNode->pitch(Ogre::Degree(90));
		TimerMeshNode->setScale(0.012, 0.012, 0.012);
		mTimerAnimationState = TimerMesh->getAnimationState("Hp_Timer Animation");
		mTimerAnimationState->setEnabled(true);
		if(bIsDead)
			TimerMeshNode->setScale(Ogre::Vector3(0,0,0));
		cNode2 = mCharacter->getNode()->createChildSceneNode(Ogre::Vector3(0,-0.7,0));
		
		PlayerLeftNode = mCharacter->getNode()->createChildSceneNode(Ogre::Vector3(-0.3,0,0));
		PlayerRightNode = mCharacter->getNode()->createChildSceneNode(Ogre::Vector3(0.3,0,0));
		PlayerFootNode= mCharacter->getNode()->createChildSceneNode(Ogre::Vector3(0,-0.7,0));
		PlayerLeftFootNode = PlayerFootNode->createChildSceneNode(Ogre::Vector3(-0.3,0,0));
		PlayerRightFootNode = PlayerFootNode->createChildSceneNode(Ogre::Vector3(0.3,0,0));
		PlayerHeadNode= mCharacter->getNode()->createChildSceneNode(Ogre::Vector3(0,0.7,0));
		PlayerFrontNode = cNode2->createChildSceneNode(Ogre::Vector3(0,0.7,1));

		if(bIsMale)
		{
			mEntity = ARoot->getScene()->createEntity(mpName + "player", "Character_M.mesh");
		}
		else
		{
			mEntity = ARoot->getScene()->createEntity(mpName + "player", "Character_F.mesh");
		}
		mEntity->setMaterialName(strMaterialName);

		cNode2->attachObject(mEntity);
		mCharacter->getNode()->roll(Ogre::Radian(Ogre::Degree(ALevel->getCameraRoll())));

		PMode=PM_FALLING;
		mAnimationState = mEntity->getAnimationState("Run");
		mAnimationState->setLoop(true);
		mAnimationState->setEnabled(true);
		mCharacter->setMovementVectorController(this);
		mFallTimer.reset();
		bIsOnGround=false;
		fMoveSpeed=0;
		fJumpSpeed=0;
		bJumpPad=false;
	}

	void Player::Rotate()
	{
		if(!bIsDead)
		{
			mCharacter->getNode()->resetOrientation();
			mCharacter->getNode()->roll(Ogre::Radian(Ogre::Degree(ALevel->getCameraRoll())));
		}
		else
		{
			mCharacter->getNode()->resetOrientation();
			mCharacter->getNode()->roll(Ogre::Radian(Ogre::Degree(iFallRotation)));
			moveVec = mFallDirection * (9.8 * fDeltaTime);
			mAnimationState->addTime(fDeltaTime*2);
		}
	}

	void Player::Update()
	{
		iFramesPassed++;
		if(iFramesPassed==50)
		{
			//player appears frozen :(
			if (mPos.distance(Ogre::Vector3(xPos,yPos+0.4,0))< 0.2)
				createPlayer();
		}
		if (bIsDead)
		{
			if(mExplosionTimer.getMilliseconds() > 500)
				mParticleSystem->getEmitter(0)->setEmissionRate(0);

			mParticleSystem->_update(ARoot->getTime());

			return;
		}
		if(ALevel->isRoundOver())
		{
			Rotate();
			return;
		}

		fDeltaTime =  ARoot->getTime();
		if (!bHasSlowMo)
		{
			if(ALevel->isSlowMo())
				fDeltaTime *=0.5;
		}
		else
		{
			bHasSlowMo = ALevel->isSlowMo();
		}

		if(mHurtTimer.getMilliseconds()<3000 && !bIsDead)
		{
			if(bIsMale)
				mEntity->setMaterialName(strMaterialNameWhite);
			else
				mEntity->setMaterialName(strMaterialNameWhite);
		}
		else
		{
			bIsHurt=false;
			if(bIsMale)
				mEntity->setMaterialName(strMaterialName);
			else
				mEntity->setMaterialName(strMaterialName);
		}

		if(!bHasTDD)
			fElapsedTime+=fDeltaTime;

		mPos = mCharacter->getNode()->getPosition();

		if (iTimeToLive < (int)fElapsedTime)
			Die();

			
		if(PMode != PM_HURT)
		{
			bool bCollision = false;
			
			mRayCaster->setOrigin(PlayerLeftFootNode->getWorldPosition());
			mRayCaster->setDirection(-ARoot->getCamera()->getUp());
			mRayCaster->setMaxDistance(0.1);
			mRayCaster->castShape(NxOgre::RayCaster::AF_NONE);
			if(mRayCaster->mReport.count()>0)
				bCollision=true;

			
			mRayCaster->setOrigin(PlayerRightFootNode->getWorldPosition());
			mRayCaster->setDirection(-ARoot->getCamera()->getUp());
			mRayCaster->setMaxDistance(0.1);
			mRayCaster->castShape(NxOgre::RayCaster::AF_NONE);
			if(mRayCaster->mReport.count()>0)
				bCollision=true;

			mRayCaster->setOrigin(PlayerFootNode->getWorldPosition());
			mRayCaster->setDirection(-ARoot->getCamera()->getUp());
			mRayCaster->setMaxDistance(0.1);
			mRayCaster->castShape(NxOgre::RayCaster::AF_NONE);
			if(mRayCaster->mReport.count()>0)
				bCollision=true;

			if(bCollision)
			{
				bJumpPad=false;
				if(!bIsOnGround)
				{
					if(mLandSoundTimer.getMilliseconds() > 500)
					{
							ASound->PlaySound(strSoundLand);
							mLandSoundTimer.reset();
					}

					if(PMode==PM_FALLING)
					{
						if(mFallTimer.getMilliseconds() > 1500)
							Hurt();
						else
						{
							mFallTimer.reset();
							
						}
						
					}
					bJumpPad=false;
					bHasDoubleJumped=bEnableDoubleJump=false;
					if(PMode!=PM_HURT)
						PMode=PM_WALKING;
					bIsOnGround=true;
				}
				
				mFallTimer.reset();
				
			}

			else
			{
				if(bIsOnGround)
				{
					if(PMode!=PM_FALLING && PMode!=PM_ATTACKING)
					{
						PMode=PM_JUMPING;
						
					}
					bIsOnGround=false;
				}
			}
		}


		float timeleft = (iTimeToLive - fElapsedTime) / iTimeToLive;
		float poss= (1-timeleft)*mTimerAnimationState->getLength();
		if (poss>mTimerAnimationState->getLength()-0.01) TimerMesh->setVisible(false);
		else
			TimerMesh->setVisible(true);

		mTimerAnimationState->setTimePosition(poss);

		
		if (bHasTDD)
			TimerMesh->setVisible(false);
		else if(timeleft>0.66)
			TimerMesh->setMaterialName("Assets/HP1");
		else if(timeleft>0.33)
			TimerMesh->setMaterialName("Assets/HP2");
		else
			TimerMesh->setMaterialName("Assets/HP3");

		if(fJumpSpeed < 1)
		{
			for (int x=0; x<ALevel->getPlayerCount(); x++)
			{
				if(x==iPlayerNo)
					continue;
				mPlayer = ALevel->getPlayer(x);

				if(mPlayer->getHeadPos().distance(PlayerFootNode->getWorldPosition())<0.5)
				{
						if(fJumpSpeed < -6)
							mPlayer->Hurt();
						bHasDoubleJumped=bEnableDoubleJump=false;
						bJumpPad=true;
						PMode=PM_JUMPING;
						mFallTimer.reset();
						fJumpSpeed=25;
				}
			}
		}
		switch(PMode)
		{
		case PM_JUMPING:

			if (mFallTimer.getMilliseconds()>2000)
			{
				PMode=PM_FALLING;
				return;
			}
			if(mJumpKickTimer.getMilliseconds()>1000)
				bEnableAttack=true;

			if(!AInput->isKey(kUp) && !bHasDoubleJumped)
				bEnableDoubleJump=true;	
			
				
			if (AInput->isKey(kAttack)&&bEnableAttack)
			{
				bEnableAttack=false;
				bJumpKick=true;
				mAnimationState->setEnabled(false);
				mAnimationState = mEntity->getAnimationState("Jump_kick");
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(false);
				mAnimationState->setTimePosition(0);
				ASound->PlaySound(strSoundAttack);
				mJumpKickTimer.reset();
				break;
			}
			if(bJumpKick)
			{
				mAnimationState->addTime(fDeltaTime);

				if(mJumpKickTimer.getMilliseconds()>400)
					bJumpKick=false;
				for (int x=0; x<ALevel->getPlayerCount(); x++)
				{
					if(x==iPlayerNo)
						continue;
					mPlayer = ALevel->getPlayer(x);

					if(mPlayer->getPos()->distance(PlayerFrontNode->getWorldPosition())<0.5)
					{
						mPlayer->Hurt();
						fMoveSpeed=0;
					}
				}
			}

			if (fJumpSpeed > 0)
			{
				if(AInput->isKey(kUp) && bEnableDoubleJump && !bHasDoubleJumped)
				{
					bHasDoubleJumped=true;
					ASound->PlaySound(strSoundJump);
					if(fJumpSpeed<25)
						fJumpSpeed=25;
				}
				if(mAnimationState->getAnimationName() !="Jump_into" && !bJumpKick)
				{
					mAnimationState->setEnabled(false);
					mAnimationState = mEntity->getAnimationState("Jump_into");
					mAnimationState->setEnabled(true);
					mAnimationState->setLoop(false);
					mAnimationState->setTimePosition(0);
				}
				mAnimationState->addTime(fDeltaTime);
				mFallTimer.reset();
				//test to see if hit head
				mRayCaster->setOrigin(PlayerHeadNode->getWorldPosition());
				mRayCaster->setDirection(ARoot->getCamera()->getUp());
				mRayCaster->setMaxDistance(0.1);
				mRayCaster->castShape(NxOgre::RayCaster::AF_NONE);
				if(mRayCaster->mReport.count()>0)
					fJumpSpeed -= fDeltaTime*100;
				else
				{
					if (!AInput->isKey(kUp)&&!bJumpPad)
						fJumpSpeed -= fDeltaTime*100;
					else
						fJumpSpeed -= fDeltaTime*30;
				}
			}
			else
			{
				if(!bJumpPad)
					fJumpSpeed = 0;

				if(mAnimationState->getAnimationName() !="Jump_outo" && !bJumpKick)
				{
					mAnimationState->setEnabled(false);
					mAnimationState = mEntity->getAnimationState("Jump_outo");
					mAnimationState->setEnabled(true);
					mAnimationState->setLoop(false);
					mAnimationState->setTimePosition(0);
				}
				mAnimationState->addTime(fDeltaTime);

			}

			if (fMoveSpeed > 5)
			{
				bLookRight=true;

				//test to see if hit vertical wall
				mRayCaster->setOrigin(mPos);
				mRayCaster->setDirection(ARoot->getCamera()->getRight());
				mRayCaster->setMaxDistance(0.4);
				mRayCaster->castShape(NxOgre::RayCaster::AF_NONE);
				if(mRayCaster->mReport.count()>0)
					fMoveSpeed -= fDeltaTime*100;
				else
				{
					if (AInput->isKey(kLeft))
						fMoveSpeed -= fDeltaTime * (fAccelSpeed*3);
					else if (!AInput->isKey(kRight))
						fMoveSpeed -= fDeltaTime * (fAccelSpeed*2);
				}
				
			}
			else if (fMoveSpeed < -5)
			{
				bLookRight=false;
				//test to see if hit vertical wall
				mRayCaster->setOrigin(mPos);
				mRayCaster->setDirection(-ARoot->getCamera()->getRight());
				mRayCaster->setMaxDistance(0.4);
				mRayCaster->castShape(NxOgre::RayCaster::AF_NONE);
				if(mRayCaster->mReport.count()>0)
					fMoveSpeed += fDeltaTime*100;
				else
				{
					if (AInput->isKey(kRight))
						fMoveSpeed += fDeltaTime * (fAccelSpeed*3);
					else if (!AInput->isKey(kLeft))
						fMoveSpeed += fDeltaTime * (fAccelSpeed*2);
				}

				

				
			}
			else
			{
				if(AInput->isKey(kLeft))
					fMoveSpeed -= fDeltaTime * (fAccelSpeed);
				else if(AInput->isKey(kRight))
					fMoveSpeed += fDeltaTime * (fAccelSpeed);
				else
				{
					fMoveSpeed=0;
					if(ALevel->getCameraRoll()==90)
						fMoveSpeed=0.01;
					else if(ALevel->getCameraRoll()==270)
						fMoveSpeed=-0.01;
				}
			}


			break;

		case PM_FALLING:
			if(mFallTimer.getMilliseconds() > 1500)
			{
				if(mAnimationState->getAnimationName()!="Falling")
				{
					mAnimationState->setEnabled(false);
					mAnimationState = mEntity->getAnimationState("Falling");
					mAnimationState->setLoop(true);
					mAnimationState->setEnabled(true);
				}
				mAnimationState->addTime(fDeltaTime*2);
			}

	
				if(AInput->isKey(kLeft))
					fMoveSpeed = -2;
				else if(AInput->isKey(kRight))
					fMoveSpeed = 2;
				else
				{
					fMoveSpeed=0;
					if(ALevel->getCameraRoll()==90)
						fMoveSpeed=0.01;
					else if(ALevel->getCameraRoll()==270)
						fMoveSpeed=-0.01;
				}
		

			break;
		case PM_ATTACKING:
			mAnimationState->addTime(fDeltaTime*2);

			if(mAnimationState->getTimePosition()>0.3)
			{
				for (int x=0; x<ALevel->getPlayerCount(); x++)
				{
					if(x==iPlayerNo)
						continue;
					mPlayer = ALevel->getPlayer(x);

					if(mPlayer->getPos()->distance(PlayerFrontNode->getWorldPosition())<0.5)
					{
						mPlayer->Hurt();
					}
					else if(mPlayer->getFootPos().distance(PlayerFrontNode->getWorldPosition())<0.7)
					{
						mPlayer->Hurt();
					}
				}
			}

			if(mAnimationState->getTimePosition()>0.9)
			{
				PMode = PM_WALKING;
			}
			break;
		case PM_HURT:
			mAnimationState->addTime(fDeltaTime*2);
			if(mHurtTimer.getMilliseconds()>1000)
			{
				PMode = PM_WALKING;
				return;
			}
			break;

		case PM_WALKING:
			if (!(AInput->isKey(kAttack)&&!bEnableAttack))
				bEnableAttack=true;
			if (AInput->isKey(kAttack)&&bEnableAttack)
			{
				bEnableAttack=false;
				fMoveSpeed=0;
				PMode = PM_ATTACKING;
				mAnimationState->setEnabled(false);
				std::stringstream strAttack;
				srand(ARoot->getTotalTime()*1000);
				int irand=rand()%3+1;
				strAttack << "Attack" << irand;
				mAnimationState = mEntity->getAnimationState(strAttack.str());
				mAnimationState->setEnabled(true);
				mAnimationState->setLoop(true);
				mAnimationState->setTimePosition(0);
				ASound->PlaySound(strSoundAttack);
				break;
			}
			if (!(AInput->isKey(kUp)&&!bEnableJump))
				bEnableJump=true;
			if (AInput->isKey(kUp)&&bEnableJump)
			{
				bEnableJump=false;
				fJumpSpeed = 30;
				ASound->PlaySound(strSoundJump);
				PMode = PM_JUMPING;
				break;
			}
			if (AInput->isKey(kLeft))
			{
				bLookRight=false;
				if (fMoveSpeed > 2)
					fMoveSpeed -= fDeltaTime * (fAccelSpeed*4);
				fMoveSpeed -= fDeltaTime * fAccelSpeed;
			}
			else if (AInput->isKey(kRight))
			{
				bLookRight=true;
				if (fMoveSpeed < -2)
					fMoveSpeed += fDeltaTime * (fAccelSpeed*4);
				fMoveSpeed += fDeltaTime * fAccelSpeed;
			}
			else
			{
				if (fMoveSpeed > 2)
					fMoveSpeed -= fDeltaTime * (fAccelSpeed*2);
				else if (fMoveSpeed < -2)
					fMoveSpeed += fDeltaTime * (fAccelSpeed*2);
				else
				{
				
					fMoveSpeed=0;
				}
			}
			
			if(fMoveSpeed==0)
			{
				if(mAnimationState->getAnimationName() !="Idol")
				{
					mAnimationState->setEnabled(false);
					mAnimationState = mEntity->getAnimationState("Idol");
					mAnimationState->setEnabled(true);
					mAnimationState->setLoop(true);
				}
				mAnimationState->addTime(fDeltaTime);
			}
			else 
			{
				if(mAnimationState->getAnimationName() !="Run")
				{
					mAnimationState->setEnabled(false);
					mAnimationState = mEntity->getAnimationState("Run");
					mAnimationState->setEnabled(true);
					mAnimationState->setLoop(true);
				}
				mAnimationState->addTime(fDeltaTime*(abs(fMoveSpeed)/5));
			}
			break;
		}
					

		
		mCharacter->getNode()->resetOrientation();
		mCharacter->getNode()->roll(Ogre::Radian(Ogre::Degree(ALevel->getCameraRoll())));
		cNode2->resetOrientation();
		PlayerLeftNode->resetOrientation();
		PlayerLeftNode->roll(Ogre::Radian(Ogre::Degree(ALevel->getCameraRoll())));
		
		if(PMode!=PM_FALLING)
		{
			if(bLookRight)
				cNode2->yaw(Ogre::Radian(Ogre::Degree(90)));
			else
				cNode2->yaw(Ogre::Radian(Ogre::Degree(-90)));
		}
		mCharacter->setPosition(Ogre::Vector3(mCharacter->getNode()->getPosition().x, mCharacter->getNode()->getPosition().y, 0));


		moveVec += ARoot->getCamera()->getUp() * (fDeltaTime * fJumpSpeed);
		//apply gravity
		moveVec += -ARoot->getCamera()->getUp() * (9.8 * fDeltaTime);

		//apply x movement
		if(!bJumpPad)
		{
			if (fMoveSpeed>fMaxSpeed)
				fMoveSpeed = fMaxSpeed;
			else if (fMoveSpeed<-fMaxSpeed)
				fMoveSpeed = -fMaxSpeed;
		}
		
		moveVec += ARoot->getCamera()->getRight() * (fDeltaTime * fMoveSpeed);

	}

	void Player::addTime()
	{
		fElapsedTime -=10;
		if(fElapsedTime<0)
			fElapsedTime =0;

	}

	void Player::Hurt()
	{
		if(mHurtTimer.getMilliseconds()<3000 || bIsDead)
			return;

		mAnimationState->setEnabled(false);
		mAnimationState = mEntity->getAnimationState("Fall_down_after_attack");
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(true);
		mAnimationState->setTimePosition(0);
		PMode=PM_HURT;
		bIsHurt=true;
		mHurtTimer.reset();
		fMoveSpeed=0;
		fJumpSpeed=0;
		ASound->PlaySound(strSoundHurt);
		
		if(!bHasTDD)
			fElapsedTime += 10;

		bHasTDD=false;
	}
	void Player::Die()
	{
		if (bIsDead)
			return;

		ParticleNode = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(mPos);
		ParticleNode->attachObject(mParticleSystem);
		

		TimerMeshNode->setScale(0,0,0);		
		iFallRotation = ALevel->getCameraRoll();
		mFallDirection = -ARoot->getCamera()->getUp();
		mAnimationState->setEnabled(false);
		mAnimationState = mEntity->getAnimationState("Fall_down_after_attack");
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(true);
		mAnimationState->setTimePosition(0);
		ASound->PlaySound(strSoundDie);
		bHasTDD=false;
		bIsDead=true;
		createPlayer();
		mExplosionTimer.reset();
		ASound->PlaySound("playerexplode.mp3");

	}
	void Player::Bounce(int direction)
	{
		switch (direction)
		{
		case(0): //up
			bHasDoubleJumped=bEnableDoubleJump=false;
			bJumpPad=true;
			PMode=PM_JUMPING;
			mFallTimer.reset();
			fJumpSpeed=35;
			break;
		case(1): //right
			bJumpPad=true;
			PMode=PM_JUMPING;
			mFallTimer.reset();
			fMoveSpeed = fMaxSpeed*2.5;
			break;
		case(2): //left
			bJumpPad=true;
			PMode=PM_JUMPING;
			mFallTimer.reset();
			fMoveSpeed = -fMaxSpeed*2.5;
			break;
		case(3): //down
			bJumpPad=true;
			fJumpSpeed=-15;
			break;
		}
	}

	void Player::move(NxVec3 &out, NxVec3 &moveVector, NxQuat &direction, NxVec3 &g, float t, NxOgre::Character*)
	{
		out = NxVec3(moveVec.x, moveVec.y, 0);
		moveVec = moveVec.ZERO;
	}
}