#include "update.h"

namespace Degenerator {

Update::Update(QApplication *app, MainWindow *mw) :
    mw(mw),
    app(app),
    paused(false)
{

    start();
}

Update::~Update()
{
    stop();
}

void Update::run()
{
    mw->on_actionStart_Rendering_triggered();
    while(running) {
        if(!queuedFunctions.isEmpty()) {
            for(int i=0;i<queuedFunctions.size();i++) {
                queuedFunctions.at(i)();
            }
            if(queuedFunctions.size()>0) {
                queuedFunctions.clear();
            }
        }
        app->processEvents();
        mw->renderOgre();
        boost::this_thread::sleep(boost::posix_time::milliseconds(15));
    }
}

void Update::start()
{
    running = true;
    thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Update::run, this)));
}

void Update::stop()
{
    running = false;
    thread->join();
}

void Update::pause(){
    paused = true;
}

void Update::unpause()
{
    paused = false;
}

void Update::addQueuedAction(boost::function<void()> func)
{
    queuedFunctions.push_back(func);
}

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> Update* Ogre::Singleton<Update>::ms_Singleton = 0;
Update* Update::getSingletonPtr(void)
{
    return ms_Singleton;
}
Update& Update::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

#else
template<> Update* Ogre::Singleton<Update>::msSingleton = 0;
Update* Update::getSingletonPtr(void)
{
    return msSingleton;
}
Update& Update::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif

}
