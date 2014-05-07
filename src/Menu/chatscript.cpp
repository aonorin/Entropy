#include "chatscript.h"
//#include "libraries/ChatScript/src/common.h"
//#include "libraries/ChatScript/src/mainSystem.h"
#include <sys/stat.h>

#define MY_MASK 0777

ChatScript::ChatScript()
{
    start();
    //fsefsefsefsefs
}

void ChatScript::run()
{
    /*
#ifdef __APPLE__
    //lastLabel = 0;
    FILE* out1 = fopen("TMP/prebuild0","wb");
    if (!out1) {
        mkdir("TMP",0777);
    }
    fclose(out1);
    char** argz;
    char* readPath = "../Resources/resources/ChatScript/";
    char* writePath = "../Resources/resources/ChatScript/";
    InitSystem(1,argz,readPath,writePath);
    InitStandalone();
    botPrefix = "Virgil";
    userPrefix = "Curtis";
    bufferIndex = 2;
    currentFileLine = 0;
    printf("\r\nEnter user name: ");
    printf("\r\n");
    inBuffer = (char*)"Curtis";
    PerformChat(inBuffer,computerID,outBuffer,NULL,outBuffer); // unknown bot, no input,no ip
    entropyLoop();
#endif
*/
}

void ChatScript::entropyLoop()
{
    /*
#ifdef __APPLE__
    //local build has no exception handling.. only server build on LINUX does
retry:
#ifdef WIN32
    DWORD time = 0;
#endif
    //    while (1) {
    if (oktest)
    {
        printf("%s\r\n    ",outBuffer);
        if (oktest == OKTEST) strcpy(inBuffer,"OK");
        else strcpy(inBuffer,"why?");
    }
    else if (systemReset)
    {
        printf("%s\r\n",outBuffer);
        *computerID = 0;	// default bot
        *inBuffer = 0;	// restart conversation
    }
    else
    {
        // output bot response
        if (*botPrefix && !regression) printf("%s ",botPrefix);
        if (autonumber) printf("%d: ",inputCount);
        if (autotopic)
        {
            topic = GetInterestingTopic();
            if (topic) printf(" (%s) ",GetTopicName(topic));
        }
        if (!regression) printf("%s\r\n",outBuffer);

        //output user prompt
        if (regression);
        else if (*userPrefix) printf("%s ",userPrefix);
        else printf("   >");
    }
    bufferIndex = 2;		//   if we recovered from a crash, we may need to reset

    PerformChat(loginID,computerID,inBuffer,NULL,outBuffer); // no ip

    currentFileLine=0;
#endif
}

char* ChatScript::chat(std::string text)
{
#ifdef __APPLE__
    inBuffer = (char*)text.c_str();
    entropyLoop();
    //std::cout  << "TOPIC NAME HERE: " << GetTopicName(topic) << std::endl;
    //if(lastLabel!=0) {

    std::cout << std::endl << "LASTLABEL: " << lastLabel << std::endl;
    //}
    if(*lastLabel != 0) {
        if(chatActions.find(lastLabel) != chatActions.end()) {
            chatActions[lastLabel].call();
        }
    }

    return outBuffer;
#else
    return "";
#endif
*/
}

void ChatScript::start()
{
    running = true;
    thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&ChatScript::run, this)));
}

void ChatScript::stop()
{
    running = false;
    thread->join();
}

void ChatScript::addChatAction(std::string pattern, gameplay::EventAction eventAction)
{
//    ChatScript::getSingletonPtr()->chatActions[pattern] = eventAction;
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> ChatScript* Ogre::Singleton<ChatScript>::ms_Singleton = 0;
ChatScript* ChatScript::getSingletonPtr(void)
{
    return ms_Singleton;
}
ChatScript& ChatScript::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> ChatScript* Ogre::Singleton<ChatScript>::msSingleton = 0;
ChatScript* ChatScript::getSingletonPtr(void)
{
    return msSingleton;
}
ChatScript& ChatScript::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
