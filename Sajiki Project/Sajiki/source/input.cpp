#include "stdafx.h"
#include "input.h"

namespace Sajiki
{
	void Input::Init(Ogre::RenderWindow *mWindow)
	{
		OIS::ParamList paramList;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		mWindow->getCustomAttribute( "WINDOW", &windowHnd );

		windowHndStr << (unsigned int) windowHnd;
		paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

		mInputSystem = OIS::InputManager::createInputSystem( paramList );

		mKeyboard = static_cast<OIS::Keyboard*>( mInputSystem->createInputObject( OIS::OISKeyboard, true ) );
		mKeyboard->setEventCallback( this );
		iMouseX = iMouseY = iMouseXrel = iMouseYrel = iMouseWheel = 0;
		bMouse1 = bMouse2 =  bMouse3 = false;
		mMouse = static_cast<OIS::Mouse*>( mInputSystem->createInputObject( OIS::OISMouse, true ) );
		mMouse->setEventCallback(this);
		for (int x=0; x<256; x++)
			keys[x]=0;

		iNumJoysticks = mInputSystem->numJoySticks();

		if (mInputSystem->numJoySticks() > 0) 
		{
			mJoysticks.resize( mInputSystem->numJoySticks() );
			for (mJoysticksIter=mJoysticks.begin(); mJoysticksIter!=mJoysticks.end(); mJoysticksIter++)
			{
				(*mJoysticksIter) = static_cast<OIS::JoyStick*>( mInputSystem->createInputObject( OIS::OISJoyStick, true ) );
				(*mJoysticksIter)->setEventCallback( this );
			}
		}
	}

	int Input::getMouseWheel()
	{
		return iMouseWheel;
	}

	void Input::Update()
	{
		for (mJoysticksIter=mJoysticks.begin(); mJoysticksIter!=mJoysticks.end(); mJoysticksIter++)
		{
			(*mJoysticksIter)->capture();
		}
		for (int x=0; x<256; x++)
			keys[x]=0;
		mKeyboard->capture();
		iMouseXrel = iMouseYrel = iMouseWheel = 0;
		mMouse->capture();
	}

	bool Input::keyPressed( const OIS::KeyEvent &e ) 
	{
		CEGUI::System::getSingleton().injectKeyDown( e.key ); 
		CEGUI::System::getSingleton().injectChar(e.text);
		keys[e.key]=true;
		return true;
	}

	bool Input::keyReleased( const OIS::KeyEvent &e ) 
	{
		return true;
	}

	bool Input::isKey(OIS::KeyCode kc)
	{
		if (kc >= OIS::KC_JOY1START)
		{

			int iJoystickNo = 256;
			if (kc >= OIS::KC_JOY1START && kc < OIS::KC_JOY2START)
				iJoystickNo = 0;
			else if (kc >= OIS::KC_JOY2START && kc < OIS::KC_JOY3START)
				iJoystickNo = 1;
			else if (kc >= OIS::KC_JOY3START && kc < OIS::KC_JOY4START)
				iJoystickNo = 2;
			else if (kc >= OIS::KC_JOY4START && kc < OIS::KC_JOY5START)
				iJoystickNo = 3;

			if (iJoystickNo+1 > iNumJoysticks)
				return false;
			else
			{
				if (kc==OIS::KC_JOY1LEFT || kc==OIS::KC_JOY2LEFT || kc==OIS::KC_JOY3LEFT || kc==OIS::KC_JOY4LEFT)
				{
					if (mJoysticks[iJoystickNo]->getJoyStickState().mAxes[1].abs<-10000)
						return true;
				}
				else if (kc==OIS::KC_JOY1RIGHT || kc==OIS::KC_JOY2RIGHT || kc==OIS::KC_JOY3RIGHT || kc==OIS::KC_JOY4RIGHT)
				{
					if (mJoysticks[iJoystickNo]->getJoyStickState().mAxes[1].abs>10000)
						return true;
				}
				else if (kc==OIS::KC_JOY1UP || kc==OIS::KC_JOY2UP || kc==OIS::KC_JOY3UP || kc==OIS::KC_JOY4UP)
				{
					if (mJoysticks[iJoystickNo]->getJoyStickState().mAxes[0].abs<-10000)
						return true;
				}
				else if (kc==OIS::KC_JOY1DOWN || kc==OIS::KC_JOY2DOWN || kc==OIS::KC_JOY3DOWN || kc==OIS::KC_JOY4DOWN)
				{
					if (mJoysticks[iJoystickNo]->getJoyStickState().mAxes[0].abs>10000)
						return true;
				}
				else if (kc==OIS::KC_JOY1B1 || kc==OIS::KC_JOY2B1 || kc==OIS::KC_JOY3B1 || kc==OIS::KC_JOY4B1)
				{
					int b = mJoysticks[iJoystickNo]->getJoyStickState().buttons;
					if (mJoysticks[iJoystickNo]->getJoyStickState().buttons==1)
						return true;
				}
				else if (kc==OIS::KC_JOY1B2 || kc==OIS::KC_JOY2B2 || kc==OIS::KC_JOY3B2 || kc==OIS::KC_JOY4B2)
				{
					int b = mJoysticks[iJoystickNo]->getJoyStickState().buttons;
					if (mJoysticks[iJoystickNo]->getJoyStickState().buttons==2)
						return true;
					if (mJoysticks[iJoystickNo]->getJoyStickState().mAxes[0].abs<-10000)
						return true;
				}

				return false;
			}
		}
		else
			return mKeyboard->isKeyDown(kc);
	}

	bool Input::isKeySingle(OIS::KeyCode kc)
	{
		return keys[kc];
	}

	bool Input::buttonPressed( const OIS::JoyStickEvent &arg, int button )
	{
		int joyno = arg.device->getID();

		return true;
	}
	bool Input::buttonReleased( const OIS::JoyStickEvent &arg, int button )
	{
		
		return true;
	}

	bool Input::axisMoved( const OIS::JoyStickEvent &arg, int axis )
	{
		return true;
	}

	bool Input::mouseMoved(const OIS::MouseEvent &e) 
	{
		iMouseX = e.state.X.abs;
		iMouseY = e.state.Y.abs;
		iMouseWheel = e.state.Z.rel;
		iMouseXrel = e.state.X.rel;
		iMouseYrel = e.state.Y.rel;
		CEGUI::System::getSingleton().injectMouseMove(iMouseXrel,iMouseYrel);
		return true; 
	}

	bool Input::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
	{ 
		bMouse1 = e.state.buttonDown(OIS::MouseButtonID(OIS::MB_Left));
		bMouse2 = e.state.buttonDown(OIS::MouseButtonID(OIS::MB_Right));
		bMouse3 = e.state.buttonDown(OIS::MouseButtonID(OIS::MB_Middle));

		if(bMouse1)
			CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
		return true; 
	}

	bool Input::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id) 
	{ 
		bMouse1 = e.state.buttonDown(OIS::MouseButtonID(OIS::MB_Left));
		bMouse2 = e.state.buttonDown(OIS::MouseButtonID(OIS::MB_Right));
		bMouse3 = e.state.buttonDown(OIS::MouseButtonID(OIS::MB_Middle));

		if(!bMouse1)
			CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
		return true; 
	}

	Ogre::Vector2 Input::getMousePos()
	{
		return Ogre::Vector2(iMouseX, iMouseY);
	}

	Ogre::Vector2 Input::getMousePosRel()
	{
		return Ogre::Vector2(iMouseXrel, iMouseYrel);
	}

	bool Input::isMouseButton(int button)
	{
		switch(button)
		{
		case 1:
			return bMouse1;
		case 2:
			return bMouse2;
		case 3:
			return bMouse3;
		default:
			return false;
		}
	}
}