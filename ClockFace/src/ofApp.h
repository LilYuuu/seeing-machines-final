#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup() override;
		void update() override;
		void draw() override;
		void exit() override;
    
        int currHr;
        int currMin;
        float currSec;
        int oriSec;
    
        float hrHandRad = 0.0;
        float minHandRad = 0.0;
        float secHandRad = 0.0;
        float oriSecHandRad = 0.0;

		void drawClockHand(string hand, float rotation);
		
};
