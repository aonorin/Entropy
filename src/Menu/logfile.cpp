#include "logfile.h"
#include "logfileserializer.h"

LogFile::LogFile(Ogre::ResourceManager *creator, const Ogre::String &name, Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader)
    : Ogre::Resource(creator, name, handle, group, isManual, loader)
{
    /* If you were storing a pointer to an object, then you would set that pointer to NULL here.
         */
    createParamDictionary("LogFile");
}

LogFile::~LogFile()
{
    unload();
}

void LogFile::loadImpl()
{
    LogFileSerializer serializer;
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
    serializer.importTextFile(stream, this);
}

void LogFile::unloadImpl()
{
    /* If you were storing a pointer to an object, then you would check the pointer here,
     and if it is not NULL, you would destruct the object and set its pointer to NULL again.
     */
    logText.clear();
}

size_t LogFile::calculateSize() const
{
    return logText.length();
}

void LogFile::setTextPages(const std::vector<std::string> &logPages)
{
    this->logPages = logPages;
}

const std::vector<std::string> &LogFile::getTextPages() const
{
    return logPages;
}

void LogFile::setText(const Ogre::String &logText)
{
    this->logText = logText;
}

const Ogre::String &LogFile::getText() const
{
    return logText;
}
