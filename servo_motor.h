#ifndef SERVO_MOTOR
#define SERVO_MOTOR

#include <iostream>
#include <pigpio.h>
#include <ctime>

//Define registers address
#define PCA9685_ADDR 0x40
#define MODE1_REG    0x00
#define PRE_SCALE    0xFE

#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09

//Define duty cycle (in a range of 12 bits = 4096) for PWM (min 2%, max 12%)
#define SERVO_MIN    82 // 0.02*4096
#define SERVO_MAX    492 // 0.12*4096

void initPCA9685(int handle);

void setServoPosition(int handle, int channel, int position);

void changeServoPositionSynchro(int handle, int channel, int currentPosition, int targetPosition);

void baisser_bras(int handle);

void lever_bras(int handle);

void fermer_pince(int handle, int pince);

void ouvrir_pince(int handle, int pince);

void check_panneau(int handle, int quelBras);

void uncheck_panneau(int handle, int quelBras);

#endif
