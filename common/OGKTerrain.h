//
//  OGKTerrain.h
//  OgreGameKit
//
//  Created by Alex on 6/5/14.
//
//

#ifndef __OgreGameKit__OGKTerrain__
#define __OgreGameKit__OGKTerrain__

#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>

class OGKTerrain
{
public:
    OGKTerrain();
    ~OGKTerrain();

    void setup(Ogre::SceneManager *sceneMgr, Ogre::Light *light);
    void update();
    
    Ogre::TerrainGroup          *mTerrainGroup;
    Ogre::TerrainGlobalOptions  *mTerrainGlobals;
    bool mTerrainsImported;

    void configureTerrainDefaults(Ogre::SceneManager *sceneMgr, Ogre::Light *light);
private:
    double mWorldSize;
};

#endif /* defined(__OgreGameKit__OGKTerrain__) */
