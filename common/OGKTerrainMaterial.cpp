//
//  OGKTerrainMaterialGenerator.cpp
//  OgreGameKit
//
//  Created by Alex on 6/6/14.
//
//

#include "OGKTerrainMaterial.h"

#include "Ogre.h"
#include <OgreTerrain.h>

// -----------------------------------------------------------------------------------------------------------------------

OGKTerrainMaterial::OGKTerrainMaterial(Ogre::String materialname)
{
    mMaterialName = materialname;
    mProfiles.push_back(OGRE_NEW OGKTerrainMaterial::Profile(this, "OgreMaterial", "Profile for rendering Ogre standard material"));
    setActiveProfile("OgreMaterial");
}

void OGKTerrainMaterial::setMaterialByName(const Ogre::String materialname) {
    mMaterialName = materialname;
};
OGKTerrainMaterial::Profile::Profile(Ogre::TerrainMaterialGenerator* parent, const Ogre::String& name, const Ogre::String& desc)
: Ogre::TerrainMaterialGenerator::Profile(parent, name, desc)
{
};

OGKTerrainMaterial::Profile::~Profile()
{
};

// -----------------------------------------------------------------------------------------------------------------------

Ogre::MaterialPtr OGKTerrainMaterial::Profile::generate(const Ogre::Terrain* terrain)
{
    const Ogre::String& matName = terrain->getMaterialName();
    
    Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(matName);
    if (!mat.isNull()) {
        Ogre::MaterialManager::getSingleton().remove(matName);
    }
    
    // Set Ogre material
    Ogre::String parentMatName = ((OGKTerrainMaterial*)getParent())->mMaterialName;
    if(Ogre::MaterialManager::getSingleton().resourceExists(parentMatName)) {
        mat = Ogre::MaterialManager::getSingleton().getByName(parentMatName);
        if(!mat.isNull()) {
            // clone the material or else when we delete the terrain
            // it will delete our material. this way it deletes the clone.
            mat = mat->clone(matName);
        }
    }
    
//#ifndef OGRE_IS_IOS
    if(!mat.isNull()) {
        // Get default pass
        Ogre::Pass *p = mat->getTechnique(0)->getPass(0);
        
        // Add terrain's global normalmap to renderpass so the fragment program can find it.
        Ogre::TextureUnitState *tu = p->createTextureUnitState(matName+"/nm");
        
        Ogre::TexturePtr nmtx = terrain->getTerrainNormalMap();
        tu->_setTexturePtr(nmtx);
    }
//#endif
    return mat;
};

Ogre::MaterialPtr OGKTerrainMaterial::Profile::generateForCompositeMap(const Ogre::Terrain* terrain)
{
    return terrain->_getCompositeMapMaterial();
};

Ogre::uint8 OGKTerrainMaterial::Profile::getMaxLayers(const Ogre::Terrain* terrain) const
{
    return 0;
};

void OGKTerrainMaterial::Profile::updateParams(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain)
{
};

void OGKTerrainMaterial::Profile::updateParamsForCompositeMap(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain)
{
};

void OGKTerrainMaterial::Profile::requestOptions(Ogre::Terrain* terrain)
{
//#ifdef OGRE_IS_IOS
//    terrain->_setMorphRequired(false);
//    terrain->_setNormalMapRequired(false);
//#else
    terrain->_setMorphRequired(true);
    terrain->_setNormalMapRequired(true); // enable global normal map
//#endif
    terrain->_setLightMapRequired(false);
    terrain->_setCompositeMapRequired(false);
};

void OGKTerrainMaterial::Profile::setLightmapEnabled(bool enabled)
{
    // nothing to do
}


