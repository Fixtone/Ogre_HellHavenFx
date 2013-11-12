#ifndef __CUTILS_H_
#define __CUTILS_H_

#include <Ogre.h>

namespace GameFrameWork
{
	class CUtils
	{
		public:

			// Detaches and destroys all objects and nodes recursively, starting from the given node
			static void Destroy(Ogre::SceneManager* sceneManager, Ogre::String sceneNodeName);
	};

	class CIdGenerator			
	{     
	
		private:
			
			static const int N = 100;
			static std::bitset<N> ids;

			static bool allIdsUsed() 
			{ 
				return ids.all();
			}

		public:

			static int getId() 
			{
				if(allIdsUsed())
					throw "Error";

				for(int i = 0; i < N; ++i )
				{
					if(!ids.test(i))
					{
						ids[i] = true;
						return i;
					}
				}
			}

			static void freeId(int i) 
			{
				ids[i]=false;
				ids.set(i);
			}

	};

}



#endif //__CUTILS_H_



