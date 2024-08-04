// SCHEMATICS
// Engines: D2, D4
// Arms: D7, D8
// DC motor: 7, 5, 2, A0 (Direction: 7, PWM: 5, Brake: 2)

// BUTTONS
// Start engines: Green
// Stop engines: Blue

// Moving forward, backward: Left joystick
// Moving left, right: Right joystick

// Arms: R1

// Hard kick: L2
// Soft kick: L1

// LIBRARY
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <MMA8653.h>
#include <PS2X_lib.h>
#include "Studuino.h"

// INSTANCES
Studuino board;
PS2X ps2x;

// CONSTANTS
// General
const int generalDelay = 70;

// Arms
const int armLeftClose = 45;
const int armLeftOpen = 135;

const int armRightClose = 135;
const int armRightOpen = 45;

// DC motor
const int directionPin = 7;
const int pwmPin = 5;
const int brakePin = 2;

const int maxSpeed = 100;
const int hardKickSpeed = 100;
const int softKickSpeed = 60;

// Sound
const int soundPin = 3;

// VARIABLES
// States
int stop = 0;
int error = 0;
byte type = 0;
byte vibrate = 0;

// Engines
int forward;
int turn;

int angle = 70;
int angle2 = 70;

int angle0 = angle;
int angle20 = angle2;

// Arms
int angleArm = 0;
int dir = 4;

// DC motor
bool directionState;

void setup()
{
	// Engines
	board.InitServomotorPort(PORT_D2);
	board.InitServomotorPort(PORT_D4);
	board.Servomotor(PORT_D2, 0);
	board.Servomotor(PORT_D4, 0);

	// Arms
	board.InitServomotorPort(PORT_D7);
	board.InitServomotorPort(PORT_D8);

	// DC motor
	pinMode(directionPin, OUTPUT);
	pinMode(pwmPin, OUTPUT);
	pinMode(brakePin, OUTPUT);

	// Always release break
	digitalWrite(brakePin, LOW);

	// Sound
	pinMode(soundPin, OUTPUT);

	// Serial
	Serial.begin(9600);

	// Controller
	error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
}

// Turn on the robot
void turnOn()
{
	// If green (triangle) button is pressed, the robot will move
	if (ps2x.ButtonPressed(PSB_GREEN))
	{
		stop = 0;
	}
}

// Turn off the robot
void turnOff()
{
	// If blue (cross) button is pressed, the robot will stop
	if (ps2x.ButtonPressed(PSB_BLUE))
	{
		stop = 1;
	}
}

void moveArms()
{
	// While pressing red button, the arm will move from 45 to 135 degrees
	if (ps2x.Button(PSB_R1))
	{
		board.Servomotor(PORT_D7, armLeftClose);
		board.Servomotor(PORT_D8, armRightClose);
	}
	else
	{
		board.Servomotor(PORT_D7, armLeftOpen);
		board.Servomotor(PORT_D8, armRightOpen);
	}
}

void kick()
{
	// When pressing L1 button, the robot will make a soft kick: DC Motor will generate power in 1 second and return immediately
	if (ps2x.Button(PSB_L1))
	{
		// DC motor will generate power in 1 second
		digitalWrite(directionPin, HIGH);
		analogWrite(pwmPin, softKickSpeed);

		// DC motor will return immediately
		digitalWrite(directionPin, LOW);
		analogWrite(pwmPin, maxSpeed);
	}

	// When pressing L2 button, the robot will make a hard kick: DC Motor will generate power in 1 second and return immediately
	if (ps2x.Button(PSB_L2))
	{
		// DC motor will generate power in 1 second
		digitalWrite(directionPin, HIGH);
		analogWrite(pwmPin, hardKickSpeed);

		// DC motor will return immediately
		digitalWrite(directionPin, LOW);
		analogWrite(pwmPin, maxSpeed);
	}
}

void sound()
{
	if (ps2x.Button(PSB_RED))
	{
		tone(soundPin, 1000, 100);
	}
	else
	{
		noTone(soundPin);
	}
}

void moveRobot()
{
	forward = ps2x.Analog(PSS_LY) - 128;
	turn = ps2x.Analog(PSS_RX) - 128;

	// If the robot is stopped, the engines will be stopped
	if (stop == 1)
	{
		angle = angle0;
		angle2 = angle20;
		board.Servomotor(PORT_D2, 0);
		board.Servomotor(PORT_D4, 0);
	}
	// Otherwise, the robot will move
	else
	{
		angle = angle0 - forward / 10 + turn / 10;
		angle2 = angle20 + forward / 10 + turn / 10;
		board.Servomotor(PORT_D2, angle);
		board.Servomotor(PORT_D4, angle2);
	}
}
void loop()
{
	ps2x.read_gamepad(false, vibrate); // read controller

	// Change the state of robot
	turnOn();
	turnOff();

	// Play sound
	sound();

	// Move arms
	moveArms();

	// Move robot
	moveRobot();

	// Kick
	kick();

	delay(generalDelay);
}