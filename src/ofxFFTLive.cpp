/*
 *  ofxFFTLive.cpp
 *  Created by lukasz karluk on 29/08/09.
 *
 */

#include "ofxFFTLive.h"

ofxFFTLive::ofxFFTLive() : ofxFFTBase() {
    soundStream = NULL;
}

ofxFFTLive::~ofxFFTLive() {
    if(soundStream) {
        delete soundStream;
        soundStream = NULL;
    }
}

void ofxFFTLive::setup() {
    setup(new ofSoundStream());
}

void ofxFFTLive::setup(ofSoundStream * soundStream) {
    this->soundStream = soundStream;
    soundStream->setup(
        this,                   // callback obj.
        0,                      // out channels.
        1,                      // in channels.
        44100,                  // sample rate.
        getNoOfBands(),         // buffer size.
        4                       // number of buffers.
    );
}

void ofxFFTLive::audioIn(float * input, int bufferSize, int nChannels) {
    mutex.lock();
    memcpy(specData, input, sizeof(float) * bufferSize);
    mutex.unlock();
}