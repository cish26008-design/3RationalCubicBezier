#pragma once

#include <vector>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // value_ptr
#include <QOpenGLFunctions_3_3_Core>
#include "globject.h"

const double pi = 3.1415926535897932385;

class CirclePoint {
    GLuint div;
    GLObject2D circlePointObj;
    QOpenGLExtraFunctions *qglf;
public:
    CirclePoint(const GLuint divArg = 10) {
        div = divArg;
    }
    CirclePoint(QOpenGLExtraFunctions *f, const GLuint divArg = 10) {
        SetOpenGLFucntions(f);
        div = divArg;
    }
    void SetOpenGLFucntions(QOpenGLExtraFunctions *f) {
        qglf = f;

        std::vector<glm::vec2> pt;
        pt.resize(div);
        for(GLuint i = 0; i < div; i++) {
            double rad = 2.0 * pi * static_cast<double>(i) / static_cast<double>(div);
            pt[i][0] = static_cast<GLfloat>(cos(rad));
            pt[i][1] = static_cast<GLfloat>(sin(rad));
        }
        pt.resize(div+1);
        pt[div] = glm::vec2(0.0f, 0.0f);

        std::vector<int> indicies;
        indicies.resize(div * 3);
        for(GLuint i = 0; i < div; i++) {
            indicies[3*i+0] = div;
            indicies[3*i+1] = i;
            indicies[3*i+2] = (i+1) % div;
        }

        circlePointObj.SetOpenGLFunctions(qglf);
        circlePointObj.UpdateBuffer(&pt, &indicies);
    }
    ~CirclePoint() {
    }
    void draw(const int fill = 0) {
        circlePointObj.bind();
        if ( fill == 0 ) {
            qglf->glDrawArrays(GL_LINE_LOOP, 0, div);
        }
        else {
            qglf->glDrawElements(GL_TRIANGLES, div * 3, GL_UNSIGNED_INT, 0);
        }
    }
};
