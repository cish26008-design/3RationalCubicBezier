#pragma once

#include <vector>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // value_ptr
#include "globject.h"
#include "settings.h"
#include "QOpenGLExtraFunctions"


template <int Dim>
class Axis {
protected:
    int colorLoc;
    GLObject3D xaxisObject;
    GLObject3D yaxisObject;
    GLObject3D zaxisObject;
    QOpenGLExtraFunctions *qglf;
public:
    Axis(const GLint cLoc, QOpenGLExtraFunctions *f, float length = 1.0f) {
        colorLoc = cLoc;
        qglf = f;
        xaxisObject.SetOpenGLFunctions(f);
        yaxisObject.SetOpenGLFunctions(f);
        zaxisObject.SetOpenGLFunctions(f);
        setLength(length);
    }
    void setLength(const float length)  {
        std::vector<glm::vec3> xaxis;
        xaxis.push_back(glm::vec3(-length, 0.0f, 0.0f));
        xaxis.push_back(glm::vec3(length, 0.0f, 0.0f));
        xaxisObject.UpdateVertexBuffer(&xaxis);

        std::vector<glm::vec3> yaxis;
        yaxis.push_back(glm::vec3(0.0f, -length, 0.0f));
        yaxis.push_back(glm::vec3(0.0f, length, 0.0f));
        yaxisObject.UpdateVertexBuffer(&yaxis);

        std::vector<glm::vec3> zaxis;
        zaxis.push_back(glm::vec3(0.0f, 0.0f, -length));
        zaxis.push_back(glm::vec3(0.0f, 0.0f, length));
        zaxisObject.UpdateVertexBuffer(&zaxis);
    }
    ~Axis() {
    }
    void draw()  {
        qglf->glUniform4fv(colorLoc, 1, glm::value_ptr(red));
        xaxisObject.bind();
        qglf->glDrawArrays(GL_LINES, 0, 2);

        qglf->glUniform4fv(colorLoc, 1, glm::value_ptr(green));
        yaxisObject.bind();
        qglf->glDrawArrays(GL_LINES, 0, 2);
        if ( Dim == 3 ) {
            qglf->glUniform4fv(colorLoc, 1, glm::value_ptr(blue));
            zaxisObject.bind();
            qglf->glDrawArrays(GL_LINES, 0, 2);
        }
    }
};

using Axis2D = Axis<2>;
using Axis3D = Axis<3>;

