
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
#define BUFF_SIZE 1024
using namespace std;

struct ponto {
	float x;
	float y;
	float z;
} Ponto;

struct pontoText {
	float x;
	float y;
} PontoText;

struct vetorNormal{
	float x;
	float y;
	float z;
}VetorNormal;

/*Escrever as coordenadas de um ponto numa posição de um dado ficheiro*/
std::string writePoint(ponto point) {
	std::stringstream sstm;
	sstm << point.x << " " << point.y << " " << point.z;
	return sstm.str();
}

std::string writeNormal(vetorNormal point) {
	std::stringstream sstm;
	sstm << point.x << " " << point.y << " " << point.z;
	return sstm.str();
}

std::string writePointTex(pontoText point) {
	std::stringstream sstm;
	sstm << point.x << " " << point.y;
	return sstm.str();
}
/*Indicar os pontos de um triangulo a escrever num dado ficheiro
  No final, vai ficar um triangulo representado numa linha do ficheiro*/
std::string formTriangle(ponto pointA, ponto pointB, ponto pointC) {
	return writePoint(pointA) + " " + writePoint(pointB) + " " + writePoint(pointC);
}

std::string formLine(ponto pointA, ponto pointB, ponto pointC,pontoText a, pontoText b, pontoText c,vetorNormal na,vetorNormal nb, vetorNormal nc) {
	return (writePoint(pointA) + " " + writePoint(pointB) + " " + writePoint(pointC)+ " " + writePointTex(a)+" "+writePointTex(b)+" "+writePointTex(c)+
		" "+ writeNormal(na) +" "+writeNormal(nb) +" "+writeNormal(nc));
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

	pontoText pontoTexA;
	pontoTexA.x = 0;
	pontoTexA.y = 0;

	vetorNormal vecNormalA;
	vecNormalA.x = 0;
	vecNormalA.y = 0;
	vecNormalA.z = 0;

	ponto pontoA;
	pontoA.x = -x;
	pontoA.y = 0;
	pontoA.z = x;

	pontoText pontoTexB;
	pontoTexB.x = 1;
	pontoTexB.y = 0;

	vetorNormal vecNormalB;
	vecNormalB.x = 0;
	vecNormalB.y = 0;
	vecNormalB.z = 0;

	ponto pontoB;
	pontoB.x = x;
	pontoB.y = 0;
	pontoB.z = x;

	pontoText pontoTexC;
	pontoTexC.x = 1;
	pontoTexC.y = 1;

	vetorNormal vecNormalC;
	vecNormalC.x = 0;
	vecNormalC.y = 0;
	vecNormalC.z = 0;

	ponto pontoC;
	pontoC.x = x;
	pontoC.y = 0;
	pontoC.z = -x;

	pontoText pontoTexD;
	pontoTexD.x = 0;
	pontoTexD.y = 1;

	vetorNormal vecNormalD;
	vecNormalD.x = 0;
	vecNormalD.y = 0;
	vecNormalD.z = 0;

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

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoC, pontoD, pontoA,pontoTexC,pontoTexD,pontoTexA,vecNormalC,vecNormalD,vecNormalA) << endl;
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
			pontoText pontoTexA;
			pontoTexA.x = 0;
			pontoTexA.y = 0;

			vetorNormal vecNormalA;
			vecNormalA.x = 0;
			vecNormalA.y = 0;
			vecNormalA.z = 0;

			ponto pontoA;
			pontoA.x = radius * sin(z * ang);
			pontoA.y = 0;
			pontoA.z = radius * cos(z * ang);

			pontoText pontoTexB;
			pontoTexB.x = 0;
			pontoTexB.y = 0;

			vetorNormal vecNormalB;
			vecNormalB.x = 0;
			vecNormalB.y = 0;
			vecNormalB.z = 0;

			ponto pontoB;
			pontoB.x = 0;
			pontoB.y = 0;
			pontoB.z = 0;

			pontoText pontoTexC;
			pontoTexC.x = 0;
			pontoTexC.y = 0;

			vetorNormal vecNormalC;
			vecNormalC.x = 0;
			vecNormalC.y = 0;
			vecNormalC.z = 0;

			ponto pontoC;
			pontoC.x = radius * sin(ang * (z + 1));
			pontoC.y = 0;
			pontoC.z = radius * cos(ang * (z + 1));

			ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;

		}

		for (int i = 0; i < stacks; ++i) {
			float raio = radius - i * (radius / stacks);
			for (int z = 0; z < slices; z++) {

				// Lado1
				float raio2 = radius - (i + 1) * (radius / stacks);

				pontoText pontoTexD;
				pontoTexD.x = 0;
				pontoTexD.y = 0;

				vetorNormal vecNormalD;
				vecNormalD.x = 0;
				vecNormalD.y = 0;
				vecNormalD.z = 0;

				ponto pontoD;
				pontoD.x = raio2 * sin(ang * z);
				pontoD.y = (i + 1) * (height / stacks);
				pontoD.z = raio2 * cos(ang * z);

				pontoText pontoTexE;
				pontoTexE.x = 0;
				pontoTexE.y = 0;

				vetorNormal vecNormalE;
				vecNormalE.x = 0;
				vecNormalE.y = 0;
				vecNormalE.z = 0;

				ponto pontoE;
				pontoE.x = raio * sin(z * ang);
				pontoE.y = i * (height / stacks);
				pontoE.z = raio * cos(z * ang);

				pontoText pontoTexF;
				pontoTexF.x = 0;
				pontoTexF.y = 0;

				vetorNormal vecNormalF;
				vecNormalF.x = 0;
				vecNormalF.y = 0;
				vecNormalF.z = 0;

				ponto pontoF;
				pontoF.x = raio * sin(ang * (z + 1));
				pontoF.y = i * (height / stacks);
				pontoF.z = raio * cos(ang * (z + 1));

				ficheiro << formLine(pontoD, pontoE, pontoF,pontoTexD,pontoTexE,pontoTexF,vecNormalD,vecNormalE,vecNormalF) << endl;

				// Lado2
				
				pontoText pontoTexG;
				pontoTexG.x = 0;
				pontoTexG.y = 0;

				vetorNormal vecNormalG;
				vecNormalG.x = 0;
				vecNormalG.y = 0;
				vecNormalG.z = 0;
				
				ponto pontoG;
				pontoG.x = raio2 * sin(z * ang);
				pontoG.y = (i + 1) * (height / stacks);
				pontoG.z = raio2 * cos(z * ang);

				pontoText pontoTexH;
				pontoTexH.x = 0;
				pontoTexH.y = 0;

				vetorNormal vecNormalH;
				vecNormalH.x = 0;
				vecNormalH.y = 0;
				vecNormalH.z = 0;

				ponto pontoH;
				pontoH.x = raio * sin(ang * (z + 1));
				pontoH.y = i * (height / stacks);
				pontoH.z = raio * cos(ang * (z + 1));

				pontoText pontoTexI;
				pontoTexI.x = 0;
				pontoTexI.y = 0;

				vetorNormal vecNormalI;
				vecNormalI.x = 0;
				vecNormalI.y = 0;
				vecNormalI.z = 0;

				ponto pontoI;
				pontoI.x = raio2 * sin(ang * (z + 1));
				pontoI.y = (i + 1) * (height / stacks);
				pontoI.z = raio2 * cos(ang * (z + 1));

				ficheiro << formLine(pontoG, pontoH, pontoI,pontoTexG,pontoTexH,pontoTexI,vecNormalG,vecNormalG,vecNormalI) << endl;
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
				//primeira slice vai ter coordenadas (0, y), onde y vai ser `(stack actual) / (# de stacks)`
				
				pontoText pontoTexA;
				pontoTexA.x = (((float) j) / slices);
				pontoTexA.y = 0.5 + (((float)i) / stacks);

				vetorNormal vecNormalA;
				vecNormalA.x = cos(nivel)*sin(angulo);
				vecNormalA.y = sin(nivel);
				vecNormalA.z = cos(nivel)*cos(angulo);

				ponto pontoA;
				pontoA.x = radius * cos(nivel)*sin(angulo);
				pontoA.y = radius * sin(nivel);
				pontoA.z = radius * cos(nivel)*cos(angulo);
				
				pontoText pontoTexB;
				pontoTexB.x = ((((float)j) + 1) / slices) ;
				pontoTexB.y = 0.5 + (((float)i) / stacks);
				
				vetorNormal vecNormalB;
				vecNormalB.x = cos(nivel)*sin(angulo2);
				vecNormalB.y = sin(nivel);
				vecNormalB.z = cos(nivel)*cos(angulo2);

				ponto pontoB;
				pontoB.x = radius * cos(nivel)*sin(angulo2);
				pontoB.y = radius * sin(nivel);
				pontoB.z = radius * cos(nivel)*cos(angulo2);
				
				vetorNormal vecNormalC;
				vecNormalC.x = cos(nivel2)*sin(angulo2);
				vecNormalC.y = sin(nivel2);
				vecNormalC.z = cos(nivel2)*cos(angulo2);

				pontoText pontoTexC;
				pontoTexC.x = ((((float)j) + 1) / slices);
				pontoTexC.y = 0.5 + ((((float)i) + 1) / stacks);
				
				ponto pontoC;
				pontoC.x = radius * cos(nivel2)*sin(angulo2);
				pontoC.y = radius * sin(nivel2);
				pontoC.z = radius * cos(nivel2)*cos(angulo2);

				ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;

				//--------------------------------------------------------------------------------------------------
				pontoText pontoTexD;
				pontoTexD.x = ((((float)j) + 1) / slices);
				pontoTexD.y = 0.5 + ((((float)i) + 1) / stacks);
				
				vetorNormal vecNormalD;
				vecNormalD.x = cos(nivel2)*sin(angulo2);
				vecNormalD.y = sin(nivel2);
				vecNormalD.z = cos(nivel2)*cos(angulo2);

				ponto pontoD;
				pontoD.x = radius * cos(nivel2)*sin(angulo2);
				pontoD.y = radius * sin(nivel2);
				pontoD.z = radius * cos(nivel2)*cos(angulo2);
				
				pontoText pontoTexE;
				pontoTexE.x = (((float)j) / slices);
				pontoTexE.y = 0.5 + ((((float)i) + 1) / stacks);
				
				vetorNormal vecNormalE;
				vecNormalE.x = cos(nivel2)*sin(angulo);
				vecNormalE.y = sin(nivel2);
				vecNormalE.z = cos(nivel2)*cos(angulo);

				ponto pontoE;
				pontoE.x = radius * cos(nivel2)*sin(angulo);
				pontoE.y = radius * sin(nivel2);
				pontoE.z = radius * cos(nivel2)*cos(angulo);

				pontoText pontoTexF;
				pontoTexF.x = (((float)j) / slices);
				pontoTexF.y = 0.5 + (((float)i) / stacks);

				vetorNormal vecNormalF;
				vecNormalF.x = cos(nivel)*sin(angulo);
				vecNormalF.y = sin(nivel);
				vecNormalF.z = cos(nivel)*cos(angulo);

				ponto pontoF;
				pontoF.x = radius * cos(nivel)*sin(angulo);
				pontoF.y = radius * sin(nivel);
				pontoF.z = radius * cos(nivel)*cos(angulo);

				ficheiro << formLine(pontoD, pontoE, pontoF, pontoTexD, pontoTexE, pontoTexF,vecNormalD,vecNormalE,vecNormalF) << endl;

				//---------------------------------------------------------------------------------------------------
				pontoText pontoTexG;
				pontoTexG.x = 1 - ((((float)j)+1) / slices);
				pontoTexG.y = 0.5 - (((float)i) / stacks);
				
				vetorNormal vecNormalG;
				vecNormalG.x = cos(-nivel)*sin(-angulo);
				vecNormalG.y = sin(-nivel);
				vecNormalG.z = cos(-nivel)*cos(-angulo);

				ponto pontoG;
				pontoG.x = radius * cos(-nivel)*sin(-angulo);
				pontoG.y = radius * sin(-nivel);
				pontoG.z = radius * cos(-nivel)*cos(-angulo);
				
				pontoText pontoTexH;
				pontoTexH.x = 1 - (((float)j) / slices);
				pontoTexH.y = 0.5 - (((float)i) / stacks);
				
				vetorNormal vecNormalH;
				vecNormalH.x = cos(-nivel)*sin(-angulo2);
				vecNormalH.y = sin(-nivel);
				vecNormalH.z = cos(-nivel)*cos(-angulo2);

				ponto pontoH;
				pontoH.x = radius * cos(-nivel)*sin(-angulo2);
				pontoH.y = radius * sin(-nivel);
				pontoH.z = radius * cos(-nivel)*cos(-angulo2);
				
				pontoText pontoTexI;
				pontoTexI.x = 1 - (((float)j) / slices);
				pontoTexI.y = 0.5 - ((((float)i)+1) / stacks);
				
				vetorNormal vecNormalI;
				vecNormalI.x = cos(-nivel2)*sin(-angulo2);
				vecNormalI.y = sin(-nivel2);
				vecNormalI.z = cos(-nivel2)*cos(-angulo2);

				ponto pontoI;
				pontoI.x = radius * cos(-nivel2)*sin(-angulo2);
				pontoI.y = radius * sin(-nivel2);
				pontoI.z = radius * cos(-nivel2)*cos(-angulo2);

				ficheiro << formLine(pontoG, pontoH, pontoI,pontoTexG, pontoTexH, pontoTexI,vecNormalG,vecNormalH,vecNormalI) << endl;

				//_--------------------------------------------------------------------------------------------------
				pontoText pontoTexJ;
				pontoTexJ.x = 1 - (((float)j) / slices);
				pontoTexJ.y = 0.5 - ((((float)i)+1) / stacks);

				vetorNormal vecNormalJ;
				vecNormalJ.x = cos(-nivel2)*sin(-angulo2);
				vecNormalJ.y = sin(-nivel2);
				vecNormalJ.z = cos(-nivel2)*cos(-angulo2);

				ponto pontoJ;
				pontoJ.x = radius * cos(-nivel2)*sin(-angulo2);
				pontoJ.y = radius * sin(-nivel2);
				pontoJ.z = radius * cos(-nivel2)*cos(-angulo2);
				
				pontoText pontoTexK;
				pontoTexK.x = 1 - ((((float)j)+1) / slices);
				pontoTexK.y = 0.5 - ((((float)i)+1) / stacks);
				
				vetorNormal vecNormalK;
				vecNormalK.x = cos(-nivel2)*sin(-angulo);
				vecNormalK.y = sin(-nivel2);
				vecNormalK.z = cos(-nivel2)*cos(-angulo);

				ponto pontoK;
				pontoK.x = radius * cos(-nivel2)*sin(-angulo);
				pontoK.y = radius * sin(-nivel2);
				pontoK.z = radius * cos(-nivel2)*cos(-angulo);

				pontoText pontoTexW;
				pontoTexW.x = 1 - ((((float)j)+1) / slices);
				pontoTexW.y = 0.5 - (((float)i) / stacks);
				
				vetorNormal vecNormalW;
				vecNormalW.x = cos(-nivel)*sin(-angulo);
				vecNormalW.y = sin(-nivel);
				vecNormalW.z = cos(-nivel)*cos(-angulo);

				ponto pontoW;
				pontoW.x = radius * cos(-nivel)*sin(-angulo);
				pontoW.y = radius * sin(-nivel);
				pontoW.z = radius * cos(-nivel)*cos(-angulo);

				ficheiro << formLine(pontoJ, pontoK, pontoW, pontoTexJ,pontoTexK, pontoTexW,vecNormalJ,vecNormalK,vecNormalW) << endl;
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
		
		pontoText pontoTexA;
		pontoTexA.x = 0;
		pontoTexA.y = 0;

		vetorNormal vecNormalA;
		vecNormalA.x = 0;
		vecNormalA.y = 0;
		vecNormalA.z = 0;
		
		ponto pontoA;
		pontoA.x = z1;
		pontoA.y = z2;
		pontoA.z = z3;

		pontoText pontoTexB;
		pontoTexB.x = 0;
		pontoTexB.y = 0;

		vetorNormal vecNormalB;
		vecNormalB.x = 0;
		vecNormalB.y = 0;
		vecNormalB.z = 0;

		ponto pontoB;
		pontoB.x = x1;
		pontoB.y = x2;
		pontoB.z = x3;

		pontoText pontoTexC;
		pontoTexC.x = 0;
		pontoTexC.y = 0;

		vetorNormal vecNormalC;
		vecNormalC.x = 0;
		vecNormalC.y = 0;
		vecNormalC.z = 0;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		pontoText pontoTexD;
		pontoTexD.x = 0;
		pontoTexD.y = 0;

		vetorNormal vecNormalD;
		vecNormalD.x = 0;
		vecNormalD.y = 0;
		vecNormalD.z = 0;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoD, pontoC, pontoB,pontoTexD,pontoTexC,pontoTexB,vecNormalD,vecNormalC,vecNormalB) << endl;
	}

	ficheiro.close();
}

/*Funcao que desenha uma divisão da face que se encontra na parte positiva do eixo dos ZZ*/
void drawSquareFront(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = x3, w1 = y1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending
	if (ficheiro.is_open()) {
		
		pontoText pontoTexA;
		pontoTexA.x = 0;
		pontoTexA.y = 0;

		vetorNormal vecNormalA;
		vecNormalA.x = 0;
		vecNormalA.y = 0;
		vecNormalA.z = 0;

		ponto pontoA;
		pontoA.x = x1;
		pontoA.y = x2;
		pontoA.z = x3;

		pontoText pontoTexB;
		pontoTexB.x = 0;
		pontoTexB.y = 0;

		vetorNormal vecNormalB;
		vecNormalB.x = 0;
		vecNormalB.y = 0;
		vecNormalB.z = 0;

		ponto pontoB;
		pontoB.x = z1;
		pontoB.y = z2;
		pontoB.z = z3;

		pontoText pontoTexC;
		pontoTexC.x = 0;
		pontoTexC.y = 0;

		vetorNormal vecNormalC;
		vecNormalC.x = 0;
		vecNormalC.y = 0;
		vecNormalC.z = 0;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		pontoText pontoTexD;
		pontoTexD.x = 0;
		pontoTexD.y = 0;

		vetorNormal vecNormalD;
		vecNormalD.x = 0;
		vecNormalD.y = 0;
		vecNormalD.z = 0;
		ponto pontoD;
		
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoC, pontoD, pontoA,pontoTexC,pontoTexD,pontoTexA,vecNormalC,vecNormalD,vecNormalA) << endl;
	}
	ficheiro.close();
}

/*Funcao que desenha uma divisão da face que se encontra na parte positiva do eixo dos YY*/
void drawSquareUp(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = y3, w1 = y1, w2 = x2, w3 = x3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		
		pontoText pontoTexA;
		pontoTexA.x = 0;
		pontoTexA.y = 0;

		vetorNormal vecNormalA;
		vecNormalA.x = 0;
		vecNormalA.y = 0;
		vecNormalA.z = 0;
		
		ponto pontoA;
		pontoA.x = x1;
		pontoA.y = x2;
		pontoA.z = x3;

		pontoText pontoTexB;
		pontoTexB.x = 0;
		pontoTexB.y = 0;

		vetorNormal vecNormalB;
		vecNormalB.x = 0;
		vecNormalB.y = 0;
		vecNormalB.z = 0;

		ponto pontoB;
		pontoB.x = z1;
		pontoB.y = z2;
		pontoB.z = z3;

		pontoText pontoTexC;
		pontoTexC.x = 0;
		pontoTexC.y = 0;

		vetorNormal vecNormalC;
		vecNormalC.x = 0;
		vecNormalC.y = 0;
		vecNormalC.z = 0;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		pontoText pontoTexD;
		pontoTexD.x = 0;
		pontoTexD.y = 0;

		vetorNormal vecNormalD;
		vecNormalD.x = 0;
		vecNormalD.y = 0;
		vecNormalD.z = 0;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoC, pontoD, pontoA,pontoTexC,pontoTexD,pontoTexA,vecNormalC,vecNormalD,vecNormalA) << endl;

		ficheiro.close();
	}
}

/*Funcao que desenha uma divisão da face que se encontra na parte negativa do eixo dos YY*/
void drawSquareBase(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = x1, z2 = y2, z3 = y3, w1 = y1, w2 = x2, w3 = x3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {

		pontoText pontoTexA;
		pontoTexA.x = 0;
		pontoTexA.y = 0;

		vetorNormal vecNormalA;
		vecNormalA.x = 0;
		vecNormalA.y = 0;
		vecNormalA.z = 0;

		ponto pontoA;
		pontoA.x = z1;
		pontoA.y = z2;
		pontoA.z = z3;

		pontoText pontoTexB;
		pontoTexB.x = 0;
		pontoTexB.y = 0;

		vetorNormal vecNormalB;
		vecNormalB.x = 0;
		vecNormalB.y = 0;
		vecNormalB.z = 0;

		ponto pontoB;
		pontoB.x = x1;
		pontoB.y = x2;
		pontoB.z = x3;

		pontoText pontoTexC;
		pontoTexC.x = 0;
		pontoTexC.y = 0;

		vetorNormal vecNormalC;
		vecNormalC.x = 0;
		vecNormalC.y = 0;
		vecNormalC.z = 0;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		pontoText pontoTexD;
		pontoTexD.x = 0;
		pontoTexD.y = 0;

		vetorNormal vecNormalD;
		vecNormalD.x = 0;
		vecNormalD.y = 0;
		vecNormalD.z = 0;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoD, pontoC, pontoB,pontoTexD,pontoTexC,pontoTexB,vecNormalD,vecNormalC,vecNormalB) << endl;

		ficheiro.close();
	}
}

/*Funcao que desenha uma divisão da face que se encontra na parte negativa do eixo dos XX*/
void drawSquareLeft(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = y1, z2 = y2, z3 = x3, w1 = x1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {
		
		pontoText pontoTexA;
		pontoTexA.x = 0;
		pontoTexA.y = 0;

		vetorNormal vecNormalA;
		vecNormalA.x = 0;
		vecNormalA.y = 0;
		vecNormalA.z = 0;
		
		ponto pontoA;
		pontoA.x = z1;
		pontoA.y = z2;
		pontoA.z = z3;

		pontoText pontoTexB;
		pontoTexB.x = 0;
		pontoTexB.y = 0;

		vetorNormal vecNormalB;
		vecNormalB.x = 0;
		vecNormalB.y = 0;
		vecNormalB.z = 0;

		ponto pontoB;
		pontoB.x = x1;
		pontoB.y = x2;
		pontoB.z = x3;

		pontoText pontoTexC;
		pontoTexC.x = 0;
		pontoTexC.y = 0;

		vetorNormal vecNormalC;
		vecNormalC.x = 0;
		vecNormalC.y = 0;
		vecNormalC.z = 0;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		pontoText pontoTexD;
		pontoTexD.x = 0;
		pontoTexD.y = 0;

		vetorNormal vecNormalD;
		vecNormalD.x = 0;
		vecNormalD.y = 0;
		vecNormalD.z = 0;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoD, pontoC, pontoB,pontoTexD,pontoTexC,pontoTexB,vecNormalD,vecNormalC,vecNormalB) << endl;

		ficheiro.close();
	}
}

/*Funcao que desenha uma divisão da face que se encontra na parte ṕositiva do eixo dos XX*/
void drawSquareRight(char *filename, float x1, float x2, float x3, float y1, float y2, float y3) {
	float z1 = y1, z2 = y2, z3 = x3, w1 = x1, w2 = x2, w3 = y3;

	std::ofstream ficheiro;

	ficheiro.open(filename, std::ofstream::out | std::ofstream::app); // open file for appending

	if (ficheiro.is_open()) {

		pontoText pontoTexA;
		pontoTexA.x = 0;
		pontoTexA.y = 0;

		vetorNormal vecNormalA;
		vecNormalA.x = 0;
		vecNormalA.y = 0;
		vecNormalA.z = 0;

		ponto pontoA;
		pontoA.x = x1;
		pontoA.y = x2;
		pontoA.z = x3;

		pontoText pontoTexB;
		pontoTexB.x = 0;
		pontoTexB.y = 0;

		vetorNormal vecNormalB;
		vecNormalB.x = 0;
		vecNormalB.y = 0;
		vecNormalB.z = 0;

		ponto pontoB;
		pontoB.x = z1;
		pontoB.y = z2;
		pontoB.z = z3;

		pontoText pontoTexC;
		pontoTexC.x = 0;
		pontoTexC.y = 0;

		vetorNormal vecNormalC;
		vecNormalC.x = 0;
		vecNormalC.y = 0;
		vecNormalC.z = 0;

		ponto pontoC;
		pontoC.x = y1;
		pontoC.y = y2;
		pontoC.z = y3;

		pontoText pontoTexD;
		pontoTexD.x = 0;
		pontoTexD.y = 0;

		vetorNormal vecNormalD;
		vecNormalD.x = 0;
		vecNormalD.y = 0;
		vecNormalD.z = 0;

		ponto pontoD;
		pontoD.x = w1;
		pontoD.y = w2;
		pontoD.z = w3;

		ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;
		ficheiro << formLine(pontoC, pontoD, pontoA,pontoTexC,pontoTexD,pontoTexA,vecNormalC,vecNormalD,vecNormalA) << endl;

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


void drawDisk(char* filename, float dist, float raioCentro, int slices) {
	float alpha, dAlpha, pAlpha;
	float value = 4 * slices;

	dAlpha = (2.0f * M_PI) / slices;

	ofstream ficheiro;
	ficheiro.open(filename);

	if (ficheiro.is_open()) {
		ficheiro.clear();

		ficheiro << writeLineNumber(value);

		//em cada iteração do ciclo, são desenhados 2 triângulos
		for (int i = 0; i < slices; i++) {
			alpha = i * dAlpha;
			pAlpha = alpha + dAlpha;

			pontoText pontoTexA;
			pontoTexA.x = 0;
			pontoTexA.y = 0;

			vetorNormal vecNormalA;
			vecNormalA.x = 0;
			vecNormalA.y = 0;
			vecNormalA.z = 0;

			ponto pontoA;
			pontoA.x = raioCentro * cosf(alpha);
			pontoA.y = 0.0;
			pontoA.z = raioCentro * sinf(alpha);

			pontoText pontoTexB;
			pontoTexB.x = 0;
			pontoTexB.y = 0;

			vetorNormal vecNormalB;
			vecNormalB.x = 0;
			vecNormalB.y = 0;
			vecNormalB.z = 0;

			ponto pontoB;
			pontoB.x = raioCentro * cosf(pAlpha);
			pontoB.y = 0.0;
			pontoB.z = raioCentro * sinf(pAlpha);

			pontoText pontoTexC;
			pontoTexC.x = 0;
			pontoTexC.y = 0;

			vetorNormal vecNormalC;
			vecNormalC.x = 0;
			vecNormalC.y = 0;
			vecNormalC.z = 0;

			ponto pontoC;
			pontoC.x = (raioCentro + dist) * cosf(pAlpha);
			pontoC.y = 0.0;
			pontoC.z = (raioCentro + dist) * sinf(pAlpha);

			ficheiro << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;

			pontoText pontoTexD;
			pontoTexD.x = 0;
			pontoTexD.y = 0;

			vetorNormal vecNormalD;
			vecNormalD.x = 0;
			vecNormalD.y = 0;
			vecNormalD.z = 0;

			ponto pontoD;
			pontoD.x = (raioCentro + dist) * cosf(pAlpha);
			pontoD.y = 0.0;
			pontoD.z = (raioCentro + dist) * sinf(pAlpha);

			pontoText pontoTexE;
			pontoTexE.x = 0;
			pontoTexE.y = 0;

			vetorNormal vecNormalE;
			vecNormalE.x = 0;
			vecNormalE.y = 0;
			vecNormalE.z = 0;

			ponto pontoE;
			pontoE.x = (raioCentro + dist) * cosf(alpha);
			pontoE.y = 0.0;
			pontoE.z = (raioCentro + dist) * sinf(alpha);

			pontoText pontoTexF;
			pontoTexF.x = 0;
			pontoTexF.y = 0;
			
			vetorNormal vecNormalF;
			vecNormalF.x = 0;
			vecNormalF.y = 0;
			vecNormalF.z = 0;

			ponto pontoF;
			pontoF.x = raioCentro * cosf(alpha);
			pontoF.y = 0.0;
			pontoF.z = raioCentro * sinf(alpha);

			ficheiro << formLine(pontoD, pontoE, pontoF,pontoTexD,pontoTexE,pontoTexF,vecNormalD,vecNormalE,vecNormalF) << endl;
		}

		for (int i = 0; i < slices; i++) {
			alpha = i * dAlpha;
			pAlpha = alpha + dAlpha;

			pontoText pontoTexG;
			pontoTexG.x = 0;
			pontoTexG.y = 0;

			vetorNormal vecNormalG;
			vecNormalG.x = 0;
			vecNormalG.y = 0;
			vecNormalG.z = 0;

			ponto pontoG;
			pontoG.y = 0.0;
			pontoG.x = raioCentro * cosf(-alpha);
			pontoG.z = raioCentro * sinf(-alpha);

			pontoText pontoTexH;
			pontoTexH.x = 0;
			pontoTexH.y = 0;

			vetorNormal vecNormalH;
			vecNormalH.x = 0;
			vecNormalH.y = 0;
			vecNormalH.z = 0;

			ponto pontoH;
			pontoH.y = 0.0;
			pontoH.x = raioCentro * cosf(-pAlpha);
			pontoH.z = raioCentro * sinf(-pAlpha);

			pontoText pontoTexI;
			pontoTexI.x = 0;
			pontoTexI.y = 0;

			vetorNormal vecNormalI;
			vecNormalI.x = 0;
			vecNormalI.y = 0;
			vecNormalI.z = 0;

			ponto pontoI;
			pontoI.y = 0.0;
			pontoI.x = (raioCentro + dist) * cosf(-pAlpha);
			pontoI.z = (raioCentro + dist) * sinf(-pAlpha);

			ficheiro << formLine(pontoG, pontoH, pontoI,pontoTexG,pontoTexH,pontoTexI,vecNormalG,vecNormalH,vecNormalI) << endl;

			pontoText pontoTexJ;
			pontoTexJ.x = 0;
			pontoTexJ.y = 0;

			vetorNormal vecNormalJ;
			vecNormalJ.x = 0;
			vecNormalJ.y = 0;
			vecNormalJ.z = 0;

			ponto pontoJ;
			pontoJ.y = 0.0;
			pontoJ.x = (raioCentro + dist) * cosf(-pAlpha);
			pontoJ.z = (raioCentro + dist) * sinf(-pAlpha);
			
			pontoText pontoTexK;
			pontoTexK.x = 0;
			pontoTexK.y = 0;
			
			vetorNormal vecNormalK;
			vecNormalK.x = 0;
			vecNormalK.y = 0;
			vecNormalK.z = 0;

			ponto pontoK;
			pontoK.y = 0.0;
			pontoK.x = (raioCentro + dist) * cosf(-alpha);
			pontoK.z = (raioCentro + dist) * sinf(-alpha);

			pontoText pontoTexL;
			pontoTexL.x = 0;
			pontoTexL.y = 0;

			vetorNormal vecNormalL;
			vecNormalL.x = 0;
			vecNormalL.y = 0;
			vecNormalL.z = 0;

			ponto pontoL;
			pontoL.y = 0.0;
			pontoL.x = raioCentro * cosf(-alpha);
			pontoL.z = raioCentro * sinf(-alpha);

			ficheiro << formLine(pontoJ, pontoK, pontoL,pontoTexJ,pontoTexK,pontoTexL,vecNormalJ,vecNormalK,vecNormalL) << endl;
		}
		ficheiro.close();
	}
}

ponto *cpoints;     // control points
int **indexes;      // indexes of the points for each patch
int patches;        // number of patches
int ncpoints;       // number of control points

void cross(float *a, float *b, float *res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

float comprimento(float *v) {
	float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

	return res;
}

void normalize(float *a) {
	float l = comprimento(a);
	if (l == 0.0f)
		return;

	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

void matrixVector(float *m, float *v, float *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void multVectorMatrix(float *v, float *m, float *res) {
	for (int i = 0; i < 4; ++i) {
		res[i] = 0;
		for (int j = 0; j < 4; ++j) {
			res[i] += v[j] * m[j * 4 + i];
		}
	}
}

void multMatrixMatrix(float *m1, float *m2, float *res) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[i * 4 + j] = 0.0f;
			for (int k = 0; k < 4; ++k)
				res[i * 4 + j] += m1[i * 4 + k] * m2[k * 4 + j];
		}
	}
}


void bezierPoint(float u, float v, ponto *pv, float *res) {
	float dU[3];
	float dV[3];
	float m[4][4] = {
		{ -1.0f,  3.0f, -3.0f, 1.0f },
		{  3.0f, -6.0f,  3.0f, 0.0f },
		{ -3.0f,  3.0f,  0.0f, 0.0f },
		{  1.0f,  0.0f,  0.0f, 0.0f }
	};

	float Px[4][4] = {
		{ pv[0].x, pv[1].x, pv[2].x, pv[3].x },
		{ pv[4].x, pv[5].x, pv[6].x, pv[7].x },
		{ pv[8].x, pv[9].x, pv[10].x, pv[11].x },
		{ pv[12].x, pv[13].x, pv[14].x, pv[15].x }
	};

	float Py[4][4] = {
		{ pv[0].y, pv[1].y, pv[2].y, pv[3].y },
		{ pv[4].y, pv[5].y, pv[6].y, pv[7].y },
		{ pv[8].y, pv[9].y, pv[10].y, pv[11].y },
		{ pv[12].y, pv[13].y, pv[14].y, pv[15].y }
	};

	float Pz[4][4] = {
		{ pv[0].z, pv[1].z, pv[2].z, pv[3].z },
		{ pv[4].z, pv[5].z, pv[6].z, pv[7].z },
		{ pv[8].z, pv[9].z, pv[10].z, pv[11].z },
		{ pv[12].z, pv[13].z, pv[14].z, pv[15].z }
	};

	float U[4] = { u * u * u, u * u, u, 1 };
	float UD[4] = { 3 * u * u, 2 * u, 1, 0 };
	float V[4] = { v * v * v, v * v, v, 1 };
	float VD[4] = { 3 * v * v, 2 * v, 1, 0 };

	float MdV[4];
	float MV[4];
	matrixVector((float *)m, V, MV);
	matrixVector((float *)m, VD, MdV);

	float dUM[4];
	float UM[4];
	multVectorMatrix(U, (float *)m, UM);
	multVectorMatrix(UD, (float *)m, dUM);

	float UMP[3][4];
	multVectorMatrix(UM, (float *)Px, UMP[0]);
	multVectorMatrix(UM, (float *)Py, UMP[1]);
	multVectorMatrix(UM, (float *)Pz, UMP[2]);

	float dUMP[3][4];
	multVectorMatrix(dUM, (float *)Px, dUMP[0]);
	multVectorMatrix(dUM, (float *)Py, dUMP[1]);
	multVectorMatrix(dUM, (float *)Pz, dUMP[2]);

	for (int j = 0; j < 3; j++) {
		res[j] = 0.0f;
		dU[j] = 0.0f;
		dV[j] = 0.0f;

		for (int i = 0; i < 4; i++) {
			res[j] += MV[i] * UMP[j][i];
			dU[j] += MV[i] * dUMP[j][i];
			dV[j] += MdV[i] * UMP[j][i];
		}
	}
}

int geradorBezier(char *outfile, int tesselation) {
	int linhas = 0;
	ponto pv[16];
	int divs = tesselation; // change this to change the tesselation level
	ostringstream pontos;
	ofstream out;

	out.open(outfile);
	if (!out.is_open()) {
		perror("ofstream.open");
		return 1;
	}
	for (int i = 0; i < patches; i++) {
		for (int j = 0; j < 16; j++) {
			pv[j] = cpoints[indexes[i][j]];
		}
		for (int u = 0; u < divs; u++) {
			float p1[3];
			float p2[3];
			float p3[3];
			float p4[3];
			for (int v = 0; v < divs; v++) {
				bezierPoint(u / (float)divs, v / (float)divs, pv, p1);
				bezierPoint((u + 1) / (float)divs, v / (float)divs, pv, p2);
				bezierPoint(u / (float)divs, (v + 1) / (float)divs, pv, p3);
				bezierPoint((u + 1) / (float)divs, (v + 1) / (float)divs, pv, p4);
				
				pontoText pontoTexA;
				pontoTexA.x = 0;
				pontoTexA.y = 0;

				vetorNormal vecNormalA;
				vecNormalA.x = 0;
				vecNormalA.y = 0;
				vecNormalA.z = 0;

				ponto pontoA;
				pontoA.y = p1[0];
				pontoA.x = p1[1];
				pontoA.z = p1[2];

				pontoText pontoTexB;
				pontoTexB.x = 0;
				pontoTexB.y = 0;

				vetorNormal vecNormalB;
				vecNormalB.x = 0;
				vecNormalB.y = 0;
				vecNormalB.z = 0;

				ponto pontoB;
				pontoB.y = p3[0];
				pontoB.x = p3[1];
				pontoB.z = p3[2];

				pontoText pontoTexC;
				pontoTexC.x = 0;
				pontoTexC.y = 0;

				vetorNormal vecNormalC;
				vecNormalC.x = 0;
				vecNormalC.y = 0;
				vecNormalC.z = 0;

				ponto pontoC;
				pontoC.y = p4[0];
				pontoC.x = p4[1];
				pontoC.z = p4[2];

				pontos << formLine(pontoA, pontoB, pontoC,pontoTexA,pontoTexB,pontoTexC,vecNormalA,vecNormalB,vecNormalC) << endl;

				pontoText pontoTexD;
				pontoTexD.x = 0;
				pontoTexD.y = 0;

				vetorNormal vecNormalD;
				vecNormalD.x = 0;
				vecNormalD.y = 0;
				vecNormalD.z = 0;

				ponto pontoD;
				pontoD.y = p2[0];
				pontoD.x = p2[1];
				pontoD.z = p2[2];
				
				pontoText pontoTexE;
				pontoTexE.x = 0;
				pontoTexE.y = 0;
				
				vetorNormal vecNormalE;
				vecNormalE.x = 0;
				vecNormalE.y = 0;
				vecNormalE.z = 0;

				ponto pontoE;
				pontoE.y = p1[0];
				pontoE.x = p1[1];
				pontoE.z = p1[2];

				pontoText pontoTexF;
				pontoTexF.x = 0;
				pontoTexF.y = 0;

				vetorNormal vecNormalF;
				vecNormalF.x = 0;
				vecNormalF.y = 0;
				vecNormalF.z = 0;

				ponto pontoF;
				pontoF.y = p4[0];
				pontoF.x = p4[1];
				pontoF.z = p4[2];

				pontos << formLine(pontoD, pontoE, pontoF,pontoTexD,pontoTexE,pontoTexF,vecNormalD,vecNormalE,vecNormalF) << endl;

				linhas += 2;
			}
		}
	}

	out << (to_string(linhas) + "\n" + pontos.str());
	out.close();

	return 0;
}

int bezierPatch(char *patch) {
	int lIndex;
	int i, j;
	char line[BUFF_SIZE];
	FILE *f = fopen(patch, "r");

	if (!f)
		return 1;

	fscanf(f, "%d\n", &lIndex);
	patches = lIndex;
	printf("%d\n", patches);
	indexes = (int **)malloc(sizeof(int *) * lIndex);
	if (!indexes)
		return 1;

	for (i = 0; i < lIndex; i++) {
		indexes[i] = (int *)malloc(sizeof(int) * 16);
		if (!indexes[i]) {
			// free previously allocated memory before returning non zero
			for (--i; i >= 0; --i) {
				free(indexes[i]);
			}
			free(indexes);

			return 1;
		}
		memset(line, 0, BUFF_SIZE);
		fgets(line, BUFF_SIZE, f);
		char* ind = NULL;
		for (j = 0, ind = strtok(line, ", "); ind && j < 16; ind = strtok(NULL, ", "), j++)
			indexes[i][j] = atoi(ind);
	}
	fscanf(f, "%d\n", &ncpoints);

	cpoints = (ponto *)malloc(sizeof(ponto) * ncpoints);
	if (!cpoints) {
		for (i = 0; i < lIndex; i++) {
			free(indexes[i]);
		}
		free(indexes);

		return 1;
	}

	for (i = 0; i < ncpoints; i++) {
		memset(line, 0, BUFF_SIZE);
		fgets(line, BUFF_SIZE, f);
		cpoints[i].x = atof(strtok(line, ", "));
		cpoints[i].y = atof(strtok(NULL, ", "));
		cpoints[i].z = atof(strtok(NULL, ", "));
	}

	fclose(f);
	return 0;
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
	//-------------------------------------------------------------------------------------------
	else if (!strcmp(argv[1], "disk")) {
		if (argc != 6) {
			cout << "Try this: 'generator' 'disk' <distance> <radiusCentre> <slices> <file name>" << endl;
			return 0;
		}

		else {
			float distancia = atof(argv[2]);
			float raioCentro = atof(argv[3]);
			int slices = atoi(argv[4]);
			drawDisk(argv[5], distancia, raioCentro, slices);
		}
	}
	else if (!strcmp(argv[1], "bezier")) {
		if (argc != 5) {
			cout << "Try this: 'generator' 'bezier' <tesselation> <patch> <file name>" << endl;
			return 0;
		}
		else {
			int res = bezierPatch(argv[3]);
			if (!res) res = geradorBezier(argv[4], atoi(argv[2]));
		}
	}

	return 0;
}