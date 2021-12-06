/*
 * Glow.cpp
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#include "Glow.h"

#define aniSteps 30
#define stepInc ( 255 / aniSteps )

Glow::Glow( int duration, CRGB* color, const uint8_t* index, int num, int level ) : Effect( level, index, num ) {
	this->color = color;
	animationDelay = duration / aniSteps;
	step = 0;
}

Glow::~Glow() {}

void Glow::start( CRGB* col ) {
	this->color = col;
	active = true;
}

bool Glow::updateLeds( unsigned long now, CRGB* leds ) {
	if( active ) {
		if( now > nextUpdate ) {
			nextUpdate = now + animationDelay;
			CRGB c;
			if( step < aniSteps ) { // get brighter
				c = blend( CRGB::Black, color, step * stepInc );
			} else { // get darker
				c = blend( color, CRGB::Black, ( step - aniSteps ) * stepInc );
			}

			for( int i = 0; i < num; i++ ) {
				leds[index[i]] = c;
			}
			if( step == aniSteps * 2 ) {
				step = 0;
				active = false;
			}

			return true;
		}
	}
	return false;
}
