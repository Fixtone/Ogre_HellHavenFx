#include "../Headers/CActorBomb.h"

CActorBomb::CActorBomb(Ogre::SceneManager* aSceneManager, Ogre::Entity* entity, Critter::Body* body)
: CActor(aSceneManager, entity, body)
, m_ActiveTimer(false)
, m_AttachedParticleTimerDone(false)
, m_ABTimerToExplode(Ogre::Math::RangeRandom(3.0f,6.0f))
, m_ABCurrentTimer(0)
{	
	body->setContactCallback(this);

	IHHFXOgre* fx = CHHParticlesManager::getSingletonPtr()->addParticlesToChildSceneNode(this->getBody()->getSceneNode(),7,false);
	BOOST_ASSERT(fx!=NULL);
	fx->SetFXListener(this);	
}

CActorBomb::~CActorBomb(void)
{
}

void CActorBomb::update(float fElapsedTime)
{	
	if(m_ActiveTimer)
	{
		m_ABCurrentTimer += fElapsedTime;
		if(m_ABCurrentTimer>m_ABTimerToExplode)
		{
			IHHFXOgre* fx = CHHParticlesManager::getSingletonPtr()->addParticlesToChildSceneNode(this->getBody()->getSceneNode(),0,true);
			BOOST_ASSERT(fx!=NULL);
			fx->SetFXListener(this);			
			m_ActiveTimer = false;	
			m_AttachedParticleTimerDone = true;
		}
	}
}

void CActorBomb::onContact(const NxOgre::ContactPair& pair)
{
	if(!m_ActiveTimer && !m_AttachedParticleTimerDone)
		m_ActiveTimer = true;	
}

void CActorBomb::OnFXStarted(IHHFXOgre* fx)
{
	//const	Ogre::Vector3& fxPosition = fx->getParentSceneNode()->getPosition();
	/*
	// create a light under the ElectricOrb effect
	if (strstr(fx->GetPath(), "ElectricOrb.hfx") != NULL)
	{
		Light	*pointLight = mSceneMgr->createLight("pointLightEO" + StringConverter::toString((unsigned int)(fx)));
		pointLight->setType(Light::LT_POINT);
		pointLight->setPosition(fxPosition + Vector3::UNIT_Y * 0.8f);
		pointLight->setDiffuseColour(0.1f, 0.1f, 1.0f);
		pointLight->setSpecularColour(0.8f, 0.8f, 1.0f);
	}*/
}

void CActorBomb::OnFXStopped(IHHFXOgre* fx)
{	
	bool destroyPhysX = CHHParticlesManager::getSingletonPtr()->isHHFXParticlesDestructPhysX(fx);
	CHHParticlesManager::getSingletonPtr()->removeHHFXParticles(fx);

	//Destroy the created node's effect parent and the effect
	Ogre::SceneNode	*parentNode = fx->getParentSceneNode();
	if (parentNode != NULL)
		m_pSceneManager->destroySceneNode(parentNode);

	m_pSceneManager->destroyMovableObject(fx);	
		
	if(destroyPhysX)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("Body Name:" + this->getBody()->getName());
		CPhysicsManager::getSingletonPtr()->getRenderSystem()->destroyBody(this->getBody());
	}
	
}