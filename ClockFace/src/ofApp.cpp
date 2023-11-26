#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    // smooth the circle outline
    ofSetCircleResolution(48);
    
    currSec = ofGetSeconds();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    cout << "Frame Rate: " << ofGetFrameRate() << endl;
    
    // get current system time
    currHr = ofGetHours();
    currMin = ofGetMinutes();
    currSec += 1/60.0;
    currSec = fmod(currSec, 60.0);
    oriSec = ofGetSeconds();
//    cout << currHr << ":" << currMin << ":" << currSec << endl;
    cout << currSec << endl;
    
    // calculate rotation radius for clock hands
    hrHandRad = PI + currHr / 6.0 * PI;
    minHandRad = PI + currMin / 30.0 * PI;
    secHandRad = PI + currSec / 30.0 * PI;
    
    oriSecHandRad = PI + oriSec / 30.0 * PI;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushStyle();
    ofNoFill();
    ofSetLineWidth(3);
    
    // clock face
    ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 200);
    
    // clock hands
    ofPushStyle();
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    
    // second hand
    drawClockHand("second", secHandRad);
    drawClockHand("second", oriSecHandRad);
    
    // minute hand
    drawClockHand("minute", minHandRad);
    
    // hour hand
    drawClockHand("hour", hrHandRad);
    
    ofPopStyle();
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::drawClockHand(string hand, float rotation){
    if (hand == "second") {
        ofPushMatrix();
//        ofTranslate(cos(rotation) * 10, sin(rotation) * 10);
        ofSetColor(255, 0, 0);
        ofRotateRad(rotation);
        ofDrawLine(0, 15, 0, 150);
        ofPopMatrix();
    } else if (hand == "minute") {
        ofPushMatrix();
//        ofTranslate(cos(rotation) * 10, sin(rotation) * 10);
        ofSetColor(0, 255, 0);
        ofRotateRad(rotation);
        ofDrawLine(0, 15, 0, 120);
        ofPopMatrix();
    } else if (hand == "hour") {
        ofPushMatrix();
//        ofTranslate(cos(rotation) * 10, sin(rotation) * 10);
        ofSetColor(255);
        ofRotateRad(rotation);
        ofDrawLine(0, 15, 0, 80);
        ofPopMatrix();
    }
    
}

