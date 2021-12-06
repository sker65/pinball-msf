/*
 * DebugEffect.h
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

#pragma once
#include "Effect.h"

class DebugEffect : public Effect {
public:
	DebugEffect( const uint8_t* index, int num, int speed );
	virtual ~DebugEffect(){};

	virtual bool updateLeds( unsigned long now, CRGB* leds );

	int animationDelay;
	uint32_t nextUpdate;
	int current;
};
