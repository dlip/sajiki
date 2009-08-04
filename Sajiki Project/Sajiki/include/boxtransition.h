#ifndef BOXTRANSITION_H
#define BOXTRANSITION_H

namespace Sajiki{
	/**
		@class BoxTransition
		@brief Transitions the levels in-game from one side of the cube to the next.
	 */
	class BoxTransition
	{
	public:
		BoxTransition();
		~BoxTransition();
		bool Update();
	private:
		Ogre::SceneNode *snCentre, *snFront, *snLeft, *snRight, *snBack, *snTop, *snBottom;
		Ogre::Entity *entFront, *entLeft, *entRight, *entBack, *entTop, *entBottom;
	protected:
		float fCameraYaw;
		float fCameraZoom;
		bool bCompleteZoom, bCompleteTurn, bStartTransition;
		Ogre::Timer mZoomTimer, mTurnTimer;

		
	};

}
#endif