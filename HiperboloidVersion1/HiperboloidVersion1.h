#pragma once

#include "resource.h"

#include <Windows.h>

#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>
#include "Interfaces.h"
#include <memory>
using std::unique_ptr;
//#include <gl/glaux.h>
#include <process.h>
#include <list>
#include <vector>
#include <iterator>
using std::_List_iterator;
using std::list;
using std::vector;
#pragma comment(lib, "OpenGL32.lib")
void Normalize(Vector& v);
void InitOpenGL(); // функция инициализции в InitInstanse
void PaintOpenGL(); // функция отрисовки OpenGL
void HandleMouseMove();

struct OpenGLCoord{
	OpenGLCoord(){}
	OpenGLCoord(float x, float y) : coordX(x), coordY(y){}
	float coordX, coordY;
};
OpenGLCoord ConvertMouseCoord(POINT currentMouse);//переводит координаты клиента в координаты OpenGL



