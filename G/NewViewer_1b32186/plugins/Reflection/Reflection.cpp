#include "Reflection.h"
#include <QCoreApplication>

const int IMAGE_WIDTH = 512;
const int IMAGE_HEIGHT = IMAGE_WIDTH;

void Reflection::onPluginLoad()
{
    GLWidget & g = *glwidget();
    g.makeCurrent();
    
    // Upside down shader
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Reflection/upside_down.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Reflection/upside_down.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    
    // Reflection down shader
    vs2 = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs2->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Reflection/reflection.vert");
    cout << "VS log:" << vs2->log().toStdString() << endl;
    
    fs2 = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs2->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Reflection/reflection.frag");
    cout << "FS log:" << fs2->log().toStdString() << endl;
    
    program2 = new QOpenGLShaderProgram(this);
    program2->addShader(vs2);
    program2->addShader(fs2);
    program2->link();

    // Setup texture
    g.glActiveTexture(GL_TEXTURE0);
    g.glGenTextures( 1, &textureId);
    g.glBindTexture(GL_TEXTURE_2D, textureId);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    g.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMAGE_WIDTH, IMAGE_HEIGHT,
		   0, GL_RGB, GL_FLOAT, NULL);
    g.glBindTexture(GL_TEXTURE_2D, 0);
    // Resize to power-of-two viewport
    g.resize(IMAGE_WIDTH,IMAGE_HEIGHT);
}


void drawRect(GLWidget &g)
{
    static bool created = false;
    static GLuint VAO_rect;

    // 1. Create VBO Buffers
    if (!created)
    {
        created = true;
        

        // Create & bind empty VAO
        g.glGenVertexArrays(1, &VAO_rect);
        g.glBindVertexArray(VAO_rect);

        // Create VBO with (x,y,z) coordinates
        float coords[] = { -1, -1, 0, 
                            1, -1, 0, 
                           -1,  1, 0, 
                            1,  1, 0};

        GLuint VBO_coords;
        g.glGenBuffers(1, &VBO_coords);
        g.glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
        g.glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        g.glEnableVertexAttribArray(0);
        g.glBindVertexArray(0);
    }

    // 2. Draw
    g.glBindVertexArray (VAO_rect);
    g.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    g.glBindVertexArray(0);
}



bool Reflection::paintGL()
{
    GLWidget & g = *glwidget();
    
    // Pass 1. Draw scene upside down
    g.glClearColor(0.5f,0.5f,0.5f,1);
    g.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    program->bind();
    program->setUniformValue("modelViewProjectionMatrix",g.camera()->projectionMatrix() * g.camera()->viewMatrix());
    program->setUniformValue("minBoundingBox", scene()->boundingBox().min());
    program->setUniformValue("maxBoundingBox", scene()->boundingBox().max());
    
    if (drawPlugin())
        drawPlugin()->drawScene();
    
    // Get texture
    g.glBindTexture(GL_TEXTURE_2D, textureId);
    g.glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                          IMAGE_WIDTH, IMAGE_HEIGHT);
    g.glGenerateMipmap(GL_TEXTURE_2D);
    
    // Pass 2. Dibujar reflejo con textura
    g.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    program2->bind();
    program2->setUniformValue("colorMap", 0);
    program2->setUniformValue("SIZE", float(IMAGE_WIDTH));
    program2->setUniformValue("modelViewProjectionMatrix",g.camera()->projectionMatrix() * g.camera()->viewMatrix());
    
    Point min = scene()->boundingBox().min();
    float r = scene()->boundingBox().radius();
    drawQuad(Point(-r,min.y(), -r), Point( r,min.y(), -r),
             Point( r,min.y(),  r), Point(-r,min.y(),  r));

    g.defaultProgram()->bind();
    g.glBindTexture(GL_TEXTURE_2D, 0);
    
    if (drawPlugin())
        drawPlugin()->drawScene();

    return true;
}


// TODO: update VBO coords after creation...
void Reflection::drawQuad(const Point& p0, const Point& p1, const Point& p2, const Point& p3)
{
    static bool created = false;
    static GLuint VAO_rect;
    GLWidget &g = *glwidget();
    g.makeCurrent();
    
    // 1. Create VBO Buffers
    if (!created)
    {
        created = true;
        
        // Create & bind empty VAO
        g.glGenVertexArrays(1, &VAO_rect);
        g.glBindVertexArray(VAO_rect);
        
        // Create VBO with (x,y,z) coordinates
        float coords[] = { p0.x(), p0.y(), p0.z(), p1.x(), p1.y(), p1.z(), p3.x(), p3.y(), p3.z(), p2.x(), p2.y(), p2.z() };
        
        GLuint VBO_coords;
        g.glGenBuffers(1, &VBO_coords);
        g.glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
        g.glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        g.glEnableVertexAttribArray(0);
        //glBindVertexArray(0);
        
        float normals[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
        
        GLuint VBO_normals;
        g.glGenBuffers(1, &VBO_normals);
        g.glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
        g.glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
        g.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        g.glEnableVertexAttribArray(1);
        g.glBindVertexArray(0);
    }
    
    // 2. Draw
    g.glBindVertexArray(VAO_rect);
    g.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    g.glBindVertexArray(0);
}
