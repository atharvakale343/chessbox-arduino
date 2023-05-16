#ifndef ORIGINAL_CODE_H
#define ORIGINAL_CODE_H

#define BITMAP_SIZE 4

#define LIGHT_UP 3
#define LEDS_PER_SQUARE 3

#define LED_PIN 2
#define TOTAL_NUM_LEDS 50
#define LED_OFFSET 2

#define RED 0xFF0000
#define GREEN 0x00FF00
#define WHITE 0xFFFFFF
#define COOL_ORANGE 0xDC4400
#define BLACK 0x0

#define IN_CHECK_COLOR RED
#define COMPUTER_MOVE_SRC_COLOR COOL_ORANGE
#define COMPUTER_MOVE_DEST_COLOR RED
#define ERROR_COLOR RED
#define LIFTED_PIECES_COLOR GREEN
#define LIFTED_COLOR_FOR_COMPUTER BLACK
#define PROMOTION_COLOR COOL_ORANGE
#define GAME_COMPLETE_COLOR COOL_ORANGE

#define USR_TO_EMPTY_POSSIBLE_SQUARES_COLOR COOL_ORANGE

#define WHITE_COLOR_SQUARE 0xFFFFFF
#define BLACK_COLOR_SQUARE 0x191970

typedef enum {
  SHOW = 1,
  NO_SHOW = 0,
} ShowAction;

typedef enum {
  CLEAR = 1,
  NO_CLEAR = 0,
} ClearAction;

typedef enum {
  DELAY = 1,
  NO_DELAY = 0,
} DelayAction;

typedef enum {
  READ_BUTTON = 26,
  CONFIRM_BUTTON = 32,
} BUTTONS;

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