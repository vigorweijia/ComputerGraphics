#include "GalaxyWidget.h"
#include "ui_GalaxyWidget.h"
#include <GL/GLU.h>
#define PI 3.1415926

GalaxyWidget::GalaxyWidget(QWidget *parent) :
    QGLWidget(parent)
    //ui(new Ui::GalaxyWidget)
{
    //ui->setupUi(this);
}

GalaxyWidget::~GalaxyWidget()
{
    //delete ui;
}

void GalaxyWidget::initializeGL()
{
    //启用阴影平滑
    glShadeModel(GL_SMOOTH);
    //黑色背景
    glClearColor(0.0,0.0,0.0,0.0);
    //设置深度缓存
    glClearDepth(1.0);
    //启用深度测试
    glEnable(GL_DEPTH_TEST);
    //所作深度测试的类型
    glDepthFunc(GL_LEQUAL);
    //告诉系统对透视进行修正
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    //加载纹理
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);
}

void GalaxyWidget::paintGL()
{
    // 清除屏幕和深度缓存
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    //移到屏幕的左半部分，并且将视图推入屏幕背后足够的距离以便我们可以看见全部的场景
    glTranslatef(0.0f,0.0f,-5.0f);

    glRotatef( xRot/16,  1.0,  0.0,  0.0 );
    glRotatef( yRot/16,  0.0,  1.0,  0.0 );
    glRotatef( zRot/16,  0.0,  0.0,  1.0 );

    //选择使用的纹理

    glBindTexture( GL_TEXTURE_2D, texture[0] );
    glBegin( GL_QUADS );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );

    glEnd();
}

//重置OpenGL窗口大小
void GalaxyWidget::resizeGL(int width, int height)
{
    //防止窗口大小变为0
    if(height == 0)
    {
        height = 1;
    }
    //重置当前的视口
    glViewport(0,0,(GLint)width,(GLint)height);
    //选择投影矩阵
    glMatrixMode(GL_PROJECTION);
    //重置投影矩阵
    glLoadIdentity();
    //设置视口大小
    gluPerspective(45.0,(GLfloat)width/(GLfloat)height,0.1,100.0);
    //选择模型观察矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//纹理装载函数
void GalaxyWidget::loadGLTextures()
{
    QImage tex,buf;
    if(!buf.load("/home/wangchuan/qtcreator-2.4.1/bin/Program/GLFrameWork/GLFrameWork/xingkong.jpg"))
    {
        //如果载入不成功，自动生成一个128*128的32位色的绿色图片
        qWarning("Could not read image file!");
        QImage dummy(128,128,QImage::Format_RGB32);
        dummy.fill(Qt::green);
            buf = dummy;
    }
    //转换成纹理类型
    tex = QGLWidget::convertToGLFormat(buf);
    //创建纹理
    glGenTextures(1, &texture[0]);
    //使用来自位图数据生成的典型纹理，将纹理名字texture[0]绑定到纹理目标上
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
