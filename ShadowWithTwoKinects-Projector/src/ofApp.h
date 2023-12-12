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
    void drawProjector(ofEventArgs& args);
    
    //change the projector res here
    #define PROJECTOR_RESOLUTION_X 1920
    #define PROJECTOR_RESOLUTION_Y 1080
    
    
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
    
    ofxKinect kinects[2];
    int a, b;
    
    // kinect 1
    ofxKinect kinect1;
    int angle_kinect1;
    
    ofParameter<float> nearThreshold_shadow;
    ofParameter<float> farThreshold_shadow;
    
    ofxCvContourFinder contourFinder_shadow;
    ofxCvGrayscaleImage thresholdImg_shadow;
    
    ofFbo shadowFbo;
    ofPixels ShadowPixels;
    ofTexture ShadowTexture;
    
    // kinect 2
    ofxKinect kinect2;
    int angle_kinect2;
    
    ofParameter<float> nearThreshold_ceiling;
    ofParameter<float> farThreshold_ceiling;
    
    ofxCvColorImage colorImg_ceiling;
    
    ofxCvContourFinder contourFinder_ceiling;
    ofxCvGrayscaleImage thresholdImg_ceiling;
    
    
    
};
