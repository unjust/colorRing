//
//  Controller.h
//  ringsColor
//
//  Created by Ivy Feraco on 8/27/17.
//
//

#ifndef Controller_h
#define Controller_h

#include <stdio.h>


#include "ofMain.h"
#include "ofxEasing.h"
#include "Ring.h"
#include <stdio.h>

class Controller {
    
    private:
    
    float baseRadius;  // added to currentRadius
    float orbitRadius;
    
    float movement;
    
    int selectedIndex;
    
    float *sonicRadius;
    
    void orbitMotion(int i);
    
    ///////////////////////////////////////////////////

    public:
    
    Controller();
    
    int QUANTITY;
    
    // need this array to be dynamic suze
    Ring* rings;
    
    float counter;
    float acceleration;
    float movementX;
    float movementY;
    
    ringAnimationType animationType;
    void setAnimationType(ringAnimationType type);
    
    void initRings(int num = 0);
    void setColor();

    void  setMovement(float m);
    float getMovement();
    
    void setOrbitRadius(float r);
    void setBaseRadius(float r);
    
    void draw();
    void position();
    
    void orbit();
    void moveOne();
    
    void lineUp();
    void tangent();
    
    void setAudioData(float* soundData);
    
};



#endif /* Controller_h */
