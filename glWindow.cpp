#include <iostream>
#include <QCoreApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <iostream>
#include <QOpenGLDebugLogger>
#include "glWindow.h"
#include "CubicBezier.h"


//float GetDevicePixelRatioOfMainWindow2D() {
//    return glMainWindow->devicePixelRatio();
//}

GLWindow::GLWindow(int width, int height, const QString &vertexFile, const QString &fragmentFile, int win3D, QOpenGLWidget *parent):
    QOpenGLWidget(parent), glWinSc(width, height) {

    QWidget::setFocusPolicy(Qt::ClickFocus);     // to enable keyPressEvent()

    vertexShaderFile = vertexFile;
    fragmentShaderFile = fragmentFile;
    mode3D  = win3D;

    resize(width, height);


	QString windowTitle = QString("2D Window");
	this->setWindowTitle(windowTitle);

    mode3D = 0;
}

void GLWindow::initializeGL() {
    initializeOpenGLFunctions();

#ifdef USE_OPENGL_DEBUG_LOGGER
    m_logger = new QOpenGLDebugLogger( this );

    // connect( m_logger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
    //          this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ),
    //          Qt::DirectConnection );
    connect(m_logger, &QOpenGLDebugLogger::messageLogged, this, &GLWindow::onMessageLogged);

    if ( m_logger->initialize() ) {
        m_logger->enableMessages();
        m_logger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::MediumSeverity);
        m_logger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::LowSeverity);
        m_logger->disableMessages(QOpenGLDebugMessage::AnySource, QOpenGLDebugMessage::AnyType, QOpenGLDebugMessage::NotificationSeverity);
        m_logger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
    }
#endif



    // create the shader program
    shaderProgram = new QOpenGLShaderProgram;
    bool success = shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);
    if(!success) {
        std::cout << "failed!" << std::endl;
    }
    success = shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if(!success) {
        std::cout << "failed!" << std::endl;
    }
    shaderProgram->bindAttributeLocation("position", 0);
    shaderProgram->link();
    shaderProgram->bind();

    colorLoc = shaderProgram->uniformLocation("incolor");
    Q_ASSERT(colorLoc != -1);
    modelviewLoc = shaderProgram->uniformLocation( "modelview");
    Q_ASSERT(modelviewLoc != -1);
    projectionLoc = shaderProgram->uniformLocation("projection");
    Q_ASSERT(projectionLoc != -1);

    shaderProgram->release();

    qglf = static_cast<QOpenGLExtraFunctions *>(this);
    axis = new Axis3D(colorLoc, qglf);

    glClearColor(1.0, 1.0, 1.0, 1.0);

}


void GLWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    cviewport = glm::ivec4(0, 0, w, h);
    SetWindowSize(w, h);
}

void GLWindow::paintGL() {

    //コントロールポイントが4つ未満なら描画できないので戻る
    if (Bezier.GetCptSize() < 4) return;

    shaderProgram->bind(); //シェーダプログラム起動、これ以降の命令はシェーダプログラムに

    shaderProgram->setUniformValue("isLine", 0);


    //glm::vec2 から Qt用の QVector2D に変換して配列を作る
    QVector4D PT_array[4];
    if (Bezier.GetCptSize() >= 4) {
        float weights[4] = { Bezier.w0, Bezier.w1, Bezier.w2, Bezier.w3 };

        for(int i = 0; i < 4; i++) {
            float safeX = Bezier.controlPoint[i].x;
            float safeY = Bezier.controlPoint[i].y;

            PT_array[i] = QVector4D(safeX, safeY, 0.0f, weights[i]);
        }
    }

    // 値を送信
    shaderProgram->setUniformValueArray("PT", PT_array, 4);

    int shaderMovingPT = m_currentMovingPt;

    shaderProgram->setUniformValue("movingPT", shaderMovingPT);
    shaderProgram->setUniformValue("maxDepth", 10);
    shaderProgram->setUniformValue("inflectionP", 1);
    shaderProgram->setUniformValue("showInflection", 1);
    shaderProgram->setUniformValue("incolor", QColor(255, 255, 255, 255));

    //画面全体を覆う四角形の描画
    float lxWin = -1.0f;
    float rxWin =  1.0f;
    float byWin = -1.0f;
    float tyWin =  1.0f;

    GLfloat vertices[] = {
        lxWin, byWin,
        rxWin, byWin,
        lxWin, tyWin,
        rxWin, tyWin
    };

    QOpenGLVertexArrayObject vao;
    vao.create();
    vao.bind();

    QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices)); // VBOへデータを転送

    // glDrawArrays の「前」にバインド処理を行う
    // 頂点シェーダーの location=0 に vertices のデータを2次元(x,y)として紐付ける
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 2);


    // 描画
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // お片付け
    shaderProgram->disableAttributeArray(0);
    vbo.release();
    vbo.destroy();
    vao.release();
    vao.destroy();
}

void GLWindow::mousePressEvent(QMouseEvent *e) {
    if ( mode3D ) {
        MousePressed(e);
    }
}

void GLWindow::mouseReleaseEvent(QMouseEvent *e) {
    if ( mode3D ) {
        MouseReleased(e);
    }
}

void GLWindow::mouseMoveEvent(QMouseEvent *e) {
    if ( mode3D ) {
        if ( MouseMove(e) ) update();
    }
}

void GLWindow::keyPressEvent(QKeyEvent* event) {
//    if ( event->modifiers() == Qt::ShiftModifier )
//        qDebug("shfit key");
    switch(event->key()) {
    case Qt::Key_Escape:
        {
           QMessageBox msgBox;
           msgBox.setText("Quit Application?");
           msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
           msgBox.setDefaultButton(QMessageBox::No);
           int ret = msgBox.exec();
           if ( ret == QMessageBox::Yes ) {
               QCoreApplication::exit(0);
           }
       }
        break;
    case '2':
        qDebug("2D mode");
        mode3D = 0;
        update();
        break;
    case '3':
        qDebug("3D mode");
        mode3D = 1;
        update();
        break;
    default:
        event->ignore();    // propagate the key to the parent widget
        break;
    }
}

#ifdef USE_OPENGL_DEBUG_LOGGER
void GLWindow::onMessageLogged( QOpenGLDebugMessage message )
{
    qDebug() << message;
}
#endif
