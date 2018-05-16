#include "ShowDegree.h"
#include "glwidget.h"

float degree;

void ShowDegree::onPluginLoad()
{
    Object obj = scene()->objects()[0];
    vector<Face> f = obj.faces();
    int accum_vertices = 0;
    for (int j = 0; j < f.size(); j++) {
        accum_vertices += f[j].numVertices();
    }
    degree = (1.0 * accum_vertices)/obj.vertices().size();
}

void ShowDegree::preFrame()
{
	
}

void ShowDegree::postFrame()
{
    cout << degree << endl;
}

void ShowDegree::onObjectAdd()
{
	
}

bool ShowDegree::drawScene()
{
	return false; // return true only if implemented
}

bool ShowDegree::drawObject(int)
{
	return false; // return true only if implemented
}

bool ShowDegree::paintGL()
{
	return false; // return true only if implemented
}

void ShowDegree::keyPressEvent(QKeyEvent *)
{
	
}

void ShowDegree::mouseMoveEvent(QMouseEvent *)
{
	
}

