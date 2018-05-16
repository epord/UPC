#include "AnimateVertices.h"
#include "glwidget.h"


void AnimateVertices::onPluginLoad()
{
	QString vs_src =
        "#version 330 core\n"
        "uniform mat4 modelViewProjectionMatrix;"
        "in vec3 vertex;"
        "in vec3 color;"
        "out vec4 frontColor;"
        "void main() {"
        "  gl_Position = modelViewProjectionMatrix * vec4(vertex,1.0);"
        "  frontColor=vec4(color,1.0);"
        "}";
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceCode(vs_src);
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    QString fs_src =
        "#version 330 core\n"
        "in vec4 frontColor;\n"
        "out vec4 fragColor;"
        "void main() {"
        "  fragColor = frontColor;"
        "}";
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceCode(fs_src);
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
}

void AnimateVertices::preFrame()
{
    // bind shader and define uniforms
    program->bind();
    program->setUniformValue("n", 6);
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
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

