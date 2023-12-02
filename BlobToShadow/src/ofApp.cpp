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

    gui.add(uiClearCircles.setup("clear circles"));


    kinect.setRegistration(true);
    kinect.init();
    kinect.open();

    color.allocate(kinect.getWidth(), kinect.getHeight());

//    box2d.init();
//    box2d.setGravity(0, 30);
//    box2d.createGround();
//    box2d.createBounds(ofRectangle(0, 0, kinect.getWidth()-50, kinect.getHeight()  ));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
//  box2d.update();
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
    
//    maskFbo.begin();
//    ofClear(0,0,0,255);
//    maskFbo.end();
    
    maskFbo.begin();
    contourFilledTex.loadData(contourFilledPix);
    contourFilledTex.draw(0,0);
    maskFbo.end();
    
  
  
//  if (uiClearCircles){
//    circles.clear();
//  }
  

}

//--------------------------------------------------------------
void ofApp::draw(){
    
  
  ofBackground(128);

  ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

//  color.draw(0,0);
  
    
  
//  contour.draw();
//    maskFbo.begin();
//     for (int i = 0; i < contour.nBlobs; i++){
//        ofxCvBlob& blob = contour.blobs[i];
//        blob.draw(0, 0);
//    }
//    maskFbo.end();
    
//    contourFilledPix.draw(0,0);
    
    maskFbo.draw(0,0);
    

//  thresholdImg.draw(0,0);

    
//  if (!contour.blobs.empty()){
//
//    createContourEdge(ofPolyline(contour.blobs.at(0).pts));
//    contourEdge->draw();
//    path.draw();
//  }
  
    
    
  ofDisableBlendMode();
    
    
  // circles are not affected by blending mode
//  for (auto circle:circles){
//      circle->draw();
//  }
    
  gui.draw();
  
}













//--------------------------------------------------------------
void ofApp::exit(){
  kinect.setCameraTiltAngle(0);
  kinect.close();
}

//--------------------------------------------------------------
void ofApp::createContourEdge(ofPolyline polyline){
    
//  delete contourEdge;
//  contourEdge = new ofxBox2dEdge();
//  contourEdge->addVertexes(polyline);
//  contourEdge->create(box2d.getWorld());


    
   
//    for( int i = 0; i < polyline.getVertices().size(); i++) {
//        if(i == 0) {
//            path.newSubPath();
//            path.moveTo(polyline.getVertices()[i] );
//        } else {
//            path.lineTo( polyline.getVertices()[i] );
//        }
//    }
//
//    path.close();
//    path.simplify();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  kinect.setCameraTiltAngle(0);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
