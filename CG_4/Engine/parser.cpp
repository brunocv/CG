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
	struct PointText * pnt = (struct PointText*) calloc(ntriangles, sizeof(struct PointText));
	struct Normal * nrm = (struct Normal*) calloc(ntriangles, sizeof(struct Normal));

	if (!ret) {
		fclose(ptr_file);
		return false;
	}

	unsigned i = 0;
	for (; i < ntriangles; i++) {
		struct Triangulo tmp;
		struct PointText tmp2;
		struct Normal tmp3;

		int res = fscanf(ptr_file,
			"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&tmp.x1, &tmp.y1, &tmp.z1,
			&tmp.x2, &tmp.y2, &tmp.z2,
			&tmp.x3, &tmp.y3, &tmp.z3, &tmp2.x1, &tmp2.y1, &tmp2.x2, &tmp2.y2, &tmp2.x3, &tmp2.y3,
			&tmp3.x1,&tmp3.y1,&tmp3.z1,&tmp3.x2,&tmp3.y2,&tmp3.z2,&tmp3.x3,&tmp3.y3,&tmp3.z3);

		if (res != 24)
			break;
		ret[i] = tmp;
		pnt[i] = tmp2;
		nrm[i] = tmp3;
	}

	fclose(ptr_file);
	if (i == ntriangles) {
		dc->modelss[index].text= (struct PointText*) calloc(ntriangles, sizeof(struct PointText));
		dc->modelss[index].normals = (struct Normal*) calloc(ntriangles, sizeof(struct Normal));
		dc->fig_length[index] = ntriangles;
		dc->fig[index] = ret;
		dc->modelss[index].text = pnt;
		dc->modelss[index].normals = nrm;
	}
	else {
		free(ret);
		free(pnt);
		free(nrm);
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

			parseFig(documents[z].modelss[j].file, &documents[z], j);
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
			parseFig(documents[i].modelss[j].file, &documents[i], j);
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
			dcc[z].modelss = (struct Model*)(calloc(5 ,sizeof(struct Model)));

			while (model) {

				name = model->Attribute("file");
				if (name) dcc[z].modelss[i].file = strdup(name);
				name = model->Attribute("texture");
				if (name) dcc[z].modelss[i].texture = strdup(name);
				//diffuse
				name = model->Attribute("diffR");
				if (name) dcc[z].modelss[i].diffR = atof(name);
				name = model->Attribute("diffG");
				if (name) dcc[z].modelss[i].diffG = atof(name);
				name = model->Attribute("diffB");
				if (name) dcc[z].modelss[i].diffB = atof(name);
				name = model->Attribute("diffA");
				if (name) dcc[z].modelss[i].diffA = atof(name);
				//emissive
				name = model->Attribute("emiR");
				if (name) dcc[z].modelss[i].emiR = atof(name);
				name = model->Attribute("emiG");
				if (name) dcc[z].modelss[i].emiG = atof(name);
				name = model->Attribute("emiB");
				if (name) dcc[z].modelss[i].emiB = atof(name);
				name = model->Attribute("emiA");
				if (name) dcc[z].modelss[i].emiA = atof(name);
				//specular
				name = model->Attribute("specR");
				if (name) dcc[z].modelss[i].specR = atof(name);
				name = model->Attribute("specG");
				if (name) dcc[z].modelss[i].specG = atof(name);
				name = model->Attribute("specB");
				if (name) dcc[z].modelss[i].specB = atof(name);
				name = model->Attribute("specA");
				if (name) dcc[z].modelss[i].specA = atof(name);
				//ambient
				name = model->Attribute("ambR");
				if (name) dcc[z].modelss[i].ambR = atof(name);
				name = model->Attribute("ambG");
				if (name) dcc[z].modelss[i].ambG = atof(name);
				name = model->Attribute("ambB");
				if (name) dcc[z].modelss[i].ambB = atof(name);
				name = model->Attribute("ambA");
				if (name) dcc[z].modelss[i].ambA = atof(name);
				i++;
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
			ret[z].modelss = (struct Model*)(calloc(5 ,sizeof(struct Model)));

			while (model) {
				name = model->Attribute("file");
				if (name) ret[z].modelss[i].file = strdup(name);
				name = model->Attribute("texture");
				if (name) ret[z].modelss[i].texture = strdup(name);
				//diffuse
				name = model->Attribute("diffR");
				if (name) ret[z].modelss[i].diffR = atof(name);
				name = model->Attribute("diffG");
				if (name) ret[z].modelss[i].diffG = atof(name);
				name = model->Attribute("diffB");
				if (name) ret[z].modelss[i].diffB = atof(name);
				name = model->Attribute("diffA");
				if (name) ret[z].modelss[i].diffA = atof(name);
				//emissive
				name = model->Attribute("emiR");
				if (name) ret[z].modelss[i].emiR = atof(name);
				name = model->Attribute("emiG");
				if (name) ret[z].modelss[i].emiG = atof(name);
				name = model->Attribute("emiB");
				if (name) ret[z].modelss[i].emiB = atof(name);
				name = model->Attribute("emiA");
				if (name) ret[z].modelss[i].emiA = atof(name);
				//specular
				name = model->Attribute("specR");
				if (name) ret[z].modelss[i].specR = atof(name);
				name = model->Attribute("specG");
				if (name) ret[z].modelss[i].specG = atof(name);
				name = model->Attribute("specB");
				if (name) ret[z].modelss[i].specB = atof(name);
				name = model->Attribute("specA");
				if (name) ret[z].modelss[i].specA = atof(name);
				//ambient
				name = model->Attribute("ambR");
				if (name) ret[z].modelss[i].ambR = atof(name);
				name = model->Attribute("ambG");
				if (name) ret[z].modelss[i].ambG = atof(name);
				name = model->Attribute("ambB");
				if (name) ret[z].modelss[i].ambB = atof(name);
				name = model->Attribute("ambA");
				if (name) ret[z].modelss[i].ambA = atof(name);
				i++;
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

int nLight(char* file) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	const char * name;
	int z = 0;

	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *a;
		tinyxml2::XMLElement *lights;
		tinyxml2::XMLElement *light;

		a = doc.FirstChildElement("scene");

		lights = a->FirstChildElement("lights");

		while (lights) {

			light = lights->FirstChildElement("light");

			while (light) {

				light = light->NextSiblingElement("light");
				z++;

			}

			lights = lights->NextSiblingElement("lights");

		}
	}
	return z;
}

void parseLight(char* file, struct Light ** dc, int size) {

	tinyxml2::XMLDocument doc;
	doc.LoadFile(file);
	const char * name;
	int z = 0;

	if (doc.ErrorID() == 0) {
		tinyxml2::XMLElement *a;
		tinyxml2::XMLElement *lights;
		tinyxml2::XMLElement *light;

		a = doc.FirstChildElement("scene");

		struct Light * ret = (struct Light*) calloc(size, sizeof(struct Light));
		lights = a->FirstChildElement("lights");

		while (lights) {

			light = lights->FirstChildElement("light");
		
			while (light) {
				
				name = light->Attribute("type");
				ret[z].type = strdup(name);
				name = light->Attribute("posX");
				if (name) ret[z].posX = atof(name);
				name = light->Attribute("posY");
				if (name) ret[z].posY = atof(name);
				name = light->Attribute("posZ");
				if (name) ret[z].posZ = atof(name);
				
				light = light->NextSiblingElement("light");
				
				z++;
			
			}

			lights = lights->NextSiblingElement("lights");
			

		}
		*dc = ret;
	}
}