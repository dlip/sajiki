#ifndef ASSET_H
#define ASSET_H

#include "level.h"
#include "player.h"

namespace Sajiki
{
	/**
		@class Asset
		@brief Virtual class for other assets to inherit from
	 */
	class Asset
	{
	public:
		virtual ~Asset();
		virtual Ogre::SceneNode* getNode(){ return mBody->getNode();}
		int getRotation(){return iRotation;}
		int getType(){return iType;}
		virtual void setRotation(int iRot);
		virtual void setTheme(int pTheme);
		virtual void setPosition(Ogre::Vector3 pos){ mBody->getNode()->setPosition(pos);}
		virtual void Update() = 0;
		NxOgre::Actor* getBody(){ return mBody;}
	private:

	protected:
		int xPos, yPos;
		int iRotation, iType, iTheme;
		Ogre::Entity *mEntity;
		NxOgre::Body *mBody;
		std::string sAssetName, sSoundName;
		Ogre::SceneNode *upNode;
		Player *mPlayer;
		bool bTouched;
		bool bCollected;
	};
}

#endif