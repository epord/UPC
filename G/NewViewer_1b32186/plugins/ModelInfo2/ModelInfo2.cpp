// GLarena, a plugin based platform to teach OpenGL programming
// © Copyright 2012-2018, ViRVIG Research Group, UPC, https://www.virvig.eu
// 
// This file is part of GLarena
//
// GLarena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "ModelInfo2.h"
#include "glwidget.h"
#include <QPainter>
#import <string>

vector<Object> objs;
int faces_count;
int verts_count;
int triangles_count;

void ModelInfo2::onPluginLoad()
{
// Counting things
    faces_count = 0;
    verts_count = 0;
    triangles_count = 0;
    objs = scene()->objects();
    for(int i = 0; i < objs.size(); i++) {
        vector<Face> f = objs[i].faces();
        faces_count += f.size();
        
        vector<Vertex> v = objs[i].vertices();
        verts_count += v.size();
        
        for (int j = 0; j < f.size(); j++) {
            if(f[j].numVertices() == 3) {
                triangles_count++;
            }
        }
    }
    
// Creating shader to write on screen
  glwidget()->makeCurrent();
    // Carregar shader, compile & link 
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/ModelInfo2/show.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/ModelInfo2/show.frag");

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
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

        float z = -0.99999;
        // Create VBO with (x,y,z) coordinates
        float coords[] = { -1, -1, z, 
                            1, -1, z, 
                           -1,  1, z, 
                            1,  1, z};

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

void ModelInfo2::postFrame()
{
  GLWidget &g=*glwidget();
  g.makeCurrent();
    const int SIZE = 1024;
    // 1. Create image with text
    QImage image(SIZE,SIZE,QImage::Format_RGB32);
    image.fill(Qt::white);    
    QPainter painter;
    painter.begin(&image);
    QFont font;
    font.setPixelSize(32);
    painter.setFont(font);
    painter.setPen(QColor(50,50,50));
    int x = 15;
    int y = 50;
    char s[50];
    sprintf(s, "Objects: %d", objs.size());
    painter.drawText(x, y, QString(s));
    sprintf(s, "Polygons: %d", faces_count);
    painter.drawText(x, y+50, QString(s));
    sprintf(s, "Vertices: %d", verts_count);
    painter.drawText(x, y+100, QString(s));
    sprintf(s, "Triangles(%%): %d%%", (100.0 * triangles_count) / faces_count);
    painter.drawText(x, y+150, QString(s));
    painter.end();

    // 2. Create texture
    const int textureUnit = 5;
    g.glActiveTexture(GL_TEXTURE0+textureUnit);
    QImage im0 = image.mirrored(false, true).convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly);
	g.glGenTextures( 1, &textureID);
	g.glBindTexture(GL_TEXTURE_2D, textureID);
	g.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, im0.width(), im0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im0.bits());
	g.glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
    // Pass 2. Draw quad using texture
    program->bind();
    program->setUniformValue("colorMap", textureUnit);
    program->setUniformValue("WIDTH", float(glwidget()->width()));
    program->setUniformValue("HEIGHT", float(glwidget()->height()));    
 
    // quad covering viewport 
    drawRect(g);
    program->release();
    g.glBindTexture(GL_TEXTURE_2D, 0);

    
    g.glDeleteTextures(1, &textureID);


}

void ModelInfo2::onObjectAdd()
{
    faces_count = 0;
    verts_count = 0;
    triangles_count = 0;
    objs = scene()->objects();
    for(int i = 0; i < objs.size(); i++) {
        vector<Face> f = objs[i].faces();
        faces_count += f.size();
        
        vector<Vertex> v = objs[i].vertices();
        verts_count += v.size();
        
        for (int j = 0; j < f.size(); j++) {
            if(f[j].numVertices() == 3) {
                triangles_count++;
            }
        }
    }
}
