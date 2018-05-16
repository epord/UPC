#include "ModelInfo1.h"
#include "glwidget.h"

vector<Object> objs;
int faces_count;
int verts_count;
int triangles_count;

void ModelInfo1::onPluginLoad()
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

void ModelInfo1::preFrame()
{
	
}

void ModelInfo1::postFrame()
{
 
    cout << "Objects: " << objs.size() << endl;
    cout << "Polygons: " << faces_count << endl;
    cout << "Vertices: " << verts_count << endl;
    cout << "Triangles (%) : " << (100.0 * triangles_count) / faces_count << "%" << endl << endl;
}

void ModelInfo1::onObjectAdd()
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

bool ModelInfo1::drawScene()
{
	return false; // return true only if implemented
}

bool ModelInfo1::drawObject(int)
{
	return false; // return true only if implemented
}

bool ModelInfo1::paintGL()
{
	return false; // return true only if implemented
}

void ModelInfo1::keyPressEvent(QKeyEvent *)
{
	
}

void ModelInfo1::mouseMoveEvent(QMouseEvent *)
{
	
}

