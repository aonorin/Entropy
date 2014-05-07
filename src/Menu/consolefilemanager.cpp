#include "consolefilemanager.h"

ConsoleFileManager::ConsoleFileManager()
//    mResourceType("ConsoleFile")
{
    // low, because it will likely reference other resources
    mLoadOrder = 30.0f;

    // this is how we register the ResourceManager with OGRE
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager("ConsoleFile", this);
}

ConsoleFileManager::~ConsoleFileManager()
{
    // and this is how we unregister it
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager("ConsoleFile");
}

ConsoleFilePtr ConsoleFileManager::load(const Ogre::String &name, const Ogre::String &group)
{
    ConsoleFilePtr textf = getByName(name);

    if (textf.isNull())
        textf = create(name, group);

    textf->load();
    return textf;
}

Ogre::Resource *ConsoleFileManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
                                            const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
                                            const Ogre::NameValuePairList *createParams)
{
    return new ConsoleFile(this, name, handle, group, isManual, loader);
}


#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> ConsoleFileManager* Ogre::Singleton<ConsoleFileManager>::ms_Singleton = 0;
ConsoleFileManager* ConsoleFileManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ConsoleFileManager& ConsoleFileManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> ConsoleFileManager* Ogre::Singleton<ConsoleFileManager>::msSingleton = 0;
ConsoleFileManager* ConsoleFileManager::getSingletonPtr(void)
{
    return msSingleton;
}
ConsoleFileManager& ConsoleFileManager::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif

