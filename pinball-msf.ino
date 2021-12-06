#include "DebugEffect.h"
#include "Flash.h"
#include "LedDriver.h"
#include "NormalPlay.h"
#include "SpeakerFlash.h"
#include <FastLED.h>
#include <Wire.h>

#define I2C_MSG_IN_SIZE 2
#define I2C_MSG_OUT_SIZE 4
#define I2C_ADDRESS 0x61
// shaker pwm pin
#define PWM_PIN 5
// data pin for led strip = 4
// see LEDDriver.cpp
// on board led
#define LED 13

int ledBlinks = 0;
int ledState = 0;
unsigned long nextBlink = 0;

// for MSF
// 48-77 top bar from left to right

int slingRightLeds[] = { 107, 108 };
int slingLeftLeds[] = { 103, 104 };

CRGB white( 255, 255, 255 );
// CRGB warmWhite100( 255, 214, 170);

const int allLeds[] = { // von unten nach oben
    0,  1,  2,  3,  4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19, 20, 21, 22, 23,
    24, 25, 26, 27, 28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43, 44, 45, 46, 47,
    48, 49, 50, 51, 52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67, 68, 69, 70, 71,
    72, 73, 74, 75, 76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 198, 109, 110, 111, 112, 113, 114, 115 };

// 0-23 right speaker
const int rightSpeaker[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };

// 24-47 left speaker
const int leftSpeaker[] = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
                            36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };

// 48-77 top bar from left to right
const int topBar[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
                       63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77 };

// 78 - 79 creature
const int creature[] = { 78, 79 };
// 92 - 93 upper scoop
const int upperScoop[] = { 92, 93 };
// 99 - 100 lower scoop
const int lowerScoop[] = { 99, 100 };
// 80 - 91 GI upper
// 94 - 98 GI left
// 101 - 102 GI
// 103 - 104 left sling shot
// 105 - 106 GI
// 107 - 108 right sling
// 109 - 115 GI right

const int playfieldGi[] = { 80, 81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  94,  95,  96,  97,
                            98, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115 };

#define GI_SENSE_PIN 8

NormalPlay normalPlay( playfieldGi, sizeof( playfieldGi ) / sizeof( int ), 15, 60, GI_SENSE_PIN );
SpeakerFlash speakerFlash( leftSpeaker, sizeof( leftSpeaker ) / sizeof( int ), rightSpeaker,
                           sizeof( rightSpeaker ) / sizeof( int ), 50, 2 );

DebugEffect debugEffect( allLeds, sizeof( allLeds ) / sizeof( int ), 100 );

CRGB creatureColor( 0, 255, 0 );

Flash creatureFlash( 50, &creatureColor, creature, sizeof( creature ) / sizeof( int ), 1 );

LedDriver ledDriver( 40 );

void setup() {
	// DDRA = 0; // all PORTA is input
	Serial.begin( 9600 );

	ledDriver.begin();

	// pinMode(GI_SENSE_PIN, INPUT_PULLUP);

	ledDriver.registerEffect( &normalPlay );
	ledDriver.registerEffect( &speakerFlash );
	ledDriver.registerEffect( &creatureFlash );
	ledDriver.registerEffect( &debugEffect );

	// this is active by default
	normalPlay.active = false;
	speakerFlash.active = false;
	debugEffect.active = false;

	// init i2c bus
	Wire.begin( I2C_ADDRESS );
	Wire.onReceive( receiveEvent );
	// Wire.onRequest(requestEvent);
	// init PWN output
	pinMode( PWM_PIN, OUTPUT );
	pinMode( LED, OUTPUT );
	// let led blink 10 times at startup
	ledBlinks = 10;
	// shaker speed 0
	analogWrite( PWM_PIN, 0 );
}

// shaker functions
// remember and limit speed
int currentSpeed = 0;
int maxSpeed = 255;

void off() { analogWrite( PWM_PIN, 0 ); }

void on() { analogWrite( PWM_PIN, currentSpeed ); }

void setSpeed( int speed ) {
	if( speed > 0 && speed <= maxSpeed ) {
		currentSpeed = speed;
	}
	on();
}

// command bytes used for sequences, but also for i2c commands
#define END 0x00
#define ON 0x01
#define OFF 0x02
#define RAMP 0x03
#define SETSPEED 0x04
#define PLAYSEQ 0x05
#define LED_DEBUG 0x06

// array that stores all sequences that can be triggered, must end with END command
// each sequence step consists of CMD, val & and delay in milliseconds, e.g. RAMP, 90, 1000 means
// RAMP up / down current speed to 90 in 1 second.
// clang-format off
int sequences[] = { 
  RAMP, 90, 5000, RAMP, 00, 5000, RAMP, 160, 5000, RAMP, 00, 5000, OFF, 0, 100, END, 
  // 16
  ON, 25, 1000, OFF, 0, 1000, ON, 25, 1500, OFF, 0, 1000, END,
};
// clang-format on

int seqIdx = -1;
int startIndex[] = { 0, 16 }; // start index for each sequence
#define MAX_SEQUENCES 2

unsigned long nextAction = 0; // timer marker for next action
int rampInc;                  // speed incs for ramps in steps
int rampNo = 0;               // counts number of step in ramp phase
int rampDelay;                // speed delay for ramps in steps

uint32_t nextCheck = 0;

void playSequence( int seq ) {
	if( seq >= 0 && seq < MAX_SEQUENCES ) {
		seqIdx = startIndex[seq];
		nextAction = millis();
	}
}

void receiveEvent( int count ) {
	if( count == I2C_MSG_IN_SIZE ) {
		byte cmd = Wire.read();
		byte value = Wire.read();
		// blink to ack the received cmd
		ledBlinks = cmd;
		switch( cmd ) {
		case ON:
			on();
			break;
		case OFF:
			off();
			break;
		case SETSPEED:
			setSpeed( value );
			break;
		case PLAYSEQ:
			playSequence( value );
			break;
		case LED_DEBUG:
			debugEffect.active = ( value != 0 );
			break;
		default:
			// unknown command
			break;
		}
	}
}

void loop() {
	uint32_t now = millis();
	// switchScanner.update(now);
	ledDriver.update( now );

	// for random creature flash
	if( now > nextCheck ) {
		nextCheck = now + 300;
		int r = random( 10 );
		if( r == 0 && !debugEffect.active ) {
			creatureFlash.start();
		}
	}

	// handle on board led blinks
	if( ledBlinks > 0 && now > nextBlink ) {
		nextBlink = now + 100;
		if( ledState ) {
			digitalWrite( LED, LOW );
			ledBlinks--;
			if( ledBlinks == 0 )
				nextBlink = 0;
		} else {
			digitalWrite( LED, HIGH );
		}
		ledState = !ledState;
	}

	// handle shaker playback sequences
	if( now > nextAction ) {
		// ramp active
		if( rampNo > 0 ) {
			nextAction = now + rampDelay;
			setSpeed( currentSpeed + rampInc );
			rampNo--;
		} else {
			// read next command from sequence
			int cmd = sequences[seqIdx++];
			if( cmd == END ) {
				seqIdx = -1; // end of sequence
				nextAction = 0;
			} else {
				int val = sequences[seqIdx++];
				int delay = sequences[seqIdx++];
				nextAction = now + delay;
				switch( cmd ) {
				case ON:
					setSpeed( val );
					break;
				case OFF:
					off();
					break;
				case RAMP:
					rampNo = delay / 200; // inc every x ms
					rampDelay = 200;
					nextAction = now;
					rampInc = ( val - currentSpeed ) / rampNo;
					break;
				}
			}
		}
	}
}
