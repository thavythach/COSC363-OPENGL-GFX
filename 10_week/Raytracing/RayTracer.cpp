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
#include "TextureBMP.h"
#include "Cylinder.h"
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
const float ETA = 1.002;

TextureBMP texture1;
TextureBMP texture2;
int tex1 = 0;

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
	float transparency = 0.1; // 90% transparent
	
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
    
    /***TEXTURES START****/
	
	/** 2D Texture for Sphere ("on a random sphere") **/
	if(ray.xindex == 1){
		// sphere is at center so...
		glm::vec3 d = glm::normalize(ray.xpt-glm::vec3(10.0,7.0, -80.0));
		float s = (0.5-atan2(d.z, d.x) + M_PI) /  (2*M_PI);
		float t = 0.5+asin(d.y)/ M_PI;
		materialCol = texture2.getColorAt(s,t);
	}
	
	/** Procedural Texture for Tetrahedron ("Yolo") **/
	if ((ray.xindex >= 12 && ray.xindex <= 16)){
		if ((int(ray.xpt.x) - int(ray.xpt.y)) % 2 == 1) materialCol = glm::vec3(0.f, 1.f, 0.f);
		else materialCol = glm::vec3(1.f, 1.f, 0.f);
	}
	
	/** 2D Texture for Plane ("Floor") **/
	if(ray.xindex == 3){
		float s,t;
		if (tex1 < 250 || tex1 > 500){
			s = (ray.xpt.x+50)/75;
			t = (ray.xpt.y+20)/75;
			materialCol = texture1.getColorAt(s,t);
			tex1 ++;

		} else {
			materialCol = glm::vec3(1.f, 0.f, 0.f);
			if (tex1 > 500) tex1 = 0;
			tex1 ++;
		}
		
	}
	
	/** Procedural Texture for Cylinder & Cube ("hohooh")  **/
	if (ray.xindex >= 4 && ray.xindex <= 10){
		float change = 0.f;
		if (ray.xindex == 10) change = 1.f;
		if ( ((int)pow(ray.xpt.x, 2))%2 == 1) materialCol = glm::vec3(0.f, 1.f, change);
		else materialCol = glm::vec3(change, 0.f, 1.f);
	}
	
	/***TEXTURES END****/
    
    /**SHADOWS**/
    if(lDotn == 0 || (shadow.xindex > -1 &&  (shadow.xdist < lightDist))){
		colorSum = ambientCol * materialCol; //is in shadow
		if (shadow.xindex == 2) colorSum += (lDotn*materialCol + specularRef) * glm::vec3(0.7)+ sceneObjects[11]->getColor()*glm::vec3(0.02);
	} else if(lDotn < 0){
		colorSum = ((ambientCol * materialCol) + specularRef); // not in shadow
		if (shadow.xindex == 2) colorSum += (lDotn*materialCol + specularRef) * glm::vec3(0.7)+ sceneObjects[11]->getColor()*glm::vec3(0.02);
    }
    else {
		colorSum = (ambientCol * materialCol) + (lDotn*materialCol) + specularRef; // misc.
    }
    
    /** reflections **/
    if( (ray.xindex == 0 || ray.xindex == 11) && step < MAX_STEPS){
       glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
       Ray reflectedRay(ray.xpt, reflectedDir);
       glm::vec3 reflectedCol = trace(reflectedRay, step+1);  //Recursion!
       colorSum = colorSum + (0.8f*reflectedCol);
	}
	
	/** refractions && transparency **/
	if (ray.xindex == 11 && step < MAX_STEPS){
		
		// establishing second ray along the direction of refraction is traced given surface is "transparent"
		glm::vec3 alphaRefractDir = glm::refract(ray.dir, normalVector, 1.0f/ETA);
		Ray alphaRay(ray.xpt, alphaRefractDir);
		alphaRay.closestPt(sceneObjects);
		if(alphaRay.xindex == -1) return backgroundCol;
		
		glm::vec3 betaNormal = sceneObjects[alphaRay.xindex]->normal(alphaRay.xpt);
		glm::vec3 betaRefractDir = glm::refract(alphaRefractDir, -betaNormal, ETA);
		Ray betaRay(alphaRay.xpt, betaRefractDir);
		betaRay.closestPt(sceneObjects);
		if(betaRay.xindex == -1) return backgroundCol;
		
		glm::vec3 refractCol = trace(betaRay, step+1); // Recursion!!!
		colorSum = colorSum * transparency + refractCol*(1-transparency); // theeee transparent object-io
	}
	
		
   
    return(colorSum);
}

/** Anti-Aliasing: Super-Sampling (1-2 Mark)**/
glm::vec3 anti_alias_trace(glm::vec3 eye, float px_size, float x, float y){
	float qtr_1_pixel = px_size * 0.25;
	float qtr_3_pixel = px_size * 0.75;
	
	glm::vec3 avg(0.25);
	glm::vec3 color(0);
	
	Ray ray;
	
	// quadrant 1
	ray = Ray(eye, glm::vec3(x + qtr_1_pixel, y + qtr_1_pixel, -EDIST));
	ray.normalize();
	color += trace(ray, 1);
	
	// quadrant 2
	ray = Ray(eye, glm::vec3(x + qtr_1_pixel, y + qtr_3_pixel, -EDIST));
	ray.normalize();
	color += trace(ray, 1);
	
	// quadrant 3
	ray = Ray(eye, glm::vec3(x + qtr_3_pixel, y + qtr_1_pixel, -EDIST));
	ray.normalize();
	color += trace(ray, 1);
	
	// quadrant
	ray = Ray(eye, glm::vec3(x + qtr_3_pixel, y + qtr_3_pixel, -EDIST));
	ray.normalize();
	color += trace(ray, 1);
	
	color *= avg;
	return color;
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

		    //Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
			//ray.normalize();				//Normalize the direction of the ray to a unit vector
		    //glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value
		    
		    // Anti-Aliasing: super-sampling
		    glm::vec3 col = anti_alias_trace(eye, cellX, xp, yp);

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

void robustCube(float x, float y, float z, float l, float w, float h, glm::vec3 color)
{   
	// generic construction
	glm::vec3 A = glm::vec3(x,y,z), B = glm::vec3(x+l,y,z);
	glm::vec3 C = glm::vec3(x+l,y+h,z), D = glm::vec3(x,y+h,z);
	glm::vec3 E = glm::vec3(x+l,y,z-w), F = glm::vec3(x+l,y+h,z-w);
	glm::vec3 G = glm::vec3(x,y+h,z-w), H = glm::vec3(x,y,z-w);
	
	// plane creation
	Plane *plane0 = new Plane(A,B,C,D,color);
	Plane *plane1 = new Plane(E,H,G,F,color);
	Plane *plane2 = new Plane(B,E,F,C,color);
	Plane *plane3 = new Plane(D,G,H,A,color);
	Plane *plane4 = new Plane(H,E,B,A,color);
	Plane *plane5 = new Plane(D,C,F,G,color);
		                           
	// added to the scene	  
	sceneObjects.push_back(plane0);                                                
	sceneObjects.push_back(plane1);
	sceneObjects.push_back(plane2);
	sceneObjects.push_back(plane3);
	sceneObjects.push_back(plane4);
	sceneObjects.push_back(plane5);
	
}

void robustTetrahedron(float x, float y, float z, float l, glm::vec3 color){
	
	// generic construction
	glm::vec3 A = glm::vec3(x,y,z), B = glm::vec3(x + l, y, z);
	glm::vec3 C = glm::vec3(x + l * 0.5, y, z + sqrt(3.0f) * 0.5 * l);
	glm::vec3 D = glm::vec3(x + l * 0.5, y + sqrt(6.0f) / 3.0f * l, z + sqrt(3.0f) * 0.25 * l );
	
	// midpoints:
	glm::vec3 CD = glm::vec3( (C.x + D.x) / 2, (C.y + D.y) / 2, (C.z + D.z) / 2); 
	glm::vec3 AB = glm::vec3( x + 1 /2, y, z );
	
	// plane creation
	Plane *tri0 = new Plane(AB,B,C,A, color);
	Plane *tri1 = new Plane(D,A,C,CD, color);
	Plane *tri2 = new Plane(A,D,D,AB, color);
	Plane *tri3 = new Plane(B,D,CD,C, color);
	
	sceneObjects.push_back(tri0);
	sceneObjects.push_back(tri1);
	sceneObjects.push_back(tri2);
	sceneObjects.push_back(tri3);
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
    
    // texture initialization
	texture1 = TextureBMP((char*)"floor.bmp");
	texture2 = TextureBMP((char*)"Earth.bmp");


	//-- Create a pointer to a sphere object
	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, -5.0, -90.0), 10.0, glm::vec3(0, 0, 1));
	Sphere *sphere2 = new Sphere(glm::vec3(10.0,7.0, -80.0), 5.0, glm::vec3(1, 0, 0));
	Sphere *sphere3 = new Sphere(glm::vec3(0.0, 5, -60.0), 2.0, glm::vec3(0, 1, 0));
	Sphere *sphere4 = new Sphere(glm::vec3(0.0, -7.0, -45.0), 3.0, glm::vec3(0, 1, 0));

	Plane *plane = new Plane (
		glm::vec3(-20., -20, -40), // PT A
		glm::vec3(20., -20, -40), // PT B
		glm::vec3(20., -20, -200), // PT C
		glm::vec3(-20., -20, -200), // PT D
		glm::vec3(0.5, 0.5, 0) // COLOUR
	);
	Cylinder *cylinder1 = new Cylinder(glm::vec3(-15, -12, -70.0), 2, 8.0, glm::vec3(1.f, 0.f, 0.f));

	//--Add the above to the list of scene objects.
	sceneObjects.push_back(sphere1); // 0
	sceneObjects.push_back(sphere2); // 1
	sceneObjects.push_back(sphere3); // 2
	sceneObjects.push_back(plane); // 3
	robustCube(-10.0, 5.0, -60.0,5,5,5,glm::vec3(1, 0.7529, 0.7961)); //4-9
	sceneObjects.push_back(cylinder1); // 10
	sceneObjects.push_back(sphere4); // 11
	robustTetrahedron(6.0, -10.0, -50.0, 5, glm::vec3(1, 1, 1)); //12-16
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
