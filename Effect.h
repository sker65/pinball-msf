/*
 * Effect.h
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#ifndef PINBALL_LW3_EFFECT_H_
#define PINBALL_LW3_EFFECT_H_

#include <FastLED.h>

class Effect {
public:
	Effect( int level, const uint8_t* index, int num );
	virtual ~Effect();

	virtual bool updateLeds( unsigned long now, CRGB* leds ) = 0;
	virtual void start() {
		startTime = millis();
		active = true;
	}

	int level;
	uint32_t startTime;
	bool active = false;
	const uint8_t* index;
	int num;
};

#endif /* PINBALL_LW3_EFFECT_H_ */
