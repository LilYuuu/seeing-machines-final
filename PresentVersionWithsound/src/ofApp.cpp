#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    clockSound.load("wall-clock-ticking.wav");
    
    /* ----- clock ----- */
    ofSetFrameRate(60);
    
    // smooth the circle outline
    ofSetCircleResolution(500);
    
    // get current system time
    currSec = ofGetSeconds();
    
    
    // Setup the parameters.
    nearThreshold_shadow.set("Near Threshold (Shadow)", 0.003f, 0.0f, 0.1f);
    farThreshold_shadow.set("Far Threshold (Shadow)", 0.0465f, 0.0f, 0.1f);
    
    // Setup the gui.
    gui.setup("Depth Threshold", "settings.json");
    gui.add(nearThreshold_shadow);
    gui.add(farThreshold_shadow);
    
    
    kinects[0].listDevices();

    
    /* ----- KINECT1 ----- */
    // shadow capturing
    kinects[0].setRegistration(true);
    kinects[0].init();
    kinects[0].open();
    
    // Fbo img for shadow initialization
    shadowFbo.allocate(640, 480, GL_RGBA);
    

    
    /* ----- KINECT2 ----- */
    // movement detection on ceiling
    kinects[1].setRegistration(true);
    kinects[1].init();
    kinects[1].open();

    // color img initialization
    colorImg_ceiling.allocate(kinects[0].getWidth(), kinects[0].getHeight());

    // Setup the parameters for kinect2.
    nearThreshold_ceiling.set("Near Threshold (Ceiling)", 0.0005f, 0.0f, 0.1f);
    farThreshold_ceiling.set("Far Threshold (Ceiling)", 0.044f, 0.0f, 0.1f);

    // Add Kinect2 to the gui.
    gui.add(nearThreshold_ceiling);
    gui.add(farThreshold_ceiling);



//    cout << "kinect : " << kinects[0].getSerial() << endl;
//    cout << "kinect2 : " << kinects[1].getSerial() << endl;


    if (kinects[0].getSerial() == "B00363717138051B")
    {
        a = 1;
        b = 0;
    }
    else
    {
        a = 0;
        b = 1;
    }
}







//--------------------------------------------------------------
void ofApp::update(){
    
    /* --------------- KINECT1 --------------- */
    
    kinects[a].update();
//    cout << "kinect /// WIDTH:" << kinects[a].getWidth() << " HEIGHT:" << kinects[a].getHeight() << endl ;
    
    if (kinects[a].isFrameNew()){
                
        ofFloatPixels rawDepthPix = kinects[a].getRawDepthPixels();
        ofFloatPixels thresholdNear, thresholdFar, thresholdResult;
        
        ofxCv::threshold(rawDepthPix, thresholdNear, nearThreshold_shadow);
        ofxCv::threshold(rawDepthPix, thresholdFar, farThreshold_shadow, true);
        ofxCv::bitwise_and(thresholdNear, thresholdFar, thresholdResult);
        
        thresholdImg_shadow.setFromPixels(thresholdResult);
        
        float minimum = thresholdImg_shadow.getWidth() * thresholdImg_shadow.getHeight() * 0;
        float maximum = thresholdImg_shadow.getWidth() * thresholdImg_shadow.getHeight() * 1;
        
//        contourFinder_shadow.findContours(thresholdImg_shadow, minimum, maximum, 1, false);
        contourFinder_shadow.findContours(thresholdImg_shadow);
        contourFinder_shadow.setSortBySize(true);
    }
    
//    // draw shadow on an fbo
//    shadowFbo.begin();
//    ofClear(255, 255, 255);
    
    //====================================//
//    if (contourFinder_shadow.nBlobs > 0) {
//        ofPushMatrix();
//
//        // center the blob on the window
//        ofTranslate(shadowFbo.getWidth()/2 - contourFinder_shadow.blobs[0].centroid.x,
//                    shadowFbo.getHeight()/2 - contourFinder_shadow.blobs[0].centroid.y);
//
//        for (int i = 0; i < contourFinder_shadow.nBlobs; i++){
//            ofxCvBlob& blob = contourFinder_shadow.blobs[i];
//            blob.draw(0, 0);
//
//
//            // for debug
//            // ofSetColor(255, 0, 0);
//            //ofFill();
//            //ofDrawCircle(blob.centroid.x, blob.centroid.y, 30);
//
//        }
//        ofPopMatrix();
//    }
    //====================================//
    
    
    // draw shadow on an fbo
    shadowFbo.begin();
    ofClear(255, 255, 255);
    
    
    
    if (contourFinder_shadow.size()>0){
        //    for (int i=0; i< contourFinder_shadow.size(); i++) {
        
        ofPolyline_ polyline = contourFinder_shadow.getPolyline(0);
        cout << "SIZE : "<< contourFinder_shadow.getPolylines().size() << endl;
        polyline.setClosed(true);
        ofPolyline_ smoothedPolyline = polyline.getSmoothed(7, 1);
        
        ofPoint centroid = contourFinder_shadow.getPolyline(0).getCentroid2D();
        
                ofTranslate(shadowFbo.getWidth()/2 - centroid.x,
                            shadowFbo.getHeight()/2 - centroid.y);
        
        ofPath path;
        path.setFillColor(ofColor(0));
        path.setFilled(true);
        
        for (int j = 0; j < smoothedPolyline.size(); j++) {
            if (j == 0) {
                path.newSubPath();
                path.moveTo(smoothedPolyline[j]);
            } else {
                path.lineTo(smoothedPolyline[j]);
            }
        }
        path.close();
        
        path.draw();
        
      }
//    }
    
    
    shadowFbo.end();
    
    // ----- (fbo → pixel) ----- //
    // Fbo is greyScale img. now transfer to Pixel to add effects
    shadowFbo.readToPixels(ShadowPixels);
    //    ofxCv::erode(ShadowPixels, 5);
    ofxCv::dilate(ShadowPixels, 5);
    ofxCv::medianBlur(ShadowPixels, 5);
    ofxCv::blur(ShadowPixels, 50);
    
    
    // ----- (pixel → Texture) ----- //
    // transfer Pixel to Texture, also draw to fbo
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

    if (kinects[b].isFrameNew()){
        // update color img
        colorImg_ceiling.setFromPixels(kinects[b].getPixels());

        // get pixel data from kinect
        const ofFloatPixels & rawDepthPix2 = kinects[b].getRawDepthPixels();

        // filter depth using thresholds
        ofFloatPixels thresholdNear2, thresholdFar2, thresholdResult2;

        ofxCv::threshold(rawDepthPix2, thresholdNear2, nearThreshold_ceiling);
        ofxCv::threshold(rawDepthPix2, thresholdFar2, farThreshold_ceiling, true);
        ofxCv::bitwise_and(thresholdNear2, thresholdFar2, thresholdResult2);

//        cout << "thresholdResult2 BEFORE /// WIDTH:" << thresholdResult2.getWidth() << " HEIGHT:" << thresholdResult2.getHeight() << endl ;

//        cout << "thresholdResult2 AFTER /// WIDTH:" << thresholdResult2.getWidth() << " HEIGHT:" << thresholdResult2.getHeight() << endl ;

        thresholdImg_ceiling.setFromPixels(thresholdResult2);
        thresholdImg_ceiling.resize(ofGetWindowWidth(), ofGetWindowHeight() );


        //before find contour. rescale the result
//        cout << "thresholdImg_ceiling BEFORE /// WIDTH:" << thresholdImg_ceiling.width << " HEIGHT:" << thresholdImg_ceiling.height << endl ;

//        cout << "thresholdImg_ceiling AFTER ////  WIDTH:" << thresholdImg_ceiling.width << " HEIGHT:" << thresholdImg_ceiling.height << endl ;

        // contour size range
        float contourSizeMin_ceiling = thresholdImg_ceiling.getWidth() * thresholdImg_ceiling.getHeight() * 0;
        float contourSizeMax_ceiling = thresholdImg_ceiling.getWidth() * thresholdImg_ceiling.getHeight() * 0.8;

        // look for contours
        contourFinder_ceiling.findContours(thresholdImg_ceiling, contourSizeMin_ceiling, contourSizeMax_ceiling, 1, false);
        

//        cout << "contourFinder_ceiling  WIDTH:" << contourFinder_ceiling.getWidth()<< " HEIGHT:" << contourFinder_ceiling.getHeight() << endl ;
        
        

    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofBackground(255);
  
    colorImg_ceiling.draw(0, 0, ofGetWindowWidth(),ofGetWindowHeight());
//    thresholdImg_ceiling.draw(0,0);
    contourFinder_ceiling.draw(0,0);

    if (contourFinder_ceiling.nBlobs > 0) {
 
        ofPushMatrix();
        ofTranslate(  (ofGetWindowWidth()/2 - contourFinder_ceiling.blobs[0].centroid.x) * (-1)  ,(ofGetWindowHeight()/2 - contourFinder_ceiling.blobs[0].centroid.y) * (-1) );

        
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
        
        /* ----- shadow ----- */
        drawShadowOnClock("second", secHandRad, shadowFbo);
        drawShadowOnClock("minute", minHandRad, shadowFbo);
        drawShadowOnClock("hour", hrHandRad, shadowFbo);
        
        ofPopMatrix();
    }
    
    
    if (contourFinder_ceiling.nBlobs > 0) {
        
        for (int i=0; i<contourFinder_ceiling.nBlobs; i++) {
            ofVec3f centroid_ceiling = contourFinder_ceiling.blobs[i].centroid;
            ofPushStyle();
            ofSetColor(255,0,0);
            ofDrawCircle(centroid_ceiling, 20);
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
    
    if (hand == "second") {
        
        ofTranslate(-80*3, -120*5);
        fbo.draw(0, 0, 160*3, 120*5);
        
    }else if (hand == "minute") {
        
        ofTranslate(-80*3, -120*3);
        fbo.draw(0, 0, 160*3, 120*3);
        
    } else if (hand == "hour") {
        
        ofTranslate(-80*3, -120*2);
        fbo.draw(0, 0, 160*3, 120*2);
        
    }
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
            angle_kinect2++;
            if(angle_kinect2<-30) angle_kinect2=-30;
            kinects[b].setCameraTiltAngle(angle_kinect2);
            break;
            
        case OF_KEY_DOWN:
            angle_kinect2--;
            if(angle_kinect2<-30) angle_kinect2=-30;
            kinects[b].setCameraTiltAngle(angle_kinect2);
            break;
            
        case OF_KEY_COMMAND:
            clockSound.play();

            break;
            
        case OF_KEY_SHIFT:
            clockSound.stop();
   
            break;
            

    }
}


//---------------------------------------------

void ofApp::drawProjector(ofEventArgs& args)
{
    ofBackground(0,0,0);
    

//    contourFinder_ceiling.draw(0,0);

    if (contourFinder_ceiling.nBlobs > 0) {
 
        ofPushMatrix();

//                cout<< "centroid X : " << contourFinder_ceiling.blobs[0].centroid.x << endl ;
                lerpPosX = ofLerp(lerpPosX ,contourFinder_ceiling.blobs[0].centroid.x,  lerpSpeed);
//                cout<< "lerpPosX : " << lerpPosX  << endl ;
                lerpPosY = ofLerp(lerpPosY ,contourFinder_ceiling.blobs[0].centroid.y,  lerpSpeed);

//        ofTranslate(  (ofGetWindowWidth()/2 - contourFinder_ceiling.blobs[0].centroid.x) * (-1)  ,(ofGetWindowHeight()/2 - contourFinder_ceiling.blobs[0].centroid.y) * (-1) );
        
        ofTranslate(  (ofGetWindowWidth()/2 - lerpPosX) * (-1)  ,(ofGetWindowHeight()/2 - lerpPosY) * (-1) );
        
        
        
        /* ----- clock ----- */
        ofPushStyle();
        ofSetColor(255);
//        ofNoFill();
        ofFill();
//        ofSetLineWidth(3);
            
        // clock face
        // smooth the circle outline
        ofSetCircleResolution(500);
        ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, ofGetWindowHeight()/2);
//        ofDrawCircle(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 10);

        
        ofPopStyle();
        
        /* ----- shadow ----- */
        drawShadowOnClock("second", secHandRad, shadowFbo);
        drawShadowOnClock("minute", minHandRad, shadowFbo);
        drawShadowOnClock("hour", hrHandRad, shadowFbo);
        
        ofPopMatrix();
        
    
    }
    
        
    
}



//
//Soundplaying = true;
//if (Soundplaying == true){
//    cout << "play" << endl;
//    clockSound.play();
//    Soundplaying = false;
//}
