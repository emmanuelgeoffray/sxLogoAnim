// Soixante circuits / Emmanuel Geoffray 

#include "testApp.h"
#include "Vehicle.h"
#define PORT 12000

//--------------------------------------------------------------
void testApp::setup(){
  ofSetWindowShape(800, 600);
  ofEnableSmoothing();
  ofBackground(255);
  radius = 2;
	svg.load("soixante5.svg"); // works
	//svg.load("soixante_plein.svg"); // works
	//svg.load("heart.svg"); // works

  vehicles.resize(200);
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].setup();
    vehicles[i].setLocation(ofRandomWidth(), ofRandomHeight());
    vehicles[i].setMaxSpeed(0.1);
    vehicles[i].setMaxForce(0.001);
  }
  isDebugMode = true;


	gui.setup("panel"); // most of the time you don't need a name
	gui.add(maxForce.setup( "maxForce",0.1 , 0.1, 1000 ));
	gui.add(maxSpeed.setup( "maxSpeed",2 , 1, 400 ));
	gui.add(historyLength.setup( "historyLength",100 , 50, 1000 ));
	gui.add(drawLogo.setup("logo", true));
	gui.add(drawHistory.setup("history", true));
	gui.add(drawPath.setup("path", true));

	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
}

//--------------------------------------------------------------
void testApp::updateOsc(){

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if(m.getAddress() == "/TSPS/personUpdated/"){
      maxForce = 1000;
      cout << "person updated" << endl;
		}
		else if(m.getAddress() == "/TSPS/personWillLeave/"){
      maxForce = 0.1;
      cout << "person left" << endl;
		}
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
      //cout << msg_string << endl;
		}

	}
}

//--------------------------------------------------------------
void testApp::update(){
  updateOsc();
  vector<Vehicle>::iterator it;
  for (it = vehicles.begin(); it < vehicles.end(); it++){
    //update gui params
    it->setMaxSpeed(maxSpeed);
    //it->setMaxSpeed(ofMap(mouseY, 0, ofGetHeight(), 5, 400));
    it->setMaxForce(maxForce);
    //it->setMaxForce(ofMap(mouseY, 0, ofGetHeight(), 0.1, 1000));
    it->setHistoryLength(historyLength);

    it->follow(svg, radius);
    it->update();
    //vehicles[i].borders();
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  ofBackgroundGradient(ofColor::white, ofColor::gray);
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	//ofRotate(mouseX);
	//float scale = ofMap(mouseY, 0, ofGetHeight(), .5, 10);
	float scale = .5 ;
	ofScale(scale, scale, scale);
	ofTranslate(-svg.getWidth() / 2, -svg.getHeight() / 2);
	//ofTranslate(-0.9, 0);
  if (drawLogo){
    svg.draw();
  }
  for (unsigned int i = 0; i < vehicles.size(); i++){
    vehicles[i].draw(drawPath, drawHistory);
  }
  ofPopMatrix();

  //ofDrawBitmapString("Hit space bar to toggle debugging lines.\nClick the mouse to generate a new path.",10,ofGetHeight()-30);
  if (isDebugMode){
    gui.draw();
    ofDrawBitmapString("Nb of Particles: " +ofToString(vehicles.size()) + "\nFPS "+ofToString(ofGetFrameRate(), 0),10,ofGetHeight()-30);
  }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if (key == ' ') {
    isDebugMode = !isDebugMode;
  }
  if (key == 'f') {
    ofToggleFullscreen();
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
  if (x >= ofGetWidth()/2 - svg.getWidth()/4
    && x < ofGetWidth()/2 + svg.getWidth()/4
    && y >= ofGetHeight()/2 - svg.getHeight()/4
    && y < ofGetHeight()/2 + svg.getHeight()/4
  ) {
    x = ofMap(x, ofGetWidth()/2 - svg.getWidth()/4, ofGetWidth()/2 + svg.getWidth()/4, 0, svg.getWidth());
    y = ofMap(y, ofGetHeight()/2 - svg.getHeight()/4, ofGetHeight()/2 + svg.getHeight()/4, 0, svg.getHeight());
    Vehicle v;
    v.setup();
    v.setLocation(x,y);
    v.setMaxSpeed(2);
    v.setMaxForce(0.01);
    vehicles.push_back(v);
  }
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
