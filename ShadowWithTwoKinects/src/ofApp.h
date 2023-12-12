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
    void keyPressed(int key);
    
    
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
    
    ofParameter<float> nearThreshold_shadow;
    ofParameter<float> farThreshold_shadow;
    
    ofxKinect kinect1;
    int angle_kinect1;
    
    
    ofxCvContourFinder contourFinder_shadow;
    ofxCvGrayscaleImage thresholdImg_shadow;
    
    ofFbo shadowFbo;
    ofPixels ShadowPixels;
    ofTexture ShadowTexture;
    
    
    
    ofxKinect kinect2;
    
    int angle2;
    
    ofParameter<float> nearThreshold2;
    ofParameter<float> farThreshold2;
    ofxCvColorImage colorImg2;
    ofxCvContourFinder contourFinder2;
    ofxCvGrayscaleImage thresholdImg2;
//    ofxCvGrayscaleImage resizeImg2;
    
    ofParameter<float> minArea2;
    ofParameter<float> maxArea2;
    
    ofFbo maskFbo2;
    ofFbo displayFbo2;
    ofPixels contourFilledPix2;
    ofTexture contourFilledTex2;
    
    
    ofxKinect kinects[2];
    int a, b;
    
    
    
    
    
    ofFbo mixFbo;
    
    
};
