#ifndef MENU_H
#define MENU_H

#include "interface.h"

namespace Sajiki{
	/**
		@class Menu
		@brief Virtual class for other menus to inherit from.
	 */
	class Menu
	{
	public:
		virtual ~Menu()
		{
			if(sheet)
			{
				sheet->destroy();
				AInterface->getSystem()->setGUISheet(0);
				AInterface->getSystem()->getRenderer()->removeAllEvents();

			}
		}
		virtual void Update() = 0;
	private:
		
		
	protected:
		CEGUI::Window* sheet;
	};

}
#endif