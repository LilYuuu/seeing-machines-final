#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    /* ----- clock ----- */
    int currHr;
    int currMin;
    float currSec;
    int oriSec;

    float hrHandRad = 0.0;
    float minHandRad = 0.0;
    float secHandRad = 0.0;
    float oriSecHandRad = 0.0;

    void drawClockHand(string hand, float rotation);
    void drawShadowOnClock(string hand, float rotation, ofFbo fbo);
    
    
    /* ----- shadow ----- */
    ofxPanel gui;
    ofParameter<float> nearThreshold;
    ofParameter<float> farThreshold;

    ofxKinect kinect;
    
    ofxCvContourFinder contour;
    
    ofxCvGrayscaleImage thresholdImg;
    
    ofFbo maskFbo;
    ofFbo displayFbo;
    ofPixels contourFilledPix;
    ofTexture contourFilledTex;
};
