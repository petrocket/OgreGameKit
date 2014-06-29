#ifndef Gui3DUtils_H
#define Gui3DUtils_H

#include <OgrePrerequisites.h>

namespace Gui3D
{
    /** \brief Returns the closest int to the argument. Example : 5.1 gives 5, 5.5 gives 6 and 5.8 gives 6.
    */
    inline Ogre::Real round(Ogre::Real value)
    {
        return (int)(value + 0.5);
    }
};

#endif
