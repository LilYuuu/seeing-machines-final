#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    /* ----- clock ----- */
    ofSetFrameRate(60);
    
    // smooth the circle outline
    ofSetCircleResolution(48);
    
    currSec = ofGetSeconds();
    
    
    /* ----- shadow ----- */
    maskFbo.allocate(640, 480, GL_RGBA);
    
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
}

//--------------------------------------------------------------
void ofApp::update(){
    
    /* ----- shadow ----- */
    kinect.update();
    
    if (kinect.isFrameNew()){
                
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
    
    // fbo
    maskFbo.begin();
    ofClear(255, 255, 255);
    if (contour.nBlobs > 0) {
        ofPushMatrix();
        // center the blob on the window
        ofTranslate(maskFbo.getWidth()/2 - contour.blobs[0].centroid.x, maskFbo.getHeight()/2 - contour.blobs[0].centroid.y);
        for (int i = 0; i < contour.nBlobs; i++){
            ofxCvBlob& blob = contour.blobs[i];
            blob.draw(0, 0);
            
            // for debug
//            ofSetColor(255, 0, 0);
//            ofFill();
//            ofDrawCircle(blob.centroid.x, blob.centroid.y, 30);
        }
        ofPopMatrix();
    }
    maskFbo.end();
    
    
    maskFbo.readToPixels(contourFilledPix);
    //    ofxCv::erode(contourFilledPix, 5);
    ofxCv::dilate(contourFilledPix, 5);
    ofxCv::medianBlur(contourFilledPix, 5);
    ofxCv::blur(contourFilledPix, 50);
    
    maskFbo.begin();
//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    contourFilledTex.loadData(contourFilledPix);
    contourFilledTex.draw(0,0);
//    ofDisableBlendMode();
    maskFbo.end();
    
    
    /* ----- clock ----- */
    // get current system time
    currHr = ofGetHours();
    currMin = ofGetMinutes();
    currSec += 1/60.0;
    currSec = fmod(currSec, 60.0);
    oriSec = ofGetSeconds();
    
    // calculate rotation radius for clock hands
    hrHandRad = PI + currHr / 6.0 * PI;
    minHandRad = PI + currMin / 30.0 * PI;
    secHandRad = PI + currSec / 30.0 * PI;
    
    oriSecHandRad = PI + oriSec / 30.0 * PI;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    
    
    
    
    /* ----- clock ----- */
    ofPushStyle();
    ofSetColor(0);
    ofNoFill();
    ofSetLineWidth(3);
    
    // clock face
    ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 200);
    ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 10);
    
    // clock hands
//    ofPushMatrix();
//    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
//    drawClockHand("second", secHandRad);
//    drawClockHand("minute", minHandRad);
//    drawClockHand("hour", hrHandRad);
//    ofPopMatrix();
    
    ofPopStyle();
    
    
    /* ----- shadow ----- */
    drawShadowOnClock("second", secHandRad, maskFbo);
    drawShadowOnClock("minute", minHandRad, maskFbo);
    drawShadowOnClock("hour", hrHandRad, maskFbo);
    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.setCameraTiltAngle(0);
    kinect.close();
}

//--------------------------------------------------------------
void ofApp::drawShadowOnClock(string hand, float rotation, ofFbo fbo){
   
    ofPushMatrix();
    
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    ofRotateRad(PI);
    ofRotateRad(rotation);
    
    ofTranslate(-80, -120);
    maskFbo.draw(0, 0, 160, 120);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawClockHand(string hand, float rotation){
    ofPushMatrix();
    if (hand == "second") {
        ofSetColor(255, 0, 0);
        ofRotateRad(rotation);
        ofDrawLine(0, 15, 0, 150);
    } else if (hand == "minute") {
        ofSetColor(0, 255, 0);
        ofRotateRad(rotation);
        ofDrawLine(0, 15, 0, 120);
    } else if (hand == "hour") {
        ofSetColor(0, 0, 255);
        ofRotateRad(rotation);
        ofDrawLine(0, 15, 0, 80);
    }
    ofPopMatrix();
}
