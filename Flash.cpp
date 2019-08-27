/*
 * Flash.cpp
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#include "Flash.h"

Flash::Flash(int duration, CRGB* color, const int* index, int num, int level) : Effect(level, index, num) {
	this->duration = duration;
	this->color = color;
}

Flash::~Flash() {
}

bool Flash::updateLeds(unsigned long now, CRGB* leds) {
	if( active ) {
		if( now > startTime + duration ) {
			active = false;
			for( int i = 0; i<num; i++) {
				leds[index[i]] = CRGB::Black;
			}
			return true;
		} else {
			for( int i = 0; i<num; i++) {
				leds[index[i]] = *color;
			}
			return true;
		}
	}
	return false;
}
