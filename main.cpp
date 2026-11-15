//
// Ver1.01   std::unique_ptr is used
//
//

#include <QApplication>
#include "mainWindow.h"
#include "shaderfile.h"
#include <QFile>

MainWindow *mainWindow;

int main(int argc, char *argv[])
{
    if ( !QFile::exists(vertFile) ) {
        qDebug() << "- Error --";
        qDebug() << vertFile << " does not exists. Modify shaderfile.h";
        exit(-1);
    }
    if ( !QFile::exists(fragFile) ) {
        qDebug() << "- Error --";
        qDebug() << fragFile << " does not exists. Modify shaderfile.h";
        exit(-1);
    }


    QApplication a(argc, argv);	// the starting line of main() should be always like this

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setVersion(4, 5);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setOption(QSurfaceFormat::DebugContext);    // remark this line to disalbe OpenGL debugging
    fmt.setSamples(4); //4x MSAA surface format for antialiasing
    QSurfaceFormat::setDefaultFormat(fmt);

	mainWindow = new MainWindow;
	mainWindow->show();

	return a.exec();			// the end line of main() should be always like this
}
