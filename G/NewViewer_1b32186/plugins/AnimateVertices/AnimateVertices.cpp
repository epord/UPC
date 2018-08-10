#include "AnimateVertices.h"
#include "glwidget.h"
#include <QTime>

QTime t(0,0,0,0);

void AnimateVertices::onPluginLoad()
{
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/AnimateVertices/animate-vertices-1.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/AnimateVertices/animate-vertices-1.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
    t.start();
}

void AnimateVertices::preFrame()
{
    // bind shader and define uniforms
    program->bind();
    program->setUniformValue("n", 2);
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
    program->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    program->setUniformValue("PI", 3.1415927535f);
    program->setUniformValue("freq", 1);
    program->setUniformValue("amplitude", 0.1f);
    program->setUniformValue("time", t.elapsed()/1000.0f);
    program->setUniformValue("f", QVector4D(0.5f,0.5f,1,0.0f));
}

void AnimateVertices::postFrame()
{
    // unbind shader
    program->release();
}

void AnimateVertices::onObjectAdd()
{
	
}

bool AnimateVertices::drawScene()
{
	return false; // return true only if implemented
}

bool AnimateVertices::drawObject(int)
{
	return false; // return true only if implemented
}

bool AnimateVertices::paintGL()
{
	return false; // return true only if implemented
}

void AnimateVertices::keyPressEvent(QKeyEvent *)
{
	
}

void AnimateVertices::mouseMoveEvent(QMouseEvent *)
{
	
}

