#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(239);
	ofSetWindowTitle("Insta");

	ofSetColor(39);

	// Initialize ofxBox2d
	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	// Add Circle
	for (int i = 0; i < 255; i++) {

		float radius = 12;
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(1.0, 0.63, 0.1);
		circle->setup(this->box2d.getWorld(), ofRandom(ofGetWidth() / 2), ofRandom(ofGetHeight() / 2), radius);

		this->circles.push_back(circle);

		ofColor circle_color;
		circle_color.setHsb(ofRandom(255), 239, 239);

		this->circles_color.push_back(circle_color);
	}

	this->direction = 1;
}

//--------------------------------------------------------------
void ofApp::update() {

	// Change Power Direction
	if (ofGetFrameNum() % 300 == 0) {
		
		this->direction *= -1;
	}

	// Circles Compute Power
	for (int i = 0; i < this->circles.size(); i++) {

		for (int j = i + 1; j < this->circles.size(); j++) {

			float distance = this->circles[i]->getPosition().distance(this->circles[j]->getPosition());
			if (distance < 80) {

				this->circles[i]->addForce(this->circles[i]->getPosition() - this->circles[j]->getPosition(), 0.75 * this->direction);
				this->circles[j]->addForce(this->circles[j]->getPosition() - this->circles[i]->getPosition(), 0.75 * this->direction);
			}
		}
	}

	// Compute box2d
	this->box2d.update();
}


//--------------------------------------------------------------
void ofApp::draw() {

	// Draw Circles
	for (int i = 0; i < this->circles.size(); i++) {

		float radius = this->circles[i]->getRadius();
		ofPoint point = this->circles[i]->getPosition();
		ofPoint velocity = this->circles[i]->getVelocity();
		
		ofSetColor(this->circles_color[i]);

		ofPushMatrix();
		ofTranslate(point);

		if (velocity.length() < radius) {

			ofDrawCircle(ofPoint(), radius);
		}
		else {

			float velocity_deg = atan2f(velocity.y, velocity.x) * RAD_TO_DEG + 180;
			ofBeginShape();
			for (int deg = velocity_deg + 90; deg < velocity_deg + 270; deg++) {

				ofVertex(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
			}
			ofVertex(-velocity * 2);
			ofEndShape(true);
		}

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}