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
#include <OgreTerrainPaging.h>
#include "OGKTerrainGenerator.h"

#define OGK_USE_TERRAIN_LAYERS
#define OGK_USE_PAGING

class OGKTerrain
{
public:
    OGKTerrain();
    ~OGKTerrain();

    void setup(Ogre::SceneManager *sceneMgr, Ogre::Light *light);
    void update(double timeSinceLastFrame);
    
    Ogre::TerrainGroup          *mTerrainGroup;
    Ogre::TerrainGlobalOptions  *mTerrainGlobals;
    Ogre::TerrainPaging         *mTerrainPaging;
    Ogre::PageManager           *mPageManager;
    
    bool mTerrainsImported;

    void configureTerrainDefaults(Ogre::SceneManager *sceneMgr, Ogre::Light *light);
    
    void modifyTerrain(Ogre::Terrain *terrain, const Ogre::Vector3& centrepos, Ogre::Real timeElapsed);
    
    enum OGKTerrainMode
    {
        MODE_NORMAL = 0,
        MODE_EDIT_HEIGHT = 1,
        MODE_EDIT_BLEND = 2,
        MODE_COUNT = 3
    };
    
    void setMode(OGKTerrainMode mode);
    
private:
    double mWorldSize;
    
    Ogre::Real mBrushSizeTerrainSpace;
    Ogre::Real mHeightUpdateCountDown;
    Ogre::Real mHeightUpdateRate;
    Ogre::uint8     mLayerEdit;
    Ogre::SceneNode *mMarkerSceneNode;
    Ogre::Entity    *mMarker;
    OGKTerrainMode mMode;
    
    OGKTerrainGenerator *mTerrainGenerator;
    
#ifdef OGK_USE_PAGING
	/// This class just pretends to provide prcedural page content to avoid page loading
	class OGKTerrainPageProvider : public Ogre::PageProvider
	{
	public:
		bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
		bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
	};
	OGKTerrainPageProvider mPageProvider;
    Ogre::TerrainPagedWorldSection *mTerrainPagedWorldSection;
#endif
};

#endif /* defined(__OgreGameKit__OGKTerrain__) */
