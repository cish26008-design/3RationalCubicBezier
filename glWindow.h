#pragma once

#define USE_OPENGL_DEBUG_LOGGER     // remark this line to disable OpenGL Debugging
#ifdef USE_OPENGL_DEBUG_LOGGER
#include <QOpenGLDebugLogger>
#endif

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "axis.h"
#include "glWinsc.h"
#include "glWinsub3d.h"

class GLWindow : public QOpenGLWidget, protected QOpenGLExtraFunctions,
                 public glWinSc, public GLWinsub3D {
    Q_OBJECT	// when you inherits QGLWidget you always need to write Q_OBJECT in the class
public:
    GLWindow(int width, int height, const QString &vertexFile, const QString &fragmentFile, int win3D = 0, QOpenGLWidget *parent = 0);
    ~GLWindow() {
        delete axis;
        delete shaderProgram;
    }
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent* event);
    void setCurrentMovingPoint(int index){ m_currentMovingPt = index;}

protected:
    int mode3D;     // 0: 2D mode, 1: 3D mode
    int m_currentMovingPt = -1;

    // DrawObject
    Axis3D *axis;

    // Shader-related
    QOpenGLExtraFunctions *qglf;
    QOpenGLShaderProgram *shaderProgram;
    QString vertexShaderFile;
    QString fragmentShaderFile;
    GLint modelviewLoc;
    GLint projectionLoc;
    GLint colorLoc;

#ifdef USE_OPENGL_DEBUG_LOGGER
protected:
    QOpenGLDebugLogger *m_logger;
public slots:
    void onMessageLogged( QOpenGLDebugMessage message );
#endif
};


