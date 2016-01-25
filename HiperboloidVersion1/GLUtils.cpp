#include "stdafx.h"
#include "HiperboloidVersion1.h"


void updating(void* params);
void moving(void* params);

//����� � ���������
Screen screen;

HGLRC  hGLRC;              // ��������� �������� ����������
HDC  hDC;              // ��������� �������� ���������� GDI
extern HWND  hWnd;				// ����� ����� ������� ���������� ����

#define hInstance hInst;
extern HINSTANCE  hInstance;              // ����� ����� ������� ���������� ���������
void InitOpenGL(){
	
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0

	};


	//�������� ������ �� ����������� ��� ����� ��������
	hDC = GetDC(hWnd);


	//������ ��������� �����
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);

	//������ ��������� �����
	SetPixelFormat(hDC, pixelFormat, &pfd);

	//�������� OpenGL, ��� ����� ��������
	hGLRC = wglCreateContext(hDC);

	//������� �������� ��������
	wglMakeCurrent(hDC, hGLRC);
	
	/*screen.add(new Block(-1.0f, 1.0f));
	screen.add(new Block(-0.8f, 1.0f));
	screen.add(new Block(-0.6f, 1.0f));*/
	for (float i = 0; i < 0.9f; i += 0.11f){
		for (float j = -0.9f; j < 0.9f; j += 0.4f){
			screen.add(new MovableBlock(j, i));
			screen.add(new Block(j + 0.2f, i-0.05f));
			//screen.add(new Block(j, i));
			//screen.add(new BlockFromLines(j, i));
		}
	}
	//screen.add(new BlockFromLines(0, 0));
	//screen.add(new Ball(-0.5f));
	for (int i = 0; i < 1; i++){
		screen.add(new Ball());
		screen.add(new Ball(0.5f));
		screen.add(new Ball(-0.5f));
	}
	/*screen.add(new Ball());*/
	_beginthread(updating, 0, NULL);
	_beginthread(moving, 0, NULL);
}




OpenGLCoord ConvertMouseCoord(POINT currentMouse){
	RECT windowSize;
	GetClientRect(hWnd, &windowSize);
	OpenGLCoord mouseOpenGLCoord;
	float x = currentMouse.x;
	float h = windowSize.right;
	float ax = 2 * x / h - 1;
	float y = currentMouse.y;
	float w = windowSize.bottom;
	float ay = 2 * (w - y) / w - 1;

	mouseOpenGLCoord.coordX = ax;
	mouseOpenGLCoord.coordY = ay;
	return mouseOpenGLCoord;
}

void PaintOpenGL(){

	//������������� ������� ������ ������
	glClearColor(0, 0, 0, 0);

	//glPushMatrix();
	

	glColor3f(1.0, 1.0, 1.0);
	screen.paint();
	
	//glPopMatrix();
	//new ������ (������ ����������)
	SwapBuffers(hDC);
	//�������� ����� ����� � ����� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glutSwapBuffers();

}
void HandleMouseMove(){
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);
	OpenGLCoord openGLCoord = ConvertMouseCoord(point);
	screen.board.setXCoord(openGLCoord.coordX);
	//InvalidateRect(hWnd, NULL, TRUE);
}

void updating(void* params){
	//long t;
	//long dt;
	while (hWnd){
		//t = GetCurrentTime();
		InvalidateRect(hWnd, NULL, NULL);
		//dt = GetCurrentTime() - t;
		//if (dt < 15)
		Sleep(15);
		
	}
}

void moving(void* params){
	try{
		while (hWnd){
			Sleep(5);
			screen.move();
		}
	}
	catch (int a){
	}
}


void Normalize(Vector& v){
	float n = sqrtf(v.x*v.x + v.y*v.y);
	v.x /= n;
	v.y /= n;

	//float r = sqrtf(v.x*v.x + v.y*v.y);
}

