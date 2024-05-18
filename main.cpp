#include <glut.h>
#include <math.h> // Sử dụng thư viện <math.h>
#include <fstream>
#include "object.h"

float angle = 0.0f, lx = 0.0f, lz = -1.0f, x = 0.0f, z = 5.0f, y = 1.0f, ly = 0.0f;
float deltaAngle = 0.0f, deltaMove = 0.0f;
int xOrigin = -1, yOrigin = -1;
const float moveSpeed = 0.5f;
const float rotateSpeed = 0.01f;
const float verticalMoveSpeed = 0.5f;

const float PI = 3.14159265f; // Định nghĩa hằng số PI

object objs;

void changeSize(int w, int h) {
	if (h == 0) h = 1;
	float ratio = 1.0f * w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void drawGrid() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = -100; i <= 100; i++) {
		glVertex3f((float)i, 0, (float)-100);
		glVertex3f((float)i, 0, (float)100);
		glVertex3f((float)-100, 0, (float)i);
		glVertex3f((float)100, 0, (float)i);
	}
	glEnd();
}

void computePos(float deltaMove, bool strafe = false) {
	if (strafe) {
		// Tính toán vector di chuyển ngang dựa trên góc xoay camera
		float strafeAngle = angle + PI / 2.0f;
		x += deltaMove * cos(strafeAngle) * moveSpeed;
		z += deltaMove * sin(strafeAngle) * moveSpeed;
	}
	else {
		x += deltaMove * lx * moveSpeed;
		z += deltaMove * lz * moveSpeed;
	}
}

void computeDir(float deltaAngle) {
	angle += deltaAngle * rotateSpeed;
	lx = sin(angle);
	lz = -cos(angle);
}

void processNormalKeys(unsigned char key, int xx, int yy) {
	switch (key) {
	case 27:
		exit(0);
	case 'w':
		computePos(1);
		break;
	case 's':
		computePos(-1);
		break;
	case 'a':
		computePos(-1, true); // Di chuyển sang trái
		break;
	case 'd':
		computePos(1, true); // Di chuyển sang phải
		break;
	case 'u':
		y += verticalMoveSpeed;
		break;
	case 'j':
		y -= verticalMoveSpeed;
		break;
	}
}

void processSpecialKeys(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP: // Nhìn lên trên
		ly += rotateSpeed;
		break;
	case GLUT_KEY_DOWN: // Nhìn xuống dưới
		ly -= rotateSpeed;
		break;
	case GLUT_KEY_LEFT: // Quay sang trái
		computeDir(-1);
		break;
	case GLUT_KEY_RIGHT: // Quay sang phải
		computeDir(1);
		break;
	}
}

void init() {
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(x, y, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);
	drawGrid();
	objs.renderObject();
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	objs.addObject("data/coconuts.obj", 10);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Grid with FPS Controls");

	init();

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}