//
//  OgreRayCollision.h
//  OgreGameKit
//
//  Created by Alex on 10/10/14.
//
//

#ifndef __OgreGameKit__OgreRay__
#define __OgreGameKit__OgreRay__

#include <Ogre.h>

class OgreRayCollision
{
private:
    Ogre::RaySceneQuery* m_raySceneQuery;//!< Ray query
    
public:
    OgreRayCollision( Ogre::SceneManager* sceneMgr );
    
    bool RaycastFromPoint( const Ogre::Vector3& point,
                           const Ogre::Vector3& normal,
                           Ogre::Vector3& result,
                           Ogre::MovableObject *resultObject);

    bool RaycastObject( const Ogre::Vector3& point,
                        const Ogre::Vector3& normal,
                        Ogre::Vector3& result,
                        Ogre::MovableObject *object,
                        bool cacheObject = true);

private:
    static void GetMeshInformation( const Ogre::MeshPtr mesh, size_t &vertex_count, Ogre::Vector3*& vertices,  size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale );
    static void GetMeshInformation( const Ogre::ManualObject* manual, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale );
    static void GetMeshInformation( const Ogre::Entity* entity, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale );
    
    // mesh data to retrieve
    size_t mSavedVertexCount;
    size_t mSavedIndexcount;
    Ogre::Vector3* mSavedVertices;
    unsigned long* mSavedIndices;
    
    Ogre::MovableObject *mSavedCollisionObject;
    
    std::pair<bool, Ogre::Real> intersects(const Ogre::Ray& ray, const Ogre::Vector3& a,
                                                    const Ogre::Vector3& b, const Ogre::Vector3& c, const Ogre::Vector3& normal,
                                           bool positiveSide, bool negativeSide);
    
};

#endif /* defined(__OgreGameKit__OgreRay__) */
