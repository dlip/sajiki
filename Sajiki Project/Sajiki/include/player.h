#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"
#include "interface.h"

namespace Sajiki
{
	typedef enum ePlayerMode
	{
		PM_JUMPING,
		PM_ATTACKING,
		PM_WALKING,
		PM_HURT,
		PM_FALLING
	};

	/**
		@class Player
		@brief The game character.
	 */
	class Player : public NxOgre::CharacterMovementVectorController
	{
	public:
		Player(){};
		~Player();
		Player(int x, int y, int no);
		void Update();
		void move(NxVec3 &out, NxVec3 &moveVector, NxQuat &direction, NxVec3 &g, float t, NxOgre::Character*);
		Ogre::Vector3* getPos(){return &mPos;}
		Ogre::Vector3 getFootPos(){return PlayerFootNode->getWorldPosition();}
		Ogre::Vector3 getHeadPos(){return PlayerHeadNode->getWorldPosition();}
		Ogre::Vector3 getTimerPos(){return TimerMeshNode->getWorldPosition();}
		void Hurt();
		void Die();
		void Bounce(int direction=0);
		void addTime();
		void createPlayer();
		bool bHasTDD;
		bool bHasSlowMo;
		void Rotate();
		bool isDead(){ return bIsDead; }
		bool isHurt(){ return bIsHurt; }
	private:
	protected:
		OIS::KeyCode kLeft, kRight, kUp, kDown, kAttack;
		int xPos, yPos;
		Ogre::Vector3 mPos, mLastPos, mFallDirection;
		NxOgre::Character *mCharacter;
		NxOgre::RayCaster *mRayCaster;
		Ogre::Timer *mTimer, mPosTimer, mHurtTimer, mFallTimer;
		int iTimeToLive;
		Ogre::SceneNode *TimerMeshNode, *PlayerCenterNode, *PlayerFrontNode, *PlayerLeftNode,*PlayerRightNode, *PlayerFootNode, *PlayerLeftFootNode, *PlayerRightFootNode, *PlayerHeadNode, *ParticleNode;
		Ogre::Entity *TimerMesh;
		Ogre::AnimationState *mAnimationState, *mTimerAnimationState;
		Ogre::SceneNode *cNode2;
		Ogre::Entity *mEntity;
		Ogre::ParticleSystem *mParticleSystem;
		bool bAddSound;
		float fElapsedTime;
		float fJumpSpeed;
		float fDeltaTime;
		bool bIsMale, bIsHurt, bIsDead, bIsOnGround;
		Ogre::Vector3 moveVec;
		std::string mpName;
		std::string strSoundWalk, strSoundJump, strSoundDie, strSoundLand,strSoundAttack, strSoundHurt;
		ePlayerMode PMode;
		float fMoveSpeed, fAccelSpeed, fMaxSpeed;
		bool bLookRight;
		int iFallRotation, iPlayerNo;
		Player *mPlayer;
		bool bEnableJump, bEnableDoubleJump, bHasDoubleJumped, bEnableAttack, bJumpKick, bJumpPad;
		int iColour;
		std::string strMaterialName, strMaterialNameWhite; 
		Ogre::Timer mLandSoundTimer, mExplosionTimer, mJumpKickTimer;
		int iFramesPassed;
	};
}

#endif