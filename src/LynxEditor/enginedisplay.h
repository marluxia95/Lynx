#ifndef ENGINEDISPLAY_H
#define ENGINEDISPLAY_H

#include <QOpenGLWidget>
#include <QTimer>

class EngineDisplay : public QOpenGLWidget
{
    Q_OBJECT
public:
    EngineDisplay();

private slots:
    void Update();

private:
    QTimer *timer;
};

#endif // ENGINEDISPLAY_H
