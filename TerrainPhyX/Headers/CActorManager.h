#ifndef CCActor_MANAGER
#define CCActor_MANAGER

//##### OGRE INCLUDES
#include <Ogre.h>
//##### OGRE INCLUDES

//##### STD INCLUDES
#include <vector>
#include <queue>
#include <algorithm>
//##### STD INCLUDES

//##### GAME INCLUDES
#include "CPhyXManager.h"
#include "CActor.h"
#include "CActorBomb.h"
//##### GAME INCLUDES

//##### BOOST INCLUDES
#include "boost\assert.hpp"
//##### BOOST INCLUDES

/**
	Manages the creation, deletion and storage CActors
**/
class CActorManager : public Ogre::Singleton<CActorManager>
{
	public:

		CActorManager()
		: m_pSceneManager(0)
		, m_pPhyxScene(0)
		, m_pPhyXRenderSys(0)		
		{
			m_CActors.clear();			
		}

		CActorManager(Ogre::SceneManager* pSceneMan,NxOgre::Scene* pScene,Critter::RenderSystem* pRenderSys);

		virtual ~CActorManager(void);

		/**	Get|Set Singleton **/
		static CActorManager& getSingleton(void);
		static CActorManager* getSingletonPtr(void);

		void init(Ogre::SceneManager* pSceneMan,NxOgre::Scene* pScene,Critter::RenderSystem* pRenderSys);

		CActor* createCActor(Ogre::Entity* ent, NxOgre::BoxDescription boxD, Critter::BodyDescription bodyD,Ogre::Vector3 pos=Ogre::Vector3::ZERO,Ogre::Quaternion quat=Ogre::Quaternion::IDENTITY);
		CActor* createCActorBomb(Ogre::Entity* ent, NxOgre::BoxDescription boxD, Critter::BodyDescription bodyD, Ogre::Vector3 pos, Ogre::Quaternion quat);
		CActor* createCActor(Ogre::Entity* ent,Critter::Body* bod);

		void addCActor(CActor* toAdd);
		void addCActor(CActor* toAdd,const Ogre::String& name);

		CActor* getCActor(CActor* toGet)const;
		CActor* getCActor(Critter::Body* toGet)const;
		CActor* getCActor(const Ogre::String& name)const;
		
		/**
			Removes the CActor
		**/
		bool removeCActor(CActor* toDel);
		
		void update(float fElapsedTime);

		void clearAll();		

	protected:	

		typedef std::vector<CActor*> CActorsList;
		CActorsList m_CActors;

		typedef std::map<Ogre::String,CActor*> NameCActorMap;		
		NameCActorMap m_NamedCActors;		

		inline CActor* getCActorFromBody(Critter::Body* bod);

		NxOgre::Scene* m_pPhyxScene;
		Critter::RenderSystem* m_pPhyXRenderSys;

		Ogre::SceneManager* m_pSceneManager;
		
};
#endif