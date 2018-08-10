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

#include "MouseSelect.h"
#include "glwidget.h"
#include <cassert>
#include <cmath>

MouseSelect::~MouseSelect() {
  cleanUp();
}

void MouseSelect::onSceneClear()
{
    cleanUp();
}

void MouseSelect::cleanUp()
{
    GLWidget &g = *glwidget();
    g.glDeleteBuffers(box_coordBuffers.size(),  &box_coordBuffers[0]);
    g.glDeleteBuffers(coordBuffers.size(),  &coordBuffers[0]);
    g.glDeleteBuffers(normalBuffers.size(), &normalBuffers[0]);
    g.glDeleteBuffers(stBuffers.size(),  &stBuffers[0]);
    g.glDeleteBuffers(colorBuffers.size(),  &colorBuffers[0]);
    g.glDeleteVertexArrays(box_VAOs.size(), &box_VAOs[0]);
    g.glDeleteVertexArrays(VAOs.size(), &VAOs[0]);
    box_coordBuffers.clear();
    coordBuffers.clear();
    normalBuffers.clear();
    stBuffers.clear();
    colorBuffers.clear();
    box_VAOs.clear();
    VAOs.clear();
    numIndices.clear();
    minBoundingBox.clear();
    maxBoundingBox.clear();
}

void MouseSelect::onPluginLoad()
{
    // Add objects and bounding box
    for(unsigned int i=0; i<scene()->objects().size(); i++)
        addVBO(i);
    
    calculateBoundingBoxes();
    objCount = scene()->objects().size();
    
    
    // Bounding box Shader
    box_vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    box_vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/MouseSelect/drawBoundingBox.vert");
    cout << "BBVS log:" << box_vs->log().toStdString() << endl;
    
    box_fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    box_fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/MouseSelect/drawBoundingBox.frag");
    cout << "BBFS log:" << box_fs->log().toStdString() << endl;
    
    box_program = new QOpenGLShaderProgram(this);
    box_program->addShader(box_vs);
    box_program->addShader(box_fs);
    box_program->link();
    cout << "Link log:" << box_program->log().toStdString() << endl;
    
    
    // Color buffer shader
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/MouseSelect/colorBuffer.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;
    
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("/Users/epord/UPC/G/NewViewer_1b32186/plugins/MouseSelect/colorBuffer.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;
    
    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;
}

void MouseSelect::onObjectAdd()
{
    addVBO( scene()->objects().size() - 1 );
    
    calculateBoundingBoxes();
    objCount = scene()->objects().size();
}

void MouseSelect::preFrame()
{
}

    
bool MouseSelect::drawScene()
{
        GLWidget &g = *glwidget();
        g.makeCurrent();
        for(unsigned int i=0; i<VAOs.size(); i++) // for each buffer (that is, for each object)
        {
            g.glBindVertexArray(VAOs[i]);
            g.glDrawArrays(GL_TRIANGLES, 0, numIndices[i]);
        }
        g.glBindVertexArray(0);
//        for(unsigned int i=0; i<VAOs.size(); i++) drawObject(i);
    return true;
}

void MouseSelect::drawBoundingBox(int i)
{
    GLWidget &g = *glwidget();
    
    box_program->bind();
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    box_program->setUniformValue("modelViewProjectionMatrix", MVP);
    box_program->setUniformValue("maxBoundingBox", QVector4D(maxBoundingBox[i].x(),maxBoundingBox[i].y(),maxBoundingBox[i].z(), 0));
    box_program->setUniformValue("minBoundingBox", QVector4D(minBoundingBox[i].x(),minBoundingBox[i].y(),minBoundingBox[i].z(), 0));
    
    g.makeCurrent();
    
    g.glBindVertexArray(box_VAOs[0]);
    g.glDrawArrays(GL_LINE_STRIP, 0, 16);
    g.glBindVertexArray(0);
    
    box_program->release();
}

void MouseSelect::postFrame()
{
    int i = scene()->selectedObject();
    
    if (i != -1) {
        drawBoundingBox(i);
    }
}

void MouseSelect::keyPressEvent ( QKeyEvent * q) {
    int selectedObjectID = q->key() - 48;
    if (selectedObjectID < objCount && selectedObjectID >= 0) {
        scene()->setSelectedObject(selectedObjectID);
    } else {
        scene()->setSelectedObject(-1);
    }
}

bool MouseSelect::drawObject(int i) {
    GLWidget &g = *glwidget();
    
    program->bind();
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);
    program->setUniformValue("color", QVector4D((i+1)/255.0f, 0, 0, 0));
    
    g.glBindVertexArray(VAOs[i]);
    g.glDrawArrays(GL_TRIANGLES, 0, numIndices[i]);
    g.glBindVertexArray(0);
    
    program->release();
    return true;
}

void MouseSelect::mouseReleaseEvent ( QMouseEvent * e) {
    if (! (e->button() & Qt::LeftButton)) return;
    if ( e->modifiers() & (Qt::ShiftModifier)) return;
    if (! (e->modifiers() & Qt::ControlModifier)) return;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    for(unsigned int i=0; i<VAOs.size(); i++)
        drawObject(i);
    
    int x = e->x();
    int y = glwidget()->height() - e->y();
    GLubyte read[4];
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, read);
    
    int selectedObjectID = read[0] - 1;
    if (selectedObjectID < objCount && selectedObjectID >= 0) {
        scene()->setSelectedObject(selectedObjectID);
    } else {
        scene()->setSelectedObject(-1);
    }
}

void MouseSelect::calculateBoundingBoxes()
{
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
    box_VAOs.push_back(VAO);
    g.glBindVertexArray(VAO);
    
    GLuint coordBufferID;
    g.glGenBuffers(1, &coordBufferID);
    box_coordBuffers.push_back(coordBufferID);
    
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


void MouseSelect::addVBO(unsigned int currentObject)
{
    //
    // For simplicity, we construct VBOs with replicated vertices (a copy
    // for each triangle to which they belong:
    //
    const Object& obj = scene()->objects()[currentObject];
    unsigned int numvertices = obj.faces().size()*3;  // it's all triangles...
    vector<float> vertices; // (x,y,z)    Final size: 9*number of triangles
    vector<float> normals;  // (nx,ny,nz) Final size: 9*number of triangles
    vector<float> colors;   // (r, g, b)  Final size: 9*number of triangles
    vector<float> texCoords;// (s, t)     Final size: 6*number of triangles
    auto verts = obj.vertices();
    auto Ns = obj.vertNormals();
    auto texcords = obj.vertTexCoords();
    
    for (auto&& f: obj.faces()) {
        Point P = verts[f.vertexIndex(0)].coord();
        vertices.push_back(P.x()); vertices.push_back(P.y()); vertices.push_back(P.z());
        Vector V=Ns[f.normalIndex(0)];
        normals.push_back(V.x()); normals.push_back(V.y()); normals.push_back(V.z());
        colors.push_back(fabs(V.x())); colors.push_back(fabs(V.y())); colors.push_back(fabs(V.z()));
        auto TC=texcords[f.texcoordsIndex(0)];
        texCoords.push_back(TC.first);  texCoords.push_back(TC.second);
        
        P = verts[f.vertexIndex(1)].coord();
        vertices.push_back(P.x()); vertices.push_back(P.y()); vertices.push_back(P.z());
        V=Ns[f.normalIndex(1)];
        normals.push_back(V.x()); normals.push_back(V.y()); normals.push_back(V.z());
        colors.push_back(fabs(V.x())); colors.push_back(fabs(V.y())); colors.push_back(fabs(V.z()));
        TC=texcords[f.texcoordsIndex(1)];
        texCoords.push_back(TC.first);  texCoords.push_back(TC.second);
        
        P = verts[f.vertexIndex(2)].coord();
        vertices.push_back(P.x()); vertices.push_back(P.y()); vertices.push_back(P.z());
        V=Ns[f.normalIndex(2)];
        normals.push_back(V.x()); normals.push_back(V.y()); normals.push_back(V.z());
        colors.push_back(fabs(V.x())); colors.push_back(fabs(V.y())); colors.push_back(fabs(V.z()));
        TC=texcords[f.texcoordsIndex(2)];
        texCoords.push_back(TC.first);  texCoords.push_back(TC.second);
    }
    
    assert(vertices.size() == 3*numvertices);
    assert(normals.size() == 3*numvertices);
    assert(colors.size() == 3*numvertices);
    assert(texCoords.size() == 2*numvertices);
    
    // Step 2: Create VAO and empty buffers (coords, normals, ...)
    GLWidget& g = *glwidget();
    GLuint VAO;
    g.glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    g.glBindVertexArray(VAO);
    
    GLuint coordBufferID;
    g.glGenBuffers(1, &coordBufferID);
    coordBuffers.push_back(coordBufferID);
    
    GLuint normalBufferID;
    g.glGenBuffers(1, &normalBufferID);
    normalBuffers.push_back(normalBufferID);
    
    GLuint stBufferID;
    g.glGenBuffers(1, &stBufferID);
    stBuffers.push_back(stBufferID);
    
    GLuint colorBufferID;
    g.glGenBuffers(1, &colorBufferID);
    colorBuffers.push_back(colorBufferID);
    
    numIndices.push_back(numvertices);
    // Step 3: Define VBO data (coords, normals, ...)
    g.glBindBuffer(GL_ARRAY_BUFFER, coordBufferID);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(0);
    
    g.glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(1);
    
    g.glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*colors.size(), &colors[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(2);
    
    g.glBindBuffer(GL_ARRAY_BUFFER, stBufferID);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    g.glEnableVertexAttribArray(3);
    
    g.glBindBuffer(GL_ARRAY_BUFFER,0);
    g.glBindVertexArray(0);
}
