#include <Ogre.h>
#include "OIS/OIS.h"
#include "Gorilla.h"

typedef void (*OGKConsoleFunctionPtr)(Ogre::StringVector&);

class OGKConsole : public Ogre::Singleton<OGKConsole>, Ogre::FrameListener, Ogre::LogListener
{
 public:
    
    OGKConsole();
    ~OGKConsole();
    
    void   init(Gorilla::Screen* screen_to_use);
    void   shutdown();
    
    void   setVisible(bool mIsVisible);
    bool   isVisible(){return mIsVisible;}
    
    void   print(const Ogre::String &text);
    
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameEnded(const Ogre::FrameEvent &evt);
    
    void onKeyPressed(const OIS::KeyEvent &arg);
    
    void addCommand(const Ogre::String &command, OGKConsoleFunctionPtr);
    void removeCommand(const Ogre::String &command);
 
    //log
#if OGRE_VERSION_MINOR < 8 && OGRE_VERSION_MAJOR < 2
    void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName );
#else
    // "bool& skip" added in Ogre 1.8
    void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skip );
#endif
 private:
    
    void  updateConsole();
    void  updatePrompt();
    
    bool                 mIsVisible;
    bool                 mIsInitialised;
    Gorilla::Screen*     mScreen;
    Gorilla::Layer*      mLayer;
    Gorilla::Caption*    mPromptText;
    Gorilla::MarkupText* mConsoleText;
    Gorilla::Rectangle*  mDecoration;
    Gorilla::GlyphData*  mGlyphData;
    
    bool                 mUpdateConsole;
    bool                 mUpdatePrompt;

    unsigned int         mStartline;
    std::list<Ogre::String>      lines;
    Ogre::String            prompt;
    std::map<Ogre::String, OGKConsoleFunctionPtr>  commands;
};



