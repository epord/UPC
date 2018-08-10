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

#include "drawvbong.h"
#include "glwidget.h"
#include <cassert>
#include <cmath>

DrawVBOng::~DrawVBOng() {
  cleanUp();
}

void DrawVBOng::onSceneClear()
{
    cleanUp();
}

void DrawVBOng::cleanUp()
{
  GLWidget &g = *glwidget();
  g.glDeleteBuffers(coordBuffers.size(),  &coordBuffers[0]);
  g.glDeleteBuffers(normalBuffers.size(), &normalBuffers[0]);
  g.glDeleteBuffers(stBuffers.size(),  &stBuffers[0]);
  g.glDeleteBuffers(colorBuffers.size(),  &colorBuffers[0]);
  g.glDeleteVertexArrays(VAOs.size(), &VAOs[0]);
  coordBuffers.clear();
  normalBuffers.clear();
  stBuffers.clear();
  colorBuffers.clear();
  VAOs.clear();
  numIndices.clear();
  minBoundingBox.clear();
  maxBoundingBox.clear();
}

bool DrawVBOng::drawScene()
{
  return false;
}

void DrawVBOng::drawBoundingBox()
{
    GLWidget &g = *glwidget();
    g.makeCurrent();
    
    g.glBindVertexArray(VAOs[0]);
    g.glDrawArrays(GL_LINE_STRIP, 0, 16);
    g.glBindVertexArray(0);
}

void DrawVBOng::onPluginLoad()
{
    calculateBoundingBoxes();
    
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/drawvbong/drawBoundingBox.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/drawvbong/drawBoundingBox.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
}

void DrawVBOng::onObjectAdd()
{
    calculateBoundingBoxes();
    cout << "ADDED OBJECT" << endl;
}

void DrawVBOng::preFrame()
{
//    drawBoundingBox();

}

void DrawVBOng::postFrame()
{
    GLWidget &g = *glwidget();
    
    for(int i = 0; i < minBoundingBox.size(); i++) {
        program->bind();
        QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
        program->setUniformValue("modelViewProjectionMatrix", MVP);
        program->setUniformValue("maxBoundingBox", QVector4D(maxBoundingBox[i].x(),maxBoundingBox[i].y(),maxBoundingBox[i].z(), 0));
        program->setUniformValue("minBoundingBox", QVector4D(minBoundingBox[i].x(),minBoundingBox[i].y(),minBoundingBox[i].z(), 0));
        
        g.makeCurrent();
        
        g.glBindVertexArray(VAOs[0]);
        g.glDrawArrays(GL_LINE_STRIP, 0, 16);
        g.glBindVertexArray(0);
        
        program->release();
    }
}

void DrawVBOng::calculateBoundingBoxes()
{
    //
    // For simplicity, we construct VBOs with replicated vertices (a copy
    // for each triangle to which they belong:
    //
    unsigned int numvertices = 16;  // every vertex is present twice to create every edge
    vector<float> vertices;
    
    // Cara frontal
    vertices.push_back(0); vertices.push_back(0); vertices.push_back(0);
    vertices.push_back(0); vertices.push_back(1); vertices.push_back(0);
    vertices.push_back(1); vertices.push_back(1); vertices.push_back(0);
    vertices.push_back(1); vertices.push_back(0); vertices.push_back(0);

    // Cara derecha
    vertices.push_back(0); vertices.push_back(0); vertices.push_back(0);
    vertices.push_back(0); vertices.push_back(0); vertices.push_back(1);
    vertices.push_back(0); vertices.push_back(1); vertices.push_back(1);
    vertices.push_back(0); vertices.push_back(1); vertices.push_back(0);
    
    // Cara trasera
    vertices.push_back(0); vertices.push_back(1); vertices.push_back(1);
    vertices.push_back(1); vertices.push_back(1); vertices.push_back(1);
    vertices.push_back(1); vertices.push_back(0); vertices.push_back(1);
    vertices.push_back(0); vertices.push_back(0); vertices.push_back(1);
    
    // Cara izquierda
    vertices.push_back(1); vertices.push_back(0); vertices.push_back(1);
    vertices.push_back(1); vertices.push_back(0); vertices.push_back(0);
    vertices.push_back(1); vertices.push_back(1); vertices.push_back(0);
    vertices.push_back(1); vertices.push_back(1); vertices.push_back(1);

    
    // Step 2: Create VAO and empty buffers (coords, normals, ...)
    GLWidget& g = *glwidget();
    GLuint VAO;
    g.glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    g.glBindVertexArray(VAO);
    
    GLuint coordBufferID;
    g.glGenBuffers(1, &coordBufferID);
    coordBuffers.push_back(coordBufferID);
    
    // Step 3: Define VBO data (coords, normals, ...)
    g.glBindBuffer(GL_ARRAY_BUFFER, coordBufferID);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(0);
    
    g.glBindBuffer(GL_ARRAY_BUFFER,0);
    g.glBindVertexArray(0);
    
//    vector<Object> objs = scene()->objects();
//    for (int j = 0; j < objs.size(); j++) {
//        Object obj = objs[j];
//        auto verts = obj.vertices();
//        Point max = verts[0].coord();
//        Point min = verts[0].coord();
//        for (int i = 1; i < verts.size(); i++) {
//            if (verts[i].coord().x() < min.x()) min.setX(verts[i].coord().x());
//            if (verts[i].coord().y() < min.y()) min.setY(verts[i].coord().y());
//            if (verts[i].coord().z() < min.z()) min.setZ(verts[i].coord().z());
//
//            if (verts[i].coord().x() > max.x()) max.setX(verts[i].coord().x());
//            if (verts[i].coord().y() > max.y()) max.setY(verts[i].coord().y());
//            if (verts[i].coord().z() > max.z()) max.setZ(verts[i].coord().z());
//        }
//        minBoundingBox.push_back(min);
//        maxBoundingBox.push_back(max);
//    }
    
    // Add boundingBox of objects
    vector<Object> objs = scene()->objects();
    for (int i = 0; i < objs.size(); i++) {
        Object& obj = objs[i];
        obj.computeBoundingBox();
        minBoundingBox.push_back(obj.boundingBox().min());
        maxBoundingBox.push_back(obj.boundingBox().max());
    }
}
