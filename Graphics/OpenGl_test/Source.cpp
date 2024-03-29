#include <GL/glut.h>
#include<iostream>
using namespace std;

int rx = 100, ry = 125;
int xCenter = 250, yCenter = 250;

void myinit(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
		glVertex2i(x, y);
	glEnd();
}

void ellipseMidPoint()
{
	float x = 0;
	float y = ry;
	float p1 = ry * ry - (rx * rx) * ry + (rx * rx) * (0.25);
	float dx = 2 * (ry * ry) * x;
	float dy = 2 * (rx * rx) * y;
	glColor3ub(rand() % 255, rand() % 255, rand() % 255);
	while (dx < dy)
	{
		setPixel(xCenter + x, yCenter + y);
		setPixel(xCenter - x, yCenter + y);
		setPixel(xCenter + x, yCenter - y);
		setPixel(xCenter - x, yCenter - y);
		if (p1 < 0)
		{
			x = x + 1;
			dx = 2 * (ry * ry) * x;
			p1 = p1 + dx + (ry * ry);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dx = 2 * (ry * ry) * x;
			dy = 2 * (rx * rx) * y;
			p1 = p1 + dx - dy + (ry * ry);
		}
	}
	glFlush();

	float p2 = (ry * ry) * (x + 0.5) * (x + 0.5) + (rx * rx) * (y
		- 1) * (y - 1) - (rx * rx) * (ry * ry);
	glColor3ub(rand() % 255, rand() % 255, rand() % 255);
	while (y > 0)
	{
		setPixel(xCenter + x, yCenter + y);
		setPixel(xCenter - x, yCenter + y);
		setPixel(xCenter + x, yCenter - y);
		setPixel(xCenter - x, yCenter - y);
		if (p2 > 0)
		{
			x = x;
			y = y - 1;
			dy = 2 * (rx * rx) * y;
			p2 = p2 - dy + (rx * rx);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dy = dy - 2 * (rx * rx);
			dx = dx + 2 * (ry * ry);
			p2 = p2 + dx -
				dy + (rx * rx);
		}
	}
	glFlush();
}

void triangle() {
	GLfloat BlueCol[3] = { 0,0,1 };
	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0); /* ������� */
		glVertex3f(0.0, 0.0, 0.0);
		glColor3ub(0, 255, 0); /* ������� */
		glVertex3f(1.0, 0.0, 0.0);
		glColor3fv(BlueCol); /* ����� */
		glVertex3f(1.0, 1.0, 0.0);
	glEnd();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(2.0);
	ellipseMidPoint();
	//triangle();
	glFlush();
}

void func()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(2.0);

	glBegin(GL_QUADS);
		glColor3f(1.0, 1.0, 1.0);
		glVertex2i(250, 450);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2i(250, 150);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2i(550, 150);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2i(550, 450);
	glEnd();

	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Testing OpenGL");
	myinit();
	glutDisplayFunc(func);
	glutMainLoop();
	return 0;
}
