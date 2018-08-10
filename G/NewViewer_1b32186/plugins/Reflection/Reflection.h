#ifndef _GLOWING_H
#define _GLOWING_H

#include "plugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


class Reflection : public QObject, public Plugin
{
     Q_OBJECT
     Q_PLUGIN_METADATA(IID "Plugin")   
     Q_INTERFACES(Plugin)

 public:
    void onPluginLoad();
    bool paintGL();
 
 private:
    void drawQuad(const Point& p0, const Point& p1, const Point& p2, const Point& p3);
    
    QOpenGLShaderProgram *program, *program2;
    QOpenGLShader *vs, *vs2;
    QOpenGLShader *fs, *fs2;  
    GLuint textureId;
 };
 
 #endif
