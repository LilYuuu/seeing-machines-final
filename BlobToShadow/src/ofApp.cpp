#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    maskFbo.allocate(600, 480, GL_RGBA);
    
    // Setup the parameters.
    nearThreshold.set("Near Threshold", 0.001f, 0.0f, 0.1f);
    farThreshold.set("Far Threshold", 0.02f, 0.0f, 0.1f);
    
    // Setup the gui.
    gui.setup("Depth Threshold", "settings.json");
    gui.add(nearThreshold);
    gui.add(farThreshold);
    
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    color.allocate(kinect.getWidth(), kinect.getHeight());
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();
    
    if (kinect.isFrameNew()){
        
        color.setFromPixels(kinect.getPixels());
        
        
        //======
        ofFloatPixels rawDepthPix = kinect.getRawDepthPixels();
        ofFloatPixels thresholdNear, thresholdFar, thresholdResult;
        
        ofxCv::threshold(rawDepthPix, thresholdNear, nearThreshold);
        ofxCv::threshold(rawDepthPix, thresholdFar, farThreshold, true);
        ofxCv::bitwise_and(thresholdNear, thresholdFar, thresholdResult);
        
        thresholdImg.setFromPixels(thresholdResult);
        
        int minimum = thresholdImg.getWidth() * thresholdImg.getHeight() * 0.01;
        int maximum = thresholdImg.getWidth() * thresholdImg.getHeight() * 1;
        
        contour.findContours(thresholdImg, minimum, maximum, 5, false);
        
        
    }
    
    maskFbo.begin();
    ofClear(255,255,255, 0);
    maskFbo.end();
    
    maskFbo.begin();
    
    for (int i = 0; i < contour.nBlobs; i++){
        ofxCvBlob& blob = contour.blobs[i];
        blob.draw(0, 0);
    }
    
    //    contour.draw();
    maskFbo.end();
    
    
    maskFbo.readToPixels(contourFilledPix);
    //    ofxCv::erode(contourFilledPix, 5);
    ofxCv::dilate(contourFilledPix, 5);
    ofxCv::medianBlur(contourFilledPix, 5);
    ofxCv::blur(contourFilledPix, 50);
    
    maskFbo.begin();
    contourFilledTex.loadData(contourFilledPix);
    contourFilledTex.draw(0,0);
    maskFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    maskFbo.draw(0,0);
    ofDisableBlendMode();
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.setCameraTiltAngle(0);
    kinect.close();
}
