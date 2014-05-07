#ifndef UPDATE_H
#define UPDATE_H

#include "boost/thread.hpp"
#include "ogrewidget.h"
#include "mainwindow.h"
#include "QApplication"
#include "boost/function.hpp"
#include "OgreSingleton.h"

namespace Degenerator {

class Update : public Ogre::Singleton<Update>
{
public:
    Update();
    Update(QApplication* app,MainWindow* mw);
    ~Update();
    void start();
    void stop();
    void pause();
    void unpause();
    void addQueuedAction(boost::function<void()> func);
    static Update& getSingleton(void);
    static Update* getSingletonPtr(void);

private:
    bool paused;
    void run();
    bool running;
    QApplication* app;
    MainWindow* mw;
    boost::shared_ptr<boost::thread> thread;
    QList<boost::function<void()> > queuedFunctions;
};

}

#endif // UPDATE_H
