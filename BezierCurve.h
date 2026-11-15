#pragma once

#include <vector>
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // value_ptr
#include "globject.h"
#include "settings.h"
#include "QOpenGLExtraFunctions"


class BezierCurve
{
    std::vector<glm::vec2> bez;
protected:
    int colorLoc;
    GLObject2D bezierObject;
    QOpenGLExtraFunctions *qglf;
    const int ptBezNum = 1000;
public:
    BezierCurve(const GLint cLoc, QOpenGLExtraFunctions *f) {
        colorLoc = cLoc;
        qglf = f;
        bezierObject.SetOpenGLFunctions(f);
    }

    void bezier_curve(std::vector<glm::vec2> pt)
    {
        //        std::cerr << pt.size() <<std::endl;
        //        std::vector<glm::vec2> p;
        bez.resize(ptBezNum);
        for (int i = 0; i < ptBezNum; i++)
        {
            GLfloat t = (GLfloat)i / (ptBezNum-1);
            glm::vec2 xy((1-t)*(1-t)*(1-t)*pt[0] + 3*(1-t)*(1-t)*t*pt[1] + 3*(1-t)*t*t*pt[2] + t*t*t*pt[3]);
            bez[i] = xy;
            //qDebug() << i << ": " << bez[i][0] << ", " << bez[i][1];
        }
        bezierObject.UpdateVertexBuffer(&bez);
        //        std::cerr << "1" <<std::endl;
        qglf->glUniform4fv(colorLoc, 1, glm::value_ptr(black));
        //        std::cerr << "2" <<std::endl;
        bezierObject.bind();
        //        std::cerr << "3" <<std::endl;
        qglf->glDrawArrays(GL_LINES, 0, bez.size());
        //        std::cerr << "4" <<std::endl;
        //bez.clear();
    }
};




