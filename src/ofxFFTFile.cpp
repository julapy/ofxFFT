/*
 *  ofxFFTFile.cpp
 *  Created by lukasz karluk on 24/08/09.
 *
 */

#include "ofxFFTFile.h"

ofxFFTFile::ofxFFTFile() : ofxFFTBase() {
    //
}

ofxFFTFile::~ofxFFTFile() {
    //
}

void ofxFFTFile::setup() {
    ofxFFTBase::setup();
}

void ofxFFTFile::update() {
    float * data;
    data = ofSoundGetSpectrum(audioNoOfBands);
    
    for(int i=0; i<audioNoOfBands; i++) {
        specData[i] = data[i];
    }
    
    ofxFFTBase::update();
}
