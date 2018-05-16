#include "myPlugin.h"
#include "glwidget.h"

void MyPlugin::onPluginLoad()
{
    const vector<Object> objs = scene()->objects();
    cout << "Objects: " << objs.size() << endl;
    const vector<Face> faces = objs[0].faces();
    const vector<Vertex> verts = objs[0].vertices();
    Face f = faces[0];
    if (f.numVertices() == 3) cout << "Triangle" << endl;
    int index = f.vertexIndex(0);
    const Vertex& v = verts[index];
    cout << "Index: " << index << endl;
    cout << "Coords: " << v.coord();
    cout << "Normal: " << f.normal();
    cout << "Faces: " << faces.size() << endl;
    cout << "Vertices: " << verts.size() << endl;
    cout << "W=" << glwidget()->width() << endl;
    cout << "H=" << glwidget()->height() << endl;
    
    // Carregar shader, compile & link
    QString vs_src =
    "#version 330 core\n"
    "uniform mat4 modelViewProjectionMatrix;"
    "in vec3 vertex;"
    "in vec3 color;"
    "out vec4 col;"
    "void main() {"
    "  gl_Position = modelViewProjectionMatrix * vec4(vertex,1.0);"
    "  col=vec4(color,1.0);"
    "}";
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceCode(vs_src);
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    QString fs_src =
    "#version 330 core\n"
    "out vec4 fragColor;"
    "in vec4 col;"
    "void main() {"
    "  fragColor=col;"
    "}";
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceCode(fs_src);
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
}

void MyPlugin::preFrame()
{
    // bind shader and define uniforms
    program->bind();
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
}

void MyPlugin::postFrame()
{
    // unbind shader
    program->release();
}

void MyPlugin::onObjectAdd()
{
	
}

bool MyPlugin::drawScene()
{
	return false; // return true only if implemented
}

bool MyPlugin::drawObject(int)
{
	return false; // return true only if implemented
}

bool MyPlugin::paintGL()
{
	return false; // return true only if implemented
}

void MyPlugin::keyPressEvent(QKeyEvent *)
{
	
}

void MyPlugin::mouseMoveEvent(QMouseEvent *)
{
	
}

