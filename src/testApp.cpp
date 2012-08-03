// Soixante circuits / Emmanuel Geoffray 

#include "testApp.h"
#include "Vehicle.h"


//--------------------------------------------------------------
void testApp::setup(){
  ofSetWindowShape(1900, 1000);
  ofEnableSmoothing();
  ofBackground(255);
  radius = 2;
	svg.load("soixante_plein.svg"); // works

  vehicles.resize(2);
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].setup();
    vehicles[i].setLocation(ofRandomWidth(), ofRandomHeight());
    vehicles[i].setMaxSpeed(0.1);
    vehicles[i].setMaxForce(0.001);
  }
  isDebugMode = true;
}

//--------------------------------------------------------------
void testApp::update(){
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].follow(svg, radius);
    vehicles[i].update();
    //vehicles[i].borders();
  }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	/*ofRotate(mouseX);*/
	float scale = ofMap(mouseY, 0, ofGetHeight(), .5, 10);
	//ofScale(scale, scale, scale);
	ofTranslate(-svg.getWidth() / 2, -svg.getHeight() / 2);
	//ofTranslate(-0.9, 0);
  if (isDebugMode){
    svg.draw();
  }
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].draw(isDebugMode);
  }
  ofPopMatrix();

  ofDrawBitmapString("Hit space bar to toggle debugging lines.\nClick the mouse to generate a new path.",10,ofGetHeight()-30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if (key == ' ') {
    isDebugMode = !isDebugMode;
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  Vehicle v;
  v.setup();
  v.setLocation(x, y);
  v.setMaxSpeed(2);
  v.setMaxForce(0.01);
  vehicles.push_back(v);

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
