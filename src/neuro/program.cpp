#include "program.h"
#include "src/Core/terminal.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include "src/Core/Entropy.h"
#include "src/Core/update.h"
#include <OGRE/Ogre.h>

namespace neuro {

Program::Program(int programType, Vec2i programPos, int id) :
    programType(programType),
    programPos(programPos),
    progress(0),
    guiSetup(false),
    timedEventID(0),
    resourcesDeleted(false)
{
    this->id = id;
    float speedMul = 0.4;
    switch(programType) {
    //EnemyPort
    case Scan:
        suspiciousness = 0.01;
        time = 750*speedMul;
        programName = "Scan";
        break;
    case Glitch:
        suspiciousness = 0.01;
        time = 1000*speedMul;
        programName = "Glitch";
        break;
    case Crack:
        suspiciousness = 0.1;
        time = 20000*speedMul;
        programName = "Crack";
        break;

        //Friendly Port
    case Trojan:
        suspiciousness = 0.01;
        time = 8000*speedMul;
        programName = "Trojan";
        break;
    case Alarm:
        suspiciousness = 0.025;
        time = 2000*speedMul;
        programName = "Alarm";
        break;
    case Daemon:
        suspiciousness = 0.025;
        time = 8000*speedMul;
        programName = "Daemon";
        break;
    case BlackICE:
        suspiciousness = 0.01;
        time = 30000*speedMul;
        programName = "Black-ICE";
        break;

        //Enemy Node
    case Decrypt:
        suspiciousness = 0.05;
        time = 3000*speedMul;
        programName = "Decrypt";
        break;
    case Infect:
        suspiciousness = 0.15;
        time = 20000*speedMul;
        programName = "Infect";
        break;

        //Friendly Node
    case HoneyPot:
        suspiciousness = 0.075;
        time = 15000*speedMul;
        programName = "Honey Pot";
        break;
    case Encrypt:
        suspiciousness = -0.25;
        time = 4000*speedMul;
        programName = "Encrypt";
        break;
    case Crash:
        suspiciousness = 1.0;
        time = 750*speedMul;
        programName = "Crash";
        break;

        //OTHER
    case Errorz:
        suspiciousness = 0.0;
        time = 500*speedMul;
        programName = "ERROR";
        break;

        //Enemy
    case EnemyCrack:
        suspiciousness = 0.0;
        time = 10000*speedMul;
        programName = "Crack";
        break;
    case EnemyInfect:
        suspiciousness = 0.0;
        time = 10000*speedMul;
        programName = "Infect";
        break;
    case Respawn:
        suspiciousness = 0.0;
        time = 90000*speedMul;
        programName = "Respawn";
        break;
    }
    boost::function<void (void)> func = boost::bind(&Program::setupGUI,this);
    Entropy::getSingletonPtr()->addQeueudFunction(func);
}

void Program::setupGUI()
{
    try{
        Terminal* terminal = Terminal::getSingletonPtr();

        Ogre::MaterialPtr healthMat = Ogre::MaterialManager::getSingleton().create("healthMat2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        healthMat->getTechnique(0)->getPass(0)->createTextureUnitState("HeatLookup.tga");
        healthMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        healthMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        healthMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);

        progressBar = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("Panel","program"+boost::lexical_cast<std::string>(this->id));
        progressBar->setMetricsMode(Ogre::GMM_PIXELS);
        progressBar->setPosition(programPos.x*terminal->pixelSize,programPos.y*terminal->pixelSize);
        progressBar->setWidth(0.01);
        progressBar->setHeight(terminal->pixelSize*0.075);
        progressBar->setParameter("uv_coords","0.4 0.4 0.4 0.4");
        progressBar->setMaterialName("healthMat2");
        Terminal::getSingletonPtr()->_panel3->addChild(progressBar);

        textArea = Ogre::OverlayManager::getSingletonPtr()->createOverlayElement("TextArea","programT"+boost::lexical_cast<std::string>(this->id));
        textArea->setDimensions(terminal->pixelSize,terminal->pixelSize);
        textArea->setMetricsMode(Ogre::GMM_PIXELS);
        textArea->setPosition(programPos.x*terminal->pixelSize+ (0.5*terminal->pixelSize), programPos.y*terminal->pixelSize + (0.5*terminal->pixelSize));
        textArea->setParameter("font_name", "bluehigh");
        textArea->setParameter("char_height", "40");
        textArea->setParameter("alignment", "center");
        textArea->setColour(Ogre::ColourValue(1,1,1,0.75));
        textArea->setCaption(programName);
        Terminal::getSingletonPtr()->_panel3->addChild(textArea);
        guiSetup = true;
    } catch(...) {
        //        std::cout << "WTF ERROR IN PROGRAM????" << std::endl;
    }
}

Program::~Program()
{
    deleteResources();
}

void Program::deleteResources()
{
    try{
        if(!resourcesDeleted) {
            resourcesDeleted = true;
            if(timedEventID!=0) {
                Update::getSingletonPtr()->removeEvent(timedEventID);
            }
            progressBar->hide();
            textArea->hide();
//            std::cout << "DELETING PROGRAM: "<<programName << std::endl;
            Terminal::getSingletonPtr()->_panel3->removeChild("program"+boost::lexical_cast<std::string>(this->id));
            Terminal::getSingletonPtr()->_panel3->removeChild("programT"+boost::lexical_cast<std::string>(this->id));
            Entropy::getSingletonPtr()->addQeueudDeleteOverlays(progressBar);
            Entropy::getSingletonPtr()->addQeueudDeleteOverlays(textArea);
        }
    } catch(...) {

    }
}

float Program::getProgress()
{
    return progress;
}

float Program::update()
{
    //    progress = std::min(progress+rate,1.0f);
    progress += rate;
    return progress;
}

void Program::update(float progress)
{
    this->progress = progress;
    if(progress*Terminal::getSingletonPtr()->pixelSize > 0) {
        if(guiSetup) {
            try{
                progressBar->setWidth(progress*Terminal::getSingletonPtr()->pixelSize);
            } catch(...) {
                //                std::cout << "WTF ERROR IN UPDATE????" << std::endl;
            }
        }
    }
}

float Program::getSuspiciousness()
{
    return suspiciousness;
}

Vec2i Program::getPos()
{
    return programPos;
}

int Program::getID()
{
    return this->id;
}

int Program::getProgamType()
{
    return programType;
}

float Program::getTime()
{
    return time;
}

int Program::getProgramID() {
    return programID++;
}

void Program::setTimedEventID(int timedEventID)
{
    //    std::cout << "Setting TimedEventID: " << timedEventID << std::endl;
    this->timedEventID = timedEventID;
}

}

