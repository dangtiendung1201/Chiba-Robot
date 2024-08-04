// SCHEMATICS
// Engines: D2, D4
// Arms: D7, D8

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
const int armLeftClose = 45;
const int armLeftOpen = 135;
const int generalDelay = 70;

const int armRightClose = 135;
const int armRightOpen = 45;
// VARIABLES
// States
int stop = 0;
int error = 0;
byte type = 0;
byte vibrate = 0;

// Engines
int angle = 70;
int angle2 = 70;

int angle0 = angle;
int angle20 = angle2;

// Arms
int angleArm = 0;
int dir = 4;

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

	// Serial
	Serial.begin(9600);

	// Controller
	error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
}

void loop()
{
	ps2x.read_gamepad(false, vibrate); // read controller

	// If green (triangle) button is pressed, the robot will move
	if (ps2x.ButtonPressed(PSB_GREEN))
	{
		stop = 0;
	}
	// If blue (cross) button is pressed, the robot will stop
	else if (ps2x.ButtonPressed(PSB_BLUE))
	{
		stop = 1;
	}

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

	// Moving the robot
	int forward = ps2x.Analog(PSS_LY) - 128;
	int turn = ps2x.Analog(PSS_RX) - 128;

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

	delay(generalDelay);
}