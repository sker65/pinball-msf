/*
 * NormalPlay.cpp
 *
 *  Created on: 04.09.2015
 *      Author: sr
 */

#include "SpeakerFlash.h"
#include "LedDriver.h"

static const TProgmemRGBPalette16 speakerPallette PROGMEM = {
    0x00FF00, 0x0000FF, 0xFFD690, 0xFF0000, 0xFFFF00, 0x00FF00, 0xFF0000, 0x000000,
    0x0000FF, 0x0000FF, 0x0000FF, 0xFFD690, 0xFFD690, 0x0000FF, 0x000000, 0xFFD690,
};

SpeakerFlash::SpeakerFlash( const uint8_t* index, int num, const uint8_t* index2, int num2, int animationSpeed,
                            int holdCycles )
    : Effect( 0, index, num ) {
	paletteIndex = 0;
	nextUpdate = 0;
	this->animationDelay = 1000 / animationSpeed;
	this->holdCycles = holdCycles;
	this->cycle = 0;
	this->index2 = index2;
	this->num2 = num2;
	this->on = false;
}

bool SpeakerFlash::updateLeds( unsigned long now, CRGB* leds ) {
	if( active ) {
		if( now > nextUpdate ) {
			on = !on;
			int r = random( 180 );
			nextUpdate = now + ( on ? ( 20 * ( r % 4 ) ) : animationDelay * ( r + 1 ) );
			CRGB col;
			CRGBPalette16 pal = speakerPallette;
			if( on ) {
				col = ColorFromPalette( pal, ( r % 7 ) * 16, 255, NOBLEND );
			} else {
				col = CRGB::Black;
			}
			for( int i = 0; i < num; i++ )
				leds[index[i]] = col;
			for( int i = 0; i < num2; i++ )
				leds[index2[i]] = col;

			return true;
		}
	}
	return false;
}
