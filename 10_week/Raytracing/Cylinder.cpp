#include "Cylinder.h"
#include <math.h>

float Cylinder::intersect(glm::vec3 pos, glm::vec3 dir){
    glm::vec3 d = pos - center;
    
    // using Ray-Cylinder Intersection Equations (split) w/ t factored out and used later.
    float a = pow(dir.x, 2) + pow(dir.z, 2);
    float b = 2 * ( (dir.x * d.x) + (dir.z * d.z) );
    float c =  pow(d.x, 2) + pow(d.z, 2) - pow(radius, 2);
    
    float delta = pow(b,2) - 4*a*c; // comes from general formula for ax^2+bx+c=0
      
	if(fabs(delta) < 0.001) return -1.0; 
    if(delta < 0.0) return -1.0;

    float t1 = -b - sqrt(delta) / (2*a); // 2*a 
    float t2 = -b + sqrt(delta) / (2*a); // comes from general formula
    if (t1 < 0.01 ) t1 = -1.0;
    if (t2 < 0.01 ) t2 = -1.0;
    if(fabs(t2) < 0.001 ) t2 = -1.0;
    
    float tLil, tBig;
    
    if (t1>t2){
		tLil = t2;
		tBig = t1;
	} else {
		tLil = t1;
		tBig = t2;
	}
	
	// Second Condition of 0 <= (y-yc) <= h
	float yPos = pos.y + dir.y * tLil;
	if ( (yPos >= center.y) && (yPos <= center.y + height) ){
		return tLil;
	} else {
		yPos = pos.y + dir.y * tBig;
		if (yPos >= center.y && yPos <= center.y + height) return tBig;
		else return -1.0;
	}
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p){
    glm::vec3 d = p - center;
    glm::vec3 n = glm::vec3(d.x, 0, d.z); 
    return glm::normalize(n);
}
