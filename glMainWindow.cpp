#include "glMainWindow.h"
#include "geoObject.h"
#include "settings.h"
#include "CubicBezier.h"
#include "mainWindow.h"
#include "settingswindow.h"

GLMainWindow *glMainWindow;

void GLMainWindow::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLWindow::paintGL();

    shaderProgram->bind();
    glEnable(GL_MULTISAMPLE);



    glm::mat4 modelview(glm::mat4(1.f));
    glm::mat4 projection(glm::mat4(1.f));
    if ( mode3D ) {
        ComputeProjectionMatrix(width(), height(), modelview, projection);
    }
    else {
        GLfloat wLeft, wRight, hUp, hDown;
        GetScreenCoordinates(wLeft, wRight, hUp, hDown);
        projection = glm::ortho(wLeft, wRight, hDown, hUp, -10.0f, 10.0f);
    }
    // save modelview and projection matrices for Project and UnProject
    cmodelView = modelview;
    cproj = projection;

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, glm::value_ptr(modelview));

    axis->draw();

    shaderProgram->setUniformValue("isLine", 1);

    //制御点の設定
    CirclePoint cp;
    cp.SetOpenGLFucntions(static_cast<QOpenGLExtraFunctions *>(this));
    float pixelSize = getPixelOnScreen(5.0f); // get screen size of 5 pixel

    std::vector<glm::vec4> ptColors = {
        blue,
        red,
        green,
        orange,
    };

    for(unsigned int i = 0; i < Bezier.GetCpt().size(); i++) {
        const glm::mat4 model2(glm::translate(glm::mat4(1.0f), glm::vec3(static_cast<float>(Bezier.GetCpt()[i][0]), static_cast<float>(Bezier.GetCpt()[i][1]), 0.0f)));
        const glm::mat4 model3(glm::scale(model2, glm::vec3(pixelSize)));
        const glm::mat4 model(modelview * model3);

        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, glm::value_ptr(model));

        glm::vec4 currentColor = ptColors[i % ptColors.size()];
        glUniform4fv(colorLoc, 1, glm::value_ptr(currentColor));

        cp.draw(1);
    }

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, glm::value_ptr(modelview));

    //制御ポリゴン
    glUniform4fv(colorLoc, 1, glm::value_ptr(gray));
    GLObject2D glObject(qglf);
    glObject.UpdateBuffer(&Bezier.GetCpt());
    glObject.SetOpenGLFunctions(qglf);
    glObject.bind();
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(Bezier.GetCpt().size()));


    //曲線の描画
    if (Bezier.GetCpt().size() > 2)
    {
        Bezier.ComputePoints();

        if (mainWindow) {
            mainWindow->UpdateCurvatureGraph();
        }

        glUniform4fv(colorLoc, 1, glm::value_ptr(black));
        glObject.UpdateVertexBuffer(&Bezier.GetPoints());
        glDrawArrays(GL_LINE_STRIP, 0, Bezier.GetPoints().size());
    }



    if(showComb){
    //Combの蓋
    glObject.UpdateBuffer(&Bezier.GetComb());    //Combの情報をGPUに転送
    glObject.SetOpenGLFunctions(qglf);
    glObject.bind();
    glUniform4fv(colorLoc, 1, glm::value_ptr(gray));
    glDrawArrays(GL_LINE_STRIP, 0 , Bezier.GetComb().size());

    //Combを描画
    glObject.UpdateBuffer(&Bezier.GetCombspike());    //Combの情報をGPUに転送
    glObject.SetOpenGLFunctions(qglf);
    glObject.bind();
    glUniform4fv(colorLoc, 1, glm::value_ptr(gray));
    glDrawArrays(GL_LINES, 0 , Bezier.GetComb().size());
    }

    if(showCircle){
    //曲率円の描画
    auto drawCircle = [&](glm::vec2 center, float radius) {
        // 半径が小さすぎる、または大きすぎる（直線に近い）場合は描画しない
        if(std::abs(radius) < 0.001f || std::abs(radius) > 1000.0f) return;

        std::vector<glm::vec2> circleVerts;
        int segments = 64; // 円の滑らかさ（頂点数）
        float absRadius = std::abs(radius); // 描画時の半径は必ずプラスにする

        for(int i = 0; i < segments; ++i) {
            float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(segments);
            float cx = center.x + absRadius * std::cos(theta);
            float cy = center.y + absRadius * std::sin(theta);
            circleVerts.push_back(glm::vec2(cx, cy));
        }

        // OpenGLのバッファを更新して描画
        glObject.UpdateBuffer(&circleVerts);
        glObject.SetOpenGLFunctions(qglf);
        glObject.bind();
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(circleVerts.size()));
    };

    // 始点(K0)の円を描画 (例として青色)
    glm::vec4 blue(0.0f, 0.0f, 1.0f, 1.0f);
    glUniform4fv(colorLoc, 1, glm::value_ptr(blue));
    drawCircle(Bezier.circleCenter0, Bezier.circleRadius0);

    // 終点(K1)の円を描画 (例として緑色)
    glm::vec4 green(0.0f, 0.7f, 0.0f, 1.0f);
    glUniform4fv(colorLoc, 1, glm::value_ptr(green));
    drawCircle(Bezier.circleCenter1, Bezier.circleRadius1);

    }

    shaderProgram->release();

}

void GLMainWindow::setWeight1(float w) {
    //ベジェ曲線の w1 を更新（変数名は環境に合わせてください）
    Bezier.w1 = w;

    //画面を再描画する
    update();
}

void GLMainWindow::setWeight0(float w) {
    Bezier.w0 = w;

    if (Bezier.controlPoint.size() >= 4) {

        Bezier.ComputePoints();

        // 現在の頂点とウェイトを使って、現在のK0を計算する
        float currentK0 = Bezier.curvatureY[0]; // (ComputePointsの中と同じ計算)


        if (mainWindow) {
            mainWindow->UpdateK0UI(currentK0);
        }
    }
    update();
}

void GLMainWindow::setWeight2(float w) {
    //ベジェ曲線の w1 を更新（変数名は環境に合わせてください）
    Bezier.w2 = w;

    //画面を再描画する
    update();
}

void GLMainWindow::setWeight3(float w) {
    //ベジェ曲線の w1 を更新（変数名は環境に合わせてください）
    Bezier.w3 = w;

    if (Bezier.controlPoint.size() >= 4) {

        Bezier.ComputePoints();

        // 現在の頂点とウェイトを使って、現在のK0を計算する
        float currentK1 = Bezier.curvatureY.back(); // (ComputePointsの中と同じ計算)


        if (mainWindow) {
        mainWindow->UpdateK1UI(currentK1);
        }
    }
    update();
}

// 始点の曲率 K0 を指定して w0 を自動計算する関数
void GLMainWindow::setCurvatureK0(float targetK0) {
    if (Bezier.controlPoint.size() < 4) return;

    glm::vec2 p0 = Bezier.controlPoint[0];
    glm::vec2 p1 = Bezier.controlPoint[1];
    glm::vec2 p2 = Bezier.controlPoint[2];

    glm::vec2 v1 = p1 - p0; // (P1 - P0)
    glm::vec2 v2 = p2 - p1; // (P2 - P1)

    // 外積の計算
    float crossp = v1.x * v2.y - v1.y * v2.x;

    // 制御点が一直線に並んでいる時はゼロ割り当てを回避
    if (std::abs(crossp) < 1e-5f) return;

    // 距離の3乗 (|P1 - P0|^3) を計算
    float len = glm::length(v1);
    float len3 = len * len * len;

    // 公式を使って新しい w0 を求める
    float new_w0 = std::abs(targetK0) * (3.0f * Bezier.w1 * Bezier.w1 * len3) / (2.0f * Bezier.w2 * std::abs(crossp));

    Bezier.w0 = new_w0;

    if (mainWindow) {
        mainWindow->UpdateW0UI(new_w0);
    }

    update(); // 画面を再描画
}

// 終点の曲率 K1 を指定して w3 を自動計算する関数
void GLMainWindow::setCurvatureK1(float targetK1) {
    if (Bezier.controlPoint.size() < 4) return;

    glm::vec2 p1 = Bezier.controlPoint[1];
    glm::vec2 p2 = Bezier.controlPoint[2];
    glm::vec2 p3 = Bezier.controlPoint[3];

    glm::vec2 v1 = p2 - p1; // (P2 - P1)
    glm::vec2 v2 = p3 - p2; // (P3 - P2)

    // 外積の計算
    float crossp = v1.x * v2.y - v1.y * v2.x;

    if (std::abs(crossp) < 1e-5f || Bezier.w1 < 1e-5f) return;

    // 距離の3乗 (|P3 - P2|^3) を計算
    float len = glm::length(v2);
    float len3 = len * len * len;

    // 公式を使って新しい w3 を求める
    float new_w3 = std::abs(targetK1) * (3.0f * Bezier.w2 * Bezier.w2 * len3) / (2.0f * Bezier.w1 * std::abs(crossp));

    if (new_w3 < 0.01f) new_w3 = 0.01f;

    Bezier.w3 = new_w3;

    if (mainWindow) {
        mainWindow->UpdateW3UI(new_w3);
    }

    update(); // 画面を再描画
}


void GLMainWindow::mousePressEvent(QMouseEvent *e) {

    if ( ctrptMoving == -1 && e->button() == Qt::LeftButton ) {
        if ( Bezier.GetCpt().size() > 2 ) {
            glm::vec3 mp(static_cast<float>(e->position().x()), static_cast<float>(cviewport[3] - e->position().y()), 0.0f);
            for(unsigned int i = 0; i < Bezier.GetCpt().size(); i++) {
                glm::vec3 cpt = glm::vec3(Bezier.GetCpt()[i], 0.0f);;
                glm::vec3 cptWin = Project(cpt);
                cptWin[2]=0.0;
                const double pixelDifference = 8.0;
                if ( glm::distance(cptWin, mp) < pixelDifference ) {
                    ctrptMoving = i;
                    break;
                }
            }
        }
        else
            GLWindow::mousePressEvent(e);
    }
    else {
        GLWindow::mousePressEvent(e);
    }
}

void GLMainWindow::mouseReleaseEvent(QMouseEvent *e) {
    if ( !mode3D ) {
        if ( e->button() == Qt::LeftButton ) {
            if ( ctrptMoving != -1) {
                int useZ = 0;
                glm::vec3 mp(static_cast<float>(e->position().x()), static_cast<float>(cviewport[3] - e->position().y()), 0.0);
                glm::dvec3 p = UnProject(mp, useZ);
                Bezier.GetCpt()[ctrptMoving] = p;    // automatic conversion from 3d to 2d
                ctrptMoving = -1;
                update();
            }
            else if ( Bezier.GetCpt().size() < 4 ) {
                int useZ = 0;
                glm::vec3 mp(static_cast<float>(e->position().x()), static_cast<float>(cviewport[3] - e->position().y()), 0.0);
                glm::dvec3 p = UnProject(mp, useZ);
                Bezier.GetCpt().push_back(p);
                update();
            }
        }
    }
    else {
        GLWindow::mouseReleaseEvent(e);
    }
}

void GLMainWindow::mouseMoveEvent(QMouseEvent *e) {
    if ( !mode3D ) {
        if ( ctrptMoving != -1 ) {
            int useZ = 0;
            glm::vec3 mp(static_cast<float>(e->position().x()), static_cast<float>(cviewport[3] - e->position().y()), 0.0);
            glm::dvec3 p = UnProject(mp, useZ);

            setCurrentMovingPoint(ctrptMoving);

            Bezier.GetCpt()[ctrptMoving] = p;    // automatic conversion from 3d to 2d

            Bezier.ComputePoints();
            if (mainWindow && !Bezier.curvatureY.empty()) {
                mainWindow->UpdateK0UI(Bezier.curvatureY.front());
                mainWindow->UpdateK1UI(Bezier.curvatureY.back());
                mainWindow->UpdateCurvatureGraph(); // グラフの更新もここで行う
            }

            update();
        }
    }
    else
        GLWindow::mouseMoveEvent(e);

}
void GLMainWindow::keyPressEvent(QKeyEvent *e) {
    GLWindow::keyPressEvent(e);
}

void GLMainWindow::setCombCheck(bool visible) {
    showComb = visible;
    update();
}

void GLMainWindow::setCofcurvatureCheck(bool visible) {
    showCircle = visible;
    update();
}
