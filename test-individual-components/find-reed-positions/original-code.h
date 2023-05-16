#ifndef ORIGINAL_CODE_H
#define ORIGINAL_CODE_H

#define BITMAP_SIZE 4

typedef enum {
    GAME_START,
    GAME_READ_BTN,
    GAME_USER_MOVE_CFM_BTN,
    GAME_COMPUTER_MOVE_CFM_BTN,
    GAME_IN_INVALID_STATE,
    GAME_COMPLETE,
} GameState;

typedef struct {
    int x;
    int y;
} BitmapPosition;

#include "read-switches.h"

#endif