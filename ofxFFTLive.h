/*
 *  AudioLiveSpectrum.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFFTBase.h"

class ofxFFTLive : public ofxFFTBase
{
		
public :
		
	 ofxFFTLive();
	~ofxFFTLive();
		
	void setup		();
    void setup      ( ofSoundStream *soundStream );
	void audioIn    ( float * input, int bufferSize, int nChannels );
    
    ofSoundStream *soundStream;
};