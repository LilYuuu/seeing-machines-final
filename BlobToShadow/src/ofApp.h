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
    
    ofxPanel gui;
    ofParameter<float> nearThreshold;
    ofParameter<float> farThreshold;

    ofxKinect kinect;
    
    ofxCvColorImage color;
    ofxCvContourFinder contour;
    
    ofxCvGrayscaleImage thresholdImg;
    ofImage ColorThresholdImg;
    
    ofFbo maskFbo;
    ofPixels contourFilledPix;
    ofTexture contourFilledTex;
};
