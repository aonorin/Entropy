#ifndef PROGRAM_H
#define PROGRAM_H

#include "neuro/square.h"
#include <string>

namespace Ogre {
class OverlayElement;
}

namespace neuro {

static int programID = 10;

class Program
{
public:
    enum ProgramType{NIL=0,Scan=10,Glitch,Crack,Trojan,Daemon,Decrypt,Infect,Encrypt,HoneyPot,IPSpoof,Alarm,Crash,Errorz,BlackICE=666,EnemyCrack,EnemyInfect,Respawn};
    //enum ProgramType{NIL,Scan,Glitch,Crack,Trojan,Daemon,Decrypt,Infect,Encrypt,HoneyPot,IPSpoof,Alarm,Crash,ERROR,BlackICE,EnemyCrack,EnemyInfect,Respawn};
    Program(int programType, Vec2i programPos,int id);
    ~Program();
    float update();
    void update(float progress);
    float getSuspiciousness();
    float getProgress();
    int getID();
    Vec2i getPos();
    int getProgamType();
    void setupGUI();
    float getTime();
    static int getProgramID();
    void setTimedEventID(int timedEventID);
    void deleteResources();
private:
    bool resourcesDeleted;
    float rate;
    float time;
    float suspiciousness;
    float progress;
    int programType;
    Vec2i programPos;
    int id;
    bool guiSetup;
    Ogre::OverlayElement* progressBar;
    Ogre::OverlayElement* textArea;
    std::string programName;
    int timedEventID;
};

}

#endif // PROGRAM_H
