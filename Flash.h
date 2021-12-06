/*
 * Flash.h
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#pragma once

#include "Effect.h"

class Flash : public Effect {
public:
	Flash( int duration, CRGB* color, const uint8_t* pos, int num, int level );
	virtual ~Flash();

	virtual bool updateLeds( unsigned long now, CRGB* leds );

private:
	int duration;
	CRGB* color;
};
