#ifndef LEVEL_H
#define LEVEL_H
#include "singleton.h"
#include "player.h"
#include "background.h"
#include "interface.h"

namespace Sajiki
{
	class Asset;
	class BrokenTile;
	class Spikes;
	class TDD;
	class Tile;
	class TimeObj;
	class JumpPad;
	class SpikeBall;
	class SlowMo;
	class RotationSwitch;
	class PlayerSpawn;
	class PointLight;

	class LevelEditor;

	typedef enum 
	{
		OT_TILE,
		OT_BROKENTILE,
		OT_TDD,
		OT_TIME,
		OT_SPIKES,
		OT_JUMPPAD,
		OT_SPIKEBALL,
		OT_SLOWMO,
		OT_ROTATIONSWITCH,
		OT_PLAYERSPAWN1,
		OT_PLAYERSPAWN2,
		OT_PLAYERSPAWN3,
		OT_PLAYERSPAWN4,
		OT_POINTLIGHT1,
		OT_POINTLIGHT2,
		OT_POINTLIGHT3,
		OT_POINTLIGHT4,
		OT_POINTLIGHT5,
		OT_LAST,
	}objType;

	typedef enum 
	{
		LM_NONE,
		LM_PLAY,
		LM_TEST,
		LM_EDIT,
		LM_ROTATE,
		LM_MENU,
		LM_QUIT
	}levMode;

	class Player;

	/**
		@class Level
		@brief Controls gameplay logic and the opening and saving level files.
	 */

	class Level: public Singleton<Level>
	{
		friend Singleton;
	public:
		void Init();
		void Update();
		void testUpdate();
		void rotateUpdate();
		void playUpdate();
		void setLevelMode(levMode l);
		int getLevelMode(){return lMode;}
		int getOldLevelMode(){return lModeOld;}
		void addObject(int oT, int x, int y, int rot);
		void removeObject(int x, int y);
		void clearLevel();
		int getLevelTheme(){ return iLevelTheme;}
		void setLevelTheme( int iT );
		void saveLevel( const char *filename );
		void loadLevel( const char *filename );
		void resetLevelObjects();
		Player* getPlayer(int no);
		int getPlayerCount(){return iPlayerCount;}
		void rotate(int iDegrees);
		bool PressInfoOk(const CEGUI::EventArgs& e);
		void setLightLevel(int lightlevel);
		bool isPlayerSpawnListComplete();
		void getLevelList(std::string ceguiListBoxName);
		std::vector<Asset *>* getSpikeballList(){return &vecSpikeballList;}
		void randomRotate();
		void slowMo();
		void setCameraProjection(Ogre::ProjectionType pt);
		bool isRoundStarted(){return bRoundStarted;}
		bool isRoundOver(){return bRoundOver;}
		bool isSlowMo(){ return bSlowMo; }
		int getCurrentRound(){return iCurrentRound;}
		float getCameraRoll(){return fCameraRoll;}
		int getLightLevel(){return iLightLevel;}
	private:
		CEGUI::Window* sheet;

		float fCameraRoll,fCameraRollTarget, fCameraRollOriginal;
		bool bSlowMo;
		Ogre::Timer mTimerRoundStart;
		void StartNextRound();
		int iCurrentRound;
		int iLightLevel;
		bool bRoundOver, bLevelTransition, bRoundStarted;

		Background *mBackground;
		Asset* oLevel[32][32];
		int iLevelTheme, iPlayerCount;
		LevelEditor *levelEditor;
		Player *mPlayer[4];
		Ogre::Timer mSlowMoTimer, mRotateTimer;
		Ogre::Quaternion mCameraOrientation;
		Ogre::Vector2 mPlayerSpawnPos[4];
		int playerScores[4];
		levMode lMode, lModeOld;
		int iRoundWinner, iGameWinner;
		Ogre::Timer mRoundOverTimer;
		BoxTransition *mBoxTransition;
		std::vector<Asset *> vecSpikeballList;
	protected:
		Level(){}
	};
}

#define ALevel Level::Instance()

#endif