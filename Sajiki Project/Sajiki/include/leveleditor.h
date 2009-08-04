#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "singleton.h"
#include "interface.h"
#include "level.h"
#include "asset.h"

namespace Sajiki
{
	/**
		@class LevelEditor
		@brief Editor for creating game levels.
	 */
	class LevelEditor
	{
	public:
		LevelEditor();
		~LevelEditor();
		void Update();
		void setCurrentObj(int oT);
		bool clearLevel(const CEGUI::EventArgs& e);
		bool saveLevel(const CEGUI::EventArgs& e);
		bool loadLevel(const CEGUI::EventArgs& e);
		bool playLevel(const CEGUI::EventArgs& e);
		bool deleteFile(const CEGUI::EventArgs& e);
		bool toggleFileMenu(const CEGUI::EventArgs& e);
		bool toggleHelp(const CEGUI::EventArgs& e);
		bool setSaveName(const CEGUI::EventArgs& e);
		bool quitLevel(const CEGUI::EventArgs& e);
		bool rotateLeft(const CEGUI::EventArgs& e);
		bool resetRotation(const CEGUI::EventArgs& e);
		bool hideSpawnError(const CEGUI::EventArgs& e);
		bool rotateRight(const CEGUI::EventArgs& e);
		int iLightLevel;
	private:
		Ogre::Vector3 mousePoint;
		Ogre::Ray mouseRay;
		Ogre::SceneNode *nCursor, *nBackground;
		Ogre::Entity *mBackground;
		bool bMouseInputEnabled;
		std::pair< bool, Ogre::Real > mouseRes;
		Asset *mAsset;
		int iCurrentObj, iCurrentRot, iIsometricZoom;
		CEGUI::Window* sheet;
		bool bTakeScreenshot;
		Ogre::Entity *entPreview;
		Ogre::SceneNode *snPreview;
	protected:
	};

}

#endif