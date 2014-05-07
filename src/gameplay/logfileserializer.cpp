#include "logfileserializer.h"
#include "logfile.h"
#include <boost/algorithm/string.hpp>

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
    //     pDest->setText(wordWrap(stream->getAsString(),95));
    pDest->setTextPages(pageWrap(wordWrap(stream->getAsString(),94),22));
}

std::string LogFileSerializer::wordWrap( std::string str, size_t width ) {
    size_t curWidth = width;
    while( curWidth < str.length() ) {
        std::string::size_type spacePos = str.rfind( ' ', curWidth );
        if( spacePos == std::string::npos )
            spacePos = str.find( ' ', curWidth );
        if( spacePos != std::string::npos ) {
            str[ spacePos ] = '\n';
            curWidth = spacePos + width + 1;
        }
    }
    return str;
}

std::vector<std::string> LogFileSerializer::pageWrap(std::string str, int pageSize)
{
    std::vector<std::string> pages;
    std::vector<std::string> strs;
    boost::split(strs,str, boost::is_any_of("\n"));
    int curPage = 0;
    int lineNum = 0;
    for(int i=0;i<strs.size();i++) {
        if(lineNum>=pageSize) {
            curPage++;
            lineNum = 0;
            std::string s("");
            pages.push_back(s);
        } else if(lineNum==0){
            std::string s("");
            pages.push_back(s);
            lineNum++;
        } else {
            lineNum++;
        }
        pages.at(curPage).append(strs.at(i));
        pages.at(curPage).append("\n");
    }

    return pages;
}
