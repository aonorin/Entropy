#ifndef LOGFILEMANAGER_H
#define LOGFILEMANAGER_H

#include <OGRE/OgreResourceManager.h>
#include "logfile.h"

class LogFileManager : public Ogre::ResourceManager, public Ogre::Singleton<LogFileManager>
{
protected:

    // must implement this from ResourceManager's interface
    Ogre::Resource *createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                               const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                               const Ogre::NameValuePairList *createParams);

public:

    LogFileManager();
    virtual ~LogFileManager();

    virtual LogFilePtr load(const Ogre::String &name, const Ogre::String &group);

    static LogFileManager &getSingleton();
    static LogFileManager *getSingletonPtr();
};
#endif // LOGFILEMANAGER_H
