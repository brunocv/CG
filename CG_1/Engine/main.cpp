
#include <sstream>
#include <string>
#include <cstring>
#include <string.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>  
#include "tinyxml2.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static float r = 10;
static float a = 315;
static float b = 45;

struct Triangulo {
	float x1;
	float y1;
	float z1;

	float x2;
	float y2;
	float z2;

	float x3;
	float y3;
	float z3;
};

static struct Triangulo * cone = NULL;
static unsigned int cone_length = 0;

static struct Triangulo * box = NULL;
static unsigned int box_length = 0;

static struct Triangulo * sphere = NULL;
static unsigned int sphere_length = 0;

static struct Triangulo * plane = NULL;
static unsigned int plane_length = 0;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawCone() {
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < cone_length; i++) {

		glColor3f(1, 0, 0);
		glVertex3f(cone[i].x1, cone[i].y1, cone[i].z1);
		glVertex3f(cone[i].x2, cone[i].y2, cone[i].z2);
		glVertex3f(cone[i].x3, cone[i].y3, cone[i].z3);
	}
	glEnd();
}

void drawBox() {

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < box_length; i++) {

		glColor3f(0, 1, 0);
		glVertex3f(box[i].x1, box[i].y1, box[i].z1);
		glVertex3f(box[i].x2, box[i].y2, box[i].z2);
		glVertex3f(box[i].x3, box[i].y3, box[i].z3);
	}
	glEnd();
}

void drawSphere() {

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < sphere_length; i++) {

		glColor3f(0, 0, 1);
		glVertex3f(sphere[i].x1, sphere[i].y1, sphere[i].z1);
		glVertex3f(sphere[i].x2, sphere[i].y2, sphere[i].z2);
		glVertex3f(sphere[i].x3, sphere[i].y3, sphere[i].z3);
	}
	glEnd();
}

void drawPlane() {

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < plane_length; i++) {

		glColor3f(1, 1, 1);
		glVertex3f(plane[i].x1, plane[i].y1, plane[i].z1);
		glVertex3f(plane[i].x2, plane[i].y2, plane[i].z2);
		glVertex3f(plane[i].x3, plane[i].y3, plane[i].z3);
	}
	glEnd();
}

void processKeys(unsigned char c, int xx, int yy){
	switch (c) {
	case 'w': {
		b += 1;
	} break;
	case 's': {
		b -= 1;
	} break;
	case 'a': {
		a -= 1;
	} break;
	case 'd': {
		a += 1;
	} break;

	case 'k': {
		r -= 1;
	} break;
	case 'j': {
		r += 1;
	} break;
	case '-': {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} break;
	case '.': {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	} break;
	case '#': {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} break;
	}

	glutPostRedisplay();
}


float deg2rad(float deg){
	return deg * M_PI / 180;
}

int parseFig(const char * file, struct Triangulo ** fig, unsigned * length){
	FILE * ptr_file = fopen(file, "r");
	if (!ptr_file)
		return false;

	unsigned ntriangles = 0;
	fscanf(ptr_file, "%u\n", &ntriangles);

	struct Triangulo * ret =(struct Triangulo*) calloc(ntriangles, sizeof(struct Triangulo));
	if (!ret) {
		fclose(ptr_file);
		return false;
	}

	unsigned i = 0;
	for (; i < ntriangles; i++) {
		struct Triangulo tmp;
		int res = fscanf(ptr_file,
			"%f %f %f %f %f %f %f %f %f\n",
			&tmp.x1, &tmp.y1, &tmp.z1,
			&tmp.x2, &tmp.y2, &tmp.z2,
			&tmp.x3, &tmp.y3, &tmp.z3);

		if (res != 9)
			break;
		ret[i] = tmp;
	}

	fclose(ptr_file);

	if (i == ntriangles) {
		*length = ntriangles;
		*fig = ret;
	}
	else {
		free(ret);
	}

	return i == ntriangles;
}

struct Document{
	std::string file1;
	std::string file2;
	std::string file3;
	std::string file4;
};

static std::string getElementText(tinyxml2::XMLElement *_element) {
	std::string value;
	if (_element != NULL) {
		value = _element->GetText();
	}

	return value;
}

struct Document parseXml(char* file) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *a;

		a = doc.FirstChildElement("scene");
		
		Document * thisDoc = new Document();

		while (a) {
			tinyxml2::XMLElement *b;
			b = a->FirstChildElement("model");

			while (b) {
				thisDoc->file1 = getElementText(b);
				const char *x = thisDoc->file1.c_str();
				printf("%s\n", x);

				b = b->NextSiblingElement("model");

				thisDoc->file2 = getElementText(b);
				const char *y = thisDoc->file2.c_str();
				printf("%s\n", y);
				
				b = b->NextSiblingElement("model");

				thisDoc->file3 = getElementText(b);
				const char *z = thisDoc->file3.c_str();
				printf("%s\n", z);

				b = b->NextSiblingElement("model");

				thisDoc->file4 = getElementText(b);
				const char *t = thisDoc->file4.c_str();
				printf("%s\n", t);

				b = b->NextSiblingElement("model");

			}
			a = a->NextSiblingElement("scene");

		}
		return *thisDoc;
	}
}

void renderScene(void) {
	float rc = r * cos(deg2rad(b));

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(
		rc * sin(deg2rad(a)), r * sin(deg2rad(b)), rc * cos(deg2rad(a)),
		0, 0, 0,
		0, 1, 0);

	//Draw stuff
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);
	glEnd();

	drawPlane();
	drawCone();
	drawSphere();
	drawBox();

	// End of frame
	glutSwapBuffers();
}

int main(int argc, char *argv[]) {

	if (argc != 2) return 0;
	else {
		Document * thisDoc = new Document();

		*thisDoc = parseXml(argv[1]);
		
		const char *file1 = thisDoc->file1.c_str();
		const char *file2 = thisDoc->file2.c_str();
		const char *file3 = thisDoc->file3.c_str();
		const char *file4 = thisDoc->file4.c_str();

		parseFig(file2, &cone, &cone_length);
		parseFig(file3, &box, &box_length);
		parseFig(file4, &sphere, &sphere_length);
		parseFig(file1, &plane, &plane_length);
		
		// init GLUT and the window
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(800, 800);
		glutCreateWindow("CG_trabalho");

		// Required callback registry
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);
		glutKeyboardFunc(processKeys);

		//  OpenGL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// enter GLUT's main cycle
		glutMainLoop();
		
		return 1;
	}

}