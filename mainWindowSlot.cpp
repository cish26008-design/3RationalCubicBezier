#include "mainWindow.h"
//#include "glMainWindow.h"

void MainWindow::Window2DCheckBoxChanged() {
}

void MainWindow::Window3DCheckBoxChanged() {

}

void MainWindow::ShowSettingsWindow() {
    if ( settingsWindow.get() == nullptr ) {
        settingsWindow.reset(new SettingsWindow);
    }
    settingsWindow->show();
}
