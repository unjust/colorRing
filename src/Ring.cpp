/*  Ring.cpp * PA4 */

#include "Ring.h"

Ring * RINGS[NUMBER_OF_RINGS];

float Ring::baseRadius = 0;
float Ring::orbitRadius = 100.;

Ring::Ring(){

	alive = false;
    
    x = 0;
    y = 0;
    z = 0;
    
    endX = 0;
    endY = 0;
    endZ = 0;
    
    setRadius(0.);
    setBaseRadius(10.);
    setFinalRadius(100.);
    
    setColor(0, .529, .878, 1);
    
    acceleration = 1.;
    spread = 0;
    hasFill = false;
}

void Ring::setOrbitRadius(float r){
    Ring::orbitRadius = r;
}

void Ring::setBaseRadius(float r){
    Ring::baseRadius = r;
}

void Ring::setRadius(float r){
    currentRadius = r;
}

void Ring::setFinalRadius(float r){
    finalRadius = r;
    alive = true; //turned on
}

float Ring::getFinalRadius() {
    return finalRadius;
}

void Ring::setPosition(float ix, float iy, float iz){
    x = ix;
    y = iy;
    z = iz;
}

void Ring::setEndPosition(float ix, float iy, float iz){
    endX = ix;
    endY = iy;
    endZ = iz;
}

void Ring::setZ(float iz){
    z = iz;
}

void Ring::setColor(float r, float g, float b, float a){
    R = r;
    G = g;
    B = b;
    A = a;
}

void Ring::setAcceleration(float a) {
    acceleration = a;
    //printf("acceleration %f\n", a);
}

void Ring::move(){
    
	float startX = x;
	float startY = y;
	float startZ = z;
	
     x += (.01 + acceleration) * (endX - startX);
     y += (.01 + acceleration) * (endY - startY);
	//z += (.001 + acceleration/10.) * (endZ - startZ);

    //    printf("x = %f y = %f \n", x, y );
    setAcceleration(acceleration * 0.9);
}

void Ring::setSpread(float spreadAmount){
    
    spread = spreadAmount * 20;
    // printf("w %f", spread);
    
}

void Ring::resetRadius() {
    currentRadius = 0;
    finalRadius = 0;
}

void Ring::draw(float iacceleration){
    
    // acceleration = iacceleration;
    GLint circle_points = 100;

    if (currentRadius < finalRadius) {
        move();
        
        float r = currentRadius + Ring::baseRadius; // factor in a base
   
        if (hasFill) {
            glColor4f(R, G, B, .2);
            glBegin(GL_POLYGON);
        } else {
            glColor4f(R, G, B, A);
            glBegin(GL_LINE_LOOP);
        }
        
        for (int i = 0; i < circle_points; i++) {
            float angle = 2 * PI * i/circle_points;
            glVertex3f(x + cos(angle) * r, y + sin(angle) * r, 0);
        }
        
        // printf("%f, %f, %f, %f < %f \n", x, y, z, currentRadius, finalRadius);
        glEnd();
        
        currentRadius += .1;
        
        // alpha was causing problems
        // if (A > 0.) A -= .001;
        
    } else {
        currentRadius = 0.0;
        A = 1.0;
    }
}


bool Ring::status(){
	return alive;
}

bool Ring::isReady(){
	return ready;
}
