/*
 *  ofxFFTLive.h
 *  Created by lukasz karluk on 29/08/09.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFFTBase.h"

class ofxFFTLive : public ofxFFTBase {
  
public:

    ofxFFTLive();
    ~ofxFFTLive();
    
    void setup();
    void audioIn(float * input, int bufferSize, int nChannels);
    
    ofSoundStream * soundStream;
};