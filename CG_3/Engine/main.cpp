
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
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "parser.h"

int frame = 0;
int timebase = 0;
float fps;
char buf[20];
GLuint buffers[3]; //sphere, disk and teapot

static float r = 1050;
static float a = 240;
static float b = 40;
#define POINT_COUNT 8

float tamanho(float *vetor) {
	float result = sqrt(vetor[0] * vetor[0] + vetor[1] * vetor[1] + vetor[2] * vetor[2]);
	return result;
}

void matriz_multiplicacao(float *m, float *v, float *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void getCatmullRomPoint(float t, float *p0, float *p1, float *p2, float *p3, float *pos, float *derivada) {
	// catmull-rom matrix
	float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
						{ 1.0f, -2.5f,  2.0f, -0.5f},
						{-0.5f,  0.0f,  0.5f,  0.0f},
						{ 0.0f,  1.0f,  0.0f,  0.0f} };

	float a[3][4];
	// Compute A = M * P
	//float *v[4] = {p0, p1, p2, p3};
	float x[4] = { p0[0],p1[0],p2[0],p3[0] };
	float y[4] = { p0[1],p1[1],p2[1],p3[1] };
	float z[4] = { p0[2],p1[2],p2[2],p3[2] };
	matriz_multiplicacao(m[0], x, a[0]);
	matriz_multiplicacao(m[0], y, a[1]);
	matriz_multiplicacao(m[0], z, a[2]);

	// Compute pos = T * A
	float T[4] = { (float)pow((double)t,3), (float)pow((double)t,2), t, 1 };
	pos[0] = { T[0] * a[0][0] + T[1] * a[0][1] + T[2] * a[0][2] + T[3] * a[0][3] };
	pos[1] = { T[0] * a[1][0] + T[1] * a[1][1] + T[2] * a[1][2] + T[3] * a[1][3] };
	pos[2] = { T[0] * a[2][0] + T[1] * a[2][1] + T[2] * a[2][2] + T[3] * a[2][3] };

	float d[4] = { (float)pow((double)3 * t,2), 2 * t, 1, 1 };
	derivada[0] = { d[0] * a[0][0] + d[1] * a[1][0] + d[2] * a[2][0] + d[3] * a[3][0] };
	derivada[1] = { d[0] * a[0][1] + d[1] * a[1][1] + d[2] * a[2][1] + d[3] * a[3][1] };
	derivada[2] = { d[0] * a[0][2] + d[1] * a[1][2] + d[2] * a[2][2] + d[3] * a[3][2] };
}

void getGlobalCatmullRomPoint(float gt, float *pos, float *derivada,struct Xml dc) {
	float t = gt * POINT_COUNT;
	float p[POINT_COUNT][3];
	int index = floor(t); 
	t = t - index; 

	for (int i = 0; i < POINT_COUNT; i++) {
		p[i][0] = dc.points[i].x;
		p[i][1] = dc.points[i].y;
		p[i][2] = dc.points[i].z;
	}

	int inddexes[4];
	inddexes[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	inddexes[1] = (inddexes[0] + 1) % POINT_COUNT;
	inddexes[2] = (inddexes[1] + 1) % POINT_COUNT;
	inddexes[3] = (inddexes[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, p[inddexes[0]], p[inddexes[1]], p[inddexes[2]], p[inddexes[3]], pos, derivada);
}

void desenhar_Catmull(struct Xml dc) {
	int i = 0;
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);

	for (i = 0; i < 100; i++) {
		float pos[3];
		float derivada[3];
		getGlobalCatmullRomPoint(i / 100.f, pos, derivada,dc);
		glColor3f(1, 1, 1);
		glVertex3f(pos[0], pos[1], pos[2]);
	}
	glEnd();
}

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
	gluPerspective(45.0f, ratio, 1.0f, 3000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void rotate(float time, float x, float y, float z) {
	static float angle;

	angle += time / 360;
	glRotatef(angle, x, y, z);
}

void draw(char *file, unsigned int size,struct Xml document) {
	if (!strcmp((document.order), "str")) {
		glScaled((GLdouble)document.scale_x, (GLdouble)document.scale_y, (GLdouble)document.scale_z);
		glTranslated((GLdouble)document.trans_x, (GLdouble)document.trans_y, (GLdouble)document.trans_z);
		if (document.rot_time != 0) rotate(document.rot_time,document.rot_x,document.rot_y,document.rot_z);
	}
	if (!strcmp((document.order), "srt")) {
		glScaled((GLdouble)document.scale_x, (GLdouble)document.scale_y, (GLdouble)document.scale_z);
		if (document.rot_time != 0) rotate(document.rot_time, document.rot_x, document.rot_y, document.rot_z);
		glTranslated((GLdouble)document.trans_x, (GLdouble)document.trans_y, (GLdouble)document.trans_z);
	}
	if (!strcmp((document.order), "tsr")) {
		glTranslated((GLdouble)document.trans_x, (GLdouble)document.trans_y, (GLdouble)document.trans_z);
		glScaled((GLdouble)document.scale_x, (GLdouble)document.scale_y, (GLdouble)document.scale_z);
		if (document.rot_time != 0) rotate(document.rot_time, document.rot_x, document.rot_y, document.rot_z);
	}
	if (!strcmp((document.order), "trs")) {
		glTranslated((GLdouble)document.trans_x, (GLdouble)document.trans_y, (GLdouble)document.trans_z);
		if (document.rot_time != 0) rotate(document.rot_time, document.rot_x, document.rot_y, document.rot_z);
		glScaled((GLdouble)document.scale_x, (GLdouble)document.scale_y, (GLdouble)document.scale_z);
	}
	if (!strcmp((document.order), "rts")) {
		if (document.rot_time != 0) rotate(document.rot_time, document.rot_x, document.rot_y, document.rot_z);
		glTranslated((GLdouble)document.trans_x, (GLdouble)document.trans_y, (GLdouble)document.trans_z);
		glScaled((GLdouble)document.scale_x, (GLdouble)document.scale_y, (GLdouble)document.scale_z);
	}
	if (!strcmp((document.order), "rst")) {
		if (document.rot_time != 0) rotate(document.rot_time, document.rot_x, document.rot_y, document.rot_z);
		glScaled((GLdouble)document.scale_x, (GLdouble)document.scale_y, (GLdouble)document.scale_z);
		glTranslated((GLdouble)document.trans_x, (GLdouble)document.trans_y, (GLdouble)document.trans_z);
	}

	if (!strcmp(file, "..\\..\\..\\Fig_files\\sphere.txt")) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, size * 3);
	}
	if (!strcmp(file, "..\\..\\..\\Fig_files\\disk.txt")) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, size * 3);
	}
	if (!strcmp(file, "..\\..\\..\\Fig_files\\teapot.txt")) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, size * 3);
	}
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

void drawChild(struct Xml * documents) {
	static double tempos1[20];

	for (int i = 0, j = 0; documents[i].nfiles > 0; i++) {
		glPushMatrix();

		j = 0;

		while (j < documents[i].nfiles) {
			if (documents[i].npontos > 0) {

				desenhar_Catmull(documents[i]);

				float pos[3];
				float der[3];
				getGlobalCatmullRomPoint(tempos1[i], pos, der, documents[i]);

				glTranslatef(pos[0], pos[1], pos[2]);
				glColor3f(1, 0, 0);
				draw(documents[i].file[j], documents[i].fig_length[j], documents[i]);
				tempos1[i] += documents[i].trans_time;

				j++;
			}
			else {
				draw(documents[i].file[j], documents[i].fig_length[j], documents[i]);
				j++;
			}
		}

		if (documents[i].child) drawChild(documents[i].child);
		glPopMatrix();
	}


}

void drawAll() {
	static double tempos[20];
	for (int i = 0, j = 0; documents[i].nfiles > 0; i++) {
		glPushMatrix();

		j = 0;

		while (j < documents[i].nfiles) {
			if (documents[i].npontos > 0) {

				desenhar_Catmull(documents[i]);

				float pos[3];
				float der[3];
				getGlobalCatmullRomPoint(tempos[i], pos, der, documents[i]);

				glTranslatef(pos[0], pos[1], pos[2]);
				glColor3f(1, 0, 0);
				draw(documents[i].file[j], documents[i].fig_length[j], documents[i]);
				tempos[i] += documents[i].trans_time;

				j++;
			}
			else{
				draw(documents[i].file[j], documents[i].fig_length[j], documents[i]);
				j++;
			}
		}
		
		if (documents[i].child) drawChild(documents[i].child);
		glPopMatrix();
	}

}

void renderScene(void) {
	float rc = r * cos(deg2rad(b));
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	//gluPerspective(50.0,1, 500, 700);
	gluLookAt(
		rc * sin(deg2rad(a)), r * sin(deg2rad(b)) , rc * cos(deg2rad(a)) ,
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
	
	drawAll();

	int time;
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		fps = frame * 1000.0 / (time - timebase);
		timebase = time;
		frame = 0;
	}

	sprintf(buf, "%f", fps);
	glutSetWindowTitle(buf);

	// End of frame
	glutSwapBuffers();
}

void infoChild(struct Xml * documents, int ordem) {
	
	int z;

	for (int i = 0; documents[i].nfiles > 0; i++) {
		z = 0;
		printf("\n\n ORDEM %d\n", ordem);
		printf("trans x %f\n", documents[i].trans_x);
		printf("trans y %f\n", documents[i].trans_y);
		printf("trans z %f\n", documents[i].trans_z);
		printf("trans time %f\n", documents[i].trans_time);
		printf("trans npontos %d \n", documents[i].npontos);
		printf("ang %f\n", documents[i].ang);
		printf("rot x %f\n", documents[i].rot_x);
		printf("rot y %f\n", documents[i].rot_y);
		printf("rot z %f\n", documents[i].rot_z);
		printf("rot time %f\n", documents[i].rot_time);
		printf("scale x %f\n", documents[i].scale_x);
		printf("scale y %f\n", documents[i].scale_y);
		printf("scale z %f\n", documents[i].scale_z);
		printf("nfiles %d\n", documents[i].nfiles);
		printf("nfilhos %d\n", documents[i].nfilhos);
		printf("order %s\n", documents[i].order);


		while (z < documents[i].nfiles) {
			printf("%s \n", documents[i].file[z]);
			z++;
		}
		z = 0;
		while (z < documents[i].npontos) {
			printf("point Z %d X = %f Y = %f Z = %f \n", z, documents[i].points[z].x, documents[i].points[z].y, documents[i].points[z].z);
			z++;
		}

		if (documents[i].child) infoChild(documents[i].child, ordem + 1);

	}
}


void info(struct Xml * documents,int ordem) {
	
	int z;

	for (int i = 0; documents[i].nfiles > 0; i++) {
		z = 0;
		printf("\n\n ORDEM %d\n",ordem);
		printf("trans x %f\n", documents[i].trans_x);
		printf("trans y %f\n", documents[i].trans_y);
		printf("trans z %f\n", documents[i].trans_z);
		printf("trans time %f\n", documents[i].trans_time);
		printf("trans npontos %d \n", documents[i].npontos);
		printf("ang %f\n", documents[i].ang);
		printf("rot x %f\n", documents[i].rot_x);
		printf("rot y %f\n", documents[i].rot_y);
		printf("rot z %f\n", documents[i].rot_z);
		printf("rot time %f\n", documents[i].rot_time);
		printf("scale x %f\n", documents[i].scale_x);
		printf("scale y %f\n", documents[i].scale_y);
		printf("scale z %f\n", documents[i].scale_z);
		printf("nfiles %d\n", documents[i].nfiles);
		printf("nfilhos %d\n", documents[i].nfilhos);
		printf("order %s\n", documents[i].order);


		while (z < documents[i].nfiles) {
			printf("%s \n", documents[i].file[z]);
			z++;
		}
		z = 0;
		while (z < documents[i].npontos) {
			printf("point Z %d X = %f Y = %f Z = %f \n",z,documents[i].points[z].x, documents[i].points[z].y, documents[i].points[z].z);
			z++;
		}

		if (documents[i].child) infoChild(documents[i].child, ordem + 1);
		
	}
}

void fillBuffersChild(struct Xml * documents,int sphere, int disk,int teapot) {
	int n = 0;
	int k = 0;
	for (int i = 0, j = 0; documents[i].nfiles > 0; i++) {

		j = 0;

		while (j < documents[i].nfiles) {
			if (!strcmp((documents[i].file[j]), "..\\..\\..\\Fig_files\\sphere.txt") && sphere == 0) {
				float *arraySphere = (float *)malloc(documents[i].fig_length[j] * 9 * sizeof(float));
				for ( k = 0, n = 0; k < documents[i].fig_length[j]; k++) {
					arraySphere[n++] = documents[i].fig[j][k].x1;
					arraySphere[n++] = documents[i].fig[j][k].y1;
					arraySphere[n++] = documents[i].fig[j][k].z1;
					arraySphere[n++] = documents[i].fig[j][k].x2;
					arraySphere[n++] = documents[i].fig[j][k].y2;
					arraySphere[n++] = documents[i].fig[j][k].z2;
					arraySphere[n++] = documents[i].fig[j][k].x3;
					arraySphere[n++] = documents[i].fig[j][k].y3;
					arraySphere[n++] = documents[i].fig[j][k].z3;
				}
				sphere = 1;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
				glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), arraySphere, GL_STATIC_DRAW);
			}
			if (!strcmp((documents[i].file[j]), "..\\..\\..\\Fig_files\\disk.txt") && disk == 0) {
				float *arrayDisk = (float *)malloc(documents[i].fig_length[j] * 9 * sizeof(float));
				for ( k = 0, n = 0; k < documents[i].fig_length[j]; k++) {
					arrayDisk[n++] = documents[i].fig[j][k].x1;
					arrayDisk[n++] = documents[i].fig[j][k].y1;
					arrayDisk[n++] = documents[i].fig[j][k].z1;
					arrayDisk[n++] = documents[i].fig[j][k].x2;
					arrayDisk[n++] = documents[i].fig[j][k].y2;
					arrayDisk[n++] = documents[i].fig[j][k].z2;
					arrayDisk[n++] = documents[i].fig[j][k].x3;
					arrayDisk[n++] = documents[i].fig[j][k].y3;
					arrayDisk[n++] = documents[i].fig[j][k].z3;
				}
				disk = 1;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
				glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), arrayDisk, GL_STATIC_DRAW);
			}
			if (!strcmp((documents[i].file[j]), "..\\..\\..\\Fig_files\\teapot.txt") && teapot == 0) {
				float *arrayTeaPot = (float *)malloc(documents[i].fig_length[j] * 9 * sizeof(float));
				for (k = 0, n = 0; k < documents[i].fig_length[j]; k++) {
					arrayTeaPot[n++] = documents[i].fig[j][k].x1;
					arrayTeaPot[n++] = documents[i].fig[j][k].y1;
					arrayTeaPot[n++] = documents[i].fig[j][k].z1;
					arrayTeaPot[n++] = documents[i].fig[j][k].x2;
					arrayTeaPot[n++] = documents[i].fig[j][k].y2;
					arrayTeaPot[n++] = documents[i].fig[j][k].z2;
					arrayTeaPot[n++] = documents[i].fig[j][k].x3;
					arrayTeaPot[n++] = documents[i].fig[j][k].y3;
					arrayTeaPot[n++] = documents[i].fig[j][k].z3;
				}
				teapot = 1;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
				glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), arrayTeaPot, GL_STATIC_DRAW);
			}
			j++;
		}

		if (documents[i].child) fillBuffersChild(documents[i].child, sphere, disk,teapot);
	}

}

void fillBuffers(struct Xml * documents) {
	int sphere = 0;
	int disk = 0;
	int teapot = 0;
	int k = 0;
	int n = 0;

	for (int i = 0, j = 0; documents[i].nfiles > 0; i++) {
		
		j = 0;

		while (j < documents[i].nfiles) {

			if (!strcmp((documents[i].file[j]), "..\\..\\..\\Fig_files\\sphere.txt") && sphere == 0) {
				float *arraySphere = (float *)malloc(documents[i].fig_length[j] * 9 * sizeof(float));
				for (k = 0, n = 0; k < documents[i].fig_length[j]; k++) {
					arraySphere[n++] = documents[i].fig[j][k].x1;
					arraySphere[n++] = documents[i].fig[j][k].y1;
					arraySphere[n++] = documents[i].fig[j][k].z1;
					arraySphere[n++] = documents[i].fig[j][k].x2;
					arraySphere[n++] = documents[i].fig[j][k].y2;
					arraySphere[n++] = documents[i].fig[j][k].z2;
					arraySphere[n++] = documents[i].fig[j][k].x3;
					arraySphere[n++] = documents[i].fig[j][k].y3;
					arraySphere[n++] = documents[i].fig[j][k].z3;
				}
				sphere = 1;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
				glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), arraySphere, GL_STATIC_DRAW);
			}
			if (!strcmp((documents[i].file[j]), "..\\..\\..\\Fig_files\\disk.txt") && disk == 0) {
				float *arrayDisk = (float *)malloc(documents[i].fig_length[j] * 9 * sizeof(float));
				for (k = 0, n = 0; k < documents[i].fig_length[j]; k++) {
					arrayDisk[n++] = documents[i].fig[j][k].x1;
					arrayDisk[n++] = documents[i].fig[j][k].y1;
					arrayDisk[n++] = documents[i].fig[j][k].z1;
					arrayDisk[n++] = documents[i].fig[j][k].x2;
					arrayDisk[n++] = documents[i].fig[j][k].y2;
					arrayDisk[n++] = documents[i].fig[j][k].z2;
					arrayDisk[n++] = documents[i].fig[j][k].x3;
					arrayDisk[n++] = documents[i].fig[j][k].y3;
					arrayDisk[n++] = documents[i].fig[j][k].z3;
				}
				disk = 1;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
				glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), arrayDisk, GL_STATIC_DRAW);
			}
			if (!strcmp((documents[i].file[j]), "..\\..\\..\\Fig_files\\teapot.txt") && teapot == 0) {
				float *arrayTeaPot = (float *)malloc(documents[i].fig_length[j] * 9 * sizeof(float));
				for (k = 0, n = 0; k < documents[i].fig_length[j]; k++) {
					arrayTeaPot[n++] = documents[i].fig[j][k].x1;
					arrayTeaPot[n++] = documents[i].fig[j][k].y1;
					arrayTeaPot[n++] = documents[i].fig[j][k].z1;
					arrayTeaPot[n++] = documents[i].fig[j][k].x2;
					arrayTeaPot[n++] = documents[i].fig[j][k].y2;
					arrayTeaPot[n++] = documents[i].fig[j][k].z2;
					arrayTeaPot[n++] = documents[i].fig[j][k].x3;
					arrayTeaPot[n++] = documents[i].fig[j][k].y3;
					arrayTeaPot[n++] = documents[i].fig[j][k].z3;
				}
				teapot = 1;
				glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
				glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), arrayTeaPot, GL_STATIC_DRAW);
			}
			j++;
		}

		if (documents[i].child) fillBuffersChild(documents[i].child,sphere,disk,teapot);
	}

}


int main(int argc, char *argv[]) {

	if (argc != 2) return 0;
	else {

		// init GLUT and the window
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(800, 800);
		glutCreateWindow("CG_trabalho");

		// Required callback registry
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);
		glutIdleFunc(renderScene);
		glutKeyboardFunc(processKeys);


#ifndef __APPLE__
		glewInit();
#endif

		//  OpenGL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glGenBuffers(3, buffers);

		int size = nFather(argv[1]);
		printf("size %d", size);
		parseXml(argv[1], &documents, size + 1);
		parseFigures(documents);
		fillBuffers(documents);
		info(documents,0);

		// enter GLUT's main cycle
		glutMainLoop();
		
		return 1;
	}

}