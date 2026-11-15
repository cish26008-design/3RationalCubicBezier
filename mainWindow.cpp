#include <QApplication>
#include "mainWindow.h"
#include "glMainWindow.h"
#include "shaderfile.h"
#include "settings.h"
#include "CurvatureWindow.h"
#include "CubicBezier.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    settingsWindow.reset(nullptr);

    centralWidget.reset(new QWidget);
    setCentralWidget(centralWidget.get());

    settingsWindow.reset(new SettingsWindow(this));
    curvatureWindow.reset(new CurvatureWindow(this));


    CreateActions();

    CreateFileMenu();

    CreateWindowMenu();


    CreateMainWindow();


    //w1スライダーとGL画面を繋ぐ
    connect(settingsWindow.get(), &SettingsWindow::weight1Changed,
            glMainWindow, &GLMainWindow::setWeight1);

    //w0スライダーとGL画面を繋ぐ
    connect(settingsWindow.get(), &SettingsWindow::weight0Changed,
            glMainWindow, &GLMainWindow::setWeight0);

    //w2スライダーとGL画面を繋ぐ
    connect(settingsWindow.get(), &SettingsWindow::weight2Changed,
            glMainWindow, &GLMainWindow::setWeight2);

    //w3スライダーとGL画面を繋ぐ
    connect(settingsWindow.get(), &SettingsWindow::weight3Changed,
            glMainWindow, &GLMainWindow::setWeight3);


    connect(settingsWindow.get(), &SettingsWindow::curvatureK0Changed,
            glMainWindow, &GLMainWindow::setCurvatureK0);

    connect(settingsWindow.get(), &SettingsWindow::curvatureK1Changed,
            glMainWindow, &GLMainWindow::setCurvatureK1);

    connect(settingsWindow.get(), &SettingsWindow::CombCheck,
            glMainWindow, &GLMainWindow::setCombCheck);

    connect(settingsWindow.get(), &SettingsWindow::CofCurvature,
            glMainWindow, &GLMainWindow::setCofcurvatureCheck);






#ifdef USE_GLWINDOW3D
    window3d->resize(512, 512);
#else
    glMainWindow->resize(512, 512);
#endif

    resize(1000, 1000);

#ifdef Q_WS_MAC // if using MacOS
    setUnifiedTitleAndToolBarOnMac(true);
#endif
}

MainWindow::~MainWindow() {
}

void MainWindow::CreateActions() {
    exitAct.reset(new QAction(tr("E&xit"), this));
    exitAct.get()->setShortcuts(QKeySequence::Quit);
    exitAct.get()->setStatusTip(tr("Exit the application"));
    connect(exitAct.get(), SIGNAL(triggered()), this, SLOT(close()), Qt::DirectConnection);

    settingsWindowAct.reset(new QAction(tr("Various Settings"), this));
    settingsWindowAct.get()->setStatusTip(tr("Settings Window"));
    connect(settingsWindowAct.get(), SIGNAL(triggered()), this, SLOT(ShowSettingsWindow()), Qt::DirectConnection);

    curvatureWindowAct.reset(new QAction(tr("Curvature Plot"), this));
    connect(curvatureWindowAct.get(), &QAction::triggered, [this]() {
        if (curvatureWindow) {
            curvatureWindow->show();
            curvatureWindow->raise(); // 前面に持ってくる
        }
    });
}

void MainWindow::CreateFileMenu() {
    fileMenu.reset(menuBar()->addMenu(tr("&File")));
    // fileMenu->setTearOffEnabled(true);
    fileMenu->addAction("&Quit", qApp, SLOT(quit()));		// qApp is defined in <QApplication>
    fileMenu->addAction(exitAct.get());


}



void MainWindow::CreateWindowMenu() {
    windowMenu.reset(menuBar()->addMenu(tr("Window")));
    windowMenu->addAction(settingsWindowAct.get());

    windowMenu->addAction(curvatureWindowAct.get());
}

void MainWindow::ShowCurvatureWindow() {
    if (curvatureWindow.get()) {
        curvatureWindow.get()->show();
        curvatureWindow.get()->raise(); // 前面に持ってくる
    }
}


void MainWindow::CreateMainWindow() {


    QBoxLayout *vboxLayout = new QVBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();

    vboxLayout->addLayout(gridLayout);

    int row = 0;

    glMainWindow = new GLMainWindow(512, 512, vertFile, fragFile);
    gridLayout->addWidget(glMainWindow, row, 0);
    row++;

//    window2DCheckBox.reset(new QCheckBox(tr("2D window"), this));
//    connect(window2DCheckBox.get(), SIGNAL(stateChanged(int)), this, SLOT(Window2DCheckBoxChanged()));
//    gridLayout->addWidget(window2DCheckBox.get(), row, 0);
//    row++;

//    window3DCheckBox.reset(new QCheckBox(tr("3D window"), this));
//    connect(window3DCheckBox.get(), SIGNAL(stateChanged(int)), this, SLOT(Window3DCheckBoxChanged()));
//    gridLayout->addWidget(window3DCheckBox.get(), row, 0);

    centralWidget.get()->setLayout(vboxLayout);

}

void MainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


void MainWindow::UpdateCurvatureGraph() {
    // もし曲率ウィンドウが存在していて、かつ画面に表示されていたら
    if (curvatureWindow && curvatureWindow->isVisible()) {
        curvatureWindow->updatePlotData(
            Bezier.curvatureX,
            Bezier.curvatureY,
            Bezier.K0max,
            Bezier.K1min
            );
    }
}

void MainWindow::UpdateW0UI(float w0) {
    if (settingsWindow) {

        settingsWindow->updateW0(w0);
    }
}

void MainWindow::UpdateK0UI(float k0) {
    if (settingsWindow) {
        settingsWindow->updateK0(k0);
    }
}

void MainWindow::UpdateW3UI(float w3) {
    if (settingsWindow) {

        settingsWindow->updateW3(w3);
    }
}

void MainWindow::UpdateK1UI(float k1) {
    if (settingsWindow) {
        settingsWindow->updateK1(k1);
    }
}


