#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofBackground(100);
    
    fftLive.setMirrorData(false);
    fftLive.setup();
    
    fftFile.setMirrorData(false);
    fftFile.setup();
    
    soundPlayer.loadSound("sound/1085.mp3");
    soundPlayer.setLoop(true);
    soundPlayer.play();
}

//--------------------------------------------------------------
void testApp::update() {
    fftLive.update();
    fftFile.update();
}

//--------------------------------------------------------------
void testApp::draw() {
    ofSetColor(255);
    ofDrawBitmapString("AUDIO FROM MIC (LIVE)", 10, 20);
    ofDrawBitmapString("AUDIO FROM FILE (SOUND/1085.MP3)", 10, 310);
    
    fftLive.draw(10, 30);
    fftFile.draw(10, 320);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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