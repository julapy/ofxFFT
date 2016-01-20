/*
 *  ofxFFTFile.cpp
 *  Created by lukasz karluk on 24/08/09.
 *
 */

#include "ofxFFTFile.h"

ofxFFTFile::ofxFFTFile() : ofxFFTBase() {
    bFrameSync = false;
    frameSyncRate = 0;
    frameSyncIndex = 0;
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
    
	if(bFrameSync == true) {
		float position = frameSyncIndex / (float)(frameSyncTotal-1);
		player->setPosition(position);
	}
    
    float * data = ofSoundGetSpectrum(bufferSize);
    ofxFFTBase::audioIn(data);
    ofxFFTBase::update();
    
    if(bFrameSync == true) {
        frameSyncIndex += 1;
        bFrameSync = (frameSyncIndex < frameSyncTotal);
    }
}

void ofxFFTFile::startFrameSync(ofSoundPlayer * soundPlayer, int frameRate) {
    if(soundPlayer == NULL) {
        return;
    }
    
    player = soundPlayer;
    if(player->isPlaying()) {
        player->setPaused(false);
    } else {
        player->play();
    }
    
    float position = player->getPosition();
    
    bFrameSync = true;
    frameSyncRate = frameRate;
    frameSyncIndex = 0;
    frameSyncTotal = 0;
    
#ifdef OF_SOUND_PLAYER_FMOD
    ofPtr<ofBaseSoundPlayer> playerPtr = player->getPlayer();
    ofFmodSoundPlayer * fmodPlayer = (ofFmodSoundPlayer *)playerPtr.get();
    int length = fmodPlayer->length;
    float internalFreq = fmodPlayer->internalFreq;
    float audioLengthInvFreq = length / internalFreq;
    frameSyncTotal = audioLengthInvFreq * frameSyncRate;
#endif
    
    frameSyncIndex = (frameSyncTotal - 1) * position;
    
    bFrameSync = (frameSyncTotal > 0);
}

void ofxFFTFile::stopFrameSync() {
    bFrameSync = false;
}