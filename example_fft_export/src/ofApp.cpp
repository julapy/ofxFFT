#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp() {

    numOfPixelsPerSoundFrame = 128;
    numOfSoundFramesPerSecond = 30;
    soundPath = "";
    soundBaseName = "";
    soundFileName = "";
    sessionDir = "";
}

ofApp::~ofApp() {
    //
}

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetFrameRate(30);
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
    
    sessionDir = ofGetTimestampString("%Y%m%d%H%M%S");
    bool bCreatedDir = ofDirectory::createDirectory(sessionDir);
    if(bCreatedDir == false) {
        return;
    }
    
    ofFile file(path);
    soundBaseName = file.getBaseName();
    soundFileName = file.getFileName();
    file.copyTo(sessionDir + "/" + soundFileName);
    
    fft.startFrameSync(&soundPlayer, numOfSoundFramesPerSecond);
    
    int numOfSoundPixelsTotal = numOfPixelsPerSoundFrame * fft.frameSyncTotal;
    int soundImageSize = sqrt(numOfSoundPixelsTotal);
    soundImageSize = ofNextPow2(soundImageSize);
    
    soundImage.allocate(soundImageSize, soundImageSize, OF_IMAGE_GRAYSCALE);
    soundImage.setColor(ofColor(0, 255));
    soundImage.update();
    
    ofRectangle screenRect(0, 0, ofGetWidth(), ofGetHeight());
    ofRectangle soundImageRect(0, 0, soundImageSize, soundImageSize);
    soundImageRect.scaleTo(screenRect, OF_ASPECT_RATIO_KEEP);
    soundImageRect.x = (int)soundImageRect.x;
    soundImageRect.y = (int)soundImageRect.y;
    soundImageRect.width = (int)soundImageRect.width;
    soundImageRect.height = (int)soundImageRect.height;
    float soundImageScale = soundImageRect.width / (float)soundImageSize;
    
    soundImageMat.makeIdentityMatrix();
    soundImageMat.preMultTranslate(ofVec3f(soundImageRect.x, soundImageRect.y));
    soundImageMat.preMultScale(ofVec3f(soundImageScale, soundImageScale, 1));
    
    ofSetFrameRate(10000);
    ofSetVerticalSync(false);
}

void ofApp::killSound() {
    
    fft.stopFrameSync();
    soundImage.clear();
}

void ofApp::saveSound() {
    
    if(soundImage.isAllocated() == false) {
        return;
    }
    
    string soundImagePath = sessionDir;
    soundImagePath += "/";
    soundImagePath += soundBaseName;
    soundImagePath += ".png";
    soundImage.save(soundImagePath);
    
    string xmlPath = sessionDir;
    xmlPath += "/";
    xmlPath += soundBaseName;
    xmlPath += ".xml";
    
    ofXml xml;
    xml.addChild("metadata");
    xml.setTo("metadata");

    xml.addChild("sound");
    xml.setValue("sound", soundFileName);
    
    xml.addChild("image");
    xml.setValue("image", soundBaseName + ".png");
    
    xml.addChild("numOfSoundFrames");
    xml.setValue("numOfSoundFrames", ofToString(fft.frameSyncTotal));
    
    xml.addChild("numOfPixelsPerSoundFrame");
    xml.setValue("numOfPixelsPerSoundFrame", ofToString(numOfPixelsPerSoundFrame));
    
    xml.save(xmlPath);
}

//--------------------------------------------------------------
void ofApp::update() {

    if(soundPath != "") {
        initSound(soundPath);
        soundPath = "";
    }
    
    if(fft.bFrameSync == false) {
        return;
    }
    
    int soundFrameIndex = fft.frameSyncIndex;

    fft.update();

    vector<float> & soundData = fft.fftData.data;
    unsigned char * soundImagePixels = soundImage.getPixels().getData();
    int soundImageIndex = soundFrameIndex * numOfPixelsPerSoundFrame;
    
    for(int i=0; i<numOfPixelsPerSoundFrame; i++) {
        float d0 = soundData[i+0];
        float d1 = soundData[i+1];
        float data = MAX(d0, d1);
        int j = soundImageIndex + i;
        soundImagePixels[j] = data * 255;
    }
    soundImage.update();
    
    bool bExportFinished = (fft.bFrameSync == false);
    if(bExportFinished == true) {

        saveSound();
        
        ofSetFrameRate(30);
        ofSetVerticalSync(true);
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
    if(fft.bFrameSync == true) {
        msg = "WRITING FRAMES: " + ofToString(fft.frameSyncIndex + 1) + " / " + ofToString(fft.frameSyncTotal);
    }
    ofDrawBitmapString(msg, 10, 20);
}

//--------------------------------------------------------------
void ofApp::exit() {
    //
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        saveSound();
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