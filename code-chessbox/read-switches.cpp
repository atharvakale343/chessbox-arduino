#include "read-switches.h"

int muxToReedSwitchMap[BITMAP_SIZE][BITMAP_SIZE]{
    {0, 11, 13, 2},
    {8, 3, 6, 10},
    {1, 14, 4, 9},
    {12, 7, 5, 15},
};

int readMux(int channel) {
    int controlPin[] = {S0_MUX_PIN, S1_MUX_PIN, S2_MUX_PIN, S3_MUX_PIN};

    // int muxChannel[16][4] = {
    //     {0, 0, 0, 0},  // channel 0
    //     {1, 0, 0, 0},  // channel 1
    //     {0, 1, 0, 0},  // channel 2
    //     {1, 1, 0, 0},  // channel 3
    //     {0, 0, 1, 0},  // channel 4
    //     {1, 0, 1, 0},  // channel 5
    //     {0, 1, 1, 0},  // channel 6
    //     {1, 1, 1, 0},  // channel 7
    //     {0, 0, 0, 1},  // channel 8
    //     {1, 0, 0, 1},  // channel 9
    //     {0, 1, 0, 1},  // channel 10
    //     {1, 1, 0, 1},  // channel 11
    //     {0, 0, 1, 1},  // channel 12
    //     {1, 0, 1, 1},  // channel 13
    //     {0, 1, 1, 1},  // channel 14
    //     {1, 1, 1, 1}   // channel 15
    // };

    // https://www.amazon.com/gp/product/B07K7JF3HX
    // different mappings from manufacturer
    int muxChannel[16][4] = {
        // s0,s1,s2,s3 channel
        {0, 0, 0, 0},  // 0
        {1, 0, 0, 0},  // 1
        {0, 1, 0, 0},  // 2
        {1, 1, 0, 0},  // 3
        {0, 0, 0, 1},  // 4 MANUFACTURER 8
        {1, 0, 0, 1},  // 5 MANUFACTURER 9
        {0, 1, 0, 1},  // 6 MANUFACTURER 10
        {1, 1, 0, 1},  // 7 MANUFACTURER 7
        {0, 0, 1, 0},  // 8 MANUFACTURER 4
        {1, 0, 1, 0},  // 9 MANUFACTURER 5
        {0, 1, 1, 0},  // 10 MANUFACTURER 6
        {1, 1, 1, 0},  // 11 MANUFACTURER 7
        {0, 0, 1, 1},  // 12
        {1, 0, 1, 1},  // 13
        {0, 1, 1, 1},  // 14
        {1, 1, 1, 1}   // 15
    };

    // loop through the 4 sig
    for (int i = 0; i < 4; i++) {
        digitalWrite(controlPin[i], muxChannel[channel][i]);
    }

    // read the value at the SIG pin
    int val = digitalRead(SIG_MUX_PIN);

    // return the value
    return val;
}

void copy_map(int from[BITMAP_SIZE][BITMAP_SIZE], int to[BITMAP_SIZE][BITMAP_SIZE]) {
    for (int i = 0; i < BITMAP_SIZE; ++i) {
        for (int j = 0; j < BITMAP_SIZE; ++j) {
            to[i][j] = from[i][j];
        }
    }
}

BitmapPosition flatTo2d(int flatIdx) {
    BitmapPosition twoDPos;
    twoDPos.x = (int)flatIdx / BITMAP_SIZE;
    twoDPos.y = flatIdx % BITMAP_SIZE;
    return twoDPos;
}

void transform_read(int map[BITMAP_SIZE][BITMAP_SIZE]) {
    int copy[BITMAP_SIZE][BITMAP_SIZE];
    copy_map(map, copy);
    for (int i = 0; i < BITMAP_SIZE; ++i) {
        for (int j = 0; j < BITMAP_SIZE; ++j) {
            BitmapPosition twoDPos = flatTo2d(muxToReedSwitchMap[i][j]);
            map[i][j] = copy[twoDPos.x][twoDPos.y];
        }
    }
}

void read_reed_switches(int read_into[BITMAP_SIZE][BITMAP_SIZE]) {
    // TASK: Use the Multiplexer to read all the read switches
    // It should put a value of 1 into read_into for every reed switch that is HIGH
    // 0, otherwise
    static_assert(BITMAP_SIZE * BITMAP_SIZE == 16);

    for (int i = 0; i < BITMAP_SIZE; ++i) {
        for (int j = 0; j < BITMAP_SIZE; ++j) {
            int reed_switch_number = (BITMAP_SIZE * i) + j;
            read_into[i][j] = readMux(reed_switch_number);
        }
    }
    transform_read(read_into);
}