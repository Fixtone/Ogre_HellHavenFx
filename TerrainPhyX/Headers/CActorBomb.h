#ifndef CACTORBOMB_H
#define CACTORBOMB_H

//##### OGRE INCLUDES
//#include <Ogre.h>
//##### OGRE INCLUDES

//##### EXTERNALS INCLUDES
//#include "../Externals/NxOgre/sdk/NxOgre.h"
//#include "../Externals/Critter/sdk/Critter.h"
//##### EXTERNALS INCLUDES

//##### GAME INCLUDES
#include "CActor.h"
//##### GAME INCLUDES

class CActorBomb: public CActor
{
	public:

		CActorBomb(Ogre::SceneManager* aSceneManager, Ogre::Entity* entity, Critter::Body* body);
		virtual ~CActorBomb(void);

		void onContact(const NxOgre::ContactPair& pair) override;		
		void OnFXStarted(IHHFXOgre *obj) override;
		void OnFXStopped(IHHFXOgre *obj) override;

		void update(float fElapsedTime);

	private:

		void addParticlesToSceneNode(Ogre::SceneNode* aSceneNode);

		bool						m_ActiveTimer;
		bool						m_AttachedParticleTimerDone;
		float						m_ABTimerToExplode;
		float						m_ABCurrentTimer;
		Ogre::SceneNode*			m_lastSpawnedNode;
		IHHFXOgre*					m_lastSpawnedFx;
};
#endif