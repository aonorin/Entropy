#include "logfileserializer.h"
#include "logfile.h"
#include "Menu/menufuncs.h"
//#include "OgreStringVector.h"

LogFileSerializer::LogFileSerializer()
{
}

LogFileSerializer::~LogFileSerializer()
{
}

void LogFileSerializer::exportTextFile(const LogFile *pText, const Ogre::String &fileName)
{
    //     std::ofstream outFile;
    //     outFile.open(fileName.c_str(), std::ios::out);
    //     outFile << pText->getText();
    //     outFile.close();
}

void LogFileSerializer::importTextFile(Ogre::DataStreamPtr &stream, LogFile *pDest)
{
    pDest->setTextPages(pageWrap(wordWrap(stream->getAsString(),94),22));
}
