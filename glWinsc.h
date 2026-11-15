#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <QOpenGLFunctions>


class glWinSc {
protected:
    // trasnformation matricies and vectors
    glm::mat2 windowToScreenMat;
    glm::vec2 windowToScreenTrans;
    glm::mat2 screenToWindowMat;
    glm::vec2 screenToWindowTrans;
    // coordinates
    glm::mat2 screenCoordinates;
    //glm::mat2 windowCoordinates;
    GLfloat scale;
    GLfloat size[2];
    GLfloat location[2];
public:
    glWinSc(const int width, const int height) {
        size[0] = width;
        size[1] = height;
        scale = 1.0f;
        location[0] = location[1] = 0.0f;
        SetScreenAndWindowCoordinates();
    }
    ~glWinSc() {}
    void SetWindowSize(const int width, const int height) {
        size[0] = width;
        size[1] = height;
        SetScreenAndWindowCoordinates();
    }
    void SetScale(const float sc) {
        scale = sc;
        SetScreenAndWindowCoordinates();
    }
    void SetLocation( const float lc[2]) {
        location[0] = lc[0];
        location[1] = lc[1];
        SetScreenAndWindowCoordinates();
    }
    void setScreenCoordinates(const GLfloat wLeft, const GLfloat wRight, const GLfloat hUp, const GLfloat hDown) {
        screenCoordinates[0][0] = wLeft;
        screenCoordinates[1][0] = wRight;
        screenCoordinates[0][1] = hUp;
        screenCoordinates[1][1] = hDown;
    }
    const glm::mat2 GetWindowCoordinates(void) const {
        return glm::mat2(0, 0, size[0]-1.0f, size[1]-1.0f);
    }
    const glm::mat2 GetScreenCoordinates(void) const {
        return screenCoordinates;
    }
    void GetScreenCoordinates(GLfloat &wLeft, GLfloat &wRight, GLfloat &hUp, GLfloat &hDown ) const {
        wLeft = screenCoordinates[0][0];
        wRight = screenCoordinates[1][0];
        hUp = screenCoordinates[0][1];
        hDown = screenCoordinates[1][1];
    }
    const glm::mat2 GetWindowToScreenMat(void) const {
        return windowToScreenMat;
    }
    const glm::mat2 GetScreenToWindowMat(void) const {
        return screenToWindowMat;
    }
    glm::vec2 WindowToScreen(const glm::vec2 v0) {
        glm::vec2 v1 = windowToScreenMat * v0 + windowToScreenTrans;
        return v1;
    }
    glm::vec2 ScreenToWindow(const glm::vec2 v0) {
        glm::vec2 v1 = screenToWindowMat * v0 + screenToWindowTrans;
        return v1;
    }
    GLfloat getPixelOnScreen(float pixelSize) const {
        GLfloat screenWidth = screenCoordinates[1][0] - screenCoordinates[0][0];
        GLfloat windowWidth = size[1];
        GLfloat tenPixel = screenWidth / windowWidth * pixelSize;
        return tenPixel;
    }
    void SetScreenAndWindowCoordinates() {
        glm::mat2 windowCoordinates = GetWindowCoordinates();
        const GLfloat aspectRatio = static_cast<float>(size[1]) / static_cast<float>(size[0]);
        GLfloat lenx = 1.5f * scale;
        GLfloat leny = lenx * aspectRatio;
        GLfloat sxl = location[0] - lenx;
        GLfloat sxr = location[0] + lenx;
        GLfloat syu = location[1] + leny;
        GLfloat syd = location[1] - leny;
//        std::cout << sxl << ", " << sxr << ", " << syu << ", " << syd << std::endl;
        screenCoordinates = glm::mat2(sxl, syu, sxr, syd);

        windowToScreenMat = inverse(windowCoordinates) * screenCoordinates;
        screenToWindowMat = inverse(screenCoordinates) * windowCoordinates;

        GLfloat wxl = 0.0;
        GLfloat wxr = windowCoordinates[1][0];
        GLfloat wyu = 0.0;
        GLfloat wyd = windowCoordinates[1][1];

        // Mathematica Code
        // m = {{a, c, tx}, {b, d, ty}, {0, 0, 1}}; w0 = {wxl, wyu,1}; w1 = {wxr, wyu, 1}; w2 = {wxr, wyd, 1}; s0 = {sxl, syu,1}; s1 = {sxr, syu, 1}; s2 = {sxr, syd, 1}; Solve[{s0 == m.w0, s1 == m.w1, s2 == m.w2}, {a, b, c, d, tx, ty}] // FullSimplify
        windowToScreenMat = glm::mat2((sxl - sxr)/(wxl - wxr), 0.0f, 0.0f, (syd - syu)/(wyd - wyu));
        windowToScreenTrans = glm::vec2((sxr*wxl - sxl*wxr)/(wxl - wxr), (syu*wyd - syd*wyu)/(wyd - wyu));

        screenToWindowMat = glm::mat2((wxl - wxr)/(sxl - sxr), 0.0f, 0.0f, (wyd - wyu)/(syd - syu));
        screenToWindowTrans = glm::vec2((-(sxr*wxl) + sxl*wxr)/(sxl - sxr), (-(syu*wyd) + syd*wyu)/(syd - syu));
    }
    GLfloat getScale() const {
        return scale;
    }
    const GLfloat *getLocation() const {
        return location;
    }
};

