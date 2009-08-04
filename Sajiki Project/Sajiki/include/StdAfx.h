#ifndef STDAFX_H
#define STDAFX_H

#include <Ogre.h>
#include <OgreWindowEventUtilities.h>
#include <nxogre.h>

//mem probs without this next one
#include <OgreNoMemoryMacros.h>
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIEditbox.h>
#include <CEGUI/elements/CEGUIEditboxProperties.h>

//regular mem handler
#include <OgreMemoryMacros.h> 

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>

#include <vector>
#include <sstream>
#include <string>


#define MAX_PLAYER_THEMES 9
#include "input.h"
#include "root.h"
#include "sound.h"


#endif