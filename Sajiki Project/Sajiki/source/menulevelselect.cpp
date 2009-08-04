#include "stdafx.h"
#include "menulevelselect.h"

namespace Sajiki
{

	MenuLevelSelect::MenuLevelSelect()
	{

		sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"sajikiplay.layout"); 
	
		AInterface->getSystem()->setGUISheet(sheet);
		CEGUI::MouseCursor::getSingleton().show();
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/btnCancel")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&MenuLevelSelect::Close, this));

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/btnOk")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&MenuLevelSelect::Ok, this));

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/lstLevels")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&MenuLevelSelect::ClickList, this));

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/btnErrorOk")->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
			CEGUI::Event::Subscriber(&MenuLevelSelect::ErrorOk, this));

		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/txtErrorLevelQueue")->hide();

		for(int x=1; x<7; x++)
		{
			std::stringstream sstream;
			sstream<<"Play/btn"<<x;
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)sstream.str().c_str())->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp, 
				CEGUI::Event::Subscriber(&MenuLevelSelect::ClickAdd, this));
		}

		ALevel->getLevelList("Play/lstLevels");
		
		CEGUI::Listbox* mylst = (CEGUI::Listbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/lstLevels");
		for (int x = 0; x<6; x++)
		{
			if(x>(int)mylst->getItemCount()-1)
				return;
			std::stringstream sstream;
			sstream<<"Play/txtLevel"<<x+1;

			strLevelQueue[x]= mylst->getListboxItemFromIndex(x)->getText().c_str();
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)sstream.str().c_str())->setText(strLevelQueue[x]);
		}

		snPreview = ARoot->getScene()->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(16,-1,0));
		entPreview = ARoot->getScene()->createEntity("PreviewPlane", "levelpreview.mesh");
		snPreview->attachObject(entPreview);
		snPreview->setScale(0.2,0.2,0.2);
		entPreview->setMaterialName("Cake");
	}

	MenuLevelSelect::~MenuLevelSelect()
	{
		snPreview->removeAndDestroyAllChildren();
		ARoot->getScene()->destroyEntity(entPreview);
	}
	bool MenuLevelSelect::ClickAdd(const CEGUI::EventArgs& e)
	{
		const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(e);
		int no = atoi(&we.window->getName().c_str()[8]);

		CEGUI::String strLevel = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/txtSelectedLevel")->getText();
		std::stringstream sstream;
		sstream<<"Play/txtLevel"<<no;
		if(strLevel != "")
		{
			strLevelQueue[no-1]=strLevel.c_str();
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)sstream.str().c_str())->setText(strLevel);
		}

		return true;
	}

	bool MenuLevelSelect::ClickList(const CEGUI::EventArgs& e)
	{

		CEGUI::Listbox* mylst = (CEGUI::Listbox*)CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/lstLevels");
		if(mylst->getSelectedCount())
			CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/txtSelectedLevel")->setText(mylst->getFirstSelectedItem()->getText());

		entPreview->setMaterialName(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/txtSelectedLevel")->getText().c_str());

		return true;
	}

	bool MenuLevelSelect::ErrorOk(const CEGUI::EventArgs& e)
	{
		CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/txtErrorLevelQueue")->hide();
		return true;
	}

	bool MenuLevelSelect::Ok(const CEGUI::EventArgs& e)
	{
		for (int x=0; x<6; x++)
		{
			if(strLevelQueue[x]=="")
			{
				CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Play/txtErrorLevelQueue")->show();
				return false;
			}
		}
		AInterface->setLevelQueue(strLevelQueue);
		ALevel->setLevelMode(LM_PLAY);
		return true;
	}

	bool MenuLevelSelect::Close(const CEGUI::EventArgs& e)
	{
		AInterface->setMenu(MENU_MAINMENU);
		return true;
	}


}

