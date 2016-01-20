#pragma once

#include "ofMain.h"
#include "ofxFFTFile.h"

class ofApp : public ofBaseApp{

public:
    
    ofApp();
    ~ofApp();
    
    void setup();

    void initSound(string path);
    void killSound();
    void saveSound();
    
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofxFFTFile fft;
    ofSoundPlayer soundPlayer;
    ofImage soundImage;
    ofMatrix4x4 soundImageMat;

    int numOfPixelsPerSoundFrame;
    int numOfSoundFramesPerSecond;
    string soundPath;
    string soundBaseName;
    string soundFileName;
    string sessionDir;
};
