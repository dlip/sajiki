#ifndef INTERFACE_H
#define INTERFACE_H
#include "singleton.h"
#include "interface.h"
#include "boxtransition.h"

namespace Sajiki{

	class Menu;
	class MenuCharacterSelect;
	class MenuCredits;
	class MenuHelp;
	class MenuMain;
	class MenuOptions;
	class MenuLevelSelect;

	typedef enum
	{
		PC_KB1,
		PC_KB2,
		PC_KB3,
		PC_KB4,
		PC_JOY1,
		PC_JOY2,
		PC_JOY3,
		PC_JOY4,
	} ePlayerControls;

	enum eMenu
	{
		MENU_NONE,
		MENU_CHARSEL,
		MENU_LEVELEDITOR,
		MENU_OPTIONS,
		MENU_CREDITS,
		MENU_HELP,
		MENU_EXIT,
		MENU_LEVELSELECT,
		MENU_MAINMENU,
	};

	typedef struct
	{
		OIS::KeyCode kUp, kDown, kLeft, kRight, kAttack;
	}sControllerLayout;

	typedef struct
	{
		int iColour;
		sControllerLayout ctrl;
		ePlayerControls pc;
		bool bIsMale;
	}sPlayerSettings;

	/**
		@class Interface
		@brief Controls main menu logic, initialises CEGUI and stores player controls.
	 */
	class Interface: public Singleton<Interface>
	{
	friend Singleton;
	public:
		void Init();
		void setMenu(eMenu eMnu);
		sPlayerSettings* getPlayerSettings(int no){return mPlayerSettings[no];}
		void setLevelQueue(std::string lvlqueue[6]);
		std::string getLevelQueueName(int no){return strLevelQueue[no];}
		int getPlayerCount() {return iPlayers;}
		CEGUI::System* getSystem(){ return mSystem; }
		CEGUI::OgreCEGUIRenderer* getRenderer(){return mRenderer;}

		bool addPlayerControls(ePlayerControls pc);
		void destroyPlayerSettings();
		void Update();
		void CreateLevelPreviews();
		std::vector<std::string *>* getvecLevelList(){return &vecLevelList;}
		int iLastMenu;
		void showCodePlane(bool b);
		int iLastOption;
	private:
		Ogre::SceneNode *sn;
		Ogre::Entity *mEntity;
		Menu *mCurrentMenu;
		sPlayerSettings *mPlayerSettings[4];
		int iPlayers;
		std::string strLevelQueue[6];
		std::vector<std::string *> vecLevelList;
		
		CEGUI::System *mSystem;
		CEGUI::OgreCEGUIRenderer *mRenderer;
	protected:
		Interface(){};
	};
	#define AInterface Interface::Instance()
}
#endif