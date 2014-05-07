#ifndef LOGFILESERIALIZER_H
#define LOGFILESERIALIZER_H

#include "OGRE/OgreSerializer.h"

class Ogre::Serializer;
class LogFile;

class LogFileSerializer : public Ogre::Serializer
{
public:
    LogFileSerializer();
    virtual ~LogFileSerializer();

    void exportTextFile(const LogFile *pText, const Ogre::String &fileName);
    void importTextFile(Ogre::DataStreamPtr &stream, LogFile *pDest);
};
#endif // LOGFILESERIALIZER_H
