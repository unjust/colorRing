#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255,255,255);
    
    setupGui();
    initSound();
    initAnimObjects();
    
    easyCam.setDistance(900);
    
}

//--------------------------------------------------------------
void ofApp::update(){

    // update the sound playing system:
    ofSoundUpdate();
    
    float* fftData = getFFTData(NUMBER_OF_BANDS);
    
    // * update rings
    // - use frequency surge to power how fast the circles orbit and camera pans
    // - beat is a bounce in z ?
    // - alternating fill
    
    float surgeAmount = 0;
    int freqGroups = ringController.QUANTITY;
    float* energyPerGroup = new float[freqGroups];
    
    for (int j = 0; j < freqGroups; j++){
       
        float sonicEnergyPerGroup = 0.;
        //int BANDS_DATA_PER_ITEM = (NUMBER_OF_BANDS / 2) / NUMBER_OF_RINGS;
        int BANDS_DATA_PER_ITEM = NUMBER_OF_BANDS / freqGroups;

        
        // currently 512 / 32; (16)
        // is it that half the window size is usable? or L and R data interleaved or repeated?
        // then this is sum of energy for a few bins
        
        int fftDataIndexBegin = j * BANDS_DATA_PER_ITEM;
        int fftDataIndexEnd = (j + 1) * BANDS_DATA_PER_ITEM;
        
        for (int i = fftDataIndexBegin; i < fftDataIndexEnd; i++){
            sonicEnergyPerGroup += fftData[i];
        
//          if (i > 0 && i < 8) {
//              surgeAmount += fftData[i];
//              // printf("y %f", fftData[i]);
//              if (fftData[i] < .2) {
//                  surgeAmount = 0;
//                  break;
//              }
//          }
        }
        energyPerGroup[j] = sonicEnergyPerGroup;
    }
    ringController.setAudioData(energyPerGroup);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // gui
    ringAttrsPanel.draw();
    
    soundPanel.draw();
    
    if (showFFT) {
        fftDraw();
    }
    
    easyCam.begin();
    
    ofSetColor(255,0,0);
    ofFill();
    
    ofDrawBox(30);
    
    if (showRings) {
        ringController.draw();
    }
    
    ofDisableAlphaBlending();
    easyCam.end();
    
    
}


//------------------------------------------------------------
void ofApp::setupGui(){
    
    float   x = 10,
            y = 10;
    
    ringAttrsParams.setName("Rings");
    ringAttrsParams.add(ringRadius.set("base radius", 10, 0, 300));
    ringAttrsParams.add(ringOrbitRadius.set("orbit", 10, 0, 300));
    ringAttrsParams.add(ringZ.set("z", 0, 0, 1000));
    ringAttrsParams.add(ringBounce.set("bounce", false));
    ringAttrsParams.add(showRings.set("circles", true));
    
    ringRadius.addListener(this, &ofApp::setRingRadius);
    ringOrbitRadius.addListener(this, &ofApp::setRingRadius);
    
    ringAttrsPanel.setup(ringAttrsParams, "ring.xml", x, y += 100);
    ringAttrsPanel.add(ringAnimationButton.setup("anim type"));
    
    ringAnimationButton.addListener(this, &ofApp::ringAnimButtonPressed);
    
    soundParams.setName("Sound");
    
    soundParams.add(volume.set("volume", 0, 0, 1));
    soundParams.add(showFFT.set("draw fft", false));
    volume.addListener(this, &ofApp::handleVolume);
    soundPanel.setup(soundParams, "soundSettings.xml", x, y += 180);
    
}

void ofApp::setRingRadius(const void* sender, float &value) {
    ofParameter<float> s = *((ofParameter<float>*) sender);
    if (s == ringRadius) {
        ringController.setBaseRadius(value);
        
    } else if (s == ringOrbitRadius) {
         ringController.setOrbitRadius(value);
    }
}

void ofApp::ringAnimButtonPressed(const void* sender) {
    int ra = (int)ringAnimation;
    (ra < 2) ? ringAnimation = (ringAnimationType)++ra : ringAnimation = (ringAnimationType)0;
    
    ringController.setAnimationType((ringAnimationType)ringAnimation);
    cout << "\n ring animation type " << ringAnimation;
}


void ofApp::handleVolume(const void* sender, float &value) {
    noesysAudio.setVolume(value);
}

//--------------------------------------------------------------
void ofApp::initSound(){
    
    playSound();
    
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
    // fftSmoothed = new float[8192];  // 8192 / 128 = 64... what is the signigicance of 8192
    fftSmoothed = new float[NUMBER_OF_BANDS];
    for (int i = 0; i < NUMBER_OF_BANDS; i++){
        fftSmoothed[i] = 0;
    }
    
}

//----------------------------------------------------------------
void ofApp::audioOut( ofSoundBuffer &outBuffer ) {
    for(int i = 0; i < outBuffer.size(); i += 2) {
        float sample = sin(phase); // generating a sine wave sample
        outBuffer[i] = sample; // writing to the left channel
        outBuffer[i + 1] = sample; // writing to the right channel
        phase += 0.05;
    }
}

//--------------------------------------------------------------
float* ofApp::getFFTData(int numBands) {
    float *val = ofSoundGetSpectrum(numBands); // request 128 for FFT

    for (int i = 0; i < numBands; i++) {
        // let the smoothed value sink to zero:
        fftSmoothed[i] *= 0.96f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) {
            fftSmoothed[i] = val[i];
        }
    }
    return fftSmoothed;
}

//----------------------------------------------------------------
void ofApp::playSound() {
    string filename = "sounds/nervous.mp3" ;
    noesysAudio.load(filename);
    noesysAudio.play();
    
    // for debugging
    noesysAudio.setVolume(volume);
}

//--------------------------------------------------------------
void ofApp::fftDraw(){
    //ofSetColor(0,255,255,80);
    
    //float bandwidth = ofGetWidth()/(NUMBER_OF_BANDS);
    float bandwidth = 20;
    int i = 1;
    int j = 0;
    int k = 0;
    int r = 200, g = 255, b = 100;

    for (int b = 0; b < NUMBER_OF_BANDS; b++) {
        r += i;
        g -= j;
        b += k;
        
        if ( r == 255 ) {
            i = 0;
            j = 1;
        } else if (g == 0) {
            j = 0;
            k = 1;
        }
        
        ofSetColor(r, g, b, 80);
        ofDrawRectangle(100 + bandwidth * b, ofGetHeight() * .9, bandwidth, -(fftSmoothed[b] * (ofGetHeight()/2)));
    }
}

//--------------------------------------------------------------
void ofApp::initAnimObjects(){

    Controller* ringController = new Controller();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    posX = x;
    posY = y;
    
    ofPoint pt;
    pt.set(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
