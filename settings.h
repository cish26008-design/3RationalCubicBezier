#ifndef SETTINGS_H
#define SETTINGS_H

#include <glm/glm.hpp>
#include <QOpenGLWidget>

class GLWinSettings
{
public:
    GLWinSettings() {
        lineWidth = 2.0f;
        useAntiAliasing = 1;
    }
    float lineWidth;
    int useAntiAliasing;
};

extern GLWinSettings glWinSettings;

const glm::vec4 black(0.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 red(1.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 green(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 blue(0.0f, 0.0f, 1.0f, 0.0f);
const glm::vec4 yellow(1.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 purple(1.0f, 0.0f, 1.0f, 0.0f);
const glm::vec4 cyan(0.0f, 1.0f, 1.0f, 0.0f);
const glm::vec4 white(1.0f, 1.0f, 1.0f, 0.0f);
const glm::vec4 gray(0.5f, 0.5f, 0.5f, 0.0f);
const glm::vec4 orange(1.0f, 0.5f, 0.0f, 0.0f);

#endif // SETTINGS_H
