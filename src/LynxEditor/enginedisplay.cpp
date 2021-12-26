#include "enginedisplay.h"

EngineDisplay::EngineDisplay() : QOpenGLWidget()
{
    initializeGL();
    resizeGL(256,256);
    resize(256,256);

    timer = new QTimer();
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(16); // 60 FPS

    connect(timer, SIGNAL(QTimer::timeout()), this, SLOT(Update));


}

void EngineDisplay::Update()
{
    paintGL();

}
