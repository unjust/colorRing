#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Ring.h"
#include "Controller.h"

#define NUMBER_OF_BANDS 512

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void ringAnimButtonPressed(const void* sender);
    void setRingRadius(const void* sender, float &value);
    
    void handleCameraControls(const void* sender, float &value);
    void handleCameraReset(const void* sender, bool &value);
    
    void handleVolume(const void* sender, float &value);
    
    void audioOut( ofSoundBuffer &outBuffer );
    
    
private:
    
    Controller ringController;
    
    ofCamera camera;
    ofEasyCam easyCam;
    
    ofVec3f sceneCenter;
    
    // gui
    
    ofxPanel soundPanel;
    ofParameterGroup soundParams;
    
    ofParameter <float>volume;
    ofParameter <bool>showFFT;
    
    ofxPanel ringAttrsPanel;
    ofParameterGroup ringAttrsParams;
    
    ofParameter <bool>ringBounce;
    ofParameter <bool>showRings;
    
    ofParameter <float>ringRadius;
    ofParameter <float>ringOrbitRadius;
    ofParameter <float>ringZ;
    
    ringAnimationType ringAnimation; // ringAnimationType ?
    ofxButton ringAnimationButton;
    
    ofxPanel cameraPanel;
    ofParameterGroup cameraParams;
    
  
    // drawing
    
    Ring ring;
    Ring rings[NUMBER_OF_RINGS];
    int selectedRingIndex;
    
    float posX;
    float posY;
    double phase;
    
    // sound
    
    ofSoundPlayer noesysAudio;
    float *fftSmoothed;
    int nBandsToGet;
    bool playingSnd;
    
    // methods
    
    void initSound();
    void playSound();
    void fftDraw();
    float* getFFTData(int numBands);
    void setupGui();
    void initAnimObjects();
    
};
