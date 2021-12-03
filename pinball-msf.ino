#include "Flash.h"
#include "Attract.h"
#include "NormalPlay.h"
#include "MultiBallPlay.h"
#include "LedDriver.h"
#include "SlingFlash.h"
#include "SpeakerFlash.h"
#include <FastLED.h>

// for MSF
// 48-77 top bar from left to right

int slingRightLeds[] = {107, 108};
int slingLeftLeds[] = {103, 104};

CRGB white(255, 255, 255);
//CRGB warmWhite100( 255, 214, 170);

Flash slingRightFlash(500, &white, slingRightLeds, 2, 1);
Flash slingLeftFlash(500, &white, slingLeftLeds, 2, 1);

SlingFlash slingRight(&slingRightFlash);
SlingFlash slingLeft(&slingLeftFlash);

const int allLeds[] = { // von unten nach oben
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
		17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
		34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
		51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
		68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
		81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
		91, 92, 93, 94, 95, 96, 97, 98, 99, 100,
		101, 102, 103, 104, 105, 106, 107, 198, 109, 110,
		111, 112, 113, 114, 115};

// 0-23 right speaker
const int rightSpeaker[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};

// 24-47 left speaker
const int leftSpeaker[] = {
		24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

// 48-77 top bar from left to right
const int topBar[] = {
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77};

// 78 - 79 creature
const int creature[] = {
		78, 79};
// 92 - 93 upper scoop
const int upperScoop[] = {
		92, 93};
// 99 - 100 lower scoop
const int lowerScoop[] = {
		99, 100};
// 80 - 91 GI upper
// 94 - 98 GI left
// 101 - 102 GI
// 103 - 104 left sling shot
// 105 - 106 GI
// 107 - 108 right sling
// 109 - 115 GI right

const int playfieldGi[] = {
		80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
		94, 95, 96, 97, 98,
		101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115};

#define GI_SENSE_PIN 8

//Attract attract(allLeds, ALL_LEDS, 20);  // array of led index, num, updates per sec

NormalPlay normalPlay(playfieldGi, sizeof(playfieldGi) / sizeof(int), 15, 60, GI_SENSE_PIN);
SpeakerFlash speakerFlash(leftSpeaker, sizeof(leftSpeaker) / sizeof(int),
													rightSpeaker, sizeof(rightSpeaker) / sizeof(int), 50, 2);

//MultiBallPlay multiBallPlay(allLeds, ALL_LEDS, 20);
CRGB creatureColor(0, 255, 0);

Flash creatureFlash(50, &creatureColor, creature, sizeof(creature) / sizeof(int), 1);

LedDriver ledDriver(40);

// synchronization between interrupt routines and FASTLED.show() needed
// switch callback also on a regular basis, not only edge
// maybe do the FASTLED.show() always at the end of a scan cycle when Timer1 stops

void setup()
{
	//DDRA = 0; // all PORTA is input
	Serial.begin(9600);

	ledDriver.begin();

	// pinMode(GI_SENSE_PIN, INPUT_PULLUP);

	//	ledDriver.registerEffect(&attract);
	ledDriver.registerEffect(&normalPlay);
	ledDriver.registerEffect(&speakerFlash);
	ledDriver.registerEffect(&creatureFlash);
	//ledDriver.registerEffect(&multiBallPlay);
	//ledDriver.registerEffect(&slingLeftFlash);
	//ledDriver.registerEffect(&slingRightFlash);

	//switchScanner.registerSwitchAction(4,4,&slingRight, EDGE);
	//switchScanner.registerSwitchAction(3,4,&slingLeft, EDGE);

	// this is active by default
	normalPlay.active = true;
	speakerFlash.active = true;
}

uint32_t nextCheck = 0;

void loop()
{
	uint32_t now = millis();
	//switchScanner.update(now);
	ledDriver.update(now);
	if (now > nextCheck)
	{
		nextCheck = now + 300;
		int r = random(10);
		if (r == 0)
		{
			creatureFlash.start();
		}
	}
}
