//
//  OGKTerrainGenerator.cpp
//  OgreGameKit
//
//  Created by Alex on 8/11/14.
//
//

#include "OGKTerrainGenerator.h"


#define BM 0xff
#define N 0x1000
#define NP 12   /* 2^N */
#define NM 0xfff

OGKTerrainGenerator::OGKTerrainGenerator(const Ogre::Real& alpha, const Ogre::Real& beta, int iterationNum, const Ogre::Real& cycle, const Ogre::Real& heightScale)
: mAlpha(alpha)
, mBeta(beta)
, mIterationNum(iterationNum)
, mCycle(cycle)
, mHeightScale(heightScale)
, mOriginPoint(0.f)
{
	for (int i = 0 ; i < OGK_TERRAIN_B ; i++) {
		p[i] = i;
		g1[i] = Ogre::Math::SymmetricRandom();
        
		g2[i] = Ogre::Vector2(Ogre::Math::SymmetricRandom(), Ogre::Math::SymmetricRandom());
		g2[i].normalise();
        
		g3[i] = Ogre::Vector3(Ogre::Math::SymmetricRandom(), Ogre::Math::SymmetricRandom(), Ogre::Math::SymmetricRandom());
		g3[i].normalise();
	}
    
	for (int i = 0 ; i < OGK_TERRAIN_B ; i++) {
		int j = (int) Ogre::Math::RangeRandom (0,OGK_TERRAIN_B);
        
		int k = p[i];
		p[i] = p[j];
		p[j] = k;
	}
    
	for (int i = 0 ; i < OGK_TERRAIN_B + 2 ; i++) {
		p[OGK_TERRAIN_B + i] = p[i];
		g1[OGK_TERRAIN_B + i] = g1[i];
		g2[OGK_TERRAIN_B + i] = g2[i];
		g3[OGK_TERRAIN_B + i] = g3[i];
	}
}

OGKTerrainGenerator::~OGKTerrainGenerator()
{
    
}

void OGKTerrainGenerator::define( Ogre::TerrainGroup* terrainGroup, long x, long y )
{
    
    // first check if we have already generated the terrain
    // Load terrain from heightmap
    Ogre::Image img;
    Ogre::String filename = terrainGroup->generateFilename(x,y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(terrainGroup->getResourceGroup(), filename)) {
        terrainGroup->defineTerrain(x, y);
        //    img.load(generatedFilename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        //    terrainGroup->defineTerrain(x, y, &img);
    }
    else {
        Ogre::uint16 terrainSize = terrainGroup->getTerrainSize();
        
        float* heightMap = OGRE_ALLOC_T(float, terrainSize*terrainSize, Ogre::MEMCATEGORY_GEOMETRY);
        
        Ogre::Vector2 worldOffset( Ogre::Real(x*(terrainSize-1)), Ogre::Real(y*(terrainSize-1)) );
        worldOffset += mOriginPoint;
        
        Ogre::Vector2 revisedValuePoint;
        for( Ogre::uint16 i=0; i<terrainSize; i++ ) {
            for( Ogre::uint16 j=0; j<terrainSize; j++ ) {
                revisedValuePoint = (worldOffset + Ogre::Vector2(j,i)) / mCycle;
                heightMap[i*terrainSize + j] = produceSingleHeight( revisedValuePoint ) * mHeightScale;
            }
        }
        
        terrainGroup->defineTerrain(x,y,heightMap);
        OGRE_FREE(heightMap, Ogre::MEMCATEGORY_GEOMETRY);
    }
}

Ogre::Real OGKTerrainGenerator::produceSingleHeight(const Ogre::Vector2& vec2)
{
	Ogre::Vector2 tempVec(vec2);
	Ogre::Real sum = 0;
	Ogre::Real scale = 1;
    
	for (int i=0;i<mIterationNum;i++) {
		sum += noise(tempVec) / scale;
		scale *= mAlpha;
		tempVec *= mBeta;
	}
	return sum;
}

Ogre::Real OGKTerrainGenerator::noise(const Ogre::Vector2& vec2)
{
	int bx0, bx1, by0, by1, b00, b10, b01, b11;
	Ogre::Real rx0, rx1, ry0, ry1, sx, sy, a, b, u, v;
    
	setup(vec2.x, bx0,bx1, rx0,rx1);
	setup(vec2.y, by0,by1, ry0,ry1);
    
	int i = p[ bx0 ];
	int j = p[ bx1 ];
    
	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];
    
	sx = sCurve(rx0);
	sy = sCurve(ry0);
    
	u = g2[b00].dotProduct( Ogre::Vector2(rx0,ry0) );
	v = g2[b10].dotProduct( Ogre::Vector2(rx1,ry0) );
	a = lerp(sx, u, v);
    
	u = g2[b01].dotProduct( Ogre::Vector2(rx0,ry1) );
	v = g2[b11].dotProduct( Ogre::Vector2(rx1,ry1) );
	b = lerp(sx, u, v);
    
	return lerp(sy, a, b);
}

void OGKTerrainGenerator::setup( const Ogre::Real& target, int& b0, int& b1, Ogre::Real& r0, Ogre::Real& r1)
{
	Ogre::Real t = target + N;
	b0 = ((int)t) & BM;
	b1 = (b0+1) & BM;
	r0 = t - (int)t;
	r1 = r0 - 1;
}

void OGKTerrainGenerator:: randomize()
{
	mOriginPoint.x = Ogre::Math::RangeRandom( -1000, 1000 );
	mOriginPoint.y = Ogre::Math::RangeRandom( -1000, 1000 );
}