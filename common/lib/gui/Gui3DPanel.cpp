/*
    Gui3D
    -------
    
    Copyright (c) 2012 Valentin Frechaud
                                                                                  
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
                                                                                  
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
                                                                                  
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/



#include "Gui3DPanel.h"
#include "Gui3DPanelColors.h"

namespace Gui3D
{

using namespace std;

Panel::Panel(Gui3D* gui, 
             Ogre::SceneManager* sceneMgr, 
             const Ogre::Vector2& size,
             Ogre::Real distanceFromPanelToInteractWith,
             const Ogre::String& atlasName,
             const Ogre::String& name)
             : PanelContainer(gui, size),
               mDistanceFromPanelToInteractWith(distanceFromPanelToInteractWith),
               mNode(NULL), mPanelCameraNode(NULL), mScreenRenderable(NULL)
{
    mScreenRenderable = 
        gui->createScreenRenderable(Ogre::Vector2(mSize.x/100, mSize.y/100), atlasName, name);

    mNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    mNode->attachObject(mScreenRenderable);

    mPanelCameraNode = mNode->createChildSceneNode();
    mPanelCameraNode->setPosition(-1, 0, 7);
    mPanelCameraNode->lookAt(mNode->getPosition(), Ogre::Node::TS_PARENT);
    
    mGUILayer = gui->createLayer(mScreenRenderable, name);
    
    mBackground = mGUILayer->createRectangle(0, 0, mSize.x, mSize.y);

    if (getPanelColors()->panelBackgroundSpriteName.length() == 0 ||
        getPanelColors()->panelBackgroundSpriteName == "none")
    {
        mBackground->background_gradient(mGui3D->getPanelColors()->panelGradientType,
                                         mGui3D->getPanelColors()->panelGradientColorStart,
                                         mGui3D->getPanelColors()->panelGradientColorEnd);
        mBackground->border(mGui3D->getPanelColors()->panelBorderSize, 
                            mGui3D->getPanelColors()->panelBorder);
    }
    else
        mBackground->background_image(getPanelColors()->panelBackgroundSpriteName);

    // Create an empty mouse pointer which follow the mouse cursor
    mMousePointer = mGUILayer->createRectangle(0, 0, 0, 0);
    showInternalMousePointer();
}


Panel::~Panel()
{
    for (size_t i=0; i < mPanelElements.size(); i++)
        delete mPanelElements[i];

    // Destroy all elements that had been created on the screen renderable
    mGui3D->destroyScreenRenderable(mScreenRenderable);
}


void Panel::setDistanceFromPanelToInteractWith(Ogre::Real distanceFromPanelToInteractWith)
{
    mDistanceFromPanelToInteractWith = distanceFromPanelToInteractWith;
}


bool Panel::injectMouseMoved(const Ogre::Ray& ray)
{
    Ogre::Matrix4 transform;
    transform.makeTransform(mNode->getPosition(), mNode->getScale(), mNode->getOrientation());
   
    Ogre::AxisAlignedBox aabb = mScreenRenderable->getBoundingBox();
    aabb.transform(transform);
    pair<bool, Ogre::Real> result = Ogre::Math::intersects(ray, aabb);

    if (result.first == false)
    {
        unOverAllElements();
        return false;
    }

    Ogre::Vector3 a,b,c,d;
    Ogre::Vector2 halfSize = (mSize/100) * 0.5f;
    a = transform * Ogre::Vector3(-halfSize.x,-halfSize.y,0);
    b = transform * Ogre::Vector3( halfSize.x,-halfSize.y,0);
    c = transform * Ogre::Vector3(-halfSize.x, halfSize.y,0);
    d = transform * Ogre::Vector3( halfSize.x, halfSize.y,0);
    
    result = Ogre::Math::intersects(ray, c, b, a);
    if (result.first == false)
        result = Ogre::Math::intersects(ray, c, d, b);
    if (result.first == false)
    {
        unOverAllElements();
        return false;
    }
    if (result.second > mDistanceFromPanelToInteractWith)
    {
        unOverAllElements();
        return false;
    }

    Ogre::Vector3 hitPos = (ray.getOrigin() + (ray.getDirection() * result.second));
    Ogre::Vector3 localPos = transform.inverse() * hitPos;
    localPos.x += halfSize.x;
    localPos.y -= halfSize.y;
    localPos.x *= 100;
    localPos.y *= 100;
   
    // Cursor clip
    localPos.x = Ogre::Math::Clamp<Ogre::Real>(localPos.x, 0, mSize.x - 10);
    localPos.y = Ogre::Math::Clamp<Ogre::Real>(-localPos.y, 0, mSize.y - 18);

    mInternalMousePos = Ogre::Vector2(localPos.x, localPos.y);
    mMousePointer->position(mInternalMousePos);

    // Let's actualize the "over" for each elements
    for (size_t i=0; i < mPanelElements.size(); i++)
        mPanelElements[i]->isOver(mInternalMousePos);

    return true;
}

}