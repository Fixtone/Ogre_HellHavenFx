#ifndef __CHHPARTICLESMANAGER_h_
#define __CHHPARTICLESMANAGER_h_

//##### OGRE INCLUDES
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>
//##### OGRE INCLUDES

//##### GAME INCLUDES
#include "IHHFXPublic.h"
//##### GAME INCLUDES

//##### STD INCLUDES
#include <vector>
#include <queue>
#include <algorithm>
//##### STD INCLUDES

//##### BOOST INCLUDES
#include <boost\foreach.hpp>
//##### BOOST INCLUDES

//##### DEFINES
#define HHTYPE_NAME "PKFX"
//##### DEFINES

#pragma warning(disable : 4481)	// nonstandard extension used: override specifier 'override'

namespace HellHeaven
{
	// this is the infos the hhfx plugin needs to compute the particles
	class CRendererSubView
	{
	public:
		CRendererSubView()
		:	m_hhfxScene(NULL)
		,	m_viewMatrix(Ogre::Matrix4::IDENTITY)
		,	m_sceneManager(NULL)
		,	m_customMaterial("")
		,	m_usePostFX(false)		
		{}
		~CRendererSubView()
		{}

		inline const Ogre::Matrix4&	viewMatrix() const		{ return m_viewMatrix; }
		inline Ogre::SceneManager&	sceneManager() const	{ assert(m_sceneManager != NULL); return *m_sceneManager; }
		inline const Ogre::String&	customMaterial() const	{ return m_customMaterial; }
		inline bool					usePostFX() const		{ return m_usePostFX; }
		inline IHHFXScene&			hhfxScene() const		{ assert(m_hhfxScene != NULL); return *m_hhfxScene; }

		inline void					setViewMatrix(const Ogre::Matrix4 &view)				{ m_viewMatrix = view; }
		inline void					setSceneManager(Ogre::SceneManager& sceneManager)		{ m_sceneManager = &sceneManager; }
		inline void					setCustomMaterial(const Ogre::String& customMaterial)	{ m_customMaterial = customMaterial; }
		inline void					setUsePostFX(bool usePostFX)							{ m_usePostFX = usePostFX; }
		inline void					setHHFXScene(IHHFXScene& HHFXScene)						{ m_hhfxScene = &HHFXScene; }

	private:

		// the scene that called the render methods
		IHHFXScene*					m_hhfxScene;
		Ogre::Matrix4				m_viewMatrix;
		Ogre::SceneManager*			m_sceneManager;
		Ogre::String				m_customMaterial;
		bool						m_usePostFX;		
	};	
}

	class CHHParticle
	{
		public:

			CHHParticle() 
			: mDestroyPhysix(false){}
			~CHHParticle(){}

			IHHFXOgre* getHHFXParticle(void){return mHHFXParticle;}
			void	   setHHFXParticle(IHHFXOgre* aHHFXParticle){mHHFXParticle = aHHFXParticle;}
			bool	   getDestroyPhysix(void){return mDestroyPhysix;}
			void       setDestroyPhysix(bool aDestroyPhysix){mDestroyPhysix = aDestroyPhysix;}

		private:

			IHHFXOgre* mHHFXParticle;
			bool	   mDestroyPhysix;
	};

	class CHHParticlesManager : public Ogre::Singleton<CHHParticlesManager> 							  
	{
		public:
			
			CHHParticlesManager(Ogre::SceneManager* aSceneManager, Ogre::Camera* aCamera, OgreBites::SdkTrayManager* aTrayMgr);
			~CHHParticlesManager();

			/**	Get|Set Singleton **/
			static CHHParticlesManager& getSingleton(void);
			static CHHParticlesManager* getSingletonPtr(void);

			void initialize();
			IHHFXOgre* addParticlesToChildSceneNode(Ogre::SceneNode* aParentSceneNode, int aEffect, bool aDestructPhysXBody);
			void updateStartFrame(float elapsedTime);
			void update(float elapsedTime);		

			Ogre::String getSelectedPack(){return m_selectedPack;}
			IHHFXScene*	 getIHHFXScene(){return m_hhfxScene;}

			void addHHParticle(IHHFXOgre* aToAdd, bool aDestroyPhysiX);
			bool isHHFXParticlesDestructPhysX(IHHFXOgre* aFx);
			void removeHHFXParticles(IHHFXOgre* aFx);

		private:

			//typedef std::vector<CHHParticle*>	CHHParticleVector;
			//CHHParticleVector					mCHHParticle;
			
			std::vector<CHHParticle*>           mCHHP;

			OgreBites::SdkTrayManager*			mTrayMgr;
			Ogre::SceneManager*					mSceneManager;
			Ogre::Camera*						mCamera;

			// HellHeaven's elements
			IHHFXScene*							m_hhfxScene;
			Ogre::String						m_selectedPack;
			Ogre::int32							m_selectedFx;
			Ogre::SceneNode*					m_lastSpawnedNode;
			IHHFXOgre*							m_lastSpawnedFx;
			Ogre::Vector3						m_lastSpawnedPosition;

			int									mNumParticles;
	};

#endif