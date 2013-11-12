#include "../Headers/CActorManager.h"

// BEGIN SINGLETON
template<> CActorManager* Ogre::Singleton<CActorManager>::msSingleton = 0;
CActorManager* CActorManager::getSingletonPtr(void)
{
    return msSingleton;
}
CActorManager& CActorManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CActorManager::CActorManager(Ogre::SceneManager* pSceneMan, NxOgre::Scene* pScene, Critter::RenderSystem* pRenderSys)
: m_pSceneManager(pSceneMan)
, m_pPhyxScene(pScene)
, m_pPhyXRenderSys(pRenderSys)
{
	m_CActors.clear();	
}

CActorManager::~CActorManager(void)
{
	clearAll();
}

void CActorManager::init(Ogre::SceneManager* pSceneMan, NxOgre::Scene* pScene, Critter::RenderSystem* pRenderSys)
{
	m_pSceneManager=pSceneMan;
	m_pPhyxScene=pScene;
	m_pPhyXRenderSys=pRenderSys;
}

void CActorManager::clearAll()
{
	if(!m_CActors.empty())
	{
		CActorsList::iterator iter=m_CActors.begin();

		for(;iter!=m_CActors.end();iter++)
		{
			if(*iter)
			{
				delete *iter;
			}
		}
	}

	m_CActors.clear();

	m_NamedCActors.clear();
}

void CActorManager::addCActor(CActor* toAdd)
{
	m_CActors.push_back(toAdd);
}

CActor* CActorManager::getCActor(CActor* toGet)const
{
	CActorsList::const_iterator iter=std::find(m_CActors.begin(),m_CActors.end(),toGet);
	if(iter!=m_CActors.end())
	{
		return *iter;
	}else
		return 0;
}

CActor* CActorManager::getCActor(Critter::Body* toGet)const
{
	CActorsList::const_iterator iter;
	for(iter = m_CActors.begin(); iter!= m_CActors.end(); iter++)
	{
		if((*iter)->getBody()->getNameHash() == toGet->getNameHash())
		{
			return *iter;
		}
	}
	return 0;
}

CActor* CActorManager::getCActor(const Ogre::String& name)const
{
	return m_NamedCActors.at(name);
}

CActor* CActorManager::createCActor(Ogre::Entity* ent,Critter::Body* bod)
{
	CActor* act=new CActor(m_pSceneManager, ent, bod);
	addCActor(act);
	return act;
}

CActor* CActorManager::createCActor(Ogre::Entity* ent, NxOgre::BoxDescription boxD, Critter::BodyDescription bodyD, Ogre::Vector3 pos, Ogre::Quaternion quat)
{
	NxOgre::Matrix44 pose;
	pose.set(CPhysicsManager::getSingletonPtr()->convertOgreQuat(quat));
	pose.set(CPhysicsManager::getSingletonPtr()->convertOgreVec3(pos));

	Critter::Body* body = m_pPhyXRenderSys->createBody(boxD,pose,ent->getMesh()->getName(), bodyD);	
	CPhysicsManager::getSingletonPtr()->getPhysicsScene()->setActorFlags(body, CWorldManager::getSingletonPtr()->getTerrainSceneGeometry() , NxOgre::Enums::ContactPairFlags_All);

	return createCActor(ent,body);
}


CActor* CActorManager::createCActorBomb(Ogre::Entity* ent, NxOgre::BoxDescription boxD, Critter::BodyDescription bodyD, Ogre::Vector3 pos, Ogre::Quaternion quat)
{	
	NxOgre::Matrix44 pose;
	pose.set(CPhysicsManager::getSingletonPtr()->convertOgreQuat(quat));
	pose.set(CPhysicsManager::getSingletonPtr()->convertOgreVec3(pos));

	bodyD.mName = "Box_" + Ogre::StringConverter::toString(m_CActors.size());

	Critter::Body* body = m_pPhyXRenderSys->createBody(boxD, pose, ent->getMesh()->getName(), bodyD);	
	CPhysicsManager::getSingletonPtr()->getPhysicsScene()->setActorFlags(body, CWorldManager::getSingletonPtr()->getTerrainSceneGeometry() , NxOgre::Enums::ContactPairFlags_All);

	CActorBomb* cActBomb = new CActorBomb(m_pSceneManager, ent, body);
	addCActor(cActBomb);	

	return cActBomb;
}


bool CActorManager::removeCActor(CActor* toDel)
{
	CActorsList::iterator iter=std::find(m_CActors.begin(),m_CActors.end(),toDel);
	if(iter!=m_CActors.end())
	{
		if(*iter)
		{
			m_pPhyXRenderSys->destroyBody((*iter)->getBody());
			delete *iter;
		}
		m_CActors.erase(iter);
		return true;
	}

	return false;
}

void CActorManager::update(float fElapsedTime)
{
	CActorsList::iterator iter;
	for(iter=m_CActors.begin();iter!=m_CActors.end();iter++)
		(*iter)->update(fElapsedTime);	
}

CActor* CActorManager::getCActorFromBody(Critter::Body* bod)
{
	CActorsList::iterator iter=m_CActors.begin();
	bool bFound=false;
	for(;iter!=m_CActors.end() && !bFound;iter++)
	{
		if((*iter)->getBody()==bod)
		{
			bFound=true;
			break;
		}
	}
	if(bFound)
		return *iter;
	else
		return 0;
}