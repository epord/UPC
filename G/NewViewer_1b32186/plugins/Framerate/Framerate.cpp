#include "Framerate.h"
#include "glwidget.h"
#include <QElapsedTimer>
#include <QTimer>

int frame_count = 0;

void Framerate::onPluginLoad()
{
    QTimer* timer = new QTimer();
    
    // setup signal and slot
    connect(timer, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
    
    // msec
    timer->start(1000);
}

void Framerate::MyTimerSlot()
{
    cout << frame_count << endl;
    frame_count = 0;
}

void Framerate::preFrame()
{
	
}

void Framerate::postFrame()
{
    frame_count++;
}

void Framerate::onObjectAdd()
{
	
}

bool Framerate::drawScene()
{
	return false; // return true only if implemented
}

bool Framerate::drawObject(int)
{
	return false; // return true only if implemented
}

bool Framerate::paintGL()
{
	return false; // return true only if implemented
}

void Framerate::keyPressEvent(QKeyEvent *)
{
	
}

void Framerate::mouseMoveEvent(QMouseEvent *)
{
	
}

