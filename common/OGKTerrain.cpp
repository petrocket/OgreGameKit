//
//  OGKTerrain.cpp
//  OgreGameKit
//
//  Created by Alex on 6/5/14.
//
//

#include "OGKTerrain.h"
#include "OGKGame.h"
#include <OgreTerrainMaterialGeneratorA.h>

//#ifdef OGRE_IS_IOS
#include "OGKTerrainMaterial.h"
//#endif

OGKTerrain::OGKTerrain() :
    mTerrainsImported(false),
    mTerrainGlobals(NULL),
    mTerrainGroup(NULL),
    mWorldSize(10000)
{
    
}

OGKTerrain::~OGKTerrain()
{
    if(mTerrainGroup) {
//        mTerrainGroup->removeAllTerrains();
        OGRE_DELETE mTerrainGroup;
        mTerrainGroup = NULL;
    }
    
    if(mTerrainGlobals) {
        OGRE_DELETE mTerrainGlobals;
        mTerrainGlobals = NULL;
    }
    
//    if(mTerrainGlobals->getDefaultMaterialGenerator())
}

void OGKTerrain::configureTerrainDefaults(Ogre::SceneManager *sceneMgr, Ogre::Light *light)
{
#ifdef OGRE_IS_IOS
    mTerrainGlobals->setMaxPixelError(16);
#else
    mTerrainGlobals->setMaxPixelError(8);
#endif
    
    // Use custom materialgenerator
    Ogre::TerrainMaterialGeneratorPtr terrainMaterialGenerator;
    OGKTerrainMaterial *terrainMaterial = OGRE_NEW OGKTerrainMaterial("TerrainMaterial");
    terrainMaterialGenerator.bind( terrainMaterial );
	mTerrainGlobals->setDefaultMaterialGenerator( terrainMaterialGenerator );
    
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
#ifdef OGRE_IS_IOS
    defaultimp.terrainSize = 65;
#else
    defaultimp.terrainSize = 1025;
#endif
    defaultimp.worldSize = mWorldSize;
    defaultimp.inputScale = 1667;
    
#ifdef OGRE_IS_IOS
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 33;
#else
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
#endif
}

void OGKTerrain::setup(Ogre::SceneManager *sceneMgr, Ogre::Light *light)
{
    // init terrain
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
#ifdef OGRE_IS_IOS
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(sceneMgr, Ogre::Terrain::ALIGN_X_Z, 65, mWorldSize);
#else
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(sceneMgr, Ogre::Terrain::ALIGN_X_Z, 1025, mWorldSize);
#endif
    mTerrainGroup->setFilenameConvention(Ogre::String("OGKTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
    
    configureTerrainDefaults(sceneMgr, light);
    
    // Load terrain from heightmap
    Ogre::Image img;
    img.load("ETterrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    mTerrainGroup->defineTerrain(0, 0, &img);

    mTerrainGroup->loadAllTerrains(false);

    mTerrainGroup->freeTemporaryResources();
}

void OGKTerrain::update()
{
#ifndef OGRE_IS_IOS
    if(mTerrainGroup &&
       !mTerrainGroup->isDerivedDataUpdateInProgress() &&
       mTerrainsImported) {
        OGKGame::getSingleton().mLog->logMessage("Saving terrain...");
        mTerrainGroup->saveAllTerrains(true);
        mTerrainsImported = false;
        OGKGame::getSingleton().mLog->logMessage("Done Saving terrain...");
    }
#endif
}