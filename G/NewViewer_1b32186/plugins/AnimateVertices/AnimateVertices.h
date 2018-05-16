#ifndef _ANIMATEVERTICES_H
#define _ANIMATEVERTICES_H

#include "plugin.h"
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

class AnimateVertices: public QObject, public Plugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "Plugin") 
	Q_INTERFACES(Plugin)

  public:
	 void onPluginLoad();
	 void preFrame();
	 void postFrame();

	 void onObjectAdd();
	 bool drawScene();
	 bool drawObject(int);

	 bool paintGL();

	 void keyPressEvent(QKeyEvent *);
	 void mouseMoveEvent(QMouseEvent *);
  private:
    QOpenGLShaderProgram* program;
    QOpenGLShader *fs, *vs; 
};

#endif
