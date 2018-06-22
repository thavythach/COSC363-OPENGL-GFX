/*========================================================================
* COSC 363  Computer Graphics (2018)
* Ray tracer 
* See Lab07.pdf for details.
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/glut.h>
#include "Plane.h"
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene


//---The most important function in a ray tracedr! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);
	glm::vec3 light(10, 40, -3);
	glm::vec3 ambientCol(0.2);   //Ambient color of light
	
    ray.closestPt(sceneObjects);		//Compute the closest point of intersetion of objects with the ray

    if(ray.xindex == -1) return backgroundCol;      //If there is no intersection return background colour

    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor(); //else return object's colour
    glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
    glm::vec3 lightVector = glm::normalize(light - ray.xpt);
    float lDotn = glm::dot(lightVector, normalVector); 

    glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
    float rDotv = glm::dot(reflVector, -ray.dir);

    glm::vec3 oneVec(1,1,1); 
    glm::vec3 zeroVec(0,0,0); 
    float specularTerm = pow(rDotv, 10);
	//float ambientTerm = 0.2f; if we want to use 

    // let's create some dank shadows
    Ray shadow(ray.xpt, lightVector);
	shadow.closestPt(sceneObjects);
	float lightDist = glm::distance(shadow.pt, light);
	
	 glm::vec3 colorSum(0);

    glm::vec3 specularRef = zeroVec;  
    if(rDotv >= 0) {
        specularRef = specularTerm * oneVec;  
    }
    
    if(lDotn == 0 || (shadow.xindex > -1 &&  (shadow.xdist < lightDist))) { //is in shadow
        colorSum = ambientCol * materialCol;
    }
    else if(lDotn < 0) { //not in shadow
        colorSum = ((ambientCol * materialCol) + specularRef);
    }
    else {
        colorSum = (ambientCol * materialCol) + (lDotn*materialCol) + specularRef;
    }
    
    if(ray.xindex == 0 && step < MAX_STEPS)
	{
       glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
       Ray reflectedRay(ray.xpt, reflectedDir);
       glm::vec3 reflectedCol = trace(reflectedRay, step+1);  //Recursion!
       colorSum = colorSum + (0.8f*reflectedCol);
	}
   
    return(colorSum);
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height

	glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a quad.

	for(int i = 0; i < NUMDIV; i++)  	//For each grid point xp, yp
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

		    glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray

		    Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
			ray.normalize();				//Normalize the direction of the ray to a unit vector
		    glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);

	//-- Create a pointer to a sphere object
	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -5.0, -90.0), 15.0, glm::vec3(0, 0, 1));
	Sphere *sphere2 = new Sphere(glm::vec3(10.0,7.0, -60.0), 5.0, glm::vec3(1, 0, 0));
	Sphere *sphere3 = new Sphere(glm::vec3(6.0, -10.0, -50.0), 2.0, glm::vec3(0, 1, 0));

	Plane *plane = new Plane (
		glm::vec3(-20., -20, -40), // PT A
		glm::vec3(20., -20, -40), // PT B
		glm::vec3(20., -20, -200), // PT C
		glm::vec3(-20., -20, -200), // PT D
		glm::vec3(0.5, 0.5, 0) // COLOUR
	);

	//--Add the above to the list of scene objects.
	sceneObjects.push_back(sphere1); 
	sceneObjects.push_back(sphere2);
	sceneObjects.push_back(sphere3); 
	sceneObjects.push_back(plane);
}



int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
