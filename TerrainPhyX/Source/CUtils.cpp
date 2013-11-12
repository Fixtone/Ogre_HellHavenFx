#include "../Headers/CUtils.h"

using namespace GameFrameWork;

void CUtils::Destroy(Ogre::SceneManager* sceneManager, Ogre::String sceneNodeName)
{
        if(!sceneManager->hasSceneNode(sceneNodeName))
                return;

       Ogre::SceneNode* sceneNode = sceneManager->getSceneNode(sceneNodeName);

        Ogre::SceneNode::ChildNodeIterator itn = sceneNode->getChildIterator();

        while(itn.hasMoreElements())
        {
               Ogre::SceneNode* node = static_cast<Ogre::SceneNode*>(itn.getNext());

                Destroy(sceneManager,node->getName());
        }

        Ogre::SceneNode::ObjectIterator ite = sceneNode->getAttachedObjectIterator();
        while (ite.hasMoreElements())
        {
                Ogre::Entity* entity = static_cast<Ogre::Entity*>(ite.getNext());

                sceneNode->detachObject(entity->getName());

                if(sceneManager->hasEntity(entity->getName()))
                        sceneManager->destroyEntity(entity->getName());
        }               

        Ogre::SceneNode* parentNode = sceneNode->getParentSceneNode();
        
        if(parentNode)
                parentNode->removeAndDestroyChild(sceneNode->getName());
}

std::bitset<CIdGenerator::N> CIdGenerator::ids;
