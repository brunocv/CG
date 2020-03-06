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

int parseFig(const char * file, struct Xml * dc, int index) {
	FILE * ptr_file = fopen(file, "r");
	if (!ptr_file)
		return false;

	unsigned ntriangles = 0;
	fscanf(ptr_file, "%u\n", &ntriangles);

	struct Triangulo * ret = (struct Triangulo*) calloc(ntriangles, sizeof(struct Triangulo));
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
		dc->fig[index] = ret;

	}
	else {
		free(ret);
	}

	return i == ntriangles;
}

void parseFigChilds(struct Xml * documents) {
	int z;
	int j;
	
	for (z = 0; documents[z].nfiles > 0; z++) {
		documents[z].fig_length = (unsigned int*)(malloc((documents[z].nfiles + 1) * sizeof(unsigned int)));
		documents[z].fig = (struct Triangulo **)(malloc((documents[z].nfiles + 1) * sizeof(struct Triangulo)));

		j = 0;

		while (j < documents[z].nfiles) {

			parseFig(documents[z].file[j], &documents[z], j);
			j++;
		}
		if (documents[z].child) parseFigChilds(documents[z].child);
	}
}


void parseFigures(struct Xml * documents) {
	int z;
	int j;
	int i;
	
	for (i = 0, j = 0; documents[i].nfiles > 0; i++) {
		j = 0;

		documents[i].fig_length = (unsigned int*)(malloc((documents[i].nfiles + 1) * sizeof(unsigned int)));
		documents[i].fig = (struct Triangulo **)(malloc((documents[i].nfiles + 1) * sizeof(struct Triangulo)));

		while (j < documents[i].nfiles) {

			parseFig(documents[i].file[j], &documents[i], j);
			j++;
		}
		if (documents[i].child) parseFigChilds(documents[i].child);
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

int nChild(char* file, tinyxml2::XMLElement *b) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int z = 0;

	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *group;

		group = b->FirstChildElement("group");

		while (group) {

			group = group->NextSiblingElement("group");
			z++;

		}
	}
	return z;
}

int pointsT(char* file, tinyxml2::XMLElement *b) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int z = 0;

	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *point;

		point = b->FirstChildElement("point");

		while (point) {

			point = point->NextSiblingElement("point");
			z++;

		}
	}
	return z;
}

char* complete(char * ordem, int k) {
	if (k >= 3) return ordem;
	if (k == 0) return "tsr";
	if (k == 1) {
		if (ordem[0] == 't') return "tsr";
		if (ordem[0] == 's') return "str";
		if (ordem[0] == 'r') return "rts";
	}
	if (k == 2) {
		if (ordem[0] == 't' && ordem[1] == 's') return "tsr";
		if (ordem[0] == 't' && ordem[1] == 'r') return "trs";
		if (ordem[0] == 'r' && ordem[1] == 't') return "rts";
		if (ordem[0] == 'r' && ordem[1] == 's') return "rst";
		if (ordem[0] == 's' && ordem[1] == 't') return "str";
		if (ordem[0] == 's' && ordem[1] == 'r') return "srt";
	}
	else return"tsr";

}

struct Xml * parseChild(char* file, struct Xml * dcc, tinyxml2::XMLElement *g, int index) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int i = 0;
	const char * name;
	float x;
	int l = 0;
	int npoints = 0;
	int z = index;
	if (doc.ErrorID() == 0) {

		tinyxml2::XMLElement *group;
		group = g;
		while (group) {
			tinyxml2::XMLElement *aux;
			tinyxml2::XMLElement *translate;
			tinyxml2::XMLElement *rotate;
			tinyxml2::XMLElement *models;
			tinyxml2::XMLElement *model;
			tinyxml2::XMLElement *scale;
			tinyxml2::XMLElement *order;
			tinyxml2::XMLElement *po;

			translate = group->FirstChildElement("translate");
			if (translate) {
				name = translate->Attribute("time");
				if (name == NULL) {
					name = translate->Attribute("X");
					if (name) dcc[z].trans_x = atof(name);
					name = translate->Attribute("Y");
					if (name) dcc[z].trans_y = atof(name);
					name = translate->Attribute("Z");
					if (name) dcc[z].trans_z = atof(name);
				}
				else {
					dcc[z].trans_time = atof(name);
					npoints = pointsT(file, translate);
					dcc[z].points = (struct Point*) calloc(npoints + 1, sizeof(struct Point));
					l = 0;
					dcc[z].npontos = npoints;
					po = translate->FirstChildElement("point");
					while (po) {
						name = po->Attribute("X");
						if (name) dcc[z].points[l].x = atof(name);
						name = po->Attribute("Y");
						if (name) dcc[z].points[l].y = atof(name);
						name = po->Attribute("Z");
						if (name) dcc[z].points[l].z = atof(name);
						po = po->NextSiblingElement("point");
						l++;
					}

				}

			}

			rotate = group->FirstChildElement("rotate");
			if (rotate) {
				name = rotate->Attribute("time");
				if (name) dcc[z].rot_time = atof(name);
				name = rotate->Attribute("angle");
				if (name) dcc[z].ang = atof(name);
				name = rotate->Attribute("axisX");
				if (name) dcc[z].rot_x = atof(name);
				name = rotate->Attribute("axisY");
				if (name) dcc[z].rot_y = atof(name);
				name = rotate->Attribute("axisZ");
				if (name) dcc[z].rot_z = atof(name);
			}

			scale = group->FirstChildElement("scale");
			dcc[z].scale_x = 1;
			dcc[z].scale_y = 1;
			dcc[z].scale_z = 1;
			if (scale) {

				name = scale->Attribute("X");
				if (name) dcc[z].scale_x = atof(name);
				name = scale->Attribute("Y");
				if (name) dcc[z].scale_y = atof(name);
				name = scale->Attribute("Z");
				if (name) dcc[z].scale_z = atof(name);
			}
			
			int k = 0;
			order = group->FirstChildElement();
			const char *check;
			while (k < 3) {
				check = order->Value();

				if (!strcmp("models", check)) break;
				if (!strcmp("translate", check)) dcc[z].order[k] = 't';
				if (!strcmp("scale", check)) dcc[z].order[k] = 's';
				if (!strcmp("rotate", check)) dcc[z].order[k] = 'r';

				order = order->NextSiblingElement();
				k++;
			}
			strcpy(dcc[z].order, complete(dcc[z].order, k));
			
			models = group->FirstChildElement("models");
			if (models) model = models->FirstChildElement("model");
			i = 0;
			dcc[z].file = (char**)(malloc(5 * sizeof(char*)));

			while (model) {

				name = model->Attribute("file");
				if (name) {
					dcc[z].file[i] = strdup(name);
					i++;
				}
				model = model->NextSiblingElement("model");
			}

			dcc[z].nfiles = i;

			aux = group->FirstChildElement("group");
			if (aux) {
				int filho = nChild(file, group);
				dcc[z].nfilhos = filho;
				dcc[z].child = (struct Xml*) calloc(filho + 1, sizeof(struct Xml));
				dcc[z].child = parseChild(file, dcc[z].child, aux, 0);
			}

			group = group->NextSiblingElement("group");

			z++;

		}
	}
	return dcc;
}

void parseXml(char* file, struct Xml ** dc, int size) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	int i = 0;
	int z = 0;
	const char * name;
	float x;
	int npoints = 0;
	int l = 0;

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
			tinyxml2::XMLElement *order;
			tinyxml2::XMLElement *po;

			translate = group->FirstChildElement("translate");
			if (translate) {
				name = translate->Attribute("time");
				if (name == NULL) {
					name = translate->Attribute("X");
					if (name) ret[z].trans_x = atof(name);
					name = translate->Attribute("Y");
					if (name) ret[z].trans_y = atof(name);
					name = translate->Attribute("Z");
					if (name) ret[z].trans_z = atof(name);
				}
				else {
					ret[z].trans_time = atof(name);
					npoints = pointsT(file, translate);
					ret[z].points = (struct Point*) calloc(npoints + 1, sizeof(struct Point));
					l = 0;
					ret[z].npontos = npoints;
					po = translate->FirstChildElement("point");
					while (po) {
						name = po->Attribute("X");
						if (name) ret[z].points[l].x = atof(name);
						name = po->Attribute("Y");
						if (name) ret[z].points[l].y = atof(name);
						name = po->Attribute("Z");
						if (name) ret[z].points[l].z = atof(name);
						po = po->NextSiblingElement("point");
						l++;
					}

				}

			}

			rotate = group->FirstChildElement("rotate");
			if (rotate) {
				name = rotate->Attribute("time");
				if (name) ret[z].rot_time = atof(name);
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
			if(scale){
				name = scale->Attribute("X");
				if (name) ret[z].scale_x = atof(name);
				name = scale->Attribute("Y");
				if (name) ret[z].scale_y = atof(name);
				name = scale->Attribute("Z");
				if (name) ret[z].scale_z = atof(name);
			}

			//Descobre ordem
			int k = 0;
			order = group->FirstChildElement();
			const char *check;
			while (k < 3) {
				check = order->Value();
				
				if (!strcmp("models", check)) break;
				if (!strcmp("translate", check)) ret[z].order[k]='t';
				if (!strcmp("scale", check)) ret[z].order[k] = 's';
				if (!strcmp("rotate", check)) ret[z].order[k] = 'r';

				order = order->NextSiblingElement();
				k++;
			}
			strcpy(ret[z].order ,complete(ret[z].order, k));
			
			
			models = group->FirstChildElement("models");
			if (models) model = models->FirstChildElement("model");
			i = 0;
			ret[z].file = (char**)(malloc(5 * sizeof(char*)));

			while (model) {

				name = model->Attribute("file");
				if (name) {
					ret[z].file[i] = strdup(name);
					i++;
				}
				model = model->NextSiblingElement("model");
			}

			ret[z].nfiles = i;

			aux = group->FirstChildElement("group");
			if (aux) {
				int filho = nChild(file, group);
				ret[z].nfilhos = filho;
				ret[z].child = (struct Xml*) calloc(filho + 1, sizeof(struct Xml));
				ret[z].child = parseChild(file, ret[z].child, aux, 0);
			}

			group = group->NextSiblingElement("group");
			z++;

		}
		*dc = ret;
	}
}