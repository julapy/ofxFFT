#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() {
    
    fps = 30;
    soundPath = "";
    numOfPixelsPerSoundFrame = 128;
    bExport = false;
}

ofApp::~ofApp() {
    //
}

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetFrameRate(fps);
    ofSetVerticalSync(true);
    ofBackground(20);
    
    fft.setMirrorData(false);
    fft.setup();
}

void ofApp::initSound(string path) {
    
    killSound();
    
    bool bLoaded = soundPlayer.load(path);
    if(bLoaded == false) {
        return;
    }
    
    soundPlayer.play();
    
    fft.startFrameSync(&soundPlayer, fps);
    
    int numOfPixelsTotal = numOfPixelsPerSoundFrame * fft.frameSyncTotal;
    int soundImageSize = sqrt(numOfPixelsTotal);
    soundImageSize = ofNextPow2(soundImageSize);
    
    soundImage.allocate(soundImageSize, soundImageSize, OF_IMAGE_GRAYSCALE);
    soundImage.setColor(ofColor(0, 255));
    soundImage.update();
    
    int soundImageW = soundImage.getWidth();
    int soundImageH = soundImage.getHeight();
    ofRectangle screenRect(0, 0, ofGetWidth(), ofGetHeight());
    ofRectangle soundImageRect(0, 0, soundImageW, soundImageH);
    soundImageRect.scaleTo(screenRect, OF_ASPECT_RATIO_KEEP);
    soundImageRect.x = (int)soundImageRect.x;
    soundImageRect.y = (int)soundImageRect.y;
    soundImageRect.width = (int)soundImageRect.width;
    soundImageRect.height = (int)soundImageRect.height;
    float soundImageScale = soundImageRect.width / (float)soundImageW;
    
    soundImageMat.makeIdentityMatrix();
    soundImageMat.preMultTranslate(ofVec3f(soundImageRect.x, soundImageRect.y));
    soundImageMat.preMultScale(ofVec3f(soundImageScale, soundImageScale, 1));
    
    bExport = true;
}

void ofApp::killSound() {
    
    bExport = false;
}

void ofApp::saveSoundImage() {
    soundImage.save("image.png");
}

//--------------------------------------------------------------
void ofApp::update() {

    if(soundPath != "") {
        initSound(soundPath);
        soundPath = "";
    }
    
    if(bExport == false) {
        return;
    }
    
    fft.update();

    vector<float> & soundData = fft.fftData.data;
    unsigned char * soundImagePixels = soundImage.getPixels().getData();
    int soundImageIndex = (fft.frameSyncCount - 1) * numOfPixelsPerSoundFrame;
    
    for(int i=0; i<numOfPixelsPerSoundFrame; i++) {
        float d0 = soundData[i+0];
        float d1 = soundData[i+1];
        float data = MAX(d0, d1);
        int j = soundImageIndex + i;
        soundImagePixels[j] = data * 255;
    }
    soundImage.update();
    
    bool bExportFinished = (fft.frameSyncCount == fft.frameSyncTotal);
    if(bExportFinished == true) {

        saveSoundImage();
        bExport = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {

    ofSetColor(255);
    
    if(soundImage.isAllocated() == true) {

        ofPushMatrix();
        ofMultMatrix(soundImageMat);
        soundImage.draw(0, 0);
        ofPopMatrix();
    }
    
    int fftW = 512;
    int fftH = 256;
    int fftPad = 10;
    fft.draw(fftPad, ofGetHeight() - fftH - fftPad, fftW, fftH);
    
    string msg = "DRAG AND DROP IN AUDIO FILE";
    if(bExport == true) {
        msg = "EXPORTING FRAMES: " + ofToString(fft.frameSyncCount) + " / " + ofToString(fft.frameSyncTotal);
    }
    ofDrawBitmapString(msg, 10, 20);
}

//--------------------------------------------------------------
void ofApp::exit() {
    saveSoundImage();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        saveSoundImage();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    for(int i=0; i<dragInfo.files.size(); i++) {
        soundPath = dragInfo.files[i];
    }
}