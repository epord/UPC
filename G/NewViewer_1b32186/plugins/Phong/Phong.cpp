#include "Phong.h"
#include "glwidget.h"

void Phong::onPluginLoad()
{
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Phong/lighting3.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Phong/lighting3.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
    
}

void Phong::preFrame()
{
    program->bind();
    program->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix() * camera()->viewMatrix());
    program->setUniformValue("modelViewMatrix", camera()->viewMatrix());
    program->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    program->setUniformValue("matAmbient", QVector4D(0.2f,0.6f,0.3f,0.0f));
    program->setUniformValue("matDiffuse", QVector4D(0.2f,0.2f,0.8f,0.0f));
    program->setUniformValue("matSpecular", QVector4D(0.9f,0.2f,0.2f,0.0f));
    
    program->setUniformValue("matShininess", 2.0f);
    
    program->setUniformValue("lightAmbient", QVector4D(0.9f,0.8f,0.9f,0.0f));
    program->setUniformValue("lightDiffuse", QVector4D(0.8f,0.6f,0.8f,0.0f));
    program->setUniformValue("lightSpecular", QVector4D(0.4f,0.9f,0.9f,0.0f));
    program->setUniformValue("lightPosition", QVector4D(3,4,2,1));
    
}

void Phong::postFrame()
{
	
}

void Phong::onObjectAdd()
{
	
}

bool Phong::drawScene()
{
	return false; // return true only if implemented
}

bool Phong::drawObject(int)
{
	return false; // return true only if implemented
}

bool Phong::paintGL()
{
	return false; // return true only if implemented
}

void Phong::keyPressEvent(QKeyEvent *)
{
	
}

void Phong::mouseMoveEvent(QMouseEvent *)
{
	
}

