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
#include "../lib/CImg/CImg.h"

using namespace std;

static unsigned int   W_Npixels = 800, H_Npixels = 600;
static double         W = 200., H = 200.;

double Precisao = 10;

void rayCastingFilter(void) {
	using namespace cimg_library;

	double R,G,B, Derivada1, Derivada2;

	CImg<unsigned char> plano("../img/01.jpg");

	double l, c;

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

		for(l = 0; l < H_Npixels; l++){
			for(c = 0; c < W_Npixels; c++){

				R = (double)plano(c, l, 0, 0)/255;
				G = (double)plano(c, l, 0, 1)/255;
				B = (double)plano(c, l, 0, 2)/255;

				if (c + 2 != W_Npixels){
					Derivada1 = (-3*plano(c, l, 0, 0) + 4*plano(c + 1, l, 0, 0) - plano(c + 2, l, 0, 0))/4;

				}else{
					Derivada1 = (3*plano(c, l, 0, 0) - 4*plano(c + 1, l, 0, 0) + plano(c + 2, l, 0, 0))/4;
				}

				if (l + 2 != H_Npixels){
					Derivada2 = (-3*plano(c, l, 0, 0) + 4*plano(c, l + 1, 0, 0) - plano(c, l + 2, 0, 0))/4;
				}else{
					Derivada2 = (3*plano(c, l, 0, 0) - 4*plano(c, l + 1, 0, 0) + plano(c, l + 2, 0, 0))/4;
				}

				if (Derivada1 > Precisao || Derivada2 > Precisao){
					glColor3f(1, 1, 1);
					
				}else{
					glColor3f(0, 0, 0);
					
				}

				glVertex2f(c, l);
			}
		}

   glEnd();

   glFlush();
}

void rayCastingOrigin(void) {
	using namespace cimg_library;

	double R,G,B, Derivada1, Derivada2;

	CImg<unsigned char> plano("../img/01.jpg");

	double l, c;

	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);

		for(l = 0; l < H_Npixels; l++){
			for(c = 0; c < W_Npixels; c++){

				R = (double)plano(c, l, 0, 0)/255;
				G = (double)plano(c, l, 0, 1)/255;
				B = (double)plano(c, l, 0, 2)/255;

				glColor3f(R, G, B);

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
			glutDestroyWindow(0); exit(0); 
			break;

		case '+': 
			Precisao += 1; 
			break;

		case '-': 
			(Precisao - 2 <= 0) ? Precisao = 2 : Precisao -= 1;
			break;

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
	// glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-2*W_Npixels)/2,(glutGet(GLUT_SCREEN_HEIGHT)-2*H_Npixels)/2);
	glutInitWindowPosition(0,(glutGet(GLUT_SCREEN_HEIGHT)-H_Npixels)/2);

	glutCreateWindow("Imagem Original");
	glutDisplayFunc(rayCastingOrigin);
	glutReshapeFunc(redimenciona);

	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH))/2,(glutGet(GLUT_SCREEN_HEIGHT)-H_Npixels)/2);
	glutCreateWindow("Filtro TOPPER");
	glutDisplayFunc(rayCastingFilter);
	glutReshapeFunc(redimenciona);
	glutKeyboardFunc(Teclado);

	glutMainLoop(); 
}