// LabaCG3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <glut.h>
#include <cmath>
#include <time.h>

#define PI 3.14159265

int camera[3] = { 6,6,6 };
int angle;
bool OX = true;
//double startpoints[4][4] = { {0, 0, 1,1},{1, 1, 1,1},{1, 0, 0,1},{0, 1, 0,1} };
double startpoints[4][4];

void axis()
{
	double axismatrix[6][3] = { {-1000, 0, 0},{1000, 0, 0},{0, -1000, 0},{0, 1000, 0},{0, 0, -1000},{0, 0, 1000} };//матрица осей
	glBegin(GL_LINES);
	int r = 0, g = 0, b = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (i == 0)
		{
			r = 1;
		}
		else if (i == 2)
		{
			r = 0;
			g = 1;
		}
		else if (i == 4)
		{
			g = 0;
			b = 1;
		}
		glColor3f(r, g, b);
		glVertex3dv(axismatrix[i]);//связь образуется между двумя точками объявленными последовательно
	}
	glEnd();//закрываем бегин
}


void multiplymatrix(double result[4][4], double A[4][4], double B[4][4])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result[i][j] = 0;
			for (int k = 0; k < 4; ++k)
			{
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void bilinearX(double points[4][4])
{
	GLdouble edges[1][3];
	for (double x = 0; x < 1; x += 0.1) {
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0, 0);
		for (double y = 0; y < 1; y += 0.1) {
			for (int i = 0; i < 3; ++i)
				edges[0][i] = (1 - x) * (1 - y) * points[0][i] + (1 - y) * x * points[1][i] + y * (1 - x) * points[2][i] + x * y * points[3][i];
			glVertex3dv(edges[0]);
		}
		glEnd();
	}
}
void bilinearY(double points[4][4])
{
	GLdouble edges[1][3];
	for (double y = 0; y < 1; y += 0.1) {
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0, 0);
		for (double x = 0; x < 1; x += 0.1) {
			for (int i = 0; i < 3; ++i)
				edges[0][i] = (1 - x) * (1 - y) * points[0][i] + (1 - y) * x * points[1][i] + y * (1 - x) * points[2][i] + x * y * points[3][i];
			glVertex3dv(edges[0]);
		}
		glEnd();
	}
}
void specialKeyDown(int key, int x, int y);
void defaultSurface()
{
	
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, 1, 0);
	axis();
	//отрисовка плоскости,  заменить на цикл
	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);

	glVertex3dv(startpoints[0]);
	glVertex3dv(startpoints[1]);

	glVertex3dv(startpoints[1]);
	glVertex3dv(startpoints[2]);

	glVertex3dv(startpoints[2]);
	glVertex3dv(startpoints[3]);

	glVertex3dv(startpoints[3]);
	glVertex3dv(startpoints[0]);
	glEnd();
	
	bilinearX(startpoints);
	bilinearY(startpoints);


	glutSpecialFunc(specialKeyDown);
	glutSwapBuffers();
}

void rotatedSurface()
{
	double cosB = cos(angle * PI / 180.0);
	double sinB = sin(angle * PI / 180.0);

	double rotationX[4][4] = { {1,0,0,0},{0,cosB,sinB,0},{0,-sinB,cosB,0},{0,0,0,1} };
	double rotationY[4][4] = { {cosB,0,-sinB,0}, {0,1,0,0}, {sinB,0,cosB,0},{0,0,0,1} };
	double resultmatrix[4][4];
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, 1, 0);
	axis();
	//отрисовка плоскости,  заменить на цикл
	glBegin(GL_POINTS);
	glColor3f(0, 0, 0);
	if (OX)
		multiplymatrix(resultmatrix, startpoints, rotationX);
	else
		multiplymatrix(resultmatrix, startpoints, rotationY);

	glVertex3dv(startpoints[0]);
	glVertex3dv(startpoints[1]);

	glVertex3dv(startpoints[1]);
	glVertex3dv(startpoints[2]);

	glVertex3dv(startpoints[2]);
	glVertex3dv(startpoints[3]);

	glVertex3dv(startpoints[3]);
	glVertex3dv(startpoints[0]);
	glEnd();
	
	bilinearX(resultmatrix);
	bilinearY(resultmatrix);


	glutSpecialFunc(specialKeyDown);
	glutSwapBuffers();
}


void View(void draw())
{

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Lab3.CG");
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutSpecialFunc(specialKeyDown);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(25.0, float(640) / float(480), 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
}

void specialKeyDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		camera[0] += 1;
		break;
	case GLUT_KEY_RIGHT:
		camera[0] -= 1;
		break;
	case GLUT_KEY_UP:
		camera[1] += 1;
		break;
	case GLUT_KEY_DOWN:
		camera[1] -= 1;
		break;
	case GLUT_KEY_F5:
		camera[2] += 1;
		break;
	case GLUT_KEY_F6:
		camera[2] -= 1;
		break;
	case GLUT_KEY_F1:
		View(defaultSurface);
		break;
	case GLUT_KEY_F2:
	{
		OX = false;
		View(rotatedSurface);
	}
		break;
	case GLUT_KEY_F3:
	{
		OX = true;
		View(rotatedSurface);
	}
	break;
	default:
		break;
	}
}


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "russian");

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++) {
			startpoints[i][j] = fRand(0,1);
		}
	for (int i = 0; i < 4; i++)
		startpoints[i][3] = 1;
			
	
		
	
	std::cout << "Введите угол поворота: ";
	std::cin >> angle;
	std::cout << "F1 - стандартный вид\nF2 - поворот по OY\nF3 - поворот по OX\nВращение камеры стрелки\nВершины куба\n";
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++) {
			std::cout << startpoints[i][j] << ',';
		}
		std::cout<<std::endl;
	}

	View(defaultSurface);
	return 0;

}

