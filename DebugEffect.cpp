/*
 * DebugEffect.cpp
 *
 *  Created on: 06.12.2021
 *      Author: S. Rinke
 *    (C) 2021 by Stefan Rinke / Rinke Solutions
 *  This work is licensed under a Creative
 *	Commons Attribution-NonCommercial-
 *	ShareAlike 4.0 International License.
 *
 *	http://creativecommons.org/licenses/by-nc-sa/4.0/
 */

#include "DebugEffect.h"
#include "LedDriver.h"

/**
 * debug effect just lights on led after the other.
 */

DebugEffect::DebugEffect( const int* index, int num, int animationSpeed ) : Effect( 0, index, num ) {
	nextUpdate = 0;
	current = 0;
	this->animationDelay = 1000 / animationSpeed;
}

bool DebugEffect::updateLeds( unsigned long now, CRGB* leds ) {
	if( active ) {
		if( now > nextUpdate ) {
			nextUpdate = now + animationDelay;

			CRGB warmWhite100( 255, 214, 170 ); // 0xFF, 0xD6, 0xAA
			CRGB black( 0, 0, 0 );
			for( int i = 0; i < num; i++ ) {
				leds[index[i]] = ( i == current ? warmWhite100 : black );
			}
			if( current++ == num )
				current = 0;

			return true;
		}
	}
	return false;
}
