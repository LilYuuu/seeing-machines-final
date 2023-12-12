#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    /* ----- clock ----- */
    ofSetFrameRate(60);
    
    // smooth the circle outline
    ofSetCircleResolution(48);
    
    // get current system time
    currSec = ofGetSeconds();
    
    
    // Setup the parameters.
    nearThreshold_shadow.set("Near Threshold", 0.02f, 0.0f, 0.1f);
    farThreshold_shadow.set("Far Threshold", 0.03f, 0.0f, 0.1f);
    
    // Setup the gui.
    gui.setup("Depth Threshold", "settings.json");
    gui.add(nearThreshold_shadow);
    gui.add(farThreshold_shadow);
    
    
    kinects[0].listDevices();

    
    /* ----- KINECT1 ----- */
    // kinect 1 , the one to get the outline of user
    kinects[0].setRegistration(true);
    kinects[0].init();
    kinects[0].open();
    
    // Fbo img for shadow initialization
    shadowFbo.allocate(640, 480, GL_RGBA);
    

    
    /* ----- KINECT2 ----- */
//    kinect 2 , the one on the ceiling

    kinects[1].setRegistration(true);
    kinects[1].init();
    kinects[1].open();

    // color img initialization
    colorImg2.allocate(kinects[0].getWidth(), kinects[0].getHeight());
    // Fbo img initialization
    maskFbo2.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);

    // Setup the parameters for kinect2.
    nearThreshold2.set("Near Threshold2", 0.03f, 0.0f, 0.1f);
    farThreshold2.set("Far Threshold2", 0.07f, 0.0f, 0.1f);

    // Add Kinect2 to the gui.
    gui.add(nearThreshold2);
    gui.add(farThreshold2);



//    cout << "kinect : " << kinects[0].getSerial() << endl;
//    cout << "kinect2 : " << kinects[1].getSerial() << endl;


    if (kinects[0].getSerial() == "B00363717138051B")
    {
        a = 0;
        b = 1;
    }
    else
    {
        a = 1;
        b = 0;
    }
}



//--------------------------------------------------------------
void ofApp::update(){
    
    /* --------------- KINECT1 --------------- */
    
    kinects[a].update();
    cout << "kinect /// WIDTH:" << kinects[a].getWidth() << " HEIGHT:" << kinects[a].getHeight() << endl ;
    
    mixFbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA);
    cout << "mixFbo /// WIDTH:" << mixFbo.getWidth() << " mixFbo HEIGHT:" << mixFbo.getHeight() << endl ;
   
    
    if (kinects[a].isFrameNew()){
                
        ofFloatPixels rawDepthPix = kinects[a].getRawDepthPixels();
        ofFloatPixels thresholdNear, thresholdFar, thresholdResult;
        
        ofxCv::threshold(rawDepthPix, thresholdNear, nearThreshold_shadow);
        ofxCv::threshold(rawDepthPix, thresholdFar, farThreshold_shadow, true);
        ofxCv::bitwise_and(thresholdNear, thresholdFar, thresholdResult);
        
        thresholdImg_shadow.setFromPixels(thresholdResult);
        
        int minimum = thresholdImg_shadow.getWidth() * thresholdImg_shadow.getHeight() * 0.01;
        int maximum = thresholdImg_shadow.getWidth() * thresholdImg_shadow.getHeight() * 1;
        
        contourFinder_shadow.findContours(thresholdImg_shadow, minimum, maximum, 5, false);
    }
    
    // draw the contour & Blob on maskFbo ( Kinect1's fbo)
    shadowFbo.begin();
    ofClear(255, 255, 255);
    if (contourFinder_shadow.nBlobs > 0) {
        ofPushMatrix();
        
        // center the blob on the window
        ofTranslate(shadowFbo.getWidth()/2 - contourFinder_shadow.blobs[0].centroid.x,
                    shadowFbo.getHeight()/2 - contourFinder_shadow.blobs[0].centroid.y);
        
        for (int i = 0; i < contourFinder_shadow.nBlobs; i++){
            ofxCvBlob& blob = contourFinder_shadow.blobs[i];
            blob.draw(0, 0);
            
            // for debug
            // ofSetColor(255, 0, 0);
            //ofFill();
            //ofDrawCircle(blob.centroid.x, blob.centroid.y, 30);
        }
        ofPopMatrix();
    }
    shadowFbo.end();
    
    // ----- (maksFbo → pixel) ----- //
    // Fbo is greyScale img. now transfer to Pixel to add effects
    shadowFbo.readToPixels(ShadowPixels);
    //    ofxCv::erode(ShadowPixels, 5);
    ofxCv::dilate(ShadowPixels, 5);
    ofxCv::medianBlur(ShadowPixels, 5);
    ofxCv::blur(ShadowPixels, 50);
    
    
    // ----- (pixel → Texture) ----- //
    // transfer Pixel to Texture, also draw to maskFbo
    shadowFbo.begin();
//    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ShadowTexture.loadData(ShadowPixels);
    ShadowTexture.draw(0,0);
//    ofDisableBlendMode();
    shadowFbo.end();
    
    
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
    
    
    
    
    
    /* --------------- KINECT2 --------------- */

    kinects[b].update();
//
    if (kinects[b].isFrameNew()){
//        // update color img
        colorImg2.setFromPixels(kinects[b].getPixels());
//
//        // get pixel data from kinect
        const ofFloatPixels & rawDepthPix2 = kinects[b].getRawDepthPixels();
//
//        // filter depth using thresholds
        ofFloatPixels thresholdNear2, thresholdFar2, thresholdResult2;
//
        ofxCv::threshold(rawDepthPix2, thresholdNear2, nearThreshold2);
        ofxCv::threshold(rawDepthPix2, thresholdFar2, farThreshold2, true);
        ofxCv::bitwise_and(thresholdNear2, thresholdFar2, thresholdResult2);
//
//        cout << "thresholdResult2 BEFORE /// WIDTH:" << thresholdResult2.getWidth() << " HEIGHT:" << thresholdResult2.getHeight() << endl ;
//
//        thresholdResult2.resize(ofGetWindowWidth(), ofGetWindowHeight() );
//
//        cout << "thresholdResult2 AFTER /// WIDTH:" << thresholdResult2.getWidth() << " HEIGHT:" << thresholdResult2.getHeight() << endl ;
//
//
//        //threshImg:GrayscaleImage
        thresholdImg2.setFromPixels(thresholdResult2);
        thresholdImg2.resize(ofGetWindowWidth(), ofGetWindowHeight() );
//
//
//        //before find contour. rescale the result
//        cout << "thresholdImg2 BEFORE /// WIDTH:" << thresholdImg2.width << " HEIGHT:" << thresholdImg2.height << endl ;
//
////        thresholdImg2.resize( ofGetWindowWidth(), ofGetWindowHeight()  );
//        thresholdImg2.updateTexture();
//
//
//        cout << "thresholdImg2 AFTER ////  WIDTH:" << thresholdImg2.width << " HEIGHT:" << thresholdImg2.height << endl ;
//
//        // contour size range
        int minimum2 = thresholdImg2.getWidth() * thresholdImg2.getHeight() * 0.05;
        int maximum2 = thresholdImg2.getWidth() * thresholdImg2.getHeight() * 0.8;
//
//        // look for contours
        contourFinder2.findContours(thresholdImg2, minimum2, maximum2, 3, false);
//        cout << "contourFinder2  WIDTH:" << contourFinder2.getWidth()<< " HEIGHT:" << contourFinder2.getHeight() << endl ;
//
    }
//
//    // draw the cameraView + contour & Blob on maskFbo2 ( Kinect2's fbo)
//    maskFbo2.begin();
//    ofClear(255, 255, 255);
//    colorImg2.draw(0, 0);
//    contourFinder2.draw(0, 0);
//
//    // draw centroids of the contour blobs
//    if (contourFinder2.nBlobs > 0) {
//        for (int i=0; i<contourFinder2.nBlobs; i++) {
//            ofVec3f centroid2 = contourFinder2.blobs[i].centroid;
////            ofPushStyle();
////            ofSetColor(255,0,0);
////            ofDrawCircle(centroid2, 20);
////            ofPopStyle();
//        }
//    }
//    maskFbo2.end();
 

}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofBackground(255);
  
//    colorImg2.draw(0, 0, ofGetWindowWidth(),ofGetWindowHeight());
//    thresholdImg2.draw(0,0);
    contourFinder2.draw(0,0);
//    maskFbo2.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());

    if (contourFinder2.nBlobs > 0) {
 
        
        ofPushMatrix();
        ofTranslate(  (ofGetWindowWidth()/2-contourFinder2.blobs[0].centroid.x)*-1  ,(ofGetWindowHeight()/2-contourFinder2.blobs[0].centroid.y)*-1  );

        
        /* ----- clock ----- */
        ofPushStyle();
            ofSetColor(0);
            ofNoFill();
            ofSetLineWidth(3);
            
        // clock face
        ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 200);
        ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 10);
            
        //        ofPushMatrix();
        //        ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
        //        ofPopMatrix();
        
            // clock hands

            //    drawClockHand("second", secHandRad);
            //    drawClockHand("minute", minHandRad);
            //    drawClockHand("hour", hrHandRad);
        
        
        
        
        
        
    
        ofPopStyle();
        
//        maskFbo2.begin();
        /* ----- shadow ----- */
        drawShadowOnClock("second", secHandRad, shadowFbo);
        drawShadowOnClock("minute", minHandRad, shadowFbo);
        drawShadowOnClock("hour", hrHandRad, shadowFbo);
        
//        maskFbo2.end();
        
        ofPopMatrix();
        
        
    }
    
    
        if (contourFinder2.nBlobs > 0) {
            for (int i=0; i<contourFinder2.nBlobs; i++) {
                ofVec3f centroid2 = contourFinder2.blobs[i].centroid;
                ofPushStyle();
                ofSetColor(255,0,0);
                ofDrawCircle(centroid2, 20);
                ofPopStyle();
            }
        }
    
    
    


    
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinects[0].setCameraTiltAngle(0);
    kinects[0].close();

    kinects[1].setCameraTiltAngle(0);
    kinects[1].close();
}

//--------------------------------------------------------------
void ofApp::drawShadowOnClock(string hand, float rotation, ofFbo fbo){
   
    ofPushMatrix();
    
    ofTranslate(ofGetWindowWidth()/2, ofGetWindowHeight()/2);
    ofRotateRad(PI);
    ofRotateRad(rotation);
    
    ofTranslate(-80*3, -120*3);
    fbo.draw(0, 0, 160*3, 120*3);
//    maskFbo.draw(0, 0, 160*2, 120*2);
    
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


//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    switch (key) {
        case OF_KEY_LEFT:
            angle_kinect1++;
            if(angle_kinect1>30) angle_kinect1=30;
            kinects[a].setCameraTiltAngle(angle_kinect1);
            break;
            
        case OF_KEY_RIGHT:
            angle_kinect1--;
            if(angle_kinect1<-30) angle_kinect1=-30;
            kinects[a].setCameraTiltAngle(angle_kinect1);
            break;
            
//        #ifdef USE_TWO_KINECTS

        case OF_KEY_UP:
            angle2++;
            if(angle2<-30) angle2=-30;
            kinects[b].setCameraTiltAngle(angle2);
            break;
            
        case OF_KEY_DOWN:
            angle2--;
            if(angle2<-30) angle2=-30;
            kinects[b].setCameraTiltAngle(angle2);
            break;
            
//        #endif
    }
}
