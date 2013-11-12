#ifndef CACTOR_H
#define CACTOR_H

//##### OGRE INCLUDES
#include <Ogre.h>
//##### OGRE INCLUDES

//##### EXTERNALS INCLUDES
#include "../Externals/NxOgre/sdk/NxOgre.h"
#include "../Externals/Critter/sdk/Critter.h"
//##### EXTERNALS INCLUDES

//##### GAME INCLUDES
#include "CPhyXManager.h"
#include "IHHFXPublic.h"
#include "CHHParticlesManager.h"
//##### GAME INCLUDES

//##### BOOST INCLUDES
#include "boost\foreach.hpp"
//##### BOOST INCLUDES

class CActor : public NxOgre::Callback
			 , public IHHFXListener
{
	public:

		CActor();
		CActor(Ogre::SceneManager* aSceneManager, Ogre::Entity* entity,Critter::Body* body);
		virtual ~CActor(void);

		Critter::Body* getBody()const{return m_pBody;}
		Ogre::Entity* getEntity()const{return m_pEntity;}		

		virtual void update(float fElapsedTime);

		//Physics Callback
		virtual void onContact(const NxOgre::ContactPair& pair);

		//Particles Callback		
		virtual void OnFXStarted(IHHFXOgre *obj);
		virtual void OnFXStopped(IHHFXOgre *obj);

	protected:

		std::vector<IHHFXOgre*> mActorHHFXParticles;

		Ogre::SceneManager*		m_pSceneManager;
		Ogre::Entity*			m_pEntity;
		Critter::Body*			m_pBody;				

		bool m_bToBeDeleted;

};
#endif