#pragma once

#include <QtGui>
#include <QMainWindow>
#include <QCheckBox>
#include <QAction>
#include <QMenuBar>
#include <QBoxLayout>
#include <QGridLayout>
#include <QStatusBar>
#include <QMessageBox>
#include "settingswindow.h"
#include <memory>
#include "CurvatureWindow.h"

class MainWindow: public QMainWindow {
    Q_OBJECT
protected:
    std::unique_ptr<SettingsWindow> settingsWindow;
    std::unique_ptr<QWidget> centralWidget;
    std::unique_ptr<QAction> exitAct;

    std::unique_ptr<QAction> settingsWindowAct;

    std::unique_ptr<QMenu> fileMenu;
    std::unique_ptr<QMenu> windowMenu;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event)
    {
        int ret = QMessageBox::warning(this, tr("Quit?"), tr("Quit application?"),
                                       QMessageBox::Yes |  QMessageBox::No, QMessageBox::No);
        if ( ret == QMessageBox::Yes  )
            QCoreApplication::exit(0);
        else
            event->ignore();
    }
    //
    void CreateActions();
    void CreateFileMenu();
    void CreateWindowMenu();

    void CreateMainWindow();
    void CreateStatusBar();
    void UpdateCurvatureGraph();
    void UpdateW0UI(float w0);
    void UpdateK0UI(float k0);
    void UpdateW3UI(float w3);
    void UpdateK1UI(float k1);
private slots:
    void ShowCurvatureWindow();

private:
    std::unique_ptr<CurvatureWindow> curvatureWindow;
    std::unique_ptr<QAction> curvatureWindowAct;

public slots:
    void Window2DCheckBoxChanged();
    void Window3DCheckBoxChanged();

    void ShowSettingsWindow();
};

extern MainWindow *mainWindow;


