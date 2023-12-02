#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
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
    void createContourEdge(ofPolyline polyline);

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
  
  ofxPanel gui;
    
  ofxButton uiLearn;
  ofxButton uiClearCircles;
  ofxButton uiClearContourEdge;
  ofxIntSlider uiThreshold;
  ofxFloatSlider uiNearThreshold;
  ofxFloatSlider uiFarThreshold;
    
  
  ofxKinect kinect;
  
  ofxCvColorImage color;
  ofxCvContourFinder contour;
  
  ofxBox2d box2d;
  vector <shared_ptr<ofxBox2dCircle>> circles;
  ofxBox2dEdge *contourEdge;
    
  
    
  ofxCvGrayscaleImage thresholdImg;
  ofImage ColorThresholdImg;
  
  ofFbo maskFbo;
  ofFbo maskTextureFbo;
  ofPixels contourFilledPix;
  ofTexture contourFilledTex;
  ofParameter<float> nearThreshold;
  ofParameter<float> farThreshold;
  ofParameter<int> blurAmount;
  
        
};
