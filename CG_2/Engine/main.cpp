
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

static float r = 1050;
static float a = 240;
static float b = 40;

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

struct Document {
	char* file;
};

struct Xml{
	float ang;
	float rot_x;
	float rot_y;
	float rot_z;
	float trans_x;
	float trans_y;
	float trans_z;
	float scale_x;
	float scale_y;
	float scale_z;
	char** file; 
	int	nfiles;
	struct Triangulo ** fig;
	unsigned int *fig_length;
	struct Xml *child;
};

static struct Xml * documents = NULL;

//Tentar passar destas estruturas para uma que represente qualquer figura e tenha outra que guaarde as translaçoes/rotates/numero de triangulos
/*
static struct Triangulo * cone = NULL;
static unsigned int cone_length = 0;

*/
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


void draw(unsigned int size, struct Triangulo * fig) {

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < size; i++) {

		glColor3f(1, 1, 1);
		glVertex3f(fig[i].x1, fig[i].y1, fig[i].z1);
		glVertex3f(fig[i].x2, fig[i].y2, fig[i].z2);
		glVertex3f(fig[i].x3, fig[i].y3, fig[i].z3);
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

int parseFig(const char * file, struct Xml * dc,int index){
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

		dc->fig_length[index] = ntriangles;
		dc -> fig[index] = ret;

	}
	else {
		free(ret);
	}

	return i == ntriangles;
}

struct Xml * parseChild(char* file, tinyxml2::XMLElement *group, struct Xml *n,int index) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int i = 0;
	const char * name;
	float x;
	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *aux;

		tinyxml2::XMLElement *translate;
		tinyxml2::XMLElement *rotate;
		tinyxml2::XMLElement *models;
		tinyxml2::XMLElement *model;
		tinyxml2::XMLElement *scale;

		if (group) {
			translate = group->FirstChildElement("translate");

			if (translate) {

				name = translate->Attribute("X");
				if (name) n[index].trans_x = atof(name);
				name = translate->Attribute("Y");
				if (name) n[index].trans_y = atof(name);
				name = translate->Attribute("Z");
				if (name) n[index].trans_z = atof(name);

			}

			rotate = group->FirstChildElement("rotate");
			if (rotate) {

				name = rotate->Attribute("angle");
				if (name) n[index].ang = atof(name);
				name = rotate->Attribute("axisX");
				if (name) n[index].rot_x = atof(name);
				name = rotate->Attribute("axisY");
				if (name) n[index].rot_y = atof(name);
				name = rotate->Attribute("axisZ");
				if (name) n[index].rot_z = atof(name);
			}

			scale = group->FirstChildElement("scale");
			n[index].scale_x = 1;
			n[index].scale_y = 1;
			n[index].scale_z = 1;
			if (scale) {

				name = scale->Attribute("X");
				if (name) n[index].scale_x = atof(name);
				name = scale->Attribute("Y");
				if (name) n[index].scale_y = atof(name);
				name = scale->Attribute("Z");
				if (name) n[index].scale_z = atof(name);
			}

			models = group->FirstChildElement("models");
			if (models) model = models->FirstChildElement("model");
			i = 0;
			n[index].file = (char**)(malloc(5 * sizeof(char*)));

			while (model) {

				name = model->Attribute("file");
				if (name) {
					n[index].file[i] = strdup(name);
					i++;
				}
				model = model->NextSiblingElement("model");
			}

			n[index].nfiles = i;

			aux = group->FirstChildElement("group");
			if (aux) n=parseChild(file, aux, n, index + 1);
			return n;

		}
	}
}


void parseXml(char* file, struct Xml ** dc, int size) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int i = 0;
	int z = 0;
	const char * name;
	float x;

	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *a;
		tinyxml2::XMLElement *group;
		tinyxml2::XMLElement *aux;

		a = doc.FirstChildElement("scene");
		
		struct Xml * ret = (struct Xml*) calloc(size, sizeof(struct Xml));
		group = a->FirstChildElement("group");
		
		while (group) {
			tinyxml2::XMLElement *translate;
			tinyxml2::XMLElement *rotate;
			tinyxml2::XMLElement *models;
			tinyxml2::XMLElement *model;
			tinyxml2::XMLElement *scale;
			
			translate = group->FirstChildElement("translate");
			if (translate) {
				name = translate->Attribute("X");
				if (name) ret[z].trans_x = atof(name);
				name = translate->Attribute("Y");
				if (name) ret[z].trans_y = atof(name);
				name = translate->Attribute("Z");
				if (name) ret[z].trans_z = atof(name);

			}

			rotate = group->FirstChildElement("rotate");
			if (rotate) {
				name = rotate->Attribute("angle");
				if (name) ret[z].ang = atof(name);
				name = rotate->Attribute("axisX");
				if (name) ret[z].rot_x = atof(name);
				name = rotate->Attribute("axisY");
				if (name) ret[z].rot_y = atof(name);
				name = rotate->Attribute("axisZ");
				if (name) ret[z].rot_z = atof(name);
			}

			scale = group->FirstChildElement("scale");
			ret[z].scale_x = 1;
			ret[z].scale_y = 1;
			ret[z].scale_z = 1;
			if (scale) {
				name = scale->Attribute("X");
				if (name) ret[z].scale_x = atof(name);
				name = scale->Attribute("Y");
				if (name) ret[z].scale_y = atof(name);
				name = scale->Attribute("Z");
				if (name) ret[z].scale_z = atof(name);
			}

			models=group->FirstChildElement("models");
			if (models) model = models->FirstChildElement("model");
			i = 0;
			ret[z].file = (char**)(malloc(5 * sizeof(char*)));

			while (model) {

				name = model->Attribute("file");
				if (name) {
					ret[z].file[i]=strdup(name);
					i++;
				}
				model = model->NextSiblingElement("model");
			}

			ret[z].nfiles = i;
			
			aux = group->FirstChildElement("group");
			if (aux) {
				struct Xml * ret2 = (struct Xml*) calloc(3, sizeof(struct Xml));
				ret[z].child=parseChild(file, aux, ret2 , 0);
			}
			
			group = group->NextSiblingElement("group");
			z++;
	
		}
		*dc = ret;
	}
}


int nFather(char* file) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int z = 0;

	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *a;
		tinyxml2::XMLElement *group;

		a = doc.FirstChildElement("scene");

		group = a->FirstChildElement("group");

		while (group) {
			
			group = group->NextSiblingElement("group");
			z++;

		}
	}
	return z;
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

	printf(" y %f\n", documents[0].fig[0][10399].y2);

	//translate->rotate->scale
	for (int i = 0, j = 0,z = 0 ; documents[i].nfiles > 0; i++) {

		glPushMatrix();

		j = 0;

		glTranslated( (GLdouble)documents[i].trans_x, (GLdouble)documents[i].trans_y,(GLdouble)documents[i].trans_z);
		glRotated((GLdouble)documents[i].ang, (GLdouble)documents[i].rot_x, (GLdouble)documents[i].rot_y, (GLdouble)documents[i].rot_z);
		glScaled((GLdouble)documents[i].scale_x, (GLdouble)documents[i].scale_y, (GLdouble)documents[i].scale_z);

		while (j < documents[i].nfiles) {
			draw(documents[i].fig_length[j], documents[i].fig[j]);
			printf("desenhei %d \n", i);
			j++;
		}

		if (documents[i].child) {
			for (z = 0; documents[i].child[z].nfiles > 0; z++) {

				glTranslated((GLdouble)documents[i].child[z].trans_x, (GLdouble)documents[i].child[z].trans_y, (GLdouble)documents[i].child[z].trans_z);
				glRotated((GLdouble)documents[i].child[z].ang, (GLdouble)documents[i].child[z].rot_x, (GLdouble)documents[i].child[z].rot_y, (GLdouble)documents[i].child[z].rot_z);
				glScaled((GLdouble)documents[i].child[z].scale_x, (GLdouble)documents[i].child[z].scale_y, (GLdouble)documents[i].child[z].scale_z);

				j = 0;

				while (j < documents[i].child[z].nfiles) {
					draw(documents[i].child[z].fig_length[j],documents[i].child[z].fig[j]);
					j++;
				}
			}
		}

		glPopMatrix();

	}
	


	// End of frame
	glutSwapBuffers();
}

int main(int argc, char *argv[]) {

	if (argc != 2) return 0;
	else {

		int size = nFather(argv[1]);
		printf("size %d", size);
		parseXml(argv[1], &documents,size+1);
		int z;
		int j;
		for (int i = 0; documents[i].nfiles > 0; i++) {
			z = 0;
			j = 0;
			printf("\n\n\n");
			printf("trans x %f\n", documents[i].trans_x);
			printf("trans y %f\n", documents[i].trans_y);
			printf("trans z %f\n", documents[i].trans_z);
			printf("ang %f\n", documents[i].ang);
			printf("rot x %f\n", documents[i].rot_x);
			printf("rot y %f\n", documents[i].rot_y);
			printf("rot z %f\n", documents[i].rot_z);
			printf("scale x %f\n", documents[i].scale_x);
			printf("scale y %f\n", documents[i].scale_y);
			printf("scale z %f\n", documents[i].scale_z);
			printf("nfiles %d\n", documents[i].nfiles);

			while (z < documents[i].nfiles) {
				printf("%s \n", documents[i].file[z]);
				z++;
			}

			if (documents[i].child) {
				for (z = 0; documents[i].child[z].nfiles > 0; z++) {
					printf("\n\n\n filho \n\n\n ");
					printf("pai %d trans x %f\n", i, documents[i].child[z].trans_x);
					printf("pai %d trans y %f\n", i, documents[i].child[z].trans_y);
					printf("pai %d trans z %f\n", i, documents[i].child[z].trans_z);
					printf("pai %d ang %f\n", i, documents[i].child[z].ang);
					printf("pai %d rot x %f\n", i, documents[i].child[z].rot_x);
					printf("pai %d rot y %f\n", i, documents[i].child[z].rot_y);
					printf("pai %d rot z %f\n", i, documents[i].child[z].rot_z);
					printf("pai %d scale x %f\n", i, documents[i].child[z].scale_x);
					printf("pai %d scale y %f\n", i, documents[i].child[z].scale_y);
					printf("pai %d scale z %f\n", i, documents[i].child[z].scale_z);
					printf("pai %d nfiles %d\n", i, documents[i].child[z].nfiles);
					j = 0;
					while (j < documents[i].child[z].nfiles) {
						printf("\n filho file %s \n", documents[i].child[z].file[j]);
						j++;
					}

				}
			}

		}
		
		for (int i = 0, j = 0; documents[i].nfiles > 0; i++) {
			j = 0;
			
			documents[i].fig_length = (unsigned int*)(malloc( (documents[i].nfiles +1) * sizeof(unsigned int)));
			documents[i].fig = (struct Triangulo **)(malloc((documents[i].nfiles + 1) * sizeof(struct Triangulo)));

			while (j < documents[i].nfiles) {

				parseFig(documents[i].file[j], &documents[i] ,j);
				j++;
			}

			if (documents[i].child) {
				for (z = 0; documents[i].child[z].nfiles > 0; z++) {
					documents[i].child[z].fig_length = (unsigned int*)(malloc((documents[i].child[z].nfiles + 1) * sizeof(unsigned int)));
					documents[i].child[z].fig = (struct Triangulo **)( malloc((documents[i].child[z].nfiles + 1) * sizeof(struct Triangulo)));

					j = 0;
					
					while (j < documents[i].child[z].nfiles ) {

						parseFig(documents[i].child[z].file[j], &documents[i].child[z], j);
						j++;
					}
				}
			}
		}
		

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