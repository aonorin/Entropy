#include "logfilemanager.h"

LogFileManager::LogFileManager()
//    mResourceType("LogFile")
{
    // low, because it will likely reference other resources
    mLoadOrder = 30.0f;

    // this is how we register the ResourceManager with OGRE
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager("LogFile", this);
}

LogFileManager::~LogFileManager()
{
    // and this is how we unregister it
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager("LogFile");
}

LogFilePtr LogFileManager::load(const Ogre::String &name, const Ogre::String &group)
{
    LogFilePtr textf = getByName(name);

    if (textf.isNull())
        textf = create(name, group);

    textf->load();
    return textf;
}

Ogre::Resource *LogFileManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                            const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                            const Ogre::NameValuePairList *createParams)
{
    return new LogFile(this, name, handle, group, isManual, loader);
}


#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> LogFileManager* Ogre::Singleton<LogFileManager>::ms_Singleton = 0;
LogFileManager* LogFileManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
LogFileManager& LogFileManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> LogFileManager* Ogre::Singleton<LogFileManager>::msSingleton = 0;
LogFileManager* LogFileManager::getSingletonPtr(void)
{
    return msSingleton;
}
LogFileManager& LogFileManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
