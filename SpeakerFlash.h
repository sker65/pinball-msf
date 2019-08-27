/*
 * NormalPlay.h
 *
 *  Created on: 04.09.2015
 *      Author: sr
 */

#ifndef SPEAKER_FLASH_H_
#define SPEAKER_FLASH_H_
#include "Effect.h"

class SpeakerFlash : public Effect {
public:
	SpeakerFlash(const int* index, int num, const int* index2, int num2, int animationSpeed, int holdCycles);
	virtual ~SpeakerFlash(){};

	virtual bool updateLeds( unsigned long now, CRGB* leds );

	uint8_t paletteIndex;
	int holdCycles;
	int cycle;
	int animationDelay;
	uint32_t nextUpdate;
	const int* index2;
	int num2;
	bool on;
};

#endif /* NORMALPLAY_H_ */
