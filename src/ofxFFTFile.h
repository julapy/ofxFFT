/*
 *  ofxFFTFile.h
 *  Created by lukasz karluk on 24/08/09.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFFTBase.h"

class ofxFFTFile : public ofxFFTBase {

public:
 
    ofxFFTFile();
    ~ofxFFTFile();
    
    void setup(ofBaseSoundPlayer * soundPlayer);
    void setup();
    
    void update();
    
};