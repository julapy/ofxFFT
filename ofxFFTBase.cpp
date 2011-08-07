/*
 *  AudioAbstract.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 29/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxFFTBase.h"

ofxFFTBase ::  ofxFFTBase() 
{
    specData        = NULL;
    fftMagnitude    = NULL;
    fftPhase        = NULL;
    fftPower        = NULL;
	fftFreq         = NULL;

    setMaxDecay( 0.995 );
	setPeakDecay( 0.96 );
	setThreshold( 0.5 );
	setMirrorData( false );
	
	renderBorder = 1;
    
	audioNoOfBands		= OFX_FFT_NO_OF_BANDS;
	audioNoOfBandsHalf	= (int)( audioNoOfBands * 0.5 );
    
    killFFT();
	initFFT();
	initAudioData( rawData, audioNoOfBandsHalf );
	initAudioData( fftData, audioNoOfBandsHalf );
}

ofxFFTBase :: ~ofxFFTBase() 
{
    killFFT();
}

void ofxFFTBase :: setup ()
{
    //
}

void ofxFFTBase :: update() 
{
    mutex.lock();
	myfft.powerSpectrum( 0, audioNoOfBandsHalf, specData, audioNoOfBands, fftMagnitude, fftPhase, fftPower, &fftAveragePower );
    mutex.unlock();
	
    updateAudioData( fftData, fftMagnitude );
    
	if( bMirrorData )
        mirrorAudioData( fftData );
}

//////////////////////////////////////////////////////
//	FFT.
//////////////////////////////////////////////////////

void ofxFFTBase :: initFFT ()
{
	specData		= new float[ audioNoOfBands ];
	
	fftMagnitude	= new float[ audioNoOfBandsHalf ];
	fftPhase		= new float[ audioNoOfBandsHalf ];
	fftPower		= new float[ audioNoOfBandsHalf ];
	fftFreq			= new float[ audioNoOfBandsHalf ];
	
	for( int i=0; i<audioNoOfBandsHalf; i++ )
	{
		specData[ i ]		= 0;
		fftMagnitude[ i ]	= 0;
		fftPhase[ i ]		= 0;
		fftPower[ i ]		= 0;
        fftFreq[ i ]        = 0;
	}
}

void ofxFFTBase :: killFFT() 
{
    if( specData )
    {
        delete[] specData;
        specData = NULL;
    }
    
    if( fftMagnitude )
    {
        delete[] fftMagnitude;
        fftMagnitude = NULL;
    }
    
    if( fftPhase )
    {
        delete[] fftPhase;
        fftPhase = NULL;
    }
    
    if( fftPower )
    {
        delete[] fftPower;
        fftPower = NULL;
    }
    
    if( fftFreq )
    {
        delete[] fftFreq;
        fftFreq = NULL;
    }
}

//////////////////////////////////////////////////////
//	AUDIO DATA.
//////////////////////////////////////////////////////

void ofxFFTBase :: initAudioData	( ofxFFTData &audioData, int dataSize )
{
	audioData.size = dataSize;
	
	audioData.data.resize( dataSize, 0 );
	audioData.dataNorm.resize( dataSize, 0 );
	audioData.dataMax.resize( dataSize, 0 );
	audioData.dataPeak.resize( dataSize, 0 );
	audioData.dataCut.resize( dataSize, 0 );
	
	audioData.linearEQIntercept	= 1.0;
	audioData.linearEQSlope		= 0.0;
}

void ofxFFTBase :: updateAudioData ( ofxFFTData &audioData, float *dataNew )
{
	audioData.data.clear();
	audioData.data.resize( audioData.size, 0 );
	
	copy( dataNew, dataNew + audioData.size, audioData.data.begin() );
	
    audioData.peakValue = 0;
    audioData.peakAverage = 0;
    
	for( int i=0; i<audioData.size; i++ )
	{
        if( audioData.peakValue < audioData.data[ i ] )
            audioData.peakValue = audioData.data[ i ];
    }
	
	for( int i=0; i<audioData.size; i++ )
	{
		float p = i / (float)( audioData.size - 1 );
		
		float dataVal;
		dataVal  = audioData.data[ i ];												// use magnitude for fft data.
		dataVal *= audioData.linearEQIntercept + p * audioData.linearEQSlope;		// scale value.
        
        float dataPeakRatio;
        dataPeakRatio = dataVal / audioData.peakValue;
		
		float dataMaxVal;
		dataMaxVal = audioData.dataMax[ i ];
		
		if( dataMaxVal < dataVal )
		{
			dataMaxVal = dataVal;
		}
		
		float dataNormVal;
		dataNormVal = 0;
		if( dataMaxVal > 0 )
			dataNormVal = dataVal / dataMaxVal;						// normalise data between 0 and 1.
        
        if( dataVal < 0.1 )
            dataNormVal = 0;
        
		dataMaxVal *= audioData.maxDecay;							// decay the max value.
		
		audioData.dataNorm[ i ]		= dataNormVal;
		
		audioData.dataMax[ i ]		= dataMaxVal;
		
		float dataPeakVal;
		dataPeakVal  = audioData.dataPeak[ i ];
		dataPeakVal *= audioData.peakDecay;							// decay peak value.
		
		if( dataPeakVal < dataNormVal )								// check if new peak.
			dataPeakVal = dataNormVal;
		
		audioData.dataPeak[ i ]		= dataPeakVal;
		
		audioData.peakAverage		+= dataPeakVal;					// sum of all peaks.
		
		int dataCutVal;												// switch data (on/off).
		if( dataPeakVal < audioData.cutThreshold )
			dataCutVal = 1;
		else
			dataCutVal = 0;
		
		audioData.dataCut[ i ]		= dataCutVal;
	}
	
	audioData.peakAverage /= audioData.size;
}

void ofxFFTBase :: mirrorAudioData ( ofxFFTData &audioData )
{
	int audioDataSizeHalf;
	audioDataSizeHalf = (int)( audioData.size * 0.5 );
	
	for( int i=0; i<audioDataSizeHalf; i++ )
	{
		int j = audioData.size - 1;
		
		audioData.dataNorm[ j - i ] = audioData.dataNorm[ i ];
		audioData.dataMax[ j - i ]	= audioData.dataMax[ i ];
		audioData.dataPeak[ j - i ]	= audioData.dataPeak[ i ];
		audioData.dataCut[ j - i ]	= audioData.dataCut[ i ];
	}
}

void ofxFFTBase :: resetAudioData ( ofxFFTData &audioData )
{
	audioData.data.resize( audioData.size );
	audioData.dataNorm.resize( audioData.size );
	audioData.dataMax.resize( audioData.size );
	audioData.dataPeak.resize( audioData.size );
	audioData.dataCut.resize( audioData.size );
}

//////////////////////////////////////////////////////
//	SETTERS / GETTERS.
//////////////////////////////////////////////////////

void ofxFFTBase :: setNoOfBands( int value )
{
	int audioNoOfBandsNew;
	audioNoOfBandsNew	= ofNextPow2( value );
	
	if( audioNoOfBands == audioNoOfBandsNew )
		return;
	
	audioNoOfBands		= OFX_FFT_NO_OF_BANDS;
	audioNoOfBandsHalf	= (int)( audioNoOfBands * 0.5 );
    
    killFFT();
	initFFT();
	initAudioData( rawData, getNoOfBands() );
	initAudioData( fftData, getNoOfBands() );
}

int ofxFFTBase :: getNoOfBands()
{
	return audioNoOfBands;
}

void ofxFFTBase :: setThreshold( float value )
{
	float cutThreshold;
	cutThreshold = cutThreshold = MIN( 1, MAX( 0, value ) );
	
	rawData.cutThreshold = cutThreshold;
	fftData.cutThreshold = cutThreshold;
}

float ofxFFTBase :: getThreshold()
{
	return fftData.cutThreshold;
}

float ofxFFTBase :: getAveragePeak()
{
	return fftData.peakAverage;
}

void ofxFFTBase :: setPeakDecay ( float value )
{
	value = MIN( 1, MAX( 0, value ) );
	
	rawData.peakDecay = value;
	fftData.peakDecay = value;
}

float ofxFFTBase :: getPeakDecay ()
{
	return fftData.peakDecay;
}

void ofxFFTBase :: setMaxDecay ( float value )
{
	value = MIN( 1, MAX( 0, value ) );
	
	rawData.maxDecay = value;
	fftData.maxDecay = value;
}

float ofxFFTBase :: getMaxDecay ()
{
	return fftData.maxDecay;
}

void ofxFFTBase :: setMirrorData ( bool value )
{
	bMirrorData = value;
}

//////////////////////////////////////////////////////
//	GET VARIOUS DATA SAMPLES.
//////////////////////////////////////////////////////

void ofxFFTBase :: getFftData ( float *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBandsHalf - 1 ) );
		float v		= fftData.dataNorm[ j ];
		data[ i ]	= v;
	}
}

void ofxFFTBase :: getFftPeakData ( float *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBandsHalf - 1 ) );
		float v		= fftData.dataPeak[ j ];
		data[ i ]	= v;
	}
}

void ofxFFTBase :: getGlitchData( int *data, int length )
{
	for( int i=0; i<length; i++ )
	{
		int j		= (int)( ( i / (float)( length - 1 ) ) * ( audioNoOfBandsHalf - 1 ) );
		float v		= fftData.dataCut[ j ];
		data[ i ]	= v;
	}
}

//////////////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////////////

void ofxFFTBase :: draw( int x, int y ) 
{
	draw( x, y, OFX_FFT_WIDTH, OFX_FFT_HEIGHT );
}

void ofxFFTBase :: draw( int x, int y, int width, int height )
{
    renderSingleBandWidth = width / (float)audioNoOfBandsHalf;
    
	glPushMatrix();
	glTranslatef( x, y, 0 );
	{
        drawData( fftData, width, height );
	}
	glPopMatrix();
}

void ofxFFTBase :: drawData ( const ofxFFTData &audioData, int width, int height )
{
	drawBg( audioData, width, height );
	drawGlitchData( audioData, width, height );
//	drawFftData( audioData, width, height );			// this is audio data before its been normalised, good for debugging.
	drawFftNormData( audioData, width, height );
	drawFftPeakData( audioData, width, height );
	drawThresholdLine( audioData, width, height );
    drawBorder( width, height );
}

void ofxFFTBase :: drawBg ( const ofxFFTData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
    int c = 240;
    
    ofFill();
	ofSetColor( c, c, c );
	ofRect( bx, by, w, h );
}

void ofxFFTBase :: drawBorder( int w, int h )
{
	int bx, by;		// border.
	bx = by = renderBorder;
    
    int c = 80;
    
	ofFill();
	ofSetColor( c, c, c );
	ofRect( 0, 0, w + bx * 2, 1 );
    ofRect( 0, h + by, w + bx * 2, 1 );
    ofRect( 0, 0, 1, h + by * 2 );
    ofRect( w + bx, 0, 1, h + by * 2 );
}

void ofxFFTBase :: drawGlitchData ( const ofxFFTData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBandsHalf; i++ )
	{
        int c = 200;
        
		ofFill();
		ofSetColor( c, c, c );
		ofRect
		(
			i * renderSingleBandWidth + bx,
			h + by,
			renderSingleBandWidth,
			-audioData.dataCut[ i ] * h
		);
	}
}

void ofxFFTBase :: drawFftData ( const ofxFFTData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBandsHalf; i++ )
	{
        int c = 100;
        
		ofFill();
		ofSetColor( c, c, c );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.data[ i ] * h );
		
        c = 232;
        
		ofNoFill();
		ofSetColor( c, c, c );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.data[ i ] * h );
	}
}

void ofxFFTBase :: drawFftNormData ( const ofxFFTData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBandsHalf; i++ )
	{
        int c = 100;
        
		ofFill();
		ofSetColor( c, c, c );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.dataNorm[ i ] * h );
		
        c = 232;
        
		ofNoFill();
		ofSetColor( c, c, c );
		ofRect( i * renderSingleBandWidth + bx, h + by, renderSingleBandWidth, -audioData.dataNorm[ i ] * h );
	}
}

void ofxFFTBase :: drawFftPeakData ( const ofxFFTData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	for( int i=0; i<audioNoOfBandsHalf; i++ )
	{
        float p = audioData.dataPeak[ i ];
        
		ofFill();
		ofSetColor( 0, 0, 0 );
		ofRect( i * renderSingleBandWidth + bx, ( 1 - p ) * ( h - 2 ) + by, renderSingleBandWidth - 1, 2 );
	}
}

void ofxFFTBase :: drawThresholdLine ( const ofxFFTData &audioData, int w, int h )
{
	int bx, by;		// border.
	bx =  by = renderBorder;
	
	ofFill();
	ofSetColor( 0, 0, 0 );
    ofRect( bx, ( 1 - audioData.cutThreshold ) * h + by - 1, w, 1 );
    ofSetColor( 255, 255, 255 );
    ofRect( bx, ( 1 - audioData.cutThreshold ) * h + by + 0, w, 1 );
	ofSetColor( 0, 0, 0 );
    ofRect( bx, ( 1 - audioData.cutThreshold ) * h + by + 1, w, 1 );
}