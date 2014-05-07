#ifndef LOGFILESERIALIZER_H
#define LOGFILESERIALIZER_H

#include "OgreSerializer.h"
#include "OgreStringVector.h"

class LogFile;

class LogFileSerializer : public Ogre::Serializer
{
public:
    LogFileSerializer();
    virtual ~LogFileSerializer();

    void exportTextFile(const LogFile *pText, const Ogre::String &fileName);
    void importTextFile(Ogre::DataStreamPtr &stream, LogFile *pDest);
    std::string wordWrap( std::string str, size_t width);
    std::vector<std::string> pageWrap(std::string str,int pageSize);
};
#endif // LOGFILESERIALIZER_H
