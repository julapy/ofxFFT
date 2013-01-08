/*
 *  ofxFFTFile.cpp
 *  Created by lukasz karluk on 24/08/09.
 *
 */

#include "ofxFFTFile.h"

ofxFFTFile::ofxFFTFile() : ofxFFTBase() {
    bFrameSync = false;
    frameSyncRate = 0;
    frameSyncStart = 0;
    frameSyncTotal = 0;
    player = NULL;
}

ofxFFTFile::~ofxFFTFile() {
    //
}

void ofxFFTFile::setup() {
    ofxFFTBase::setup();
}

void ofxFFTFile::update() {
    
	if(bFrameSync) {
		float position = (ofGetFrameNum() - frameSyncStart) / (float)frameSyncTotal;
		player->setPosition( position );
	}
    
    float * data;
    data = ofSoundGetSpectrum(audioNoOfBands);
    
    for(int i=0; i<audioNoOfBands; i++) {
        specData[i] = data[i];
    }
    
    ofxFFTBase::update();
}

void ofxFFTFile::startFrameSync(ofSoundPlayer * soundPlayer, int frameRate) {
    if(soundPlayer == NULL) {
        return;
    }
    
    player = soundPlayer;
	player->setPosition(0.0);
    player->play();
    
    bFrameSync = true;
    frameSyncRate = frameRate;
    frameSyncStart = ofGetFrameNum();
    frameSyncTotal = 0;
    
#ifdef OF_SOUND_PLAYER_FMOD
    ofPtr<ofBaseSoundPlayer> playerPtr = player->getPlayer();
    ofFmodSoundPlayer * fmodPlayer = (ofFmodSoundPlayer *)playerPtr.get();
    int length = fmodPlayer->length;
    float internalFreq = fmodPlayer->internalFreq;
    float audioLengthInvFreq = length / internalFreq;
    frameSyncTotal = audioLengthInvFreq * frameSyncRate;
#endif
    
    bFrameSync = (frameSyncTotal > 0);
}