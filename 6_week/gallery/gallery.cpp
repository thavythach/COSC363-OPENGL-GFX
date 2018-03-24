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

GLuint texId[6];

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

void skybox(){
	glEnable(GL_TEXTURE_2D);

  ////////////////////// LEFT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[0]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1000,  0, 1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1000, 0., -1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000., -1000);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1000, 1000, 1000);
  glEnd();

  ////////////////////// FRONT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(-1000,  0, -1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(1000, 0., -1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(1000, 1000, -1000);
  glTexCoord2f(0.0, 1.0); glVertex3f(-1000,  1000, -1000);
  glEnd();

 ////////////////////// RIGHT WALL ///////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[2]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f(1000,  0, -1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(1000, 0, 1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0); glVertex3f(1000,  1000,  -1000);
  glEnd();


  ////////////////////// REAR WALL ////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[3]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0); glVertex3f( 1000, 0, 1000);
  glTexCoord2f(1.0, 0.0); glVertex3f(-1000, 0,  1000);
  glTexCoord2f(1.0, 1.0); glVertex3f(-1000, 1000,  1000);
  glTexCoord2f(0.0, 1.0); glVertex3f( 1000, 1000, 1000);
  glEnd();
  
  
  /////////////////////// TOP //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[4]);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,  1.0); glVertex3f(-1000, 1000, -1000);
  glTexCoord2f(0.0,  0.0); glVertex3f(1000, 1000,  -1000);
  glTexCoord2f(1.0,  0.0); glVertex3f(1000, 1000,  1000);
  glTexCoord2f(1.0,  1.0); glVertex3f(-1000, 1000, 1000);
  glEnd();
  
  /////////////////////// FLOOR //////////////////////////
  glBindTexture(GL_TEXTURE_2D, texId[5]);
  glBegin(GL_QUADS);
  glTexCoord2f(1.0,  0.0); glVertex3f(-1000, 0., 1000);
  glTexCoord2f(1.0,  1.0); glVertex3f(1000, 0.,  1000);
  glTexCoord2f(0.0,  1.0); glVertex3f(1000, 0., -1000);
  glTexCoord2f(0.0,  0.0); glVertex3f(-1000, 0., -1000);
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
	float xlook, zlook;
	float cdr=3.14159265/180.0;	//Conversion from degrees to radians
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	xlook = -100.0*sin(lookAngle*cdr);
	zlook = -100.0*cos(lookAngle*cdr);
	gluLookAt (0, 500, 0, xlook, 500, zlook, 0, 1, 0);  //camera rotation

	skybox();

	glFlush();
}

//--------------------------------------------------------------
 void special(int key, int x, int y)
 {
    if(key==GLUT_KEY_LEFT) lookAngle+=5;		 //Turn left
    else if(key==GLUT_KEY_RIGHT) lookAngle-=5;   //Turn right

	glutPostRedisplay();
}
//-------------------------------------------------------------------

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH );
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (50, 50);

   glutCreateWindow ("Sky Box");
   initialise();
   glutDisplayFunc(display); 
   glutSpecialFunc(special);
 
   glutMainLoop();
   return 0;
}
