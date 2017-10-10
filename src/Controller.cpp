//
//  Controller.cpp
//  ringsColor
//
//  Created by Ivy Feraco on 8/27/17.
//  small change to test out git
//
//

#include "Controller.h"


Controller::Controller() {
    initRings();
    
    counter = 0.;
    acceleration = 1;
}

void Controller::initRings(int num) {
    QUANTITY = (num) ? num : NUMBER_OF_RINGS; // some default
    
    // init rings
    rings = new Ring[QUANTITY];

    for (int r = 0; r < QUANTITY; r++) {
        rings[r] = Ring();
        rings[r].setPosition(0., -300 + (r * 20), 0);
        rings[r].ident = r;
        // rings[r].setColor(ofRandom(255), ofRandom(255), ofRandom(255), 100);
    }
}

void Controller::setColor() {
    int i = 0,
        total = QUANTITY;
    
    if (selectedIndex) {
        i = selectedIndex;
        total = selectedIndex + 1;
    }
    
    for (i; i < total; i++) {
        //rings[i].setColor();
    }
}

void Controller::setOrbitRadius(float r){
    Ring::orbitRadius = r;
}

void Controller::setBaseRadius(float r){
    Ring::baseRadius = r;
}

void Controller::setAnimationType(ringAnimationType type) {
    animationType = type;
    printf(" animation type here %d \n", (int)animationType );
    if (animationType == tangental) {
        for (int i = 0; i < QUANTITY; i++) {
            rings[i].resetRadius();
        }
    }
}

void Controller::setAudioData(float *soundData) {
    sonicRadius = soundData;
}

float Controller::getMovement() {
    float delta = rings[selectedIndex].endX - rings[selectedIndex].x;
    
    if (delta <= 0) {
        movement--;
    } else {
        movement++;
    }
    
    return movement;
}

void Controller::setMovement(float m) {
    movement = m;
}

void Controller::moveOne() {
    float endX, endY, endZ;
    
    selectedIndex = 0;

    for (int i = 0; i < QUANTITY; i++) {
        float input = TWO_PI * i/QUANTITY;
        
        endZ = 0.;
        
        
        if (i == selectedIndex) {
            endX = getMovement() * sin(input);
            endY = getMovement() * cos(input);
            
            rings[i].setEndPosition(endX, endY, endZ);
            
        } else {
            orbitMotion(i);
        }
        
        rings[i].setEndPosition(endX, endY, endZ);
    }
    
}

void Controller::orbit() {
    for (int i = 0; i < QUANTITY; i++) {
        orbitMotion(i);
    }
}

void Controller::orbitMotion(int i) {
    float endX, endY, endZ;
    float input = TWO_PI * i/QUANTITY + (counter * acceleration);
    
    endX = (Ring::orbitRadius) * sin(input);
    endY = (Ring::orbitRadius) * cos(input);
    endZ = 0.;
    rings[i].setEndPosition(endX, endY, endZ);
}

void Controller::lineUp() {
    float endX, endY;
    float x = 0;

    for (int i = 0; i < QUANTITY; i++) {
        endX = x;
        endY = -300 + i * 20; // copy initial position set by App for now
        rings[i].setEndPosition(endX, endY, 0.);
    }
}

void Controller::tangent() {
   
    float endX, endY, endZ;
    
    for (int i = 0; i < QUANTITY; i++) {
        endX = 0;
        endY = 0 + rings[i].getFinalRadius();
        rings[i].setEndPosition(endX, endY, 0.);
    }
}

void Controller::draw() {

    for (int r = 0; r < QUANTITY; r++) {
        if (animationType == tangental) {
            rings[r].setFinalRadius((Ring::baseRadius * r) + (sonicRadius[r] * 100.));
        } else {
            // why * 120 ?
            rings[r].setFinalRadius(Ring::baseRadius + (sonicRadius[r] * 120.));
        }
        rings[r].draw();
    }
    
    if (animationType == orbital) {
        // orbit();
        moveOne();
    } else if (animationType == tangental) {
        tangent();
    } else if (animationType == line) {
        lineUp();
    }
    
    counter += .001;
}
