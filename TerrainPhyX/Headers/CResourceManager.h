#ifndef __CRESOURCEMANAGER_h_
#define __CRESOURCEMANAGER_h_

//##### OGRE INCLUDES
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkTrays.h>
//##### OGRE INCLUDES

class CResourceManager : public Ogre::Singleton<CResourceManager> 
{
	public:
	
		CResourceManager();
		~CResourceManager();
	
		/**	Get|Set Singleton **/
		static CResourceManager& getSingleton(void);
		static CResourceManager* getSingletonPtr(void);

		void initialize(Ogre::SceneManager* aSceneManager);
		void loadPlayStateResources(OgreBites::SdkTrayManager* aTrayMgr);

	private:

		Ogre::SceneManager*			mSceneManager;		
};

#endif