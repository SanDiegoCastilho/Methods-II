/*
PRA RODAR:
-------modo 1
g++ Luz.cpp Material.cpp Objeto.cpp Cone.cpp Esfera.cpp snowPOO.cpp -o main -lGL -lGLU -lGLEW -lglut -lm -Wno-write-strings
-------modo 2
g++ -c Funcoes.cpp -o Funcoes.o && g++ -c Luz.cpp -o Luz.o && g++ -c Objeto.cpp -o Objeto && g++ -c Material.cpp -o Material.o && g++ -c Cone.cpp -o Cone.o &&  g++ -c Esfera.cpp -o Esfera.o && g++ -c snowPOO.cpp -o snowPOO.o
g++ -o main Cone.o Esfera.o Objeto.o Material.o Luz.o Funcoes.o snowPOO.o -lGL -lGLU -lGLEW -lglut -lm -Wno-write-strings
--------modo 3
g++ -c *.cpp && g++ -o main *.o -O2 -L -lm -lpthread -lX11 -lGL -lGLU -lglut -Wno-write-strings && ./main
fazer camera e cenario
*/

#include <cmath>
#include <cstdio>
#include <string>
#include <iostream>

#include <GL/freeglut.h>

#include "CImg/CImg.h"


using namespace std;

static unsigned int   W_Npixels = 700, H_Npixels = 700;
static double         W = 200., H = 200.;

char planoFundo[10][30];
int indiceFundo = 4;
int controlador = 4;
double Precisao = 10;


//FUNÇÃO QUE PINTA A TELA

void rayCasting(void) {
	using namespace cimg_library;

	double R,G,B, Derivada;


	CImg<unsigned char> plano("img/monochrome.jpg");

	double l, c;

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

		for(l = 0; l < H_Npixels; l++){
			for(c = 0; c < W_Npixels; c++){

				R = (double)plano(c, l, 0, 0)/255;
				G = (double)plano(c, l, 0, 1)/255;
				B = (double)plano(c, l, 0, 2)/255;

				Derivada = (plano(c + 1, l, 0, 1) - plano(c, l, 0, 1))/2;

				if (Derivada < Precisao){
					glColor3f(0, 0, 0);
					
				}else{
					glColor3f(1, 1, 1);
					
				}

				glVertex2f(c, l);
			}
		}

   glEnd();

   glFlush();
}

void setGlobais(){
	glClearColor(0.8, 0.9, 0.9, 0.0);
}

//Pra fechar a janeça com o ESC
void Teclado(unsigned char key, int x, int y) {	
	switch((char)key) {
		case 27: //ESC
			glutDestroyWindow(0); exit(0); break;

		case '+': 
			Precisao += 5; break;

		case '-': 
			if (Precisao - 5 <= 0) Precisao = 5;
			else Precisao -= 5; break;

		default:
			break;
	}

	glutPostRedisplay();
}

void redimenciona(int w, int h) {
	int hSize = w*H/W;
	int wSize = h*W/H;

	if (h >= w) 
		glViewport(0, (h-hSize)/2, w, hSize);
	
	else if (h < w) 
		glViewport((w-wSize)/2, 0,  wSize, h);
	
 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, W_Npixels, H_Npixels, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv) {
	setGlobais();

	glutInit(&argc, argv);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
	glutInitWindowSize(W_Npixels, H_Npixels);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-W_Npixels)/2,(glutGet(GLUT_SCREEN_HEIGHT)-H_Npixels)/2);

	glutCreateWindow("_-_-_-_-_-_ Snowman _-_-_-_-_-_");
	glutDisplayFunc(rayCasting);
	glutReshapeFunc(redimenciona);
	glutKeyboardFunc(Teclado);

	glutMainLoop(); 
}