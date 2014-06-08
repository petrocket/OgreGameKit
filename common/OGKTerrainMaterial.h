//
//  OGKTerrainMaterialGenerator.h
//  OgreGameKit
//
//  Created by Alex on 6/6/14.
//
//

#ifndef __OgreGameKit__OGKTerrainMaterialGenerator__
#define __OgreGameKit__OGKTerrainMaterialGenerator__

#include "OgreTerrainMaterialGenerator.h"

class OGKTerrainMaterial : public Ogre::TerrainMaterialGenerator
{
public:
    
    OGKTerrainMaterial(Ogre::String materialName);
    
    void setMaterialByName(const Ogre::String materialname);
    
    class Profile : public Ogre::TerrainMaterialGenerator::Profile
    {
    public:
        Profile(Ogre::TerrainMaterialGenerator* parent, const Ogre::String& name, const Ogre::String& desc);
        ~Profile();
        
        bool isVertexCompressionSupported() const { return false; }
        
        Ogre::MaterialPtr generate(const Ogre::Terrain* terrain);
        
        Ogre::MaterialPtr generateForCompositeMap(const Ogre::Terrain* terrain);
        
        Ogre::uint8 getMaxLayers(const Ogre::Terrain* terrain) const;
        
        void updateParams(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain);
        
        void updateParamsForCompositeMap(const Ogre::MaterialPtr& mat, const Ogre::Terrain* terrain);
        
        void requestOptions(Ogre::Terrain* terrain);
        
        void setLightmapEnabled(bool enabled);
        
    };
protected:
    Ogre::String mMaterialName;
};

#endif /* defined(__OgreGameKit__OGKTerrainMaterialGenerator__) */
