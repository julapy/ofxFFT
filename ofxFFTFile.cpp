/*
 *  AudioFileSpectrum.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 24/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFFTFile.h"

ofxFFTFile :: ofxFFTFile() : ofxFFTBase()
{
	audio = NULL;
}

ofxFFTFile :: ~ofxFFTFile()
{
	//
}

void ofxFFTFile :: setup( string fileName )
{
	audio = new ofFmodSoundPlayer();
	audio->loadSound( fileName, false );
	audio->play();
	audio->setPosition( 0.0 );
	audio->setVolume( 1 );
	audio->setLoop( true );

	setup();
}

void ofxFFTFile :: setup( ofFmodSoundPlayer *audioPtr )
{
	audio = audioPtr;
	
	setup();
}

void ofxFFTFile :: setup ()
{
	ofxFFTBase :: setup();
	
	if( audio != NULL )
	{
		float audioLengthInvFreq = audio->length / (float)audio->internalFreq;
		audioTotalFrames = floor( audioLengthInvFreq * ofGetFrameRate() );
	}
	
	setFrameRateSync( false );
}

void ofxFFTFile :: update ()
{
	if( audio != NULL && frameRateSync )
	{
		float position;
		position = ( ofGetFrameNum() - frameStart ) / (float)audioTotalFrames;
		
		audio->setPosition( position );
	}
	
	float *data;
	data = ofSoundGetSpectrum( audioNoOfBands );
	
	for( int i=0; i<audioNoOfBands; i++)
	{
		specData[ i ] = data[ i ];
	}
	
	ofxFFTBase :: update();
}

/////////////////////////////////////////////
//	SETTERS / GETTERS.
/////////////////////////////////////////////

void ofxFFTFile :: setFrameRateSync( bool b )
{
	frameRateSync = b;
	
	frameStart = ofGetFrameNum();
}