// Seek_Arrive
// Daniel Shiffman <http://www.shiffman.net>
// Nature of Code, Spring 2011

// Two "vehicles" follow the mouse position

// Implements Craig Reynold's autonomous steering behaviors
// One vehicle "seeks"
// One vehicle "arrives"
// See: http://www.red3d.com/cwr/

// C++ / openFrameworks version by
// Soixante circuits / Emmanuel Geoffray 

#include "testApp.h"
#include "Vehicle.h"

//the ofPolyline class is perfect for a series of connect point
ofPolyline path;
float radius;

vector<Vehicle> vehicles;

//--------------------------------------------------------------
void testApp::setup(){
  ofSetWindowShape(640, 360);
  ofEnableSmoothing();
  ofBackground(255);
  radius = 20;
  newPath();

  vehicles.resize(2);
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].setup();
    vehicles[i].setLocation(ofRandomWidth(), ofRandomHeight());
    vehicles[i].setMaxSpeed(ofRandom(2, 5));
    vehicles[i].setMaxForce(ofRandom(0.05, 0.1));
  }
  isDebugMode = true;
}

//--------------------------------------------------------------
void testApp::update(){
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].follow(path, radius);
    vehicles[i].update();
    vehicles[i].borders();
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  // draw radius
  for (unsigned int i = 0; i < path.size()-1; i++){
      ofVec2f start = path[i];
      ofVec2f end = path[i+1];
      ofBeginShape();
      ofFill();
      ofSetColor(200);
      ofVec2f normal = (end - start).getRotated(90).getNormalized()*radius;
      ofVertex(start.x + normal.x, start.y + normal.y);
      ofVertex(start.x + normal.getRotated(180).x, start.y + normal.getRotated(180).y);
      ofVertex(end.x + normal.getRotated(180).x, end.y + normal.getRotated(180).y);
      ofVertex(end.x + normal.x, end.y + normal.y);
      ofEndShape(true);
  }
  ofSetColor(0);
  path.draw();

  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].draw(isDebugMode);
  }
  ofDrawBitmapString("Hit space bar to toggle debugging lines.\nClick the mouse to generate a new path.",10,ofGetHeight()-30);
}

//--------------------------------------------------------------
void testApp::newPath() {
  path.clear();
  // A path is a series of connected points
  // A more sophisticated path might be a curve
  path.addVertex(0, ofGetHeight()/2);
  path.addVertex(ofRandom(0, ofGetWidth()/2), ofRandom(0, ofGetHeight()));
  path.addVertex(ofRandom(ofGetWidth()/2, ofGetWidth()), ofRandom(0, ofGetHeight()));
  path.addVertex(ofGetWidth(), ofGetHeight()/2);
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  newPath();
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
