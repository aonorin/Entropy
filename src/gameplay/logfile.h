#ifndef LOGFILES_H
#define LOGFILES_H

#include <OgreResourceManager.h>

class LogFile : public Ogre::Resource
{
public:
    LogFile(Ogre::ResourceManager *creator, const Ogre::String &name,
            Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
            Ogre::ManualResourceLoader *loader = 0);
    virtual ~LogFile();

    void setText(const Ogre::String &logText);
    const Ogre::String &getText() const;

    void setTextPages(const std::vector<std::string> &logPages);
    const std::vector<std::string> &getTextPages() const;

protected:
     // must implement these from the Ogre::Resource interface
     void loadImpl();
     void unloadImpl();
     size_t calculateSize() const;

private:
     Ogre::String logText;
     std::vector<std::string> logPages;
};

class LogFilePtr : public Ogre::SharedPtr<LogFile>
 {
 public:
     LogFilePtr() : Ogre::SharedPtr<LogFile>() {}
     explicit LogFilePtr(LogFile *rep) : Ogre::SharedPtr<LogFile>(rep) {}
     LogFilePtr(const LogFilePtr &r) : Ogre::SharedPtr<LogFile>(r) {}
     LogFilePtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<LogFile>()
     {
         if( r.isNull() )
             return;
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<LogFile*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
     }

     /// Operator used to convert a ResourcePtr to a TextFilePtr
     LogFilePtr& operator=(const Ogre::ResourcePtr& r)
     {
         if(pRep == static_cast<LogFile*>(r.getPointer()))
             return *this;
         release();
         if( r.isNull() )
             return *this; // resource ptr is null, so the call to release above has done all we need to do.
         // lock & copy other mutex pointer
         OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
         OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
         pRep = static_cast<LogFile*>(r.getPointer());
         pUseCount = r.useCountPointer();
         useFreeMethod = r.freeMethod();
         if (pUseCount)
         {
             ++(*pUseCount);
         }
         return *this;
     }
 };

#endif // LOGFILES_H
