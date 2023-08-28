
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "imageloader.h"
#pragma comment(lib, "glew32.lib")

using namespace std;

class Planeta{
public:
	float raio, distancia, orbita, orbitaVelocidade, InclinacaoEixo, EixoAni;
	Planeta(float _raio, float _distancia, float _orbita, float _orbitaVelocidade, float _InclinacaoEixo, float _EixoAni){
		raio = _raio;
		distancia = _distancia;
		orbita = _orbita;
		orbitaVelocidade = _orbitaVelocidade;
		InclinacaoEixo = _InclinacaoEixo;
		EixoAni = _EixoAni;
	}

	void desenhaOrbitaPequena(void){
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distancia, 100.0, 100.0);
		glPopMatrix();
	}

	void desenhaLua(void){
		GLUquadricObj *quadric;
		quadric = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbita, 0.0, 1.0, 0.0);
		glTranslatef(distancia, 0.0, 0.0);
		gluSphere(quadric, raio, 20.0, 20.0);
		glPopMatrix();
	}

};

//Sol, Planetas e Estrelas
Planeta sol(5.0, 0, 0, 0, 0, 0);				//Sol
Planeta mer(1.0, 7, 0, 4.74, 02.11, 0);		//Mercurio
Planeta ven(1.5, 11, 0, 3.50, 177.0, 0);		//Venus
Planeta ter(2.0, 16, 0, 2.98, 23.44, 0);		//Terra
Planeta mar(1.2, 21, 0, 2.41, 25.00, 0);		//Marte
Planeta jup(3.5, 28, 0, 1.31, 03.13, 0);		//Jupiter
Planeta sat(3.0, 37, 0, 0.97, 26.70, 0);		//Saturno
Planeta ura(2.5, 45.5, 0, 0.68, 97.77, 0);	//Urano
Planeta nep(2.3, 53.6, 0, 0.54, 28.32, 0);	//Netuno
Planeta plu(0.3, 59, 0, 0.47, 119.6, 0);		//Plutao
Planeta lun(.40, 3, 0, 5.40, 0, 0);			//Lua     (Terra)
Planeta pho(.20, 1.8, 0, 2.30, 0, 0);		//Phobos   (Marte)
Planeta dei(.24, 2.4, 0, 3.60, 0, 0);		//Deimos   (Marte)
Planeta eur(.24, 4, 0, 4.40, 0, 0);			//Europa   (Jupiter)
Planeta gan(.24, 4.7, 0, 5.00, 0, 0);		//Ganymede (Jupiter)
Planeta cal(.24, 5.3, 0, 2.30, 0, 0);		//Callisto (Jupiter)
Planeta tit(.75, 3.7, 0, 2.40, 0, 0);		//Titan	   (Saturno)
Planeta nix(.10, 1.5, 0, 5.00, 0, 0);		//Nix	   (Plutao)
Planeta puc(.26, 2.9, 0, 7.00, 0, 0);		//Puck	   (Urano)
Planeta tri(.36, 3.2, 0, 3.40, 0, 0);		//Triton   (Netuno)

int animado = 0;
int orbitaGrandeAtiva = 1;
int orbitaPequenaAtiva = 1;
int luaAtiva = 1;
int trocaCamera = 0;
int rotuloAtivo = 0;
int zoom = 50;
int logoCena = 1;


GLuint carregaTextura(Image* image) {
	//http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint solTexture, merTexture, venTexture, terTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, pluTexture, estTexture, logTexture;

void writeBitmapString(void *font, char *string)
{
	char *c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

//Setup de Texturização
void setupTex(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* est = loadBMP("estrelas.bmp");		estTexture = carregaTextura(est);		delete est;
	Image* sol = loadBMP("sol.bmp");		solTexture = carregaTextura(sol);		delete sol;
	Image* mer = loadBMP("mercurio.bmp");	merTexture = carregaTextura(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = carregaTextura(ven);		delete ven;
	Image* ter = loadBMP("terra.bmp");		terTexture = carregaTextura(ter);		delete ter;
	Image* mar = loadBMP("marte.bmp");		marTexture = carregaTextura(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = carregaTextura(jup);		delete jup;
	Image* sat = loadBMP("saturno.bmp");		satTexture = carregaTextura(sat);		delete sat;
	Image* ura = loadBMP("urano.bmp");		uraTexture = carregaTextura(ura);		delete ura;
	Image* nep = loadBMP("netuno.bmp");	nepTexture = carregaTextura(nep);		delete nep;
	Image* plu = loadBMP("plutao.bmp");		pluTexture = carregaTextura(plu);		delete plu;
	Image* log = loadBMP("logo.bmp");		logTexture = carregaTextura(log);		delete log;




}

// Especificação dos parâmetros de iluminação
void DefineIluminacao (float r, float g, float b)
{
  GLfloat luzAmbiente[4]={r,g,b,1.0};
  GLfloat luzDifusa[4]={r,g,b,1.0};       // "cor"
  GLfloat luzEspecular[4]={r, g, b, 1.0};// "brilho" 1.0
  GLfloat posicaoLuz[4]={0.0, 0.0, 0.0, 1.0};//0.0, 0.0, 0.0, 1.0
// Capacidade de brilho do material
  GLfloat especularidade[4]={1.0,1.0,1.0,1.0};//1.0,1.0,1.0,1.0
  GLint especMaterial = 10;
  glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,especMaterial);
// Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
  glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
  glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
// Habilita o modelo de colorização de Gouraud
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

//Rastro da Orbita
void rastroOrbita(void){
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distancia, 100.0, 100.0);
	glutWireTorus(0.001, ven.distancia, 100.0, 100.0);
	glutWireTorus(0.001, ter.distancia, 100.0, 100.0);
	glutWireTorus(0.001, mar.distancia, 100.0, 100.0);
	glutWireTorus(0.001, jup.distancia, 100.0, 100.0);
	glutWireTorus(0.001, sat.distancia, 100.0, 100.0);
	glutWireTorus(0.001, ura.distancia, 100.0, 100.0);
	glutWireTorus(0.001, nep.distancia, 100.0, 100.0);
	glutWireTorus(0.001, plu.distancia, 100.0, 100.0);
	glPopMatrix();
}

//Desenha a Capa/Logo
void desenhaLogo(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, logTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-100, -100, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(100, -100, -100);
	glTexCoord2f(1.0, 1.0); glVertex3f(100, 100, -100);
	glTexCoord2f(0.0, 1.0); glVertex3f(-100, 100, -100);
	glEnd();

	glutSwapBuffers();
}

//Desenha a Cena
void desenhaCena(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (trocaCamera == 0)gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (trocaCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (trocaCamera == 2)gluLookAt(0.0, zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (orbitaGrandeAtiva == 1) rastroOrbita();

	GLUquadric *quadric;
	quadric = gluNewQuadric();

	//Sol
	glPushMatrix();
	glRotatef(sol.orbita, 0.0, 1.0, 0.0);
	glTranslatef(sol.distancia, 0.0, 0.0);

	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 0.0, 0.0);//-1.2, 7.0, 0.0
		glColor3ub(255, 255, 255);//255, 255, 255
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Sol");

	}
	glPushMatrix();
	glRotatef(sol.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(sol.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, solTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sol.raio, 20.0, 20.0);
    DefineIluminacao(0.7, 0.7, 0.7);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Mercurio
	glPushMatrix();
	glRotatef(mer.orbita, 0.0, 1.0, 0.0);
	glTranslatef(mer.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Mercurio");
	}
	glPushMatrix();
	glRotatef(mer.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(mer.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Venus
	glPushMatrix();
	glRotatef(ven.orbita, 0.0, 1.0, 0.0);
	glTranslatef(ven.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Venus");
	}
	glPushMatrix();
	glRotatef(ven.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(ven.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ven.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//Terra, orbita, Lua
	glPushMatrix();
	glRotatef(ter.orbita, 0.0, 1.0, 0.0);
	glTranslatef(ter.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Terra");
	}
	glPushMatrix();
	glRotatef(ter.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(ter.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, terTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ter.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		lun.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		lun.desenhaLua();
	}
	glPopMatrix();

	//Marte, orbitas, Luas
	glPushMatrix();
	glRotatef(mar.orbita, 0.0, 1.0, 0.0);
	glTranslatef(mar.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Marte");
	}
	glPushMatrix();
	glRotatef(mar.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(mar.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mar.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		pho.desenhaOrbitaPequena();
		dei.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		pho.desenhaLua();
		dei.desenhaLua();
	}
	glPopMatrix();

	//Jupiter, orbitas, Luas
	glPushMatrix();
	glRotatef(jup.orbita, 0.0, 1.0, 0.0);
	glTranslatef(jup.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Jupiter");
	}
	glPushMatrix();
	glRotatef(jup.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(jup.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, jup.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		eur.desenhaOrbitaPequena();
		gan.desenhaOrbitaPequena();
		cal.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		eur.desenhaLua();
		gan.desenhaLua();
		cal.desenhaLua();
	}
	glPopMatrix();

	//Saturno, orbita, Lua
	glPushMatrix();
	glRotatef(sat.orbita, 0.0, 1.0, 0.0);
	glTranslatef(sat.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Saturno");
	}
	glPushMatrix();
	glRotatef(sat.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(sat.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sat.raio, 20.0, 20.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 30.0, 30.0);
	glutWireTorus(0.4, 5.0, 30.0, 30.0);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		tit.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		tit.desenhaLua();
	}
	glPopMatrix();

	glColor3ub(255, 255, 255);

	//Urano, orbita, Lua
	glPushMatrix();
	glRotatef(ura.orbita, 0.0, 1.0, 0.0);
	glTranslatef(ura.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Urano");
	}
	glPushMatrix();
	glRotatef(ura.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(ura.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ura.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		puc.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		puc.desenhaLua();
	}
	glPopMatrix();

	//Netuno, orbita, Lua
	glPushMatrix();
	glRotatef(nep.orbita, 0.0, 1.0, 0.0);
	glTranslatef(nep.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Netuno");
	}
	glPushMatrix();
	glRotatef(nep.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(nep.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, nep.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		tri.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		tri.desenhaLua();
	}
	glPopMatrix();

	//Plutao, orbita, Lua
	glPushMatrix();
	glRotatef(plu.orbita, 0.0, 1.0, 0.0);
	glTranslatef(plu.distancia, 0.0, 0.0);
	if (rotuloAtivo == 1){
		glRasterPos3f(0.0, 3.0, 0.0);
		glColor3ub(255, 255, 255);
		writeBitmapString(GLUT_BITMAP_HELVETICA_12, "Plutao");
	}
	glPushMatrix();
	glRotatef(plu.InclinacaoEixo, 1.0, 0.0, 0.0);
	glRotatef(plu.EixoAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pluTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, plu.raio, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (orbitaPequenaAtiva == 1){
		nix.desenhaOrbitaPequena();
	}
	if (luaAtiva == 1){
		nix.desenhaLua();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, estTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, estTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}

//Desenha Capa/Logo depois a Cena
void desenhaCenasEmOrdem(void){
	if (logoCena == 1){
		desenhaLogo();
	}
	else {
		desenhaCena();
	}
}

void resize(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

//Animação
void animar(int n){
	if (animado){
        sol.orbita += sol.orbitaVelocidade;
		mer.orbita += mer.orbitaVelocidade;
		ven.orbita += ven.orbitaVelocidade;
		ter.orbita += ter.orbitaVelocidade;
		mar.orbita += mar.orbitaVelocidade;
		jup.orbita += jup.orbitaVelocidade;
		sat.orbita += sat.orbitaVelocidade;
		ura.orbita += ura.orbitaVelocidade;
		nep.orbita += nep.orbitaVelocidade;
		plu.orbita += plu.orbitaVelocidade;
		lun.orbita += lun.orbitaVelocidade;
		pho.orbita += pho.orbitaVelocidade;
		dei.orbita += dei.orbitaVelocidade;
		eur.orbita += eur.orbitaVelocidade;
		gan.orbita += gan.orbitaVelocidade;
		cal.orbita += cal.orbitaVelocidade;
		tit.orbita += tit.orbitaVelocidade;
		nix.orbita += nix.orbitaVelocidade;
		puc.orbita += puc.orbitaVelocidade;
		tri.orbita += tri.orbitaVelocidade;
		if (sol,mer, ven, ter, mar, jup, sat, ura, nep, plu, lun, pho, dei, eur, gan, cal, tit, nix, puc, tri.orbita > 360.0){
			sol,mer, ven, ter, mar, jup, sat, ura, nep, plu, lun, pho, dei, eur, gan, cal, tit, nix, puc, tri.orbita -= 360.0;
		}
		sol.EixoAni += 0.5;//sol
		mer.EixoAni += 10.0;
		ven.EixoAni += 10.0;
		ter.EixoAni += 10.0;
		mar.EixoAni += 10.0;
		jup.EixoAni += 10.0;
		sat.EixoAni += 10.0;
		ura.EixoAni += 10.0;
		nep.EixoAni += 10.0;
		plu.EixoAni += 10.0;
		if (sol,mer, ven, ter, mar, jup, sat, ura, nep, plu.EixoAni > 360.0){
			sol,mer, ven, ter, mar, jup, sat, ura, nep, plu.EixoAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animar, 1);
	}
}

//Controles do Mouse
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoCena) logoCena = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
	glutPostRedisplay();
}

void mouseWheel(int wheel, int direction, int x, int y)
{
	if (direction > 0 && zoom < 100) zoom++;
	if (direction < 0 && zoom > -75) zoom--;
	glutPostRedisplay();
}

//Controles do Teclado
void keyInput(unsigned char key, int x, int y){
	switch (key){
	case 27: exit(0); break;
	case ' ': if (animado) animado = 0; else{ animado = 1; animar(1); } break;
	case 'o': if (orbitaPequenaAtiva) orbitaPequenaAtiva = 0; else orbitaPequenaAtiva = 1; glutPostRedisplay(); break;
	case 'O': if (orbitaGrandeAtiva) orbitaGrandeAtiva = 0; else orbitaGrandeAtiva = 1; glutPostRedisplay(); break;
	case 'm': if (luaAtiva) luaAtiva = 0; else luaAtiva = 1; glutPostRedisplay(); break;
	case 'M': if (luaAtiva) luaAtiva = 0; else luaAtiva = 1; glutPostRedisplay(); break;
	case 'l': if (rotuloAtivo) rotuloAtivo = 0; else rotuloAtivo = 1; glutPostRedisplay(); break;
	case 'L': if (rotuloAtivo) rotuloAtivo = 0; else rotuloAtivo = 1; glutPostRedisplay(); break;
	case '1': trocaCamera = 0; glutPostRedisplay(); break;
	case '2': trocaCamera = 1; glutPostRedisplay(); break;
	case '3': trocaCamera = 2; glutPostRedisplay(); break;
	}
}

//Intruções para o Prompt
void intructions(void){
	cout << "Espaço para Iniciar/Pausar a Simulacao." << endl;
	cout << "ESC para Sair da Simulacao." << endl;
	cout << "O para Mostrar / Esconder as Orbitas maiores." << endl;
	cout << "o para Mostrar / Esconder as Orbitas menores." << endl;
	cout << "M/m para Mostrar / Esconder as Luas." << endl;
	cout << "L/l para Mostrar / Esconder os Rotulos" << endl;
	cout << "1, 2 and 3  Para Trocar os angulos de camera." << endl;
	cout << "Scroll para dar Zoom." << endl;
}

int main(int argc, char **argv){
	intructions();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("Solar System");
	glutDisplayFunc(desenhaCenasEmOrdem);
	glutReshapeFunc(resize);
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWheel);


	setupTex();
	DefineIluminacao(0.7, 0.7, 0.7);
	glutMainLoop();
}
