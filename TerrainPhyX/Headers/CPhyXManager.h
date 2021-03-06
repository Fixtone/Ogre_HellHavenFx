#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

//##### OGRE INCLUDES
#include <Ogre.h>
//##### OGRE INCLUDES

//##### EXTERNALS INCLUDES
#include "../Externals/NxOgre/sdk/NxOgre.h"
#include "../Externals/Critter/sdk/Critter.h"
//##### EXTERNALS INCLUDES

//##### BOOST INCLUDES
#include <boost\foreach.hpp>
//##### BOOST INCLUDES

//##### STD INCLUDES
#include <vector>
//##### STD INCLUDES

//##### GAME INCLUDES
#include "CWorldManager.h"
#include "CHHParticlesManager.h"
//##### # GAME INCLUDES INCLUDES

class CPhysicsManager : public Ogre::Singleton<CPhysicsManager>, public NxOgre::Callback
{
	public:

		CPhysicsManager(Ogre::SceneManager* aSceneManager);
		~CPhysicsManager();

		/**	Get|Set Singleton **/
		static CPhysicsManager& getSingleton(void);
		static CPhysicsManager* getSingletonPtr(void);

		/* Destroys the physics world. Needs to be called when a game level quits. */
		void destroyPhysicsWorld();		

		/* Returns Critter RenderSysten*/
		Critter::RenderSystem* getRenderSystem(){return mRenderSystem;}

		/* Returns the NxOgre scene. */
		NxOgre::Scene* getPhysicsScene(){return mPhysicsScene;}

		/* Returns the NxOgre world. */
		NxOgre::World* getPhysicsWorld(){return mPhysicsWorld;}

		/* Pauses all physics simulations. Called when pausing the game level (and entering Menu State). */
		void pause();

		/* See pause(). Resumes all simulations. */
		void resume();

		/* Updates all the Physics in the game. Needs to be called every frame in the Game Loop. */
		void update(const double);

		Critter::Body* makeBox(const NxOgre::Matrix44& globalPose, const NxOgre::Vec3& initialVelocity, NxOgre::GroupIdentifier group = 0);

		void destroyPhysicsBodies();

		virtual void onContact(const NxOgre::ContactPair& pair);
		virtual void onVolumeEvent(NxOgre::Volume* volume, NxOgre::Shape* volumeShape, NxOgre::RigidBody* collisionBody, NxOgre::Shape* rigidBodyShape, unsigned int collisionEvent);
		void CPhysicsManager::setDebugPhysX(bool aActive);
		//virtual void onContactNotify(NxContactPair& pair, NxOgre::NxU32 events);

		NxOgre::Vec3 convertOgreVec3(Ogre::Vector3 vec)
		{
			NxOgre::Vec3 v;
			v.x=vec.x;
			v.y=vec.y;
			v.z=vec.z;

			return v;
		}

		NxOgre::Quat convertOgreQuat(Ogre::Quaternion qu)
		{
			NxOgre::Quat q;
			q.w=qu.w;
			q.x=qu.x;
			q.y=qu.y;
			q.z=qu.z;

			return q;
		}

		Ogre::Quaternion convertNxQuat(NxOgre::Quat qu)
		{
			Ogre::Quaternion q;
			q.w=qu.w;
			q.x=qu.x;
			q.y=qu.y;
			q.z=qu.z;

			return q;
		}

		Ogre::Vector3 convertNxVec3(NxOgre::Vec3 v)
		{
			return Ogre::Vector3(v.x,v.y,v.z);
		}

	private:

		Critter::RenderSystem*		mRenderSystem;
		NxOgre::World*				mPhysicsWorld;
		NxOgre::Scene*				mPhysicsScene;
		NxOgre::Material*			mDefaultMaterial;
		Ogre::SceneManager*			mSceneMgr;
		int							mEntityCounter ;

		/* Adds ".mesh" at the end of "filename". */
		Ogre::String OgreFilename(const char* filename);

		/* Adds nxogre: at the start of the "filename" char array and ".nxs" at the end. */
		Ogre::String PhysxFilename(const char* filename);

		std::vector<Critter::Body*>	mBoxVector;
		
};

#endif
