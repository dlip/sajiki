#ifndef MENULEVELSELECT_H
#define MENULEVELSELECT_H

#include "Menu.h"

namespace Sajiki{
	/**
		@class MenuLevelSelect
		@brief Menu to select levels in the game queue.
	 */
	class MenuLevelSelect: public Menu
	{
	public:
		MenuLevelSelect();
		~MenuLevelSelect();
		void Update(){}
	private:
		bool Close(const CEGUI::EventArgs& e);
		bool Ok(const CEGUI::EventArgs& e);
		bool ClickList(const CEGUI::EventArgs& e);
		bool ClickAdd(const CEGUI::EventArgs& e);
		bool ErrorOk(const CEGUI::EventArgs& e);
		std::string strLevelQueue[6];
		Ogre::SceneNode *snPreview;
		Ogre::Entity *entPreview;
	protected:

	};
}
#endif