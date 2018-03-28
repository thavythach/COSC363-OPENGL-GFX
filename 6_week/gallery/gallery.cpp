//===================================================================
//  COSC363 Computer Graphics (2018). University of Canterbury.
//
//  Tower.cpp
//  Created by Thavy Thach 
//===================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include "loadTGA.h"
#include <vector>
using namespace std;

#define GL_CLAMP_TO_EDGE 0x812F   //To get rid of seams between textures
float lookAngle = 0.0;		//Camera rotation
float xeye =0.0, zeye=0.0;
float cdr=3.14159265/180.0;	//Conversion from degrees to radians
float xlook = -100.0*sin(lookAngle*cdr);
float zlook = -100.0*cos(lookAngle*cdr);
float skyboxLen = 1600.0f;

float rSnowman = 1.0f, gSnowman = 1.0f, bSnowman = 1.0f;
int sTick = 0;

int shTick = 0;
float theta = 0;
float shWalk = -390;
float shHead = 3;

float lightVal = 1.0f;
int lTick = 0;

float wMoleculeZ0 = -4;
float wMoleculeZ1 = -6;
float wMoleculeZ2 = -2;
float wMoleculeZ3 = 0;

float vaseAngle = 0.0;
const int N = 50;  // Total number of vertices on the base curve
float vx[N] = {0, 8, 8, 7.5, 6.7, 5, 5.5, 4, 4, 5, 5.6, 6.1, 6.8, 7.1, 7.5, 8, 8.4, 8.7, 9, 9.3, 
	          9.8, 10, 10.2, 10.4, 10.6, 10.9, 11, 11.1, 11.2, 11.3, 11.4, 11.3, 11.2, 11.1, 11, 10.5, 9.5, 8.2, 7, 6.2,
			  6, 6.2, 6.8, 7.6, 8.5, 7, 6.1, 5.3, 4.7, 4.5};
float vy[N] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	           19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			   39, 40, 41, 42, 43, 43, 42, 41, 40, 39};
float vz[N] = {0};

GLuint texId[9];

void animationTimer(int value){
	
	// snowman
	if (rSnowman <= 0.0f) sTick = 1;
	else if (rSnowman >= 1.0f)sTick = 0;
	
	if (sTick == 0){
		rSnowman -= 0.01f;
		gSnowman -= 0.50f;
		bSnowman -= 0.51f;
	} else {
		rSnowman += 0.01f;
		gSnowman += 0.01f;
		bSnowman += 0.01f;
	}
	
	// sheep
	if (shTick == 0){
		theta ++;
		shWalk -= 10;
		shHead = -3;
		if (theta == 15) shTick = 1;
	} else {
		theta --;
		shWalk += 10;
		shHead = 3;
		if (theta == -15) shTick = 0;
	}
	
	if (lightVal <= 0.0f) lTick = 1;
	else if (lightVal >= 1.0f) lTick = 0;
	
	if (lTick == 0){
		lightVal -= 0.01f;
	} else {
		lightVal += 0.01f;
	}
	
	// water molecules
	int wMax = 5;
	if (wMoleculeZ0 > wMax) wMoleculeZ0 = -4;
	else if (wMoleculeZ1 > wMax) wMoleculeZ1 = -6;
	else if (wMoleculeZ2 > wMax) wMoleculeZ2 = -2;
	else if (wMoleculeZ3 > wMax) wMoleculeZ3 = 0;
	
	wMoleculeZ0 += 0.2;
	wMoleculeZ1 += 0.2;
	wMoleculeZ2 += 0.2;
	wMoleculeZ3 += 0.2;
	
	glutPostRedisplay();
	glutTimerFunc(50, animationTimer, 0);
}

// adapted from vase.cpp, new version for reverse vectors is given below
void normal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(-nx, -ny, -nz);
}

void loadGLTextures()				// Load bitmaps And Convert To Textures
{
	
	vector <string> tgaTextures;
 
    // Initialize vector with strings using push_back 
    // command
	tgaTextures.push_back("alpha-island_lf.tga");
	tgaTextures.push_back("alpha-island_bk.tga");
	tgaTextures.push_back("alpha-island_rt.tga");
	tgaTextures.push_back("alpha-island_ft.tga");
	tgaTextures.push_back("alpha-island_up.tga");
	tgaTextures.push_back("alpha-island_dn.tga");
	tgaTextures.push_back("brick_tx1.tga");
	tgaTextures.push_back("brick_tx2.tga");
	tgaTextures.push_back("brick_tx3.tga");

	glGenTextures(9, texId); 		// Create texture ids
	
	// Robust way to load and bind textures
	for (int i=0; i<tgaTextures.size(); i++){
		glBindTexture(GL_TEXTURE_2D, texId[i]);
		if (i <= 5){
			loadTGA(tgaTextures[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		if (i < 9){
			loadTGA(tgaTextures[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
		}
	}

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//========================================================================================

void halfPyramid(){
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId[6]);
	
	glBegin(GL_TRIANGLES);
		// Right
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 1000.0f, 0.0f);
		glTexCoord2f(1.0, 1.0); glVertex3f(1000.0f, -1000.0f, 1000.0f);
		glTexCoord2f(0.0, 1.0); glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glEnd();  
	
	glBegin(GL_TRIANGLES);

		// Back
		glTexCoord2f(1.0, 0.0);	glVertex3f(0.0f, 1000.0f, 0.0f);
		glTexCoord2f(1.0, 1.0); glVertex3f(1000.0f, -1000.0f, -1000.0f);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glEnd();  

	glBegin(GL_TRIANGLES);
		// Left
	glTexCoord2f(1.0, 0.0);	glVertex3f( 0.0f, 1000.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);	glVertex3f(-1000.0f,-1000.0f,-1000.0f);
	glTexCoord2f(0.0, 1.0);	glVertex3f(-1000.0f,-1000.0f, 1000.0f);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

void building(){
	
	glTranslatef(-1000, 250,-2000);
	glScalef(0.6f,0.9f,0.4f);
	
	glColor4f(0.3, 0.5, 0.5, 1.0); // bottom house color
	
	glPushMatrix();
		glTranslatef(0,1350,0);
		halfPyramid();
	glPopMatrix();
	
	float otherSide = 9600;
	
	glPushMatrix();
		glTranslatef(0,1350,0);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texId[7]);
		// back bottom left
		glBegin(GL_TRIANGLES);
			glTexCoord2f(1.0, 0.0); glVertex3f( 0.0f, 1000.0f, otherSide);
			glTexCoord2f(1.0, 1.0);  glVertex3f(-1000.0f, -1000.0f, 1000.0f);
			glTexCoord2f(0.0, 1.0);  glVertex3f(-1000.0f, -1000.0f, -1000.0f+otherSide);
		glEnd();
		
		// back top right
		glBegin(GL_TRIANGLES);
			glTexCoord2f(1.0, 0.0); glVertex3f( 0.0f, 1000.0f, 0.0f);
			glTexCoord2f(1.0, 1.0);  glVertex3f(-1000.0f, -1000.0f, 1000.0f);
			glTexCoord2f(0.0, 1.0); glVertex3f(0.0f, 1000.0f, otherSide);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, texId[8]);
		// front bottom left
		glBegin(GL_TRIANGLES);
			glTexCoord2f(1.0, 0.0); glVertex3f( 0.0, 1000.0f, otherSide);
			glTexCoord2f(1.0, 1.0); glVertex3f(1000.0f, -1000.0f, 1000.0f);
			glTexCoord2f(0.0, 1.0); glVertex3f(1000.0f, -1000.0f, -1000.0f+otherSide);
		glEnd();
		
		// front top right
		glBegin(GL_TRIANGLES);
			glTexCoord2f(1.0, 0.0); glVertex3f( 0.0f, 1000.0f, 0.0f);
			glTexCoord2f(1.0, 1.0); glVertex3f(1000.0f, -1000.0f, 1000.0f);
			glTexCoord2f(0.0, 1.0); glVertex3f(0.0f, 1000.0f, otherSide);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0, 1350,9600);
		glRotatef(180.0f,0,1,0);
		halfPyramid();
	glPopMatrix();
	
	glColor3f(0.5,0.5, 0.5);
	// right house, right
	glPushMatrix();
		glTranslatef(0,0,-1000);
		glScalef(2.0f,0.7f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	// left house, left
	glPushMatrix();
		glTranslatef(0,0,1000+otherSide);
		glScalef(2.0f,0.7f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	// right house, back
	glPushMatrix();
		glTranslatef(-1000,0,0);
		glRotatef(90.0f,0,1,0);
		glScalef(2.0f,0.7f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	// left house, back
	glPushMatrix();
		glTranslatef(-1000,0,otherSide);
		glRotatef(90.0f,0,1,0);
		glScalef(2.0f,0.7f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	// right house, front
	glPushMatrix();
		glTranslatef(1000,0,0);
		glRotatef(90.0f,0,1,0);
		glScalef(2.0f,0.7f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	
	// left house, front
	glPushMatrix();
		glTranslatef(1000,0,otherSide);
		glRotatef(90.0f,0,1,0);
		glScalef(2.0f,0.7f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();	
	
	// right house, front
	glPushMatrix();
		glTranslatef(1100,0,1500);
		glRotatef(90.0f,0,1,0);
		glScalef(5.0f,0.9f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	// left house, front
	glPushMatrix();
		glTranslatef(1100,0,-1500+otherSide);
		glRotatef(90.0f,0,1,0);
		glScalef(5.0f,0.9f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHTING);
	
	
	// both houses, back
	glPushMatrix();
		glTranslatef(-1100,0,4800);
		glRotatef(90.0f,0,1,0);
		glScalef(10.0f,0.8f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
	
}

void snowman(){
	// Animated Object 1: Minimum 5 GLUT objects
	
	glColor3f(0,0,0); // black eyes
	// left eye
	glPushMatrix();
		glTranslatef(25,155,10);
		glutSolidDodecahedron(); 
	glPopMatrix();
	
	// right eye
	glPushMatrix();
		glTranslatef(25,155,-10);
		glutSolidDodecahedron(); 
	glPopMatrix();
	
	glColor3f(1.0f,0.5f,0.0f);// orange nose
	
	glPushMatrix();
		glTranslatef(25,145,0);
		glutSolidCone(10, 25, 25, 25); 
	glPopMatrix();
	
	glColor3f(rSnowman, gSnowman, bSnowman);
	
	// bottom
	glPushMatrix();
		glTranslatef(0,50,0);
		glScalef(30.0f,30.0f,30.0f);
		glutSolidDodecahedron(); 
	glPopMatrix();
	
	// middle
	glPushMatrix();
		glTranslatef(0,100,0);
		glScalef(25.0f,25.0f,25.0f);
		glutSolidDodecahedron(); 
	glPopMatrix();
	
	// top
	glPushMatrix();
		glTranslatef(0,150,0);
		glScalef(30.0f,30.0f,30.0f);
		glutSolidIcosahedron(); 
	glPopMatrix();
	
}

void skybox(){
	glEnable(GL_TEXTURE_2D);
	
	////////////////////// LEFT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-skyboxLen,  0, skyboxLen);
	glTexCoord2f(1.0, 0.0); glVertex3f(-skyboxLen, 0., -skyboxLen);
	glTexCoord2f(1.0, 1.0); glVertex3f(-skyboxLen, skyboxLen, -skyboxLen);
	glTexCoord2f(0.0, 1.0); glVertex3f(-skyboxLen, skyboxLen, skyboxLen);
	glEnd();

	////////////////////// FRONT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-skyboxLen,  0, -skyboxLen);
	glTexCoord2f(1.0, 0.0); glVertex3f(skyboxLen, 0., -skyboxLen);
	glTexCoord2f(1.0, 1.0); glVertex3f(skyboxLen, skyboxLen, -skyboxLen);
	glTexCoord2f(0.0, 1.0); glVertex3f(-skyboxLen,  skyboxLen, -skyboxLen);
	glEnd();

	////////////////////// RIGHT WALL ///////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(skyboxLen,  0, -skyboxLen);
	glTexCoord2f(1.0, 0.0); glVertex3f(skyboxLen, 0, skyboxLen);
	glTexCoord2f(1.0, 1.0); glVertex3f(skyboxLen, skyboxLen,  skyboxLen);
	glTexCoord2f(0.0, 1.0); glVertex3f(skyboxLen,  skyboxLen,  -skyboxLen);
	glEnd();


	////////////////////// REAR WALL ////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f( skyboxLen, 0, skyboxLen);
	glTexCoord2f(1.0, 0.0); glVertex3f(-skyboxLen, 0,  skyboxLen);
	glTexCoord2f(1.0, 1.0); glVertex3f(-skyboxLen, skyboxLen,  skyboxLen);
	glTexCoord2f(0.0, 1.0); glVertex3f( skyboxLen, skyboxLen, skyboxLen);
	glEnd();


	/////////////////////// TOP //////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0,  1.0); glVertex3f(-skyboxLen, skyboxLen, -skyboxLen);
	glTexCoord2f(0.0,  0.0); glVertex3f(skyboxLen, skyboxLen,  -skyboxLen);
	glTexCoord2f(1.0,  0.0); glVertex3f(skyboxLen, skyboxLen,  skyboxLen);
	glTexCoord2f(1.0,  1.0); glVertex3f(-skyboxLen, skyboxLen, skyboxLen);
	glEnd();

	/////////////////////// FLOOR //////////////////////////
	glBindTexture(GL_TEXTURE_2D, texId[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0,  0.0); glVertex3f(-skyboxLen, 0., skyboxLen);
	glTexCoord2f(1.0,  1.0); glVertex3f(skyboxLen, 0.,  skyboxLen);
	glTexCoord2f(0.0,  1.0); glVertex3f(skyboxLen, 0., -skyboxLen);
	glTexCoord2f(0.0,  0.0); glVertex3f(-skyboxLen, 0., -skyboxLen);
	glEnd();
}

void sheep(){
	glScalef(15.0f,15.0f,15.0f);
	
	glColor3f(0., 0., 1.);		// sheep head
	glPushMatrix();
	  glTranslatef(0, 3.0, shHead);
	  glutSolidCube(1.4);
	glPopMatrix();

	glColor3f(1., 1., 1.);			// sheep body
	glPushMatrix();
	  glTranslatef(0, 2.2, 0);
	  glRotatef(90.0,1,0,0);
	  glScalef(3, 5, 3);
	  glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//back Right leg
	glPushMatrix();
		glTranslatef(-0.8,4,0);
		glRotatef(-theta, 1, 0, 0);
		glTranslatef(0.8,-4,0);
		glTranslatef(-0.8, 0.5, 1.0);
		glScalef(1, 1.0, 1);
		glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//back Left leg
	glPushMatrix();
		glTranslatef(0.8,4,0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(-0.8,-4,0);
		glTranslatef(0.8, 0.5, 1.0);
		glScalef(1, 1.0, 1);
		glutSolidCube(1);
	glPopMatrix();
	
	glColor3f(0., 0., 1.);			//front Right leg
	glPushMatrix();
		glTranslatef(-0.8,4,0);
		glRotatef(-theta, 1, 0, 0);
		glTranslatef(0.8,-4,0);
		glTranslatef(-0.8, 0.5, -1.0);
		glScalef(1, 1.0, 1);
		glutSolidCube(1);
	glPopMatrix();

	glColor3f(0., 0., 1.);			//front Left leg
	glPushMatrix();
		glTranslatef(0.8,4,0);
		glRotatef(theta, 1, 0, 0);
		glTranslatef(-0.8,-4,0);
		glTranslatef(0.8, 0.5, -1.0);
		glScalef(1, 1.0, 1);
		glutSolidCube(1);
	glPopMatrix();
	
	
}

void firelight(){
	
	glScalef(20.0f,20.0f,20.0f);
	
	for (int i=0; i < 5; i++){
		
		if (i==3){
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT1);
			glDisable(GL_LIGHT0);	
		}
		// base
		glColor3f(0.0f,0.0f,0.0f);
		glPushMatrix();
			glTranslatef(0,i*3,0);
			glRotatef(-45,1,0,0);
			glScalef(1, 2.4, 0.5);
			glutSolidCube(1.0);
		glPopMatrix();
		
		// light	
		glColor3f(lightVal, 0.5f, 0.0f);
		
		glPushMatrix();
			glTranslatef(0,1+(i*3),-1);
			glRotatef(-45,1,0,0);
			glScalef(1.0f, 0.4f, 0.2f);
			glutSolidCube(1.0);
		glPopMatrix();
		
		if (i==3){
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHTING);
		}
	}
	
}

void water(){
	
	glScalef(20,20,20);
	
	// water lake base
	glColor3f(0,0.4f,1.0f);
	glPushMatrix();
		glTranslatef(-0.5,0,0);
		glRotatef(90,0,1,0);
		glScalef(15,0.2f,5.0f);
		glutSolidCube(1.0);
	glPopMatrix();
	
	// water molecule 0
	glColor3f(0,0.1f,1.0f);
	glPushMatrix();
		glTranslatef(-2,0.2,wMoleculeZ0);
		glRotatef(90,0,1,0);
		glScalef(1,0.1,0.7);
		glutSolidCube(1.0);
	glPopMatrix();
	
	// water molecule 1
	glColor3f(0,0.1f,1.0f);
	glPushMatrix();
		glTranslatef(-1,0.2,wMoleculeZ1);
		glRotatef(90,0,1,0);
		glScalef(1,0.1,0.7);
		glutSolidCube(1.0);
	glPopMatrix();
	
	// water molecule 2
	glColor3f(0,0.1f,1.0f);
	glPushMatrix();
		glTranslatef(0,0.2,wMoleculeZ2);
		glRotatef(90,0,1,0);
		glScalef(1,0.1,0.7);
		glutSolidCube(1.0);
	glPopMatrix();
	
	// water molecule 3
	glColor3f(0,0.1f,1.0f);
	glPushMatrix();
		glTranslatef(1,0.2,wMoleculeZ3);
		glRotatef(90,0,1,0);
		glScalef(1,0.1,0.7);
		glutSolidCube(1.0);
	glPopMatrix();
	
}

void vase(){
	//float white[4] = {1., 1., 1., 1.};
	//float black[4] = {0};
	float wx[N], wy[N], wz[N]; 
	float angStep = 10*3.1415926/180.0;  
	//glMaterialfv(GL_FRONT, GL_SPECULAR, black);  // suppress specular reflections

	glColor4f (lightVal, 0.75, 0.5, 1.0);
	glRotatef(vaseAngle,0,1,0);
	glPushMatrix();
		for (int j=0; j< 36; j++){
			glBegin(GL_TRIANGLE_STRIP);
				for (int i=0; i<N; i++){
					wx[i] = (vx[i]*cos(angStep)) + (vz[i]*sin(angStep));
					wy[i] = vy[i];
					wz[i] = (-vx[i]*sin(angStep)) + (vz[i]*cos(angStep));
					
					
					if(i > 0) normal(wx[i-1], wy[i-1], wz[i-1],vx[i-1], vy[i-1], vz[i-1], vx[i], vy[i], vz[i] );
					glTexCoord2f(j/36.0, i/(float)N); glVertex3f(vx[i], vy[i], vz[i]);
					 
					if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1],vx[i], vy[i], vz[i], wx[i], wy[i], wz[i] );
					glTexCoord2f(j/36.0, i/(float)N); glVertex3f(wx[i], wy[i], wz[i]); 
					
					vx[i] = wx[i];
					vy[i] = wy[i];
					vz[i] = wz[i];
				}
			glEnd();
		}
	glPopMatrix();
	
	//glMaterialfv(GL_FRONT, GL_SPECULAR, white);  // suppress specular reflections

}

void gallery(){
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
		glScalef(0.5,0.5,0.5);
		building();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-500,0,0);
		snowman();
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glPushMatrix();
		glTranslatef(shWalk,0,600);
		glRotatef(90.0f,0,1,0);
		sheep();
	glPopMatrix();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	
	glPushMatrix();
		glTranslatef(-203,25,130);
		glRotatef(90,0,1,0);
		firelight();
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(-203,25,-220);
		glRotatef(90,0,1,0);
		firelight();
	glPopMatrix();
	
	
	glPushMatrix();
		glTranslatef(-500,25,-500);
		water();
	glPopMatrix();
	
	glPopMatrix();
		glTranslatef(-500,25,-750);
		vase();
	glPopMatrix();	
}

//---------------------------------------------------------------------
void initialise(void) 
{
    loadGLTextures();

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	
	// light 0
	float ambient[4] = {0.2, 0.2, 0.2, 1.0};
	float white[4]   = {1.0, 1.0, 1.0, 1.0};
	float mat_col[4] = {1.0, 1.0, 0.0, 1.0};     //Yellow
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_col);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50); 
	
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_col);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 125); 
	

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(70.0, 1.0, 100.0, 5000.0);   //Perspective projection
}

//---------------------------------------------------------------------
void display(void)
{
	
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt (xeye, 100, zeye, xlook, 100, zlook, 0, 1, 0);  //camera rotation
	
   float lgt_pos1[] = {-390, 30, 600.0f, 1.0f};  //light0 position (directly above the origin)
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos1);   //light position
   
   float lgt_pos2[] = {-210, 100, -220, 1.0f};  //light1 position (directly above the origin)
   glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos1);   //light position


	glPushMatrix();
		skybox();
	glPopMatrix();
	
	glPushMatrix();
		gallery();
	glPopMatrix();
	
	glFlush();
	
}


//--------------------------------------------------------------
 void special(int key, int x, int y)
 {
    if(key==GLUT_KEY_RIGHT ) lookAngle+=5;		 //Turn left
    else if(key==GLUT_KEY_LEFT) lookAngle-=5;   //Turn right
    else if(key == GLUT_KEY_DOWN)
	{  //Move backward
		xeye -= 50*sin(lookAngle*cdr);
		zeye += 50*cos(lookAngle*cdr);
	}
	else if(key == GLUT_KEY_UP)
	{ //Move forward
		xeye += 50*sin(lookAngle*cdr);
		zeye -= 50*cos(lookAngle*cdr);
	}
    
	xlook = xeye + 100*sin(lookAngle*cdr);
	zlook = zeye - 100*cos(lookAngle*cdr);
	glutPostRedisplay();
}
//-------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH );
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (50, 50);

   glutCreateWindow ("3D Model Gallery Made By Thavy Thach");
   initialise();
   glutDisplayFunc(display); 
   glutSpecialFunc(special);
   glutTimerFunc(50, animationTimer, 0);
 
   glutMainLoop();
   return 0;
}
