/*
 *  ofxFFTFile.cpp
 *  Created by lukasz karluk on 24/08/09.
 *
 */

#include "ofxFFTFile.h"

ofxFFTFile::ofxFFTFile() : ofxFFTBase() {
    bFrameSync = false;
    frameSyncRate = 0;
    frameSyncCount = 0;
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
        frameSyncCount += 1;
		float position = frameSyncCount / (float)frameSyncTotal;
		player->setPosition(position);
	}
    
    float * data;
    data = ofSoundGetSpectrum(bufferSize);
    
    for(int i=0; i<bufferSize; i++) {
        buffer[i] = data[i];
    }
    
    ofxFFTBase::update();
}

void ofxFFTFile::startFrameSync(ofSoundPlayer * soundPlayer, int frameRate) {
    if(soundPlayer == NULL) {
        return;
    }
    
    player = soundPlayer;
    if(player->getIsPlaying()) {
        player->setPaused(false);
    } else {
        player->play();
    }
    
    float position = player->getPosition();
    
    bFrameSync = true;
    frameSyncRate = frameRate;
    frameSyncCount = 0;
    frameSyncTotal = 0;
    
#ifdef OF_SOUND_PLAYER_FMOD
    ofPtr<ofBaseSoundPlayer> playerPtr = player->getPlayer();
    ofFmodSoundPlayer * fmodPlayer = (ofFmodSoundPlayer *)playerPtr.get();
    int length = fmodPlayer->length;
    float internalFreq = fmodPlayer->internalFreq;
    float audioLengthInvFreq = length / internalFreq;
    frameSyncTotal = audioLengthInvFreq * frameSyncRate;
#endif
    
    frameSyncCount = frameSyncTotal * position;
    
    bFrameSync = (frameSyncTotal > 0);
}

void ofxFFTFile::stopFrameSync() {
    bFrameSync = false;
}