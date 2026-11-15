#pragma once

#include "glWindow.h"

class GLMainWindow: public GLWindow {
public:
    GLMainWindow(int width, int height, const QString &vertexFile, const QString &fragmentFile, int win3D = 0, QOpenGLWidget *parent = 0):
        GLWindow(width, height, vertexFile, fragmentFile, win3D, parent) {
        ctrptMoving = -1;
    }
    ~GLMainWindow() {}
//    void initializeGL();
//    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent* event);
    void drawBezier();
    bool showComb = false;
    bool showCircle = false;

public slots:
    void setWeight0(float w);
    void setWeight1(float w);
    void setWeight2(float w);
    void setWeight3(float w);
    void setCurvatureK0(float targetK0);
    void setCurvatureK1(float targetK1);
    void setCombCheck(bool visible);
    void setCofcurvatureCheck(bool vusuable);

protected:
    int ctrptMoving;
};

extern GLMainWindow *glMainWindow;

