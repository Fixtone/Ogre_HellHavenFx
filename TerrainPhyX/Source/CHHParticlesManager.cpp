#include "../Headers/CHHParticlesManager.h"

// BEGIN SINGLETON
template<> CHHParticlesManager* Ogre::Singleton<CHHParticlesManager>::msSingleton = 0;
CHHParticlesManager* CHHParticlesManager::getSingletonPtr(void)
{
    return msSingleton;
}
CHHParticlesManager& CHHParticlesManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON


CHHParticlesManager::CHHParticlesManager(Ogre::SceneManager* aSceneManager, Ogre::Camera* aCamera, OgreBites::SdkTrayManager* aTrayMgr)
: mNumParticles(0)
{
	mCamera = aCamera;
	mSceneManager = aSceneManager;
	mTrayMgr = aTrayMgr;
}

CHHParticlesManager::~CHHParticlesManager()
{
}

void CHHParticlesManager::initialize()
{
	/////////////////////////////////////////////////////////////////////////////////////////
	// Add the compositor for post fx
	/////////////////////////////////////////////////////////////////////////////////////////

	// Adding compositor for post fx
	/*Ogre::CompositorInstance* comp = Ogre::CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "PopcornOgre/Compositor/Distortion");

	if (!comp)
		Ogre::LogManager::getSingleton().logMessage(Ogre::LML_CRITICAL, "[PKFX ERROR] Cannot load compositor Distortion !");
	else
		comp->setEnabled(true);*/

	/////////////////////////////////////////////////////////////////////////////////////////
	// Hellheaven
	/////////////////////////////////////////////////////////////////////////////////////////
	// Retrieve the HellHeaven's scene from an empty fx. for each Ogre::SceneManager a HHFXScene is associated. Effect's params	
	Ogre::MovableObject	*hhfx = mSceneManager->createMovableObject(HHTYPE_NAME);

	if (hhfx != NULL)
	{
		OgreAssert(dynamic_cast<IHHFXOgre*>(hhfx) != NULL, "Object should be of type \"PKFX\"");
		IHHFXOgre	*hhFx = static_cast<IHHFXOgre*>(hhfx);
		m_hhfxScene = &hhFx->GetHHFXScene();
		
		// we got the hh scene, destroy the effect
		mSceneManager->destroyMovableObject(hhfx);
	}
	OgreAssert(m_hhfxScene != NULL, "failed creating PKFx Scene !");
	m_hhfxScene->SetWorldScale(1.0f);

	// load a pack
	m_selectedPack = "Resources/assets/particles/HHParticles/particlesPacks/DesertPKFxPack";
	bool hhfxPackLoaded = m_hhfxScene->GetHHFXBase().LoadPack(m_selectedPack.c_str(), true);
	OgreAssert(hhfxPackLoaded, "pkfx pack did not load correctly or contains no effects !");
}

IHHFXOgre* CHHParticlesManager::addParticlesToChildSceneNode(Ogre::SceneNode* aParentSceneNode, int aEffect, bool aDestructPhysXBody)
{	
	// effect's params
	Ogre::NameValuePairList params;

	params["pack"] = m_selectedPack.c_str();
	params["fx"] = m_hhfxScene->GetHHFXBase().GetHHFXPackExplorer().GetEffects()[aEffect];
	params["run"] = "yes";

	// spawn a new effect at this location
	Ogre::MovableObject	*fx = mSceneManager->createMovableObject(HHTYPE_NAME, &params);

	IHHFXOgre* hhfx = NULL;
	if (fx != NULL)
	{
		hhfx = static_cast<IHHFXOgre*>(fx);		
		addHHParticle(hhfx, aDestructPhysXBody);
		mNumParticles++;

		// Here, check if the effect is still active after we've run it.
		// It might not be active anymore for fire-and-forget effects, such as an instantaneous explosion.
		// For such effects, don't bother creating a parent scene node.
		if (hhfx->IsFXActive())
		{
			Ogre::SceneNode* childSceneNode = aParentSceneNode->createChildSceneNode("ChildNodeParticlesFX_" + Ogre::StringConverter::toString(mNumParticles));
			childSceneNode->attachObject(hhfx);					

			// set the camera's orientation to the node
			Ogre::Quaternion orientation;
			Ogre::Quaternion camOri = mCamera->getOrientation();

			orientation.FromAngleAxis(Ogre::Radian(camOri.getYaw()), Ogre::Vector3::UNIT_Y);
			childSceneNode->setOrientation(orientation);			
		}
	}	

	return hhfx;
}

void CHHParticlesManager::addHHParticle(IHHFXOgre* aToAdd, bool aDestroyPhysiX)
{
	CHHParticle* cHHParticle = new CHHParticle();
	cHHParticle->setDestroyPhysix(aDestroyPhysiX);
	cHHParticle->setHHFXParticle(aToAdd);

	mCHHP.push_back(cHHParticle);
}

void CHHParticlesManager::updateStartFrame(float elapsedTime)
{	
	// update the hhfx scene
	m_hhfxScene->Update(elapsedTime);
}

void CHHParticlesManager::update(float elapsedTime)
{
	// setting render params for the particles renderers
	HellHeaven::CRendererSubView view;

	Ogre::Matrix4 worldTransforms;

	const Ogre::Vector3& camPos = mCamera->getPosition();
	const Ogre::Quaternion& camOri = mCamera->getOrientation();

	worldTransforms.makeTransform(camPos, Ogre::Vector3::UNIT_SCALE, camOri);
	worldTransforms = worldTransforms.transpose();

	view.setHHFXScene(*m_hhfxScene);
	view.setViewMatrix(worldTransforms);
	view.setSceneManager(*mSceneManager);

	m_hhfxScene->Render(view, camPos);
}

bool CHHParticlesManager::isHHFXParticlesDestructPhysX(IHHFXOgre* aFx)
{
	bool find = false;

	Ogre::String str = aFx->getName();
	BOOST_FOREACH(CHHParticle* particle, mCHHP)
	{		
		Ogre::String strComp = particle->getHHFXParticle()->getName();
		if(!str.compare(strComp))
		{
			if(particle->getDestroyPhysix())
			{
				find = true;				
				break;
			}
			else
				find = false;				
		}
	}		
	
	if(find)
		return true;
	else
		return false;
}

void CHHParticlesManager::removeHHFXParticles(IHHFXOgre* aFx)
{
	Ogre::String str = aFx->getName();
	unsigned int cont = 0;
	BOOST_FOREACH(CHHParticle* particle, mCHHP)
	{		
		Ogre::String strComp = particle->getHHFXParticle()->getName();
		if(!str.compare(strComp))
		{
			break;
		}
		cont++;
	}

	mCHHP.erase(mCHHP.begin()+cont);
}