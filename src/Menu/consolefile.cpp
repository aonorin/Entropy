#include "consolefile.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>

ConsoleFile::ConsoleFile(Ogre::ResourceManager *creator, const Ogre::String &name, Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader)
    : Ogre::Resource(creator, name, handle, group, isManual, loader)
{
    /* If you were storing a pointer to an object, then you would set that pointer to NULL here.
         */
    createParamDictionary("ConsoleFile");
}

ConsoleFile::ConsoleFile(std::string name)
{
    mName = name;
}

ConsoleFile::~ConsoleFile()
{
    unload();
}

void ConsoleFile::loadImpl()
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, mGroup, true, this);
    consoleFileText = stream->getAsString();
    boost::split(consoleLines,consoleFileText, boost::is_any_of(">>>>"));
}

void ConsoleFile::load()
{
    std::ifstream myfile;
    std::string line;
    myfile.open (mName.c_str());
    while ( myfile.good() )
    {
        std::getline(myfile,line);
        consoleLines.push_back(line);
    }
    myfile.close();
}

void ConsoleFile::save()
{
    std::ofstream outFile;
    outFile.open(mName.c_str(), std::ios::out);
    for(int i=0;i<consoleLines.size();i++) {
//        std::cout << "SAVING LINE: " << consoleLines.at(i) << std::endl;
        outFile << consoleLines.at(i);
        outFile << "\n";
    }
    outFile.close();
}

void ConsoleFile::clearLines()
{
    if(consoleLines.size()>0)
    {
        consoleLines.clear();

    }
}

void ConsoleFile::addLine(std::string line)
{
    consoleLines.push_back(line);
    if(consoleLines.size()>100) {
        consoleLines.erase(consoleLines.end());
    }
}

void ConsoleFile::unloadImpl()
{
    /* If you were storing a pointer to an object, then you would check the pointer here,
     and if it is not NULL, you would destruct the object and set its pointer to NULL again.
     */
    consoleFileText.clear();
}

std::vector<std::string> ConsoleFile::getLines()
{
    return consoleLines;
}

size_t ConsoleFile::calculateSize() const
{
    return consoleFileText.length();
}

void ConsoleFile::setText(const Ogre::String &consoleFileText)
{
    this->consoleFileText = consoleFileText;
}

const Ogre::String &ConsoleFile::getText() const
{
    return consoleFileText;
}
