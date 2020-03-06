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

struct Point {
	float x;
	float y;
	float z;
};

struct Xml {
	float ang;
	float rot_x;
	float rot_y;
	float rot_z;
	float rot_time;
	float trans_x;
	float trans_y;
	float trans_z;
	float trans_time;
	float scale_x;
	float scale_y;
	float scale_z;
	char order[3];
	char** file;
	int	nfiles;
	int nfilhos;
	int npontos;
	struct Point * points;
	struct Triangulo ** fig;
	unsigned int *fig_length;
	struct Xml *child;
};

static struct Xml * documents = NULL;
int parseFig(const char * file, struct Xml * dc, int index);
void parseFigChilds(struct Xml * documents);
void parseFigures(struct Xml * dc);
int nFather(char* file);
int nChild(char* file, tinyxml2::XMLElement *b);
int pointsT(char* file, tinyxml2::XMLElement *b);
char* complete(char * ordem, int k);
struct Xml * parseChild(char* file, struct Xml * dcc, tinyxml2::XMLElement *g, int index);
void parseXml(char* file, struct Xml ** dc, int size);


