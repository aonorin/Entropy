#ifndef CONSOLEFILE_H
#define CONSOLEFILE_H

#include <OGRE/OgreResourceManager.h>

namespace Ogre {
    class ResourceManager;
    class Resource;
    class ManualResourceLoader;
}

class ConsoleFile : public Ogre::Resource
{
public:
    ConsoleFile(Ogre::ResourceManager *creator, const Ogre::String &name,
            Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
            Ogre::ManualResourceLoader *loader = 0);
    virtual ~ConsoleFile();
    ConsoleFile(std::string name);

    void setText(const Ogre::String &logText);
    const Ogre::String &getText() const;
    void load();
    void save();
    void addLine(std::string line);
    void clearLines();
    std::vector<std::string> getLines();

protected:
     // must implement these from the Ogre::Resource interface
     void loadImpl();
     void unloadImpl();
     size_t calculateSize() const;

private:
     Ogre::String consoleFileText;
     std::vector<std::string> consoleLines;
};

class ConsoleFilePtr : public Ogre::SharedPtr<ConsoleFile>
 {
 public:
     ConsoleFilePtr() : Ogre::SharedPtr<ConsoleFile>() {}
     explicit ConsoleFilePtr(ConsoleFile *rep) : Ogre::SharedPtr<ConsoleFile>(rep) {}
     ConsoleFilePtr(const ConsoleFilePtr &r) : Ogre::SharedPtr<ConsoleFile>(r) {}
     ConsoleFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<ConsoleFile>()
     {
         if( r.isNull() )
             return;
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<ConsoleFile*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
     }

     /// Operator used to convert a ResourcePtr to a TextFilePtr
     ConsoleFilePtr& operator=(const Ogre::ResourcePtr& r)
     {
         if(pRep == static_cast<ConsoleFile*>(r.getPointer()))
             return *this;
         release();
         if( r.isNull() )
             return *this; // resource ptr is null, so the call to release above has done all we need to do.
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<ConsoleFile*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
         return *this;
     }
 };
#endif // CONSOLEFILE_H
