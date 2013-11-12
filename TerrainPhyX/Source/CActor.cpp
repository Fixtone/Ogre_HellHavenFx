#include "../Headers/CActor.h"

CActor::CActor(Ogre::SceneManager* aSceneManager, Ogre::Entity* entity, Critter::Body* body)
: m_pSceneManager(aSceneManager)
,m_pEntity(entity)
,m_pBody(body)
,m_bToBeDeleted(false)
{
 	m_pBody->setContactCallback(this);
}

CActor::~CActor(void)
{
}

void CActor::update(float fElapsedTime)
{
	//do nothing
}

void CActor::onContact(const NxOgre::ContactPair& pair)
{
	//do nothing
}

void CActor::OnFXStarted(IHHFXOgre* fx)
{
	//do nothing
}

void CActor::OnFXStopped(IHHFXOgre* fx)
{
	//do nothing
}