#ifndef GALAXYWIDGET_H
#define GALAXYWIDGET_H

#include <QWidget>
#include <QtOpenGL/QGLWidget>
#include <QtGui>

namespace Ui {
class GalaxyWidget;
}

class GalaxyWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GalaxyWidget(QWidget *parent = 0);
    ~GalaxyWidget();
    void setXRot(int v){xRot = v;}
    void setYRot(int v){yRot = v;}
    void setZRot(int v){zRot = v;}

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void loadGLTextures();
    GLuint texture[1];

private:
    Ui::GalaxyWidget *ui;
    int xRot;
    int yRot;
    int zRot;
};

#endif // GALAXYWIDGET_H
