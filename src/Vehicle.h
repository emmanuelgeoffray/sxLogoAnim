#pragma once

#include "ofMain.h"
#include "ofxSVGTiny.h"

class Vehicle {

  public:
    void setup(){
      location.set(30, 30);
      velocity.set(0, 0);
      acceleration.set(0, 0);
      r = 6;
      maxSpeed = 4;
      maxForce = 0.1;
    }

    // Method to update location
    void update(){
      // Update velocity
      velocity += acceleration;
      // Limit speed
      velocity.limit(maxSpeed);
      location += velocity;
      // Reset acceleration to 0 each cycle
      acceleration *= 0;
      
      history.push_back(location);
      while (history.size() > historyLength){
        history.pop_front();
      }
    }

    void applyForce(const ofVec2f & force) {
      // We could add mass here if we want A = F / M
      acceleration += force;
    }

    // This function implements Craig Reynolds' path following algorithm
    // http://www.red3d.com/cwr/steer/PathFollow.html
    void follow(ofxSVGTiny & svg, float radius) {

      // Predict location 25 (arbitrary choice) frames ahead
      ofVec2f predict = velocity;
      predict.normalize();
      predict *= 25;
      predictLoc = location + predict;

      // Now we must find the normal to the path from the predicted location
      // We look at the normal for each line segment and pick out the closest one
      float worldRecord = 100000000000;  // Start with a very high record distance that can easily be beaten

      // Loop through all path
      for (int j = 0; j < svg.getNumPath(); j++){
        vector<ofPolyline> &lines = svg.getPathAt(j).getOutline();
        for (unsigned int k = 0; k < lines.size(); k++)
        {
          ofPolyline  & p = lines[k];
	
          // Loop through all points of the path
          for (unsigned int i = 0; i < p.size()-1; i++) {

            // Look at a line segment
            ofVec3f& a = p[i];
            ofVec3f& b = p[i+1];

            // Get the normal point to that line
            ofVec2f normalPointToThatLine = getNormalPoint(predictLoc, a, b);

            // This only works because we know our path goes from left to right
            if (isInside(normalPointToThatLine, a, b)){

              // How far away are we from the path?
              float distance = (predictLoc - normalPointToThatLine).length();
              // Did we beat the record and find the closest line segment?
              if (distance < worldRecord) {
                worldRecord = distance;
                // If so the target we want to steer towards is the normal
                normalPoint = normalPointToThatLine;

                // Find target point a little further ahead of normal
                dir = b - a;
                dir.normalize();
                dir *= 10;  // This could be based on velocity instead of just an arbitrary 10 pixels
                target = normalPoint + dir;
              }
            }
          }
        }
      }
      // Only if the distance is greater than the path's radius do we bother to steer
      //if (worldRecord > radius) {
        seek(target);
        // for debug
        isSeeking = true;
      //} else isSeeking = false;
    }

    bool isInside(const ofVec2f & x, const ofVec3f & a, const ofVec3f & b){
      ofVec2f aa;
      ofVec2f bb;
      aa.x = min(a.x, b.x);
      aa.y = min(a.y, b.y);
      bb.x = max(a.x, b.x);
      bb.y = max(a.y, b.y);
      return x.x >= aa.x && x.x <=bb.x && x.y >= aa.y && x.y <=bb.y;
    }



    // A function to get the normal point from a point (p) to a line segment (a-b)
    // This function could be optimized to make fewer new Vector objects
    ofVec2f getNormalPoint(const ofVec2f & p, const ofVec3f & a, const ofVec3f & b) {
      // Vector from a to p
      ofVec2f ap = p - a;
      // Vector from a to b
      ofVec2f ab = b - a;
      ab.normalize(); // Normalize the line
      // Project vector "diff" onto line by using the dot product
      ab *= ap.dot(ab);
      ab += a;
      return ab;
    }

    // Wraparound
    void borders() {
      if (location.x < -r) location.x = ofGetWidth()+r;
      if (location.y < -r) location.y = ofGetHeight()+r;
      if (location.x > ofGetWidth()+r) location.x = -r;
      if (location.y > ofGetHeight()+r) location.y = -r;
    }

    void boundaries() {
      float d = 25;

      ofVec2f desired = ofVec2f(0,0);

      if (location.x < d) {
        desired = ofVec2f(maxSpeed, velocity.y);
      } 
      else if (location.x > ofGetWidth() -d) {
        desired = ofVec2f(-maxSpeed, velocity.y);
      } 

      if (location.y < d) {
        desired = ofVec2f(velocity.x, maxSpeed);
      } 
      else if (location.y > ofGetHeight()-d) {
        desired = ofVec2f(velocity.x, -maxSpeed);
      } 

      if (desired != ofVec2f(0,0)) {
        desired.normalize();
        desired *= maxSpeed;
        ofVec2f steer = desired - velocity;
        steer.limit(maxForce);
        applyForce(steer);
      }
    }  

    // A method that calculates a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    void arrive(const ofVec2f & target) {
      ofVec2f desired = target - location;  // A vector pointing from the location to the target
      float d = desired.length();
      // Normalize desired and scale with arbitrary damping within 100 pixels
      desired.normalize();
      if (d < 100) {
        float m = ofMap(d, 0, 100, 0, maxSpeed);
        desired *= m;
      } else {
        desired *= maxSpeed;
      }

      // Steering = Desired minus velocity
      ofVec2f steer = desired - velocity;
      steer.limit(maxForce);  // Limit to maximum steering force

      applyForce(steer);
    }

    // A method that calculates a steering force towards a target
    // STEER = DESIRED MINUS VELOCITY
    void seek(const ofVec2f & target) {
      ofVec2f desired = target - location;  // A vector pointing from the location to the target

      // Normalize desired and scale to maximum speed
      desired.normalize();
      desired *= maxSpeed;
      // Steering = Desired minus velocity
      ofVec2f steer = desired - velocity;
      steer.limit(maxForce);  // Limit to maximum steering force

      applyForce(steer);
    }

    void draw( bool debug = false, bool drawHistory = false){
      // Draw the debugging stuff
      if (debug) {
        ofFill();
        ofSetColor(0);
        ofLine(location.x, location.y, predictLoc.x, predictLoc.y);
        ofEllipse(predictLoc.x, predictLoc.y, 4, 4);

        // Draw normal location
        ofSetColor(0);
        ofLine(predictLoc.x, predictLoc.y, normalPoint.x, normalPoint.y);
        ofSetColor(0, 0, 255);
        ofEllipse(normalPoint.x, normalPoint.y, 4, 4);
        if (isSeeking) ofSetColor(255, 0, 0);
        else ofSetColor(0);
        ofEllipse(target.x+dir.x, target.y+dir.y, 8, 8);
      }

      // Draw history
      ofSetColor(0);
      ofEllipse(location.x, location.y, 1, 1);
      if (drawHistory){ 
         ofNoFill();
         ofBeginShape();
         for (unsigned int i = 0; i < history.size(); i++){
         ofVertex(history[i].x, history[i].y);
         }
         ofEndShape();
      }
       

      // Draw a triangle rotated in the direction of velocity
      /*
      float theta = ofVec2f(1, 0).angle(velocity) + 90;
      ofPushMatrix();
      ofTranslate(location.x,location.y);
      ofRotate(theta);
      ofBeginShape();
      ofFill();
      ofSetColor(127);
      ofVertex(0, -r*2);
      ofVertex(-r, r*2);
      ofVertex(r, r*2);
      ofEndShape(true);
      // stroke
      ofBeginShape();
      ofNoFill();
      ofSetColor(0);
      ofVertex(0, -r*2);
      ofVertex(-r, r*2);
      ofVertex(r, r*2);
      ofEndShape(true);

      ofPopMatrix();
      */
    }

    void setLocation( float x, float y){
      location.set(x, y);
    }

    void setVelocity( float x, float y){
      velocity.set(x, y);
    }

    void setMaxForce( float f){maxForce = f;}
    void setMaxSpeed( float s){maxSpeed = s;}
    void setHistoryLength( float h){historyLength = h;}

    const ofVec2f& getLocation() const{
      return location;
    }

    const ofVec2f& getVelocity() const{
      return velocity;
    }

  private:
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    float r;
    float maxForce;
    float maxSpeed;
    unsigned int historyLength;

    deque<ofVec2f> history;
    
    //for debug
    ofVec2f predictLoc;
    ofVec2f normalPoint;
    ofVec2f target;
    ofVec2f dir;
    bool isSeeking;
};
