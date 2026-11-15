#pragma once

#include <iostream>
#include <QMouseEvent>
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "geoObject.h"

const double glWindow3DOrgDist = 10.0;

static void FitValidDegreeRange(double &deg) {
    while( deg < 0.0 ) deg += 360.0;
    while( deg > 360.0) deg -= 360.0;
}

class GLWinsub3D  {
public:
    GLWinsub3D() {
        SetDefaultEyePosition();
        mouseEvent = -1;
        pressedKey = -1;
    }
    ~GLWinsub3D() {

    }
    void SetDefaultEyePosition() {
        dist = glWindow3DOrgDist;
        theta1 = 0.;       // 15
        theta2 = 90.;       // 70
    }
    void ComputeEyeCoordinates() {
        eyeY = dist * sin((double)theta1/180.0*pi);
        double l = dist * cos((double)theta1/180.0*pi);
        eyeX = l * cos((double)theta2/180.0*pi);
        eyeZ = l * sin((double)theta2/180.0*pi);
    }
    void ComputeProjectionMatrix(int width, int height, glm::mat4 &view, glm::mat4 &projection) {
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        projection = glm::perspective(glm::radians(20.0f), aspect, 0.01f, 100.0f);

        ComputeEyeCoordinates();
        glm::vec3 eyePos = glm::vec3(static_cast<float>(eyeX), static_cast<float>(eyeY), static_cast<float>(eyeZ));
        glm::vec3 eyeFront = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector;
        if ( theta1 > 90.0 && theta1 < 270.0 ) {
            upVector = glm::vec3(0.0f, -1.0f, 0.0f);
        }
        else {
            upVector = glm::vec3(0.0f, 1.0f, 0.0f);

        }
        view = glm::lookAt(eyePos, eyeFront, upVector);
    }
    glm::vec3 UnProject(const glm::dvec3 &win, int useZcoord) {
        if (useZcoord) {
            return unProject(win, cmodelView, cproj, cviewport);
        }
        // project the origin, and get the depth
        glm::vec3 z = glm::project(glm::dvec3(0.0, 0.0, 0.0), cmodelView, cproj, cviewport);
        return unProject(glm::dvec3(win[0], win[1], z[2]), cmodelView, cproj, cviewport);
    }
    glm::dvec3 Project(const glm::dvec3 &sc) {
        return glm::project(sc, cmodelView, cproj, cviewport);
    }
    void MousePressed(QMouseEvent * e) {
        if ( (e->button() == Qt::LeftButton && pressedKey == Qt::Key_Shift) || (e->button() == Qt::RightButton) ) {
            savedX = e->position().x();
            savedY = e->position().y();
            savedDist = dist;
            mouseEvent = 1;
        }
        else if ( e->button() == Qt::LeftButton ) {
            savedX = e->position().x();
            savedY = e->position().y();
            savedTheta1 = theta1;
            savedTheta2 = theta2;
            mouseEvent = 2;
        }
    }
    void MouseReleased(QMouseEvent *) {
        mouseEvent = -1;
    }
    int MouseMove(QMouseEvent *e) {
        int x = e->position().x();
        int y = e->position().y();
        if ( mouseEvent == 1 ) {
            double f = (y - savedY) * 0.05;
            double distTemp = savedDist + glWindow3DOrgDist * f;
            if ( distTemp > 0 ) dist = distTemp;
            return 1;   // request update()
        }
        else if ( mouseEvent == 2 ) {
            theta1 = savedTheta1 - (-(y - savedY) * 0.4);
            theta2 = savedTheta2 - (-(x - savedX) * 0.4);
            FitValidDegreeRange(theta1);
            FitValidDegreeRange(theta2);
            return 1;   // request update()
        }
        return 0;
    }
protected:
    int mouseEvent;

    double dist;
    double orgDist;
    double theta1, theta2;
    double eyeX, eyeY, eyeZ;
    double savedTheta1, savedTheta2;
    double savedX, savedY;
    double savedDist;
    int pressedKey;
public:
    glm::dmat4 cmodelView;
    glm::dmat4 cproj;
    glm::ivec4 cviewport;

};
