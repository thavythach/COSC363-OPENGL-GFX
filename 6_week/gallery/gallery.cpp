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

GLuint texId[6];

void animationTimer(int value){
	
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
	
	glutPostRedisplay();
	glutTimerFunc(50, animationTimer, 0);
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
    
	glGenTextures(6, texId); 		// Create texture ids
	
	// Robust way to load and bind textures
	for (int i=0; i<tgaTextures.size(); i++){
		glBindTexture(GL_TEXTURE_2D, texId[i]);
		loadTGA(tgaTextures[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	}

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

//========================================================================================

void halfPyramid(){
	
	glBegin(GL_TRIANGLES);
		// Right
		glColor3f(178.0f,0.0f,0.0f);     // Red
		glVertex3f(0.0f, 1000.0f, 0.0f);
		glColor3f(0.1f,0.1f,0.1f);     // Red
		glVertex3f(1000.0f, -1000.0f, 1000.0f);
		glColor3f(178.0f,0.0f,0.0f);     // Red
		glVertex3f(1000.0f, -1000.0f, -1000.0f);
	glEnd();  
	
	glBegin(GL_TRIANGLES);

		// Back
		glColor3f(178.0f,0.0f,0.0f);     // Red
		glVertex3f(0.0f, 1000.0f, 0.0f);
		glColor3f(0.1f,0.1f,0.1f);     // Red
		glVertex3f(1000.0f, -1000.0f, -1000.0f);
		glColor3f(178.0f,0.0f,0.0f);     // Red
		glVertex3f(-1000.0f, -1000.0f, -1000.0f);
	glEnd();  

	glBegin(GL_TRIANGLES);
		// Left
		glColor3f(178.0f,0.0f,0.0f);     // Red
		glVertex3f( 0.0f, 1000.0f, 0.0f);
		glColor3f(0.1f,0.1f,0.1f);     // Red
		glVertex3f(-1000.0f,-1000.0f,-1000.0f);
		glColor3f(178.0f,0.0f,0.0f);     // Red
		glVertex3f(-1000.0f,-1000.0f, 1000.0f);
	glEnd();
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
		
		// back bottom left
		glBegin(GL_TRIANGLES);
			glColor4f(0.5, 0.5, 0.5, 1.0); glVertex3f( 0.0f, 1000.0f, otherSide);
			glColor4f(1.0, 0.0, 0.0, 1.0); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
			glColor4f(0.0, 0.0, 1.0, 1.0); glVertex3f(-1000.0f, -1000.0f, -1000.0f+otherSide);
		glEnd();
		
		
		// back top right
		glBegin(GL_TRIANGLES);
			glColor4f(0.5, 0.5, 0.5, 1.0); glVertex3f( 0.0f, 1000.0f, 0.0f);
			glColor4f(1.0, 0.0, 0.0, 1.0); glVertex3f(-1000.0f, -1000.0f, 1000.0f);
			glColor4f(0.0, 0.0, 1.0, 1.0); glVertex3f(0.0f, 1000.0f, otherSide);
		glEnd();
		
		
		// front bottom left
		glBegin(GL_TRIANGLES);
			glColor4f(0.5, 0.5, 0.5, 1.0); glVertex3f( 0.0, 1000.0f, otherSide);
			glColor4f(1.0, 0.0, 0.0, 1.0); glVertex3f(1000.0f, -1000.0f, 1000.0f);
			glColor4f(0.0, 0.0, 1.0, 1.0); glVertex3f(1000.0f, -1000.0f, -1000.0f+otherSide);
		glEnd();
		
		// front top right
		glBegin(GL_TRIANGLES);
			glColor4f(0.5, 0.5, 0.5, 1.0); glVertex3f( 0.0f, 1000.0f, 0.0f);
			glColor4f(1.0, 0.0, 0.0, 1.0); glVertex3f(1000.0f, -1000.0f, 1000.0f);
			glColor4f(0.0, 0.0, 1.0, 1.0); glVertex3f(0.0f, 1000.0f, otherSide);
		glEnd();
		
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0, 1350,9600);
		glRotatef(180.0f,0,1,0);
		halfPyramid();
	glPopMatrix();
	
	glColor3f(0.1f, 0.35f, 0.05f);
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
	
	// left house, front
	glPushMatrix();
		glTranslatef(1100,0,-1500+otherSide);
		glRotatef(90.0f,0,1,0);
		glScalef(5.0f,0.9f, 0.1f);
		glutSolidCube(1000.0);
	glPopMatrix();
	
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

//---------------------------------------------------------------------
void initialise(void) 
{
    loadGLTextures();

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glClearColor (0.0, 0.0, 0.0, 0.0);

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

	glPushMatrix();
		skybox();
	glPopMatrix();
	
	
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
		glScalef(0.5,0.5,0.5);
		building();
	glPopMatrix();
	
	glPushMatrix();
		snowman();
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
