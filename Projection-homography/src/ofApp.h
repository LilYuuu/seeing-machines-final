#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

//change the projector res here
#define PROJECTOR_RESOLUTION_X 1680
#define PROJECTOR_RESOLUTION_Y 1050

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawProjector(ofEventArgs& args);
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void keyPressed(int key);

    
    int angle;
    
  ofxPanel gui;
  ofxKinect kinect;
  
  ofxCvColorImage color;
  ofxCvContourFinder contour;
  

  
  ofImage contourImg;
  ofPixels contourPixels;
  ofTexture contourTexture;
  ofFbo contourFbo;
    
  ofImage warpedImg;
  ofPixels warpedImgPixels;
  ofTexture warpedImgTexture;
  ofFbo warpedImgFbo;
    
  ofxCvGrayscaleImage thresholdImg;
  ofImage ColorThresholdImg;
  ofParameter<float> nearThreshold;
  ofParameter<float> farThreshold;
  ofParameter<int> blurAmount;
    
  std::vector<glm::vec2> srcPoints;
  std::vector<glm::vec2> dstPoints;
    
  int activePoint;
  
  cv::Mat homographyMat;
  bool homographyReady;

  ofParameter<bool> adjustMapping;

        
};
