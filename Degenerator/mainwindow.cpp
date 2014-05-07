#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "boost/function.hpp"
#include "src/update.h"
#include "algorithms/testalgorithm.h"
#include "algorithms/corruptedsphere.h"
#include "algorithms/flockingmesh.h"
#include "algorithms/strangemesh.h"
#include "algorithms/lsystemmesh.h"
#include "algorithms/wolframcamesh.h"
#include "algorithms/gameoflifemesh.h"

bool yes = false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isClosing(false),
    mRoot(0)
{
    ui->setupUi(this);
    int width = QApplication::desktop()->screenGeometry().width();
    int height = QApplication::desktop()->screenGeometry().height();

    //Main Menu
    window()->showMaximized();
    window()->setFixedHeight(50);
    window()->setFixedWidth(width*0.5f);
    window()->move(width*0.25,0);

    ogre = new Degenerator::OgreWidget(this);
    //ogre2 = new Degenerator::OgreWidget();
    yes = true;

    //File Menu
    QKeySequence seq(tr("Ctrl+N"));
    ui->menuFile->addAction("New Project",this,SLOT(newFile()),seq);

    seq = QKeySequence(tr("Ctrl+O"));
    ui->menuFile->addAction("Open Project",this,SLOT(openFile()),seq);

    seq = QKeySequence(tr("Ctrl+S"));
    ui->menuFile->addAction("Save Project",this,SLOT(saveFile()),seq);

    //Edit Menu
    seq = QKeySequence(tr("Ctrl+R"));
    ui->menuEdit->addAction("Render Mesh",this,SLOT(renderMesh(bool)),seq);

    seq = QKeySequence(tr("Ctrl+E"));
    ui->menuEdit->addAction("Export Mesh",this,SLOT(exportMesh(bool)),seq);


    //Object Browser
    ui->ObjectBrowser->setFixedWidth(width*0.2);
    ui->ObjectBrowser->setFixedHeight(height*0.6);
    ui->ObjectBrowser->move(width*0.0,height*0.2);
    ui->toolBox->setItemText(0,"Algorithms");
    ui->toolBox->setItemText(1,"Materials");
    ui->toolBox->setItemText(2,"Lighting");
    ui->toolBox->setItemText(3,"Shadows");

    ui->toolBox->setFixedWidth(width*0.2f);

    //Algorithms
    algorithms["Test Algorithm"] = new Degenerator::TestAlgorithm();
    algorithms["Corrupted Sphere"] = new Degenerator::CorruptedSphere();
    algorithms["Flocking"] = new Degenerator::FlockingMesh();
    algorithms["Strange Attractor"] = new Degenerator::StrangeMesh();
    algorithms["LSystem Fractal"] = new Degenerator::LSystemMesh();
    algorithms["Wolfram CA"] = new Degenerator::WolframCAMesh();
    algorithms["Wolfram CA 3D"] = new Degenerator::WolframCA3DMesh();
    algorithms["Game Of Life"] = new Degenerator::GameOfLifeMesh();
    algorithms["Game Of Life 3D"] = new Degenerator::GameOfLife3DMesh();

    ui->AlgorithmsBox->addItem("Test Algorithm");
    ui->AlgorithmsBox->addItem("Corrupted Sphere");
    ui->AlgorithmsBox->addItem("Flocking");
    ui->AlgorithmsBox->addItem("Strange Attractor");
    ui->AlgorithmsBox->addItem("LSystem Fractal");
    ui->AlgorithmsBox->addItem("Wolfram CA");
    ui->AlgorithmsBox->addItem("Wolfram CA 3D");
    ui->AlgorithmsBox->addItem("Game Of Life");
    ui->AlgorithmsBox->addItem("Game Of Life 3D");

    ui->AlgorithmsBox->setFixedWidth(width*0.19);
    ui->AlgorithmParams->setFixedWidth(width*0.2);

    //Materials
    ui->MaterialBox->setFixedWidth(width*0.2);

    //Object Editor
    ui->ObjectEditor->setFixedWidth(width*0.2);
    ui->ObjectEditor->setFixedHeight(height*0.6);
    ui->ObjectEditor->move(width*0.8,height*0.2);
    ui->toolBox_2->setItemText(0,"Edit Object");
    ui->toolBox_2->setItemText(1,"Meta Data");
    ui->toolBox_2->setItemText(2,"Physics");
    ui->toolBox_2->setItemText(2,"Scripts");
    ui->toolBox_2->setFixedWidth(width*0.2f);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{

}

void MainWindow::on_actionExit_triggered()
{
    //close();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    bool alt = e->modifiers().testFlag(Qt::AltModifier);
    if (e->key() == Qt::Key_F11 && alt)
    {
        if (this->isFullScreen())
            this->showNormal();
        else
            this->showFullScreen();
    }
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    std::cout << "Resize Event " << e->size().width() << ", " << e->size().height() << std::endl;
    std::cout << "Sending to ogre\n";
    //ogre->resize(ui->Render->size());
    //ogre2->resize(ui->Render->size());
}

void MainWindow::on_actionStart_Rendering_triggered()
{
    std::cout << "Initializing OGRE\n";

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    //CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    char cCurrentPath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(mainBundleURL, TRUE, (UInt8 *)cCurrentPath, PATH_MAX))
    {
        std::cerr << "Entropy::BaseApplication::go - CURRENT DIRECTORY NOT FOUND." << std::endl;
        return;
    }

    CFRelease(mainBundleURL);
    std::string workingDirectory(cCurrentPath);
    workingDirectory.append("/Contents/MacOS/");
    chdir(workingDirectory.c_str());

    std::string resourcePath(workingDirectory);
    resourcePath.append("../Resources/entropy_resources.cfg");
    std::string pluginPath(workingDirectory);
    pluginPath.append("../Resources/plugins.cfg");
    std::cout << "Current working directory: " << workingDirectory << std::endl;
    std::cout << "Resources path: " << resourcePath << std::endl;

    mRoot = new Ogre::Root(pluginPath);
    mTimer = new Ogre::Timer();
    mTimer->reset();

    ogre->go(mRoot);
    //ogre->resize(ui->Render->size());
    //ogre->repaint();

    //ogre2->go(mRoot, ogre->getSceneManager());
    //ogre2->resize(ui->Render->size());
    //ogre2->repaint();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    isClosing = true;
    delete mRoot;
}

Ogre::Root* MainWindow::getRoot() {
    return mRoot;
}

Degenerator::OgreWidget* MainWindow::getOgreWidget() {
    return ogre;
}

Ui::MainWindow* MainWindow::getUI() {
    return ui;
}

std::string MainWindow::getMaterialName()
{
    return ui->MaterialBox->document()->toPlainText().toStdString();
}

Degenerator::Algorithm* MainWindow::getAlgorithm()
{
    return algorithms[ui->AlgorithmsBox->currentText().toStdString()];
}

Degenerator::AStruct MainWindow::generateMesh()
{
    if(!ui->AlgorithmParams->document()->isEmpty()) {
        QStringList params = ui->AlgorithmParams->document()->toPlainText().split(",");
        std::vector<std::string> stdParams;
        for(int i=0;i<params.size();i++) {
            stdParams.push_back(params.at(i).toStdString());
        }
        return getAlgorithm()->generateMesh(stdParams);
    } else {
        std::vector<std::string> stdParams;
        return getAlgorithm()->generateMesh(stdParams);
    }

}

void MainWindow::renderOgre() {
    if (mRoot)
    {
        int timeSinceLastFrame = 1;
        int startTime = 0;

        startTime = mTimer->getMillisecondsCPU();
        mRoot->renderOneFrame();
        timeSinceLastFrame = mTimer->getMillisecondsCPU() - startTime;

        ogre->updateOgre( timeSinceLastFrame / 1000.0f);
        //ogre2->updateOgre( timeSinceLastFrame / 1000.0f);

        if(ogre->getMeshNode()!=0) {
            ogre->getSceneManager()->getEntity("DegeneratorObject")->getParentSceneNode()->yaw(Ogre::Radian(5 * (timeSinceLastFrame / 1000.0f)));
        }
        //ogre->repaint();
        //ogre2->repaint();
    }
}

//SLOTS
void MainWindow::renderMesh(bool checked) {
    boost::function<void()> func = boost::bind(&Degenerator::OgreWidget::renderMesh,ogre);
    Degenerator::Update::getSingletonPtr()->addQueuedAction(func);
}

void MainWindow::exportMesh(bool checked)
{
    fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Mesh"), "",
                                                    tr("Mesh File (*.mesh);;All Files (*)"));
    boost::function<void()> func = boost::bind(&Degenerator::OgreWidget::exportMesh,ogre);
    Degenerator::Update::getSingletonPtr()->addQueuedAction(func);
}

void MainWindow::newFile()
{
    ui->MaterialBox->document()->clear();
}

void MainWindow::openFile()
{
    QString fileString;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Degenerator Project"), "",
                                                    tr("Degenerator Project (*.dgn);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);
        //        contacts.empty();   // empty existing contacts
        in >> fileString;
        if (fileString.isEmpty()) {
            QMessageBox::information(this, tr("No data in file"),
                                     tr("The file you are attempting to open contains no data."));
        } else {
            unserializeState(fileString);
        }
    }
}

void MainWindow::saveFile()
{
//    std::string saveString(getMaterialName());

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Degenerator File"), "",
                                                    tr("Degenerator File (*.dgn);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);
        out << serializeState();
    }
}

QString MainWindow::serializeState()
{
    QString saveState;
    saveState.append(QString().setNum(ui->AlgorithmsBox->currentIndex()));
    saveState.append("*");
    saveState.append(ui->AlgorithmParams->document()->toPlainText());
    saveState.append("*");
    saveState.append(QString().fromStdString(getMaterialName()));
    saveState.append("*");
    std::cout << "Save state: " << saveState.toStdString() << std::endl;
    return saveState;
}

void MainWindow::unserializeState(QString savedState)
{
    QStringList state = savedState.split("*");
    ui->AlgorithmsBox->setCurrentIndex(state.at(0).toInt());
    ui->AlgorithmParams->document()->setPlainText(state.at(1));
    ui->MaterialBox->document()->setPlainText(state.at(2));
}
