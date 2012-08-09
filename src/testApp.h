#pragma once

#include "ofMain.h"
#include "ofxSVGTiny.h"
#include "Vehicle.h"
#include "ofxGui.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void updateOsc();

    bool isDebugMode;
		ofxSVGTiny svg;
    float radius;
    vector<Vehicle> vehicles;
    ofxFloatSlider maxSpeed, maxForce, historyLength;
    ofxToggle drawLogo, drawHistory, drawPath;
    ofxPanel gui;
    ofxOscReceiver receiver;
};
