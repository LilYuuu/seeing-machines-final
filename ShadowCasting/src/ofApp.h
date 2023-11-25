#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxBox2d.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;
    
        ofxKinect kinect;
    
        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage thresholdImg;
        
        ofxCvContourFinder contourFinder;
        
        ofParameter<float> nearThreshold;
        ofParameter<float> farThreshold;
        ofParameter<float> minArea;
        ofParameter<float> maxArea;
        
        ofxPanel gui;
		
};
