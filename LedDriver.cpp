/*
 * LedDriver.cpp
 *
 *  Created on: 31.08.2015
 *      Author: sr
 */

#include "LedDriver.h"

#define LED_PIN 4
// speaker left / right, topbar, creature, playfield + scoops
#define NUM_LEDS ( 24 + 24 + 30 + 2 + 36 ) // was 78 -> 24 + 24 + 30 // for JP 62
#define BRIGHTNESS 255                     // was 155
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];

LedDriver::LedDriver( int updatesPerSecond ) {
	this->nextUpdate = 0;
	this->millisBetweenUpdates = 1000 / updatesPerSecond;
	this->numberOfEffects = 0;
}

LedDriver::~LedDriver() {}

void LedDriver::begin() {
	pinMode( LED_PIN, OUTPUT ); // LED
	                            // pinMode(11, OUTPUT); // LED
	                            // pinMode(10, OUTPUT); // LED

	FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>( leds, NUM_LEDS ).setCorrection( TypicalPixelString );

	FastLED.setBrightness( BRIGHTNESS );
	numberOfEffects = 0;
	CRGB warmWhite100( 255, 214, 0xAA ); // 0xFF, 0xD6, 0xAA

	for( int i = 0; i < NUM_LEDS; i++ ) {
		leds[i] = CRGB::Black;
	}
	FastLED.show();
	//	FastLED.delay(100);
}

void LedDriver::update( uint32_t now ) {
	// render effects
	int level = 0;
	bool show = false;
	while( level < 2 ) {
		for( int i = 0; i < numberOfEffects; i++ ) {
			if( effects[i]->level == level ) {
				if( effects[i]->updateLeds( now, leds ) ) {
					show = true;
				}
			}
		}
		level++;
	}
	if( show ) {
		FastLED.show();
	}
}

void LedDriver::registerEffect( Effect* effect ) { effects[numberOfEffects++] = effect; }
