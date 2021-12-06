/*
 * Glow.h
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#pragma once

#include "Effect.h"

class Glow : public Effect {
public:
	Glow( int duration, CRGB* color, const uint8_t* pos, int num, int level );
	virtual ~Glow();

	virtual bool updateLeds( unsigned long now, CRGB* leds );

	void start( CRGB* color );

private:
	CRGB* color;
	int animationDelay;
	uint32_t nextUpdate;
	int step;
};
