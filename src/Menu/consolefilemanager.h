#ifndef CONSOLEFILEMANAGER_H
#define CONSOLEFILEMANAGER_H

#include <OGRE/OgreResourceManager.h>
#include "src/Menu/consolefile.h"
class ConsoleFileManager : public Ogre::ResourceManager, public Ogre::Singleton<ConsoleFileManager>
{
public:
    ConsoleFileManager();

    Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                               const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                               const Ogre::NameValuePairList *createParams);

    virtual ~ConsoleFileManager();

    virtual ConsoleFilePtr load(const Ogre::String &name, const Ogre::String &group);

    static ConsoleFileManager &getSingleton();
    static ConsoleFileManager *getSingletonPtr();
};

#endif // CONSOLEFILEMANAGER_H
