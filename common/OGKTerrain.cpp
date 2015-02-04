//
//  OGKTerrain.cpp
//  OgreGameKit
//
//  Created by Alex on 6/5/14.
//
//

#include "OGKTerrain.h"
#include "OGKGame.h"
#include "OGKInputManager.h"
#include <OgreTerrainMaterialGeneratorA.h>
#include "macUtils.h"

#include "OGKTerrainMaterial.h"

// max range for a int16
#define TERRAIN_PAGE_MIN_X (-0x7FFF)
#define TERRAIN_PAGE_MIN_Y (-0x7FFF)
#define TERRAIN_PAGE_MAX_X 0x7FFF
#define TERRAIN_PAGE_MAX_Y 0x7FFF

#define HOLD_LOD_DISTANCE 3000.0

OGKTerrain::OGKTerrain() :
    mBrushSizeTerrainSpace(0.02),
    mHeightUpdateRate(1.0 / 20.0), // update terrain at max 20fps
    mHeightUpdateCountDown(0),
    mMode(MODE_NORMAL),
    mPageManager(0),
    mTerrainPaging(0),
    mTerrainsImported(false),
    mTerrainGlobals(NULL),
    mTerrainGroup(NULL),
    mWorldSize(2000.0)
{
#ifdef OGRE_IS_IOS
    // add documents folder for loading generated terrain
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::iOSDocumentsDirectory() + "/../Documents", "FileSystem", "General");
#endif
}

OGKTerrain::~OGKTerrain()
{
    if (mTerrainPaging) {
        OGRE_DELETE mTerrainPaging;
        mTerrainPaging = 0;
        OGRE_DELETE mPageManager;
        mPageManager = 0;
    }
    
    if(mTerrainGroup) {
        OGRE_DELETE mTerrainGroup;
        mTerrainGroup = NULL;
    }
    
    if(mTerrainGlobals) {
        OGRE_DELETE mTerrainGlobals;
        mTerrainGlobals = NULL;
    }
}

void OGKTerrain::configureTerrainDefaults(Ogre::SceneManager *sceneMgr, Ogre::Light *light)
{
    mTerrainGlobals->setMaxPixelError(1);
    mTerrainGlobals->setCompositeMapDistance(1000);
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
#if OGRE_IS_IOS
    mTerrainGlobals->setLightMapSize(256);
    mTerrainGlobals->setCompositeMapSize(256);
    mTerrainGlobals->setLayerBlendMapSize(256);
#else
    mTerrainGlobals->setLightMapSize(512);
    mTerrainGlobals->setCompositeMapSize(512);
    mTerrainGlobals->setLayerBlendMapSize(512);
#endif
    mTerrainGlobals->setCompositeMapAmbient(sceneMgr->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
    
    Ogre::TerrainMaterialGeneratorA::SM2Profile* matProfile =
    static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile*>(mTerrainGlobals->getDefaultMaterialGenerator()->getActiveProfile());
    matProfile->setLightmapEnabled(true);
#if OGRE_IS_IOS
    matProfile->setGlobalColourMapEnabled(false);
    matProfile->setLayerParallaxMappingEnabled(false);
    matProfile->setLayerSpecularMappingEnabled(true);
    matProfile->setLayerNormalMappingEnabled(false);
#else
    matProfile->setGlobalColourMapEnabled(false);
    matProfile->setLayerParallaxMappingEnabled(false);
    matProfile->setLayerSpecularMappingEnabled(true);
    matProfile->setLayerNormalMappingEnabled(true);
#endif
    

    //mTerrainGlobals->setSkirtSize(1.0);
    //mTerrainGlobals->setUseVertexCompressionWhenAvailable(false);
    
#ifndef OGK_USE_TERRAIN_LAYERS
    // Use custom materialgenerator
    Ogre::TerrainMaterialGeneratorPtr terrainMaterialGenerator;
    OGKTerrainMaterial *terrainMaterial = OGRE_NEW OGKTerrainMaterial("TerrainMaterial");
    terrainMaterialGenerator.bind( terrainMaterial );
	mTerrainGlobals->setDefaultMaterialGenerator( terrainMaterialGenerator );
#endif
    
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
#ifdef OGK_USE_TERRAIN_LAYERS
    defaultimp.terrainSize = 513;
    defaultimp.inputScale = 128;
    defaultimp.layerList.resize(1);
    defaultimp.layerList[0].worldSize = 24;
#ifdef OGRE_IS_IOS
    defaultimp.layerList[0].textureNames.push_back("splatting1_medium_res.png");
#else
    defaultimp.layerList[0].textureNames.push_back("splatting1_diffuse_and_spec.png");
    defaultimp.layerList[0].textureNames.push_back("splatting1_normal.png");
#endif
//    defaultimp.layerList[1].worldSize = 200;
//    defaultimp.layerList[1].textureNames.push_back("splatting2.png");
    //defaultimp.layerList[1].textureNames.push_back("splatting2_normal.png");
#else
    defaultimp.terrainSize = 1025;
    defaultimp.inputScale = 1667;
#endif
    defaultimp.worldSize = mWorldSize;
    
}

void OGKTerrain::modifyTerrain(Ogre::Terrain *terrain, const Ogre::Vector3& centrepos, Ogre::Real timeElapsed)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
    Ogre::Vector3 tsPos;
    terrain->getTerrainPosition(centrepos, &tsPos);
    
    OIS::Keyboard *keyboard = OGKInputManager::getSingletonPtr()->getKeyboard();
    
    if (keyboard->isKeyDown(OIS::KC_EQUALS) || keyboard->isKeyDown(OIS::KC_ADD) ||
        keyboard->isKeyDown(OIS::KC_MINUS) || keyboard->isKeyDown(OIS::KC_SUBTRACT))
    {
        switch(mMode)
        {
            case MODE_EDIT_HEIGHT:
            {
                // we need point coords
                Ogre::Real terrainSize = (terrain->getSize() - 1);
                long startx = (tsPos.x - mBrushSizeTerrainSpace) * terrainSize;
                long starty = (tsPos.y - mBrushSizeTerrainSpace) * terrainSize;
                long endx = (tsPos.x + mBrushSizeTerrainSpace) * terrainSize;
                long endy= (tsPos.y + mBrushSizeTerrainSpace) * terrainSize;
                startx = std::max(startx, 0L);
                starty = std::max(starty, 0L);
                endx = std::min(endx, (long)terrainSize);
                endy = std::min(endy, (long)terrainSize);
                for (long y = starty; y <= endy; ++y)
                {
                    for (long x = startx; x <= endx; ++x)
                    {
                        Ogre::Real tsXdist = (x / terrainSize) - tsPos.x;
                        Ogre::Real tsYdist = (y / terrainSize)  - tsPos.y;
                        
                        Ogre::Real weight = std::min((Ogre::Real)1.0,
                                               Ogre::Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Ogre::Real(0.5 * mBrushSizeTerrainSpace));
                        weight = 1.0 - (weight * weight);
                        
                        float addedHeight = weight * 0.01 * timeElapsed;
                        float newheight;
                        if (keyboard->isKeyDown(OIS::KC_EQUALS) || keyboard->isKeyDown(OIS::KC_ADD))
                            newheight = terrain->getHeightAtPoint(x, y) + addedHeight;
                        else
                            newheight = terrain->getHeightAtPoint(x, y) - addedHeight;
                        terrain->setHeightAtPoint(x, y, newheight);
                        
                    }
                }
                if (mHeightUpdateCountDown == 0)
                    mHeightUpdateCountDown = mHeightUpdateRate;
            }
                break;
            case MODE_EDIT_BLEND:
            {
                Ogre::TerrainLayerBlendMap* layer = terrain->getLayerBlendMap(mLayerEdit);
                
                // we need image coords
                Ogre::Real imgSize = terrain->getLayerBlendMapSize();
                long startx = (tsPos.x - mBrushSizeTerrainSpace) * imgSize;
                long starty = (tsPos.y - mBrushSizeTerrainSpace) * imgSize;
                long endx = (tsPos.x + mBrushSizeTerrainSpace) * imgSize;
                long endy= (tsPos.y + mBrushSizeTerrainSpace) * imgSize;
                startx = std::max(startx, 0L);
                starty = std::max(starty, 0L);
                endx = std::min(endx, (long)imgSize);
                endy = std::min(endy, (long)imgSize);
                for (long y = starty; y <= endy; ++y)
                {
                    for (long x = startx; x <= endx; ++x)
                    {
                        Ogre::Real tsXdist = (x / imgSize) - tsPos.x;
                        Ogre::Real tsYdist = (y / imgSize)  - tsPos.y;
                        
                        Ogre::Real weight = std::min((Ogre::Real)1.0,
                                               Ogre::Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Ogre::Real(0.5 * mBrushSizeTerrainSpace));
                        weight = 1.0 - (weight * weight);
                        
                        float paint = weight * timeElapsed;
                        size_t imgY = imgSize - y;
                        float val;
                        if (keyboard->isKeyDown(OIS::KC_EQUALS) || keyboard->isKeyDown(OIS::KC_ADD))
                            val = layer->getBlendValue(x, imgY) + paint;
                        else
                            val = layer->getBlendValue(x, imgY) - paint;
                        val = Ogre::Math::Clamp(val, 0.0f, 1.0f);
                        layer->setBlendValue(x, imgY, val);
                        
                    }
                }
                
                layer->update();
            }
                break;
            case MODE_NORMAL:
            case MODE_COUNT:
                break;
        };
    }
#endif
}

void OGKTerrain::setup(Ogre::SceneManager *sceneMgr, Ogre::Light *light)
{
    // init terrain
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    
#ifdef OGK_USE_TERRAIN_LAYERS
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(sceneMgr, Ogre::Terrain::ALIGN_X_Z, 513, mWorldSize);
#else
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(sceneMgr, Ogre::Terrain::ALIGN_X_Z, 1025, mWorldSize);
#endif
    mTerrainGroup->setFilenameConvention(Ogre::String("OGKTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

#ifdef OGK_USE_PAGING
    mTerrainGroup->setAutoUpdateLod( Ogre::TerrainAutoUpdateLodFactory::getAutoUpdateLod(Ogre::BY_DISTANCE) );
#endif
    configureTerrainDefaults(sceneMgr, light);
    
#ifdef OGK_USE_PAGING
    // Paging setup
    mPageManager = OGRE_NEW Ogre::PageManager();
    // Since we're not loading any pages from .page files, we need a way just
    // to say we've loaded them without them actually being loaded
    mPageManager->setPageProvider(&mPageProvider);
    mPageManager->addCamera(sceneMgr->getCurrentViewport()->getCamera());
    mTerrainPaging = OGRE_NEW Ogre::TerrainPaging(mPageManager);
    Ogre::PagedWorld* world = mPageManager->createWorld();
    mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(world, mTerrainGroup, 2000, 3000,
                                       TERRAIN_PAGE_MIN_X, TERRAIN_PAGE_MIN_Y,
                                       TERRAIN_PAGE_MAX_X, TERRAIN_PAGE_MAX_Y);
    
    mTerrainGenerator = OGRE_NEW OGKTerrainGenerator();
    mTerrainPagedWorldSection->setDefiner( mTerrainGenerator );
#else
    // Load terrain from heightmap
    Ogre::Image img;
#ifdef OGK_USE_TERRAIN_LAYERS
    img.load("height.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#else
    img.load("ETterrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif
    mTerrainGroup->defineTerrain(0, 0, &img);
    
#if OGRE_IS_IOS
    mTerrainGroup->loadAllTerrains(true);
    mTerrainGroup->update(true);
#else
    mTerrainGroup->loadAllTerrains(false);
#endif
    
#endif
    
    
#ifdef OGK_USE_TERRAIN_LAYERS
    // blending
    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    if(ti.hasMoreElements()) {
        Ogre::Terrain *t = ti.getNext()->instance;
        // don't need blend map for layer 0 (not used)
        //t->getLayerBlendMap(0)->loadImage("blend0", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        //t->getLayerBlendMap(1)->loadImage("blend1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    }
#endif

    mTerrainGroup->freeTemporaryResources();
    
#ifndef OGRE_IS_IOS
    // marker for editing terrain
    mMarker = sceneMgr->createEntity("OGKTerrainMarker", "TerrainMarker.mesh");
    mMarkerSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mMarkerSceneNode->attachObject(mMarker);
    mMarkerSceneNode->setScale(0.05,0.05,0.05);
    mMarker->setVisible(false);
#endif
    
    mTerrainsImported = true;
}

void OGKTerrain::setMode(OGKTerrainMode mode)
{
    mMode = mode;
}

void OGKTerrain::update(double timeSinceLastFrame)
{
#ifndef OGRE_IS_IOS
    if(mTerrainGroup &&
        !mTerrainGroup->isDerivedDataUpdateInProgress()) {        
        Ogre::Terrain *terrain = mTerrainGroup->getTerrain(0, 0);
        if(terrain && terrain->isLoaded() && mTerrainsImported) {
            OGKGame::getSingleton().mLog->logMessage("Saving terrain...");
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
            OGKGame::getSingleton().mLog->logMessage("Done Saving terrain...");
        }
    }

    if(mMode != MODE_NORMAL) {
        // EDIT MODE
        OIS::Mouse *mouse = OGKInputManager::getSingletonPtr()->getMouse();
        if(mouse) {
            OIS::MouseState state = mouse->getMouseState();
            
            Ogre::Real x = (float)state.X.abs / (float)state.width;
            Ogre::Real y = (float)state.Y.abs / (float)state.height;
            
            OGKScene *scene = OGKGame::getSingletonPtr()->mGameSceneManager->getActiveScene();
            if(scene && scene->mCamera) {
                Ogre::Ray ray = scene->mCamera->getCamera()->getCameraToViewportRay(x, y);
                Ogre::TerrainGroup::RayResult rayResult = mTerrainGroup->rayIntersects(ray);
                if (rayResult.hit) {
                    mMarker->setVisible(true);
                    mMarkerSceneNode->setPosition(rayResult.position);
                    
                    // figure out which terrains this affects
                    Ogre::TerrainGroup::TerrainList terrainList;
                    Ogre::Real brushSizeWorldSpace = mWorldSize * mBrushSizeTerrainSpace;
                    Ogre::Sphere sphere(rayResult.position, brushSizeWorldSpace);
                    mTerrainGroup->sphereIntersects(sphere, &terrainList);
                    
                    for (Ogre::TerrainGroup::TerrainList::iterator ti = terrainList.begin();
                         ti != terrainList.end(); ++ti)
                        modifyTerrain(*ti, rayResult.position, timeSinceLastFrame);
                }
                else {
                    mMarker->setVisible(false);
                }
            }
        }
    }
    else {
        mMarker->setVisible(false);
    }
    
    if(mHeightUpdateCountDown > 0) {
        mHeightUpdateCountDown -= timeSinceLastFrame;
        if(mHeightUpdateCountDown <= 0) {
            mTerrainGroup->update();
            mHeightUpdateCountDown = 0;
        }
    }
#endif
    
#ifdef OGK_USE_PAGING
    if(mTerrainGroup) {
        mTerrainGroup->autoUpdateLodAll(false, Ogre::Any( Ogre::Real(HOLD_LOD_DISTANCE) ));
    }
#endif
}