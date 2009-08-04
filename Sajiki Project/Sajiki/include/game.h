#ifndef GAME_H
#define GAME_H
#include "interface.h"

namespace Sajiki
{
	/**
		@class Game
		@brief Main game loop.
	 */
	class Game : public Ogre::FrameListener, public Ogre::WindowEventListener
	{
	public:
		
		Game();
		~Game();
		bool frameStarted (const Ogre::FrameEvent &evt);
		bool frameEnded (const Ogre::FrameEvent &evt);
		void windowClosed(Ogre::RenderWindow* rw);
		void Run();
	private:
	protected:
		bool bGameStarted;
	};
}

#endif