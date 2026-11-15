#pragma once

#include <QString>

#ifdef __APPLE__
    static const QString vertFile = QString("/Users/ylab/Desktop/program/RationalCubicBezier2/Shader/simple.vert");
    static const QString fragFile = QString("/Users/ylab/Desktop/program/RationalCubicBezier2/Shader/simple.frag");
#else
    static const QString vertFile = QString("C:/Users/yinxi/Desktop/program/QtSkeletonWithSettings3/Shader/simple.vert");
    static const QString fragFile = QString("C:/Users/yinxi/Desktop/program/QtSkeletonWithSettings3/Shader/simple.frag");
#endif

