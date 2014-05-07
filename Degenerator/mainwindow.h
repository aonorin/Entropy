#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/ogrewidget.h"
#include "algorithm.h"

namespace Degenerator {
class OgreWidget;
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isClosing;
    void renderOgre();

    Degenerator::OgreWidget* renderWidget() { return ogre; }
    Ogre::Root* getRoot();
    Degenerator::OgreWidget* getOgreWidget();
    Ui::MainWindow* getUI();
    std::string getMaterialName();
    std::map<std::string,Degenerator::Algorithm*> algorithms;
    Degenerator::Algorithm* getAlgorithm();
    Degenerator::AStruct generateMesh();
    QString fileName;

public slots:
    void on_actionExit_triggered();
    void on_actionStart_Rendering_triggered();

    void renderMesh(bool checked);
    void exportMesh(bool checked);
    void newFile();
    void openFile();
    void saveFile();

private:
    Ui::MainWindow *ui;
    Ogre::Root*	    mRoot;
    Degenerator::OgreWidget*  ogre;
    Degenerator::OgreWidget*  ogre2;
    Ogre::Timer*    mTimer;

    Ogre::String	mResourcesCfg;
    Ogre::String	mPluginsCfg;

    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);

    QString serializeState();
    void unserializeState(QString savedState);
};

#endif // MAINWINDOW_H
