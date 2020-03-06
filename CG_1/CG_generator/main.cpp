
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

struct ponto {
	float x;
	float y;
	float z;
} Ponto;

/*Escrever as coordenadas de um ponto numa posição de um dado ficheiro*/
std::string writePoint(ponto point) {
	std::stringstream sstm;
	sstm << point.x << " " << point.y << " " << point.z;
	return sstm.str();
}

/*Indicar os pontos de um triangulo a escrever num dado ficheiro
  No final, vai ficar um triangulo representado numa linha do ficheiro*/
std::string formTriangle(ponto pointA, ponto pointB, ponto pointC) {
	return writePoint(pointA) + " " + writePoint(pointB) + " " + writePoint(pointC);
}

std::string writeValue(float value) {
	std::stringstream sstm;
	sstm << value;
	return sstm.str();
}

std::string writeLineNumber(float value) {
	return writeValue(value) + "\n";
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
/*PLANE*/

void drawPlane(char *filename, float tamanho) {
	float x = tamanho / 2;

	ponto pontoA;
	pontoA.x = -x;
	pontoA.y = 0;
	pontoA.z = x;

	ponto pontoB;
	pontoB.x = x;
	pontoB.y = 0;
	pontoB.z = x;

	ponto pontoC;
	pontoC.x = x;
	pontoC.y = 0;
	pontoC.z = -x;

	ponto pontoD;
	pontoD.x = -x;
	pontoD.y = 0;
	pontoD.z = -x;

	ofstream ficheiro;
	ficheiro.open(filename);


	if (ficheiro.is_open()) {
		ficheiro.clear();

		//para cada plano o ficheiro terá de ter sempre só 2 linhas
		ficheiro << writeLineNumber(2.0);

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoC, pontoD, pontoA) << endl;
		ficheiro.close();
	}

	else
		cout << "Unable to open file\n";
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
/*CONE*/

void drawCone(char *filename, float radius, float height, int slices, int stacks) {
	// put code to draw cone in here
	float ang = (2 * M_PI) / slices;

	//número de triângulos que desenham o cone
	float value = slices + (stacks * slices * 2);

	ofstream ficheiro;
	ficheiro.open(filename);

	if (ficheiro.is_open()) {
		ficheiro.clear();

		ficheiro << writeLineNumber(value);

		for (int z = 0; z < slices; z++) {
			ponto pontoA;
			pontoA.x = radius * sin(z * ang);
			pontoA.y = 0;
			pontoA.z = radius * cos(z * ang);

			ponto pontoB;
			pontoB.x = 0;
			pontoB.y = 0;
			pontoB.z = 0;

			ponto pontoC;
			pontoC.x = radius * sin(ang * (z + 1));
			pontoC.y = 0;
			pontoC.z = radius * cos(ang * (z + 1));

			ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;

		}

		for (int i = 0; i < stacks; ++i) {
			float raio = radius - i * (radius / stacks);
			for (int z = 0; z < slices; z++) {

				// Lado1
				float raio2 = radius - (i + 1) * (radius / stacks);

				ponto pontoD;
				pontoD.x = raio2 * sin(ang * z);
				pontoD.y = (i + 1) * (height / stacks);
				pontoD.z = raio2 * cos(ang * z);

				ponto pontoE;
				pontoE.x = raio * sin(z * ang);
				pontoE.y = i * (height / stacks);
				pontoE.z = raio * cos(z * ang);

				ponto pontoF;
				pontoF.x = raio * sin(ang * (z + 1));
				pontoF.y = i * (height / stacks);
				pontoF.z = raio * cos(ang * (z + 1));

				ficheiro << formTriangle(pontoD, pontoE, pontoF) << endl;

				// Lado2
				ponto pontoG;
				pontoG.x = raio2 * sin(z * ang);
				pontoG.y = (i + 1) * (height / stacks);
				pontoG.z = raio2 * cos(z * ang);

				ponto pontoH;
				pontoH.x = raio * sin(ang * (z + 1));
				pontoH.y = i * (height / stacks);
				pontoH.z = raio * cos(ang * (z + 1));

				ponto pontoI;
				pontoI.x = raio2 * sin(ang * (z + 1));
				pontoI.y = (i + 1) * (height / stacks);
				pontoI.z = raio2 * cos(ang * (z + 1));

				ficheiro << formTriangle(pontoG, pontoH, pontoI) << endl;
			}
		}
		ficheiro.close();
	}

	else
		cout << "Unable to open file\n";
}

//------------------------------------------------------------------------------------------------------------------------------------------------
/*SPHERE*/

void drawSphere(char *filename, float radius, int slices, int stacks) {
	//XOZ
	float b = (2 * M_PI) / slices;
	//XOY
	float a = M_PI / stacks;

	float value = slices * ((stacks / 2) + 1) * 4;

	ofstream ficheiro;
	ficheiro.open(filename);

	if (ficheiro.is_open()) {
		ficheiro.clear();

		ficheiro << writeLineNumber(value);

		for (int i = 0; i < stacks / 2 + 1; i++) {

			for (int j = 0; j < slices; j++) {
				float angulo = b * j;
				float angulo2 = angulo + b;
				float nivel = i * a;
				float nivel2 = nivel + a;
				//--------------------------------------------------------------------------------------------------
				ponto pontoA;
				pontoA.x = radius * cos(nivel)*sin(angulo);
				pontoA.y = radius * sin(nivel);
				pontoA.z = radius * cos(nivel)*cos(angulo);

				ponto pontoB;
				pontoB.x = radius * cos(nivel)*sin(angulo2);
				pontoB.y = radius * sin(nivel);
				pontoB.z = radius * cos(nivel)*cos(angulo2);

				ponto pontoC;
				pontoC.x = radius * cos(nivel2)*sin(angulo2);
				pontoC.y = radius * sin(nivel2);
				pontoC.z = radius * cos(nivel2)*cos(angulo2);

				ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;

				//--------------------------------------------------------------------------------------------------
				ponto pontoD;
				pontoD.x = radius * cos(nivel2)*sin(angulo2);
				pontoD.y = radius * sin(nivel2);
				pontoD.z = radius * cos(nivel2)*cos(angulo2);

				ponto pontoE;
				pontoE.x = radius * cos(nivel2)*sin(angulo);
				pontoE.y = radius * sin(nivel2);
				pontoE.z = radius * cos(nivel2)*cos(angulo);

				ponto pontoF;
				pontoF.x = radius * cos(nivel)*sin(angulo);
				pontoF.y = radius * sin(nivel);
				pontoF.z = radius * cos(nivel)*cos(angulo);

				ficheiro << formTriangle(pontoD, pontoE, pontoF) << endl;

				//---------------------------------------------------------------------------------------------------
				ponto pontoG;
				pontoG.x = radius * cos(-nivel)*sin(-angulo);
				pontoG.y = radius * sin(-nivel);
				pontoG.z = radius * cos(-nivel)*cos(-angulo);

				ponto pontoH;
				pontoH.x = radius * cos(-nivel)*sin(-angulo2);
				pontoH.y = radius * sin(-nivel);
				pontoH.z = radius * cos(-nivel)*cos(-angulo2);

				ponto pontoI;
				pontoI.x = radius * cos(-nivel2)*sin(-angulo2);
				pontoI.y = radius * sin(-nivel2);
				pontoI.z = radius * cos(-nivel2)*cos(-angulo2);

				ficheiro << formTriangle(pontoG, pontoH, pontoI) << endl;

				//_--------------------------------------------------------------------------------------------------
				ponto pontoJ;
				pontoJ.x = radius * cos(-nivel2)*sin(-angulo2);
				pontoJ.y = radius * sin(-nivel2);
				pontoJ.z = radius * cos(-nivel2)*cos(-angulo2);

				ponto pontoK;
				pontoK.x = radius * cos(-nivel2)*sin(-angulo);
				pontoK.y = radius * sin(-nivel2);
				pontoK.z = radius * cos(-nivel2)*cos(-angulo);

				ponto pontoW;
				pontoW.x = radius * cos(-nivel)*sin(-angulo);
				pontoW.y = radius * sin(-nivel);
				pontoW.z = radius * cos(-nivel)*cos(-angulo);

				ficheiro << formTriangle(pontoJ, pontoK, pontoW) << endl;
			}
		}
		ficheiro.close();
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------
/*CUBE*/

/*Funcao que desenha uma divisão da face que se encontra na parte negativa do eixo dos ZZ*/
void drawSquareBack(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = x3, w1 = y1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		ponto pontoA;
		pontoA.x = z1;
		pontoA.y = z2;
		pontoA.z = z3;

		ponto pontoB;
		pontoB.x = x1;
		pontoB.y = x2;
		pontoB.z = x3;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoD, pontoC, pontoB) << endl;
	}

	ficheiro.close();
}

/*Funcao que desenha uma divisão da face que se encontra na parte positiva do eixo dos ZZ*/
void drawSquareFront(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = x3, w1 = y1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending
	if (ficheiro.is_open()) {
		ponto pontoA;
		pontoA.x = x1;
		pontoA.y = x2;
		pontoA.z = x3;

		ponto pontoB;
		pontoB.x = z1;
		pontoB.y = z2;
		pontoB.z = z3;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoC, pontoD, pontoA) << endl;
	}
	ficheiro.close();
}

/*Funcao que desenha uma divisão da face que se encontra na parte positiva do eixo dos YY*/
void drawSquareUp(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = y3, w1 = y1, w2 = x2, w3 = x3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		ponto pontoA;
		pontoA.x = x1;
		pontoA.y = x2;
		pontoA.z = x3;

		ponto pontoB;
		pontoB.x = z1;
		pontoB.y = z2;
		pontoB.z = z3;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoC, pontoD, pontoA) << endl;

		ficheiro.close();
	}
}

/*Funcao que desenha uma divisão da face que se encontra na parte negativa do eixo dos YY*/
void drawSquareBase(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = y3, w1 = y1, w2 = x2, w3 = x3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		ponto pontoA;
		pontoA.x = z1;
		pontoA.y = z2;
		pontoA.z = z3;

		ponto pontoB;
		pontoB.x = x1;
		pontoB.y = x2;
		pontoB.z = x3;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoD, pontoC, pontoB) << endl;

		ficheiro.close();
	}
}

/*Funcao que desenha uma divisão da face que se encontra na parte negativa do eixo dos XX*/
void drawSquareLeft(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = y1, z2 = y2, z3 = x3, w1 = x1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		ponto pontoA;
		pontoA.x = z1;
		pontoA.y = z2;
		pontoA.z = z3;

		ponto pontoB;
		pontoB.x = x1;
		pontoB.y = x2;
		pontoB.z = x3;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoD, pontoC, pontoB) << endl;

		ficheiro.close();
	}
}

/*Funcao que desenha uma divisão da face que se encontra na parte ṕositiva do eixo dos XX*/
void drawSquareRight(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = y1, z2 = y2, z3 = x3, w1 = x1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		ponto pontoA;
		pontoA.x = x1;
		pontoA.y = x2;
		pontoA.z = x3;

		ponto pontoB;
		pontoB.x = z1;
		pontoB.y = z2;
		pontoB.z = z3;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formTriangle(pontoA, pontoB, pontoC) << endl;
		ficheiro << formTriangle(pontoC, pontoD, pontoA) << endl;

		ficheiro.close();
	}
}

/*Funcao principal que junta as faces da caixa, usando funções auxiliares para construir essas mesmas faces*/
void drawBox(char *filename, float xDimensions, float altura, float comprimento, int divisions) {
	//cleanFile(filename);
	float deltaa = xDimensions / divisions;
	float deltab = altura / divisions;

	//conta o número de triângulos que compõe a caixa
	float value = (divisions*divisions) * 2 * 6;

	float bi = -altura / 2;
	float ai = xDimensions / 2;

	ofstream ficheiro;
	ficheiro.open(filename);

	if (ficheiro.is_open()) {
		ficheiro.clear();

		//escrever no ficheiro o número de triângulos que compôe a caixa
		ficheiro << writeLineNumber(value);

	}
	ficheiro.close();

	//desenhar as faces do eixo dos ZZ             
	for (int i = 1; i <= divisions; i++) {
		for (int j = 1; j <= divisions; j++) {
			drawSquareBack(filename, ai, bi, -comprimento / 2,
				ai - deltaa, bi + deltab, -comprimento / 2);

			drawSquareFront(filename, ai, bi, comprimento / 2,
				ai - deltaa, bi + deltab, comprimento / 2);


			ai = (xDimensions / 2) - j * deltaa;
		}
		bi = (-altura / 2) + i * deltab;
		ai = xDimensions / 2;
	}

	//--------------------------------------------------------------------------------------------------------------------------
	float deltaX = xDimensions / divisions;
	float deltaZ = comprimento / divisions;

	float auxDimensions = xDimensions / 2;
	float auxComprimento = comprimento / 2;

	float xi = xDimensions / 2;
	float zi = comprimento / 2;


	//desenhar as faces do eixo dos YY
	for (int i = 1; i <= divisions; i++) {
		for (int j = 1; j <= divisions; j++) {
			drawSquareBase(filename, xi, -altura / 2, zi,
				xi - deltaX, -altura / 2, zi - deltaZ);


			drawSquareUp(filename, xi, altura / 2, zi,
				xi - deltaX, altura / 2, zi - deltaZ);



			xi = (xDimensions / 2) - j * deltaX;
		}
		zi = (comprimento / 2) - (i * deltaZ);
		xi = (xDimensions / 2);
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------
	float deltaY1 = altura / divisions;
	float deltaZ1 = comprimento / divisions;

	float yy = -altura / 2;
	float zz = -comprimento / 2;


	//desenhar as faces do eixo dos XX
	for (int i = 1; i <= divisions; i++) {
		for (int j = 1; j <= divisions; j++) {
			drawSquareRight(filename, xDimensions / 2, yy, zz,
				xDimensions / 2, yy + deltaY1, zz + deltaZ1);

			drawSquareLeft(filename, -xDimensions / 2, yy, zz,
				-xDimensions / 2, yy + deltaY1, zz + deltaZ1);


			zz = -comprimento / 2 + j * deltaZ1;
		}

		yy = (-altura / 2) + (i * deltaY1);
		zz = -comprimento / 2;
	}
}

int main(int argc, char *argv[]) {
	if (argc <= 1) {
		cout << "Try one of this words: plane | cone | sphere | box" << endl;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	else if (!strcmp(argv[1], "plane")) {
		if (argc != 4) {
			cout << "Try this: 'generator' plane' <size> <file name>" << endl;
			return 0;
		}

		else {
			float tamanho = atof(argv[2]);
			drawPlane(argv[3], tamanho);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	else if (!strcmp(argv[1], "cone")) {
		if (argc != 7) {
			cout << "Try this: 'generator' 'cone' <radius> <height> <slices> <stacks> <file name>" << endl;
			return 0;
		}

		else {
			float radius = atof(argv[2]);
			float height = atof(argv[3]);
			int slices = atoi(argv[4]);
			int stacks = atoi(argv[5]);
			drawCone(argv[6], radius, height, slices, stacks);
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------
	else if (!strcmp(argv[1], "sphere")) {
		if (argc != 6) {
			cout << "Try this: 'generator' 'sphere' <radius> <slices> <stacks> <file name>" << endl;
			return 0;
		}

		else {
			float radius = atof(argv[2]);
			int slices = atoi(argv[3]);
			int stacks = atoi(argv[4]);
			drawSphere(argv[5], radius, slices, stacks);
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------
	else if (!strcmp(argv[1], "box")) {
		if (argc != 7) {
			cout << "Try this: 'generator' 'box' <xDimensions> <yDimensions> <zDimensions> <divisions> <file name>" << endl;
			return 0;
		}

		else {
			float xDimensions = atof(argv[2]);
			float altura = atof(argv[3]);
			float comprimento = atof(argv[4]);
			int divisions = atoi(argv[5]);
			drawBox(argv[6], xDimensions, altura, comprimento, divisions);
		}
	}

	return 0;
}