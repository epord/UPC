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

#include "Selection.h"
#include "glwidget.h"
#include <cassert>
#include <cmath>

Selection::~Selection() {
  cleanUp();
}

void Selection::onSceneClear()
{
    cleanUp();
}

void Selection::cleanUp()
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

bool Selection::drawScene()
{
  return false;
}

void Selection::drawBoundingBox()
{
    GLWidget &g = *glwidget();
    g.makeCurrent();
    
    g.glBindVertexArray(VAOs[0]);
    g.glDrawArrays(GL_LINE_STRIP, 0, 16);
    g.glBindVertexArray(0);
}

void Selection::onPluginLoad()
{
    calculateBoundingBoxes();
    objCount = scene()->objects().size();
    
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Selection/drawBoundingBox.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/Selection/drawBoundingBox.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
}

void Selection::onObjectAdd()
{
    calculateBoundingBoxes();
    objCount = scene()->objects().size();
}

void Selection::preFrame()
{

}

void Selection::postFrame()
{
    GLWidget &g = *glwidget();
    
    int i = scene()->selectedObject();
    cout << i << endl;
    if (i != -1) {
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

void Selection::keyPressEvent ( QKeyEvent * q) {
    int selectedObjectID = q->key() - 48;
    if (selectedObjectID < objCount && selectedObjectID >= 0) {
        scene()->setSelectedObject(selectedObjectID);
    } else {
        scene()->setSelectedObject(-1);
    }
}

void Selection::calculateBoundingBoxes()
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
    
    // Add boundingBox of objects
    vector<Object> objs = scene()->objects();
    for (int i = 0; i < objs.size(); i++) {
        Object& obj = objs[i];
        obj.computeBoundingBox();
        minBoundingBox.push_back(obj.boundingBox().min());
        maxBoundingBox.push_back(obj.boundingBox().max());
    }
}
