#ifndef _READ_SWITCHES_H
#define _READ_SWITCHES_H

#include <Arduino.h>

#include "code-chessbox.h"

// Mux control pins
#define S0_MUX_PIN 15
#define S1_MUX_PIN 13
#define S2_MUX_PIN 12
#define S3_MUX_PIN 14

// Mux in "SIG" pin
#define SIG_MUX_PIN 27

extern int muxToReedSwitchMap[BITMAP_SIZE][BITMAP_SIZE];

int readMux(int channel);
void read_reed_switches(int read_into[BITMAP_SIZE][BITMAP_SIZE]);
BitmapPosition flatTo2d(int flatIdx);

#endif