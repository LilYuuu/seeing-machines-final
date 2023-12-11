#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    contourFbo.allocate(600, 480, GL_RGBA);
//    contourFbo.allocate(PROJECTOR_RESOLUTION_X, PROJECTOR_RESOLUTION_Y);
    warpedImg.allocate(600,480, OF_IMAGE_COLOR);
    
    srcPoints.push_back(glm::vec2(0, 0));
    srcPoints.push_back(glm::vec2(1, 0));
    srcPoints.push_back(glm::vec2(0, 1));
    srcPoints.push_back(glm::vec2(1, 1));

    dstPoints.push_back(glm::vec2(0, 0));
    dstPoints.push_back(glm::vec2(1, 0));
    dstPoints.push_back(glm::vec2(0, 1));
    dstPoints.push_back(glm::vec2(1, 1));

    activePoint = -1;
    homographyReady = false;

    adjustMapping.set("Adjust Mapping", true);

    // Setup the parameters.
    nearThreshold.set("Near Threshold", 0.001f, 0.0f, 0.1f);
    farThreshold.set("Far Threshold", 0.02f, 0.0f, 0.1f);

    // Setup the gui.
    gui.setup("Depth Threshold", "settings.json");
    gui.add(nearThreshold);
    gui.add(farThreshold);
    gui.add(adjustMapping);
//    gui.add(projectWarped);
 
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    // zero the tilt on startup
    angle = 0;
    kinect.setCameraTiltAngle(angle);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    

  kinect.update();
  
  if (kinect.isFrameNew()){
      
//    color.setFromPixels(kinect.getPixels());

    ofFloatPixels rawDepthPix = kinect.getRawDepthPixels();
    ofFloatPixels thresholdNear, thresholdFar, thresholdResult;

    ofxCv::threshold(rawDepthPix, thresholdNear, nearThreshold);
    ofxCv::threshold(rawDepthPix, thresholdFar, farThreshold, true);
    ofxCv::bitwise_and(thresholdNear, thresholdFar, thresholdResult);
     
      
    //draw the greyscale img from result pixel
    thresholdImg.setFromPixels(thresholdResult);
    
    int minimum = thresholdImg.getWidth() * thresholdImg.getHeight() * 0.01;
    int maximum = thresholdImg.getWidth() * thresholdImg.getHeight() * 1;
    // find contour from the greyscale result
    contour.findContours(thresholdImg, minimum, maximum, 5, false);
      
  }
    
    //clean Fbo
    contourFbo.begin();
    ofClear(255,255,255, 0);
    contourFbo.end();
    
    //draw the blobs to Fbo
    contourFbo.begin();
     for (int i = 0; i < contour.nBlobs; i++){
        ofxCvBlob& blob = contour.blobs[i];
        blob.draw(0, 0);
    }
    contourFbo.end();
    
    //read Fbo to pixel
    contourFbo.readToPixels(contourPixels);
    
    //add blur in pixel
    //ofxCv::erode(contourFilledPix, 5);
    ofxCv::dilate(contourPixels, 5);
    ofxCv::medianBlur(contourPixels, 5);
    ofxCv::blur(contourPixels, 50);
    
    contourImg.setFromPixels(contourPixels);
    contourImg.update();
    
    //clean Fbo
    contourFbo.begin();
    ofClear(255,255,255, 0);
    contourFbo.end();
    
    //read pixel to texture
    contourFbo.begin();
     contourTexture.loadData(contourPixels);
     contourTexture.draw(0,0);
    contourFbo.end();
    
    if (adjustMapping)
    {
      // Copy points from glm to cv format.
      std::vector<cv::Point2f> cvSrcPoints;
      std::vector<cv::Point2f> cvDstPoints;
      for (int i = 0; i < srcPoints.size(); i++)
      {
        // Scale points to projector dimensions.
        cvSrcPoints.push_back(cv::Point2f(srcPoints[i].x * 600, srcPoints[i].y * 480));
        cvDstPoints.push_back(cv::Point2f(dstPoints[i].x * 600, dstPoints[i].y * 480));
          
        
      }

      // Generate a homography from the two sets of points.
      homographyMat = cv::findHomography(cv::Mat(cvSrcPoints), cv::Mat(cvDstPoints));
      homographyReady = true;
    }
    
    if (homographyReady)
     {
         warpedImgFbo.begin();
         ofClear(255,255,255, 0);
         warpedImgFbo.end();
         
       // Read the contourFBO to pixels.
         contourFbo.readToPixels(warpedImgPixels);
         
       // Warp the pixels into a new image.
       warpedImg.setFromPixels(warpedImgPixels);
       ofxCv::warpPerspective(warpedImgPixels, warpedImg, homographyMat, CV_INTER_LINEAR);
       warpedImg.update();
         
    
       warpedImgTexture.loadData(warpedImg);
       warpedImgFbo.begin();
       warpedImgTexture.draw(0,0);
         
       warpedImgFbo.end();
     }
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
  
  ofBackground(255);

//  ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
  contourTexture.draw(0,0);
  

    
    if (homographyReady)
    {
      // Draw warped image on the right.
      warpedImgTexture.draw( 640, 0);
        
 
      ofPushMatrix();
      ofTranslate(640,0);
      ofRotateZDeg(22.49);
      warpedImgTexture.draw( 0, 0);

      ofPopMatrix();
//      ofDisableBlendMode();
        
    }
    
    
    if (adjustMapping)
    {
      // Draw mapping points.
      for (int i = 0; i < srcPoints.size(); i++)
      {
        ofPushStyle();
//        ofSetColor(255, 255, 0);
        
//        glm::vec2 srcPt = glm::vec2(ofMap(srcPoints[i].x, 0, 1, 0, 640),
//                                    ofMap(srcPoints[i].y, 0, 1, 0, 480));
//        ofDrawCircle(srcPt, 10);

        ofSetColor(255, 0, 0);
        glm::vec2 dstPt = glm::vec2(ofMap(dstPoints[i].x, 0, 1, 640, 1280),
                                    ofMap(dstPoints[i].y, 0, 1, 0, 480));
        ofDrawCircle(dstPt, 10);
        ofSetColor(255, 0, 255);
//        ofDrawLine(srcPt, dstPt);
        ofPopStyle();
          
          
      }
    }
    
    
//    ofDisableBlendMode();
  gui.draw();
  
}



void ofApp::drawProjector(ofEventArgs& args)
{
  
    ofBackground(255);
//  ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

    
    if (homographyReady)
      {
//      testing draw different versions of Img, texture, Fbo on the projector
        warpedImg.draw(200, 200);
        warpedImgTexture.draw(200,200);
          
//      warpedImgFbo.draw(1280,0);
//      contourImg.draw(0,480);
//      contourTexture.draw(640,480);
//      contourFbo.draw(1280,480);
      }
      else
      {
          contourFbo.draw(0, 0);
      }

      if (adjustMapping)
      {
        // Draw mapping dst points.
        for (int i = 0; i < dstPoints.size(); i++)
        {
            ofPushStyle();
          ofSetColor(255, 0, 0);
          glm::vec2 dstPt = glm::vec2(dstPoints[i].x * PROJECTOR_RESOLUTION_X, dstPoints[i].y * PROJECTOR_RESOLUTION_Y);
          ofDrawCircle(dstPt, 10);
            ofPopStyle();
        }
      }
    
//    ofDisableBlendMode();
}


//-------------------

void ofApp::mouseDragged(int x, int y, int button)
{
  if (adjustMapping)
  {
    if (activePoint > -1)
    {
      // Move the active Point under the mouse, but stick to edges.
      // false to make it can move outside the edges
      glm::vec2 normPt = glm::vec2(ofMap(x, 640, 1280, 0, 1, false),
                                   ofMap(y, 0, 480, 0, 1, false));
      dstPoints[activePoint] = normPt;
    }
  }
  
}

void ofApp::mousePressed(int x, int y, int button)
{
  if (adjustMapping)
  {
    // Try to snap to a dst point.
    for (int i = 0; i < dstPoints.size(); i++)
    {
      glm::vec2 dstPt = glm::vec2(ofMap(dstPoints[i].x, 0, 1, 640, 1280), ofMap(dstPoints[i].y, 0, 1, 0, 480));
      glm::vec2 mousePt = glm::vec2(x, y);
      if (glm::distance(dstPt, mousePt) < 20)
      {
        // Close enough, let's grab this one.
        activePoint = i;
        break;
      }
    }
  }
  else
  {
    // Simply call the mouseDragged handler.
    mouseDragged(x, y, button);
  }
}

void ofApp::mouseReleased(int x, int y, int button)
{
  if (adjustMapping)
  {
    activePoint = -1;
  }
}


void ofApp::keyPressed (int key) {
    switch (key) {case OF_KEY_UP:
            angle++;
            if(angle>30) angle=30;
            kinect.setCameraTiltAngle(angle);
            break;
            
        case OF_KEY_DOWN:
            angle--;
            if(angle<-30) angle=-30;
            kinect.setCameraTiltAngle(angle);
            break;
    }
}










