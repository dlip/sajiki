#ifndef INPUT_H
#define INPUT_H

#include "singleton.h"

namespace Sajiki
{
	/**
		@class Input
		@brief Controls input from the keyboard, mouse and joystick.
	 */
	class Input: public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener, public Sajiki::Singleton<Input>
	{
		friend Singleton;
	public:
		void Init(Ogre::RenderWindow *mWindow);
		void Update();
		bool keyPressed( const OIS::KeyEvent &e );
		bool keyReleased( const OIS::KeyEvent &e );
		bool isKey(OIS::KeyCode kc);
		bool isKeySingle(OIS::KeyCode kc);

		// MouseListener
		bool mouseMoved(const OIS::MouseEvent &e);
		bool isMouseButton(int button);
		bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
		Ogre::Vector2 getMousePos();
		Ogre::Vector2 getMousePosRel();
		int getMouseWheel();
		//JoystickListener
		bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
		bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
		bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

		bool keys[OIS::KC_LAST];
	private:
		OIS::InputManager *mInputSystem;
		OIS::Keyboard     *mKeyboard;
		OIS::Mouse     *mMouse;
		std::vector<OIS::JoyStick*> mJoysticks;
		std::vector<OIS::JoyStick*>::iterator mJoysticksIter;
		int iMouseX, iMouseY, iMouseXrel, iMouseYrel, iMouseWheel;
		bool bMouse1, bMouse2, bMouse3;
		int iNumJoysticks;
	protected:
		Input(){};
	};

	#define AInput Input::Instance()
}
#endif