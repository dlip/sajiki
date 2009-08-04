#ifndef BACKGROUND_H
#define BACKGROUND_H

namespace Sajiki
{
	/**
		@class Background
		@brief In-game background with animations and particles.
	 */
	class Background
	{
	public:
		Background(int pTheme);
		~Background();
		void Update();
	private:
		Ogre::SceneNode *nBackground, *nBackground2, *nBackground3, *nBackground4;
		Ogre::Entity *mBackground, *mBackground2, *mBackground3, *mBackground4;
		Ogre::AnimationState *mAnimationState, *mAnimationState2, *mAnimationState3;
		Ogre::ParticleSystem *mParticleSystem[15];
		Ogre::SceneNode *mParticleNode[5];
		int iTheme;
	protected:

	};
}

#endif