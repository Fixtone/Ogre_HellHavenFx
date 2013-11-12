#include "../Headers/CTransitionAnimatorManager.h"

// BEGIN SINGLETON
template<> CTransitionAnimatorManager* Ogre::Singleton<CTransitionAnimatorManager>::msSingleton = 0;
CTransitionAnimatorManager* CTransitionAnimatorManager::getSingletonPtr(void)
{
    return msSingleton;
}
CTransitionAnimatorManager& CTransitionAnimatorManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CTransitionAnimatorManager::CTransitionAnimatorManager(Ogre::SceneManager* aSceneManager)
: mCameraTransition(0)
{
	mSceneManager = aSceneManager;      
}

CTransitionAnimatorManager::~CTransitionAnimatorManager()
{
	if(mCameraTransition)
	{
		mSceneManager->destroyAnimationState("CameraTrack");
		mCameraTransition = NULL;
	}
}

void CTransitionAnimatorManager::createCameraTransition(OgreBites::SdkCameraMan* mCameraMan, Ogre::Camera* aCamera, Ogre::Vector3 begin, Ogre::Vector3 end, Ogre::Vector3 lbegin, Ogre::Vector3 lend, float duration)
{
	mCameraMan->setStyle(OgreBites::CS_MANUAL);   // we will be controlling the camera ourselves, so disable the camera man
	Ogre::SceneNode* headNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	headNode->setPosition(Ogre::Vector3(0,500,320));
    aCamera->setAutoTracking(true, headNode);   // make the camera face the head

	// create a camera node and attach camera to it
	Ogre::SceneNode* camNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	camNode->attachObject(aCamera);
		
	// set up a 10 second animation for our camera, using spline interpolation for nice curves
	Ogre::Animation* anim = mSceneManager->createAnimation("CameraTrack", 10);
	anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);

	// create a track to animate the camera's node
	Ogre::NodeAnimationTrack* track = anim->createNodeTrack(0, camNode);

	// create keyframes for our track
	track->createNodeKeyFrame(0)->setTranslate(Ogre::Vector3(2000, 3000, -2000));
	track->createNodeKeyFrame(2.5)->setTranslate(Ogre::Vector3(0, 500, 1000));
	track->createNodeKeyFrame(5)->setTranslate(Ogre::Vector3(-5000, 1000, 2000));
	track->createNodeKeyFrame(7.5)->setTranslate(Ogre::Vector3(0, 2000, -3000));
	track->createNodeKeyFrame(10)->setTranslate(Ogre::Vector3(2000, 3000, -2000));

	// create a new animation state to track this
	mCameraTransition = mSceneManager->createAnimationState("CameraTrack");
	mCameraTransition->setEnabled(true);
}

void CTransitionAnimatorManager::update(const float elapsedSeconds)
{	
	if(mCameraTransition)
		mCameraTransition->addTime(elapsedSeconds);
}