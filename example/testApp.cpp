#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    ofSetFrameRate( 30 );
    ofSetVerticalSync( true );
    
    ofxFFTLive* fftLive;
    fftLive = new ofxFFTLive();
    fftLive->setMirrorData( false );
    fftLive->setup();
    
    fft = fftLive;
}

//--------------------------------------------------------------
void testApp::update()
{
    fft->update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    fft->draw( 10, 10 );
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