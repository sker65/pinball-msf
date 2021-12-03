/*
 * NormalPlay.cpp
 *
 *  Created on: 04.09.2015
 *      Author: sr
 */

#include "NormalPlay.h"
#include "LedDriver.h"

// HSV Rainbow
extern const TProgmemRGBPalette16 normalPallette PROGMEM = {
    0xFFD690, 0xFFD690, 0xFFD690, 0x0000FF, 0xFFD6A0, 0xFFD6A0, 0xFF0000, 0xFF0000,
    0x0000FF, 0x0000FF, 0x0000FF, 0xFFD690, 0xFFD690, 0x0000FF, 0x0000FF, 0xFFD690,
};

NormalPlay::NormalPlay( const int* index, int num, int animationSpeed, int holdCycles, int pin )
    : Effect( 0, index, num ) {
	paletteIndex = 0;
	nextUpdate = 0;
	this->animationDelay = 1000 / animationSpeed;
	this->holdCycles = holdCycles;
	this->cycle = 0;
	this->giSense = pin;
}

CRGB warmWhite100( 255, 214, 170 ); // 0xFF, 0xD6, 0xAA

bool NormalPlay::updateLeds( unsigned long now, CRGB* leds ) {
	if( active ) {
		//		if( digitalRead(this->giSense) == LOW ) {
		//			CRGB black(0, 0, 0);
		//			for (int i = 0; i < num; i++) {
		//				leds[index[i]] = black;
		//			}
		//		} else
		if( now > nextUpdate ) {
			nextUpdate = now + animationDelay;

			// add a hold at 0, 16, 32
			if( ( paletteIndex % 16 ) == 0 ) {
				if( cycle++ > holdCycles ) {
					paletteIndex = paletteIndex + 1;
					cycle = 0;
				}
			} else {
				paletteIndex = paletteIndex + 1; /* color motion speed */
			}
			int colorIndex = paletteIndex;
			CRGBPalette16 pal = normalPallette;

			for( int i = 0; i < num; i++ ) {
				leds[index[i]] = ColorFromPalette( pal, colorIndex, 255, LINEARBLEND );
			}
			return true;
		}
	}
	return false;
}
