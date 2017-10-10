/* ring.h *  PA4 *  Created by ivy on 4/05/06. */

#ifndef RINGS_
#define RINGS_


#define NUMBER_OF_RINGS 10 // SUBBANDS
//#define RINGS_POINTS WINDOW_SIZE/2

#include "ofMain.h"
#include <stdio.h>

enum ringAnimationType {
    line = 0,
    orbital = 1,
    tangental = 2
};

class Ring {
	
	private:

   
    float R, G, B, A;
    
    float finalRadius; // TODO deltaRadius instead
    float iorbitRadius;

    float acceleration;
    
    bool alive;
    bool ready;
	
	public:
    
    float x, y, z;
    float endX, endY, endZ;
    
    Ring();
    
    static float baseRadius;  // added to currentRadius
    static float orbitRadius;

    int ident;
    bool bounce;

    float currentRadius;

    bool hasFill;

    void setColor(float r, float g, float b, float a);
    
    void setRadius(float r);
    void setFinalRadius(float r);
    float getFinalRadius();

    static void setBaseRadius(float r);
    static void setOrbitRadius(float r);

    float spread;
    void setSpread(float spreadAmount);

    void setPosition(float ix, float iy, float iz);
    void setEndPosition(float ix, float iy, float iz);

    void setAcceleration(float a);
    void setZ(float iz);

    void move();
    void position();
    void draw(float iacceleration = 0);

    bool isReady();
    bool status();

    void resetRadius();
    //~Ring();
	
};


//we'll be getting half as many frequencies as the window

#endif
