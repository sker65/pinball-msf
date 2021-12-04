/*
 * LedDriver.h
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#ifndef PINBALL_LW3_LEDDRIVER_H_
#define PINBALL_LW3_LEDDRIVER_H_

#define MAX_EFFECTS 20
#include <stdint.h>

#include "Effect.h"

class LedDriver {
public:
	LedDriver( int updatesPerSecond );
	virtual ~LedDriver();

	void update( uint32_t now );

	void registerEffect( Effect* effect );

	void begin();

private:
	// effects / mode
	Effect* effects[MAX_EFFECTS];
	int numberOfEffects;
	int millisBetweenUpdates;
	uint32_t nextUpdate;
};

#endif /* PINBALL_LW3_LEDDRIVER_H_ */
