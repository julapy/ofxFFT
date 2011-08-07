/*
 *  AudioFileSpectrum.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 24/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFFTBase.h"

class ofxFFTFile : public ofxFFTBase
{

public :
	
	 ofxFFTFile();
	~ofxFFTFile();
	
	void  setup				( string fileName );
	void  setup				( ofSoundPlayer *audio );
	void  setup				();
	
	void  update			( );
	
	void  setFrameRateSync	( bool b );
	
	ofSoundPlayer*			audio;
	
private :
	
	int						audioTotalFrames;
	bool					frameRateSync;
	int						frameStart;
	
};