#include <QtGui/QApplication>
#include "mainwindow.h"
#include "src/update.h"

#include <OgreEntity.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Degenerator::Update* update = new Degenerator::Update(&a,&w);

    //Ogre::Entity* ent = w.getOgreWidget()->getSceneManager()->createEntity("The Big Head", "ogrehead.mesh");
    //Ogre::SceneNode* node = w.getOgreWidget()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    //node->attachObject(ent);

    a.exec();
    update->stop();
    return 0;
}
