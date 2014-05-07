#pragma once

#include "boost/thread/thread.hpp"
// Ogre includes
#include <OGRE/OgreSingleton.h>
#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif
#include "src/gameplay/EventAction.h"

class ChatScript : public Ogre::Singleton<ChatScript>
{
public:
    ChatScript();
    void start();
    void stop();
    char* chat(std::string text);
    static ChatScript& getSingleton(void);
    static ChatScript* getSingletonPtr(void);
    std::tr1::unordered_map<std::string,gameplay::EventAction> chatActions;
    static void addChatAction(std::string pattern,gameplay::EventAction eventAction);

private:
    char* botPrefix;
    char* userPrefix;
    int bufferIndex;
    FILE* inputFile;
    void parseChat();
    void entropyLoop();
    bool running;
    void run();
    unsigned int topic;
    boost::shared_ptr<boost::thread> thread;
};
