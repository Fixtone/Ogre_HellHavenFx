#ifndef TRANSITIONNAMIMATORSMANAGER_H
#define TRANSITIONNAMIMATORSMANAGER_H

//##### OGRE INCLUDES
#include <Ogre.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>
//##### OGRE INCLUDES

class CTransitionAnimatorManager : public Ogre::Singleton<CTransitionAnimatorManager>
{
	public:

		CTransitionAnimatorManager(Ogre::SceneManager* aSceneManager);
		~CTransitionAnimatorManager();

		/**	Get|Set Singleton **/
		static CTransitionAnimatorManager& getSingleton(void);
		static CTransitionAnimatorManager* getSingletonPtr(void);

		void createCameraTransition(OgreBites::SdkCameraMan* mCameraMan, Ogre::Camera* aCamera, Ogre::Vector3 begin, Ogre::Vector3 end, Ogre::Vector3 lbegin, Ogre::Vector3 lend, float duration);
		
		/** Update internal stuff - PROCESS */
		void update(const float elapsedSeconds);

	private:

		Ogre::SceneManager*		mSceneManager;
		Ogre::AnimationState*	mCameraTransition;
};

#endif