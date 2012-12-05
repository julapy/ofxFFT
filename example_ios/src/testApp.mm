#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(127,127,127);
    ofSetOrientation(OF_ORIENTATION_90_LEFT);

    fftLive.setMirrorData(false);
    fftLive.setup();
}

//--------------------------------------------------------------
void testApp::update(){
    fftLive.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    int pad = 10;
	fftLive.draw(pad, pad, ofGetWidth() - pad * 2, ofGetHeight() - pad * 2);
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

