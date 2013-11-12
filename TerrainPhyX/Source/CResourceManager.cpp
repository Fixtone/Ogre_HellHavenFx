#include "../Headers/CResourceManager.h"

// BEGIN SINGLETON
template<> CResourceManager* Ogre::Singleton<CResourceManager>::msSingleton = 0;
CResourceManager* CResourceManager::getSingletonPtr(void)
{
    return msSingleton;
}
CResourceManager& CResourceManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CResourceManager::CResourceManager() 
{
}

CResourceManager::~CResourceManager()
{	  
}

void CResourceManager::initialize(Ogre::SceneManager* aSceneManager)
{
	mSceneManager = aSceneManager;
}

void CResourceManager::loadPlayStateResources(OgreBites::SdkTrayManager* aTrayMgr)
{
	aTrayMgr->showLoadingBar(1, 1, 0);

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// ok then, load all the textures in the HellHeaven resource group
	Ogre::String resourceGroup("Game");	
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(resourceGroup);		

	/////////////////////////////////////////////////////////////////////////////////////////
	// Preload the distortion texture with hardware gamma correction
	/////////////////////////////////////////////////////////////////////////////////////////

	Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "[PKFX] ---------- Preload texture hardware gamma ----------");

	// ok then, load all the textures in the HellHeaven resource group
	resourceGroup.clear();
	resourceGroup.append("Popcorn");	
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(resourceGroup);

	Ogre::StringVectorPtr strVectorPtr = Ogre::ResourceGroupManager::getSingleton().listResourceNames(resourceGroup);

	for (unsigned int i = 0; i < strVectorPtr->size(); ++i)
	{
		Ogre::String texName = strVectorPtr->at(i);

		try
		{
			Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "[PKFX] Trying to load " + texName + " as texture...");

			// should load all the textures with gamma correction 
			// except the ones used for distortion post effect
			if (!texName.compare("ParticleDeformBlur_01.dds") || !texName.compare("RainDeform_01.dds") || !texName.compare("RainDropsDeform_01.dds"))
				Ogre::TexturePtr pTex = Ogre::TextureManager::getSingleton().load(texName, resourceGroup, Ogre::TEX_TYPE_2D, Ogre::MIP_DEFAULT, 1.0f, false, Ogre::PF_UNKNOWN, false);			
			else			
				// gamma corrected
				Ogre::TexturePtr pTex = Ogre::TextureManager::getSingleton().load(texName, resourceGroup, Ogre::TEX_TYPE_2D, Ogre::MIP_DEFAULT, 1.0f, false, Ogre::PF_A8R8G8B8, true);			
		}
		catch (Ogre::Exception e) // texture not loaded because it surely is not one
		{
			continue;
		}
	}
	Ogre::LogManager::getSingleton().logMessage(Ogre::LML_NORMAL, "[PKFX] ---------- Done ----------"); 

	aTrayMgr->hideLoadingBar();
}