#include <FastLED.h>

#include "chess-engine.h"
#include "code-chessbox.h"

CRGB leds[TOTAL_NUM_LEDS];

Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
    {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
    {BORDER, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, BLACK_ROOK, BORDER},
    {BORDER, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BORDER},
    {BORDER, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, BORDER},
    {BORDER, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, WHITE_ROOK, BORDER},
    {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
};

// what the current positions are after a read
int led_indices[BITMAP_SIZE][BITMAP_SIZE] = {
    {2, 5, 8, 11},
    {23, 20, 17, 14},
    {26, 29, 32, 35},
    {47, 44, 41, 38},
};

// what the current positions are after a read
int intermediateReedVals[BITMAP_SIZE][BITMAP_SIZE] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
};

// what the current positions are after a read
int finalReedVals[BITMAP_SIZE][BITMAP_SIZE] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
};

// what the previous positions are while doing a read so that you can produce a difference
int prevReedVals[BITMAP_SIZE][BITMAP_SIZE] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
};

// what the previous positions are while doing a read so that you can produce a difference
int placeLightReed[BITMAP_SIZE][BITMAP_SIZE] = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1},
};

// map for difference between prev and curr
int differenceReedVals[BITMAP_SIZE][BITMAP_SIZE] = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
};

// generic function to handle button presses
void button_handler(ulong time, uint8_t button_pin, void (*button_action)(), ulong* last_button_click,
                    int* prev_state) {
    int curr_state = digitalRead(button_pin);
    if (curr_state == 0 && (*prev_state == 1)) {
        if (millis() > *last_button_click + 100) {  // debounce w/o delay
            button_action();
            *last_button_click = millis();
        }
    }
    *prev_state = curr_state;
}

typedef enum MoveState {
    USER_TO_EMPTY,
    USER_TO_OPPONENT,
    INVALID,
} MoveState;

void computeDifference(int put_difference_into[BITMAP_SIZE][BITMAP_SIZE], int bitmap1[BITMAP_SIZE][BITMAP_SIZE],
                       int bitmap2[BITMAP_SIZE][BITMAP_SIZE]) {
    // TASK: Store results into put_difference_into
    // TASK: Set the value to 0 for all array positions that are the same value in `bitmap1` and
    // `bitmap2` 1, otherwise Absolute difference
    for (int i = 0; i < BITMAP_SIZE; i++) {
        for (int j = 0; j < BITMAP_SIZE; j++) {
            if (bitmap1[i][j] == bitmap2[i][j]) {
                put_difference_into[i][j] = 0;
            } else {
                put_difference_into[i][j] = 1;
            }
        }
    }
}

BitmapPosition pieceToBitmapPosition(PiecePosition piecePosition) {
    return (BitmapPosition){.x = piecePosition.x - 1, .y = piecePosition.y - 1};
}
PiecePosition bitmapToPiecePosition(BitmapPosition bitmapPosition) {
    return (PiecePosition){.x = bitmapPosition.x + 1, .y = bitmapPosition.y + 1};
}
int getBitmapValue(int bitmap[BITMAP_SIZE][BITMAP_SIZE], BitmapPosition bitmapPosition) {
    return bitmap[bitmapPosition.x][bitmapPosition.y];
}

enum MoveState check_validity_of_diff(PiecePosition* change1, PiecePosition* change2,
                                      int differenceReedVals[BITMAP_SIZE][BITMAP_SIZE],
                                      Piece piecesPosition[BOARD_ROWS][BOARD_COLS]) {
    // TASK:
    // if number of differences is 2:
    //       make sure that the pieces that were lifted were 1 piece from the player and the other
    //       from opponent return MoveState::USER_TO_OPPONENT
    //       put the changes into `change1` and `change2`
    // else if number of differences is 1:
    //      make sure that the piece that was lifted was a user piece
    //       put the changes into `change1`
    //        return MoveState::USER_TO_EMPTY
    // else:
    //
    //      print a helpful message to Serial and return MoveState::INVALID
    int count = 0;

    BitmapPosition first = {-1, -1};
    BitmapPosition second = {-1, -1};

    for (int i = 0; i < BITMAP_SIZE; i++) {
        for (int j = 0; j < BITMAP_SIZE; j++) {
            if (differenceReedVals[i][j] == 1) {
                count++;
                if (first.x == -1 && first.y == -1) {
                    first.x = i;
                    first.y = j;
                } else if (first.x != -1 && first.y != -1) {
                    second.x = i;
                    second.y = j;
                }
            }
        }
    }
    if (count == 1 || count == 2) {
        *change1 = bitmapToPiecePosition(first);
        Piece first_piece = get_piece_at_position(*change1);
        if (count == 1) {
            // check if one is user piece
            if (getPieceColor(first_piece) == WHITE_PIECE) {
                return MoveState::USER_TO_EMPTY;
            }
        }
        if (count == 2) {
            *change2 = bitmapToPiecePosition(second);
            Piece second_piece = get_piece_at_position(*change2);
            // check if one is user piece and the other opponent piece
            if (isOppPieceColor(first_piece, second_piece)) {
                return MoveState::USER_TO_OPPONENT;
            };
        }
    }
    Serial.println("WARN: Not a Valid Move Diff!");
    return MoveState::INVALID;
}

int is_good_game_state(int reed_vals[BITMAP_SIZE][BITMAP_SIZE], ShowAction showAction) {
    // TASK:
    // (1) Read values from the sensors using the function
    // (2) Do a row-by-row sum of the values sensed,
    // (3) For each row, make sure those many pieces also exist in `piecesPosition`
    // (4) If this is false, print a helpful message to Serial (this is a bad problem to have)
    for (int i = 0; i < BITMAP_SIZE; i++) {
        for (int j = 0; j < BITMAP_SIZE; j++) {
            BitmapPosition bitPos = {i, j};
            PiecePosition piecePos = bitmapToPiecePosition(bitPos);
            if (getBitmapValue(reed_vals, bitPos) == 0 && get_piece_at_position(piecePos) != EMPTY) {
                if (showAction == SHOW) {
                    Serial.printf("WARN: At (%d, %d), piecesPosition has a piece, but reed_vals does not!\n", i, j);
                }
                return 0;
            }
            if (getBitmapValue(reed_vals, bitPos) == 1 && get_piece_at_position(piecePos) == EMPTY) {
                if (showAction == SHOW) {
                    Serial.printf("WARN: At (%d, %d), reed_vals has a value, but piecesPosition does not!\n", i, j);
                }
                return 0;
            }
        }
    }
    return 1;
}

GameState game_state = GAME_START;
MoveState move_state;

void initBitmapPosition(BitmapPosition* pos) {
    pos->x = -1;
    pos->y = -1;
}

void initBitmapPositions(BitmapPosition posArr[BITMAP_SIZE * BITMAP_SIZE]) {
    for (int i = 0; i < BITMAP_SIZE * BITMAP_SIZE; i++) {
        initBitmapPosition(&posArr[i]);
    }
}

BitmapPosition getActiveBit(int map[BITMAP_SIZE][BITMAP_SIZE]) {
    BitmapPosition activeBitPos;
    initBitmapPosition(&activeBitPos);
    for (int i = 0; i < BITMAP_SIZE; i++) {
        for (int j = 0; j < BITMAP_SIZE; j++) {
            if (map[i][j] == 1) {
                if (activeBitPos.x != -1 && activeBitPos.y != -1) {
                    Serial.println("ERROR: Multiple true values in getActiveBit were discovered!");
                }
                activeBitPos.x = i;
                activeBitPos.y = j;
            }
        }
    }
    return activeBitPos;
}

// returns the number of active bits
int getActiveBits(BitmapPosition putPositionsInto[BITMAP_SIZE * BITMAP_SIZE], int map[BITMAP_SIZE][BITMAP_SIZE]) {
    initBitmapPositions(putPositionsInto);
    int count = 0;
    for (int i = 0; i < BITMAP_SIZE; i++) {
        for (int j = 0; j < BITMAP_SIZE; j++) {
            if (map[i][j] == 1) {
                putPositionsInto[count].x = i;
                putPositionsInto[count].y = j;
                count += 1;
            }
        }
    }
    return count;
}

int hasOnlyOneActiveBit(int map[BITMAP_SIZE][BITMAP_SIZE]) {
    // TASK:
    // Make sure that the map has only one value that is 1 in it
    // return 1 if this is the case
    // else 0
    int count_one = 0, count_zero = 0;
    for (int i = 0; i < BITMAP_SIZE; i++) {
        for (int j = 0; j < BITMAP_SIZE; j++) {
            if (map[i][j] == 1) {
                count_one++;
            } else if (map[i][j] == 0) {
                count_zero++;
            }
        }
    }
    return count_one == 1 && count_zero == BITMAP_SIZE * BITMAP_SIZE - 1 ? 1 : 0;
}

bool in_bounds(int x, int y, int bound) { return x < bound && x >= 0 && y < bound && y >= 0; }

int promotionMove(PiecePosition destPos) {
    Piece piece = get_piece_at_position(destPos);
    if (is_pawn(piece, NO_COLOR)) {
        if (getPieceColor(piece) == WHITE_PIECE) {
            if (destPos.x == 1) {
                return 1;
            }
        }
        if (getPieceColor(piece) == BLACK_PIECE) {
            if (destPos.x == BOARD_ROWS - 2) {
                return 1;
            }
        }
    }
    return 0;
}

int default_square_colors[BITMAP_SIZE][BITMAP_SIZE] = {
    {
        WHITE_COLOR_SQUARE,
        BLACK_COLOR_SQUARE,
        WHITE_COLOR_SQUARE,
        BLACK_COLOR_SQUARE,
    },
    {BLACK_COLOR_SQUARE, WHITE_COLOR_SQUARE, BLACK_COLOR_SQUARE, WHITE_COLOR_SQUARE},
    {
        WHITE_COLOR_SQUARE,
        BLACK_COLOR_SQUARE,
        WHITE_COLOR_SQUARE,
        BLACK_COLOR_SQUARE,
    },
    {BLACK_COLOR_SQUARE, WHITE_COLOR_SQUARE, BLACK_COLOR_SQUARE, WHITE_COLOR_SQUARE},
};

int computer_color_map[BITMAP_SIZE][BITMAP_SIZE] = {0};
int user_color_map[BITMAP_SIZE][BITMAP_SIZE] = {0};

void init_color_map(int color_map[BITMAP_SIZE][BITMAP_SIZE]) {
    memcpy(color_map, default_square_colors, sizeof(default_square_colors));
}

void makeMove(Piece piecesPosition[BOARD_ROWS][BOARD_COLS], Move move, ShowAction showAction) {
    // TASK:
    // Update the `piecesPosition` according to the move
    if (in_bounds(move.dest.x, move.dest.y, BOARD_ROWS) && in_bounds(move.source.x, move.source.y, BOARD_COLS)) {
        if (showAction == SHOW) {
            BitmapPosition source = pieceToBitmapPosition(move.source);
            BitmapPosition dest = pieceToBitmapPosition(move.dest);
            Serial.printf("INFO: Changing piecesPosition from (%d, %d) to (%d, %d)\n", source.x, source.y, dest.x,
                          dest.y);
        }
        piecesPosition[move.dest.x][move.dest.y] = piecesPosition[move.source.x][move.source.y];
        piecesPosition[move.source.x][move.source.y] = EMPTY;
        if (promotionMove(move.dest)) {
            PieceColor pawnColor = getPieceColor(piecesPosition[move.dest.x][move.dest.y]);
            // promote the pawn to a queen of same color
            piecesPosition[move.dest.x][move.dest.y] = pawnColor == WHITE_PIECE ? WHITE_QUEEN : BLACK_QUEEN;

            Serial.printf("INFO: Promotion move detected at (%d, %d)!\n", move.dest.x, move.dest.y);
            blink_square(8, move.dest, PROMOTION_COLOR);
        }
        init_color_map(user_color_map);
        if (kingIsInCheck(WHITE_KING)) {
            Piece kingPiece = get_king_piece_of_color(WHITE_PIECE);
            PiecePosition kingPos;
            int found = locate_piece_from(&kingPos, kingPiece, 0, 0);
            if (!found) {
                Serial.printf("ERROR: Could not find the WHITE KING on the board!\n");
            }
            light_up_chessboard(SHOW, default_square_colors);
            blink_square(8, kingPos, IN_CHECK_COLOR);

            BitmapPosition kingPosBit = pieceToBitmapPosition(kingPos);
            user_color_map[kingPosBit.x][kingPosBit.y] = IN_CHECK_COLOR;
        }
    } else {
        Serial.println("ERROR: Out of bounds in `makeMove`!");
    }
}

void paint_leds(long color, DelayAction delayAction, ShowAction showAction) {
    // TASK:
    // Reset LEDs to some default color
    // Create/Use the global FastLED object

    for (int i = LED_OFFSET; i < TOTAL_NUM_LEDS; i++) {
        leds[i] = CRGB(color);
        if (delayAction == DELAY) {
            delay(25);
            if (showAction == SHOW) {
                FastLED.show();
            }
        }
    }
    if (showAction == SHOW) {
        FastLED.show();
    }
}

void clear_leds(ShowAction showAction) {
    // TASK:
    // Reset LEDs to some default color
    // Create/Use the global FastLED object
    for (int i = LED_OFFSET; i < TOTAL_NUM_LEDS; i++) {
        leds[i] = BLACK;
    }
    if (showAction == SHOW) {
        FastLED.show();
    }
}

void light_up_square(PiecePosition position, long color, ShowAction showAction) {
    // TASK
    // Light up the LEDs according to the sources and destinations
    // use `led_indices`
    // Create/Use the global FastLED object

    // Find the order number of the first led to be lit in red and green
    int led_idx = getBitmapValue(led_indices, pieceToBitmapPosition(position));
    for (int i = 1; i <= LIGHT_UP; i++) {
        leds[led_idx + (i % LEDS_PER_SQUARE)] = CRGB(color);
    }
    if (showAction == SHOW) {
        FastLED.show();
    }
}

void set_computer_color_map_with_move(Move move) {
    init_color_map(computer_color_map);
    BitmapPosition src = pieceToBitmapPosition(move.source);
    BitmapPosition dest = pieceToBitmapPosition(move.dest);
    computer_color_map[src.x][src.y] = COMPUTER_MOVE_SRC_COLOR;

    if (get_piece_at_position(move.dest) == EMPTY) {
        light_up_square(move.source, COMPUTER_MOVE_SRC_COLOR, SHOW);
        blink_square(4, move.dest, RED);
        computer_color_map[dest.x][dest.y] = BLACK;
        return;
    }
    computer_color_map[dest.x][dest.y] = COMPUTER_MOVE_DEST_COLOR;
}

int getLedIdxFromBitmap(int flatIdx) {
    BitmapPosition twoDPos = flatTo2d(flatIdx);
    return getBitmapValue(led_indices, twoDPos);
}

void blink_square(uint blink_times, PiecePosition position, long color) {
    if (blink_times == 0) {
        return;
    }
    light_up_square(position, color, SHOW);
    delay(150);
    light_up_square(position, BLACK, SHOW);
    delay(150);

    blink_square(blink_times - 1, position, color);
}

void chessboard_blink(uint blink_times, long color) {
    if (blink_times == 0) {
        return;
    }
    paint_leds(color, NO_DELAY, SHOW);
    delay(150);
    paint_leds(BLACK, NO_DELAY, SHOW);
    delay(150);

    chessboard_blink(blink_times - 1, color);
}

void light_up_chessboard(ShowAction showAction, int colorMap[BITMAP_SIZE][BITMAP_SIZE]) {
    for (int i = 0; i < BITMAP_SIZE; i += 1) {
        for (int j = 0; j < BITMAP_SIZE; j += 1) {
            int led_index = getLedIdxFromBitmap(i * BITMAP_SIZE + j);
            for (int led = 1; led <= LIGHT_UP; led++) {
                leds[led_index + (led % LEDS_PER_SQUARE)] = colorMap[i][j];
            }
        }
    }

    if (showAction == SHOW) {
        FastLED.show();
    }
}

Move computer_move = {};

#define N_COMPUTER_MOVES 3
Move computer_moves[N_COMPUTER_MOVES] = {};
int computer_moves_len = 0;

#define SAVED_COMPUTER_MOVES 2
Move saved_computer_moves[SAVED_COMPUTER_MOVES] = {};
int saved_computer_moves_idx = 0;

bool computerMoveVisible = false;
bool checked_user_has_valid_move = false;

void set_game_state(GameState nextGameState) {
    game_state = nextGameState;
    Serial.printf("INFO: Game state changed to %d\n", nextGameState);
}

void confirm_button_action() {
    int destinationMap[BITMAP_SIZE][BITMAP_SIZE];
    int sourceMap[BITMAP_SIZE][BITMAP_SIZE];
    int tempMap[BITMAP_SIZE][BITMAP_SIZE];

    read_reed_switches(finalReedVals);

    switch (game_state) {
        case GAME_USER_MOVE_CFM_BTN: {
            switch (move_state) {
                case MoveState::USER_TO_OPPONENT:
                    computeDifference(destinationMap, intermediateReedVals, finalReedVals);
                    computeDifference(sourceMap, prevReedVals, finalReedVals);
                    break;
                case MoveState::USER_TO_EMPTY:
                    computeDifference(destinationMap, intermediateReedVals, finalReedVals);
                    computeDifference(tempMap, prevReedVals, finalReedVals);
                    computeDifference(sourceMap, tempMap, destinationMap);
                    break;
                default:
                    Serial.println("ERROR: UNREACHABLE");
                    exit(1);
                    break;
            }
            if (!hasOnlyOneActiveBit(sourceMap)) {
                Serial.println("ERROR: sourceMap had multiple true values!");
                chessboard_blink(2, ERROR_COLOR);
                init_color_map(user_color_map);
                light_up_chessboard(SHOW, user_color_map);
                set_game_state(GAME_READ_BTN);
                // set_game_state(GAME_IN_INVALID_STATE);
                return;
            }
            BitmapPosition sourceBit = getActiveBit(sourceMap);
            if (!hasOnlyOneActiveBit(destinationMap)) {
                Serial.println("ERROR: destinationMap had multiple true values!");
                chessboard_blink(2, ERROR_COLOR);
                init_color_map(user_color_map);
                light_up_chessboard(SHOW, user_color_map);
                set_game_state(GAME_READ_BTN);
                // set_game_state(GAME_IN_INVALID_STATE);
                return;
            }
            BitmapPosition destBit = getActiveBit(destinationMap);
            Move move = {};
            move.source = bitmapToPiecePosition(sourceBit);
            move.dest = bitmapToPiecePosition(destBit);

            read_reed_switches(placeLightReed);
            makeMove(piecesPosition, move, SHOW);
            set_game_state(GAME_COMPUTER_MOVE_CFM_BTN);
            break;
        }
        case GAME_COMPUTER_MOVE_CFM_BTN:
            // we're not actually checking if the user made what the LEDs told them to do
            read_reed_switches(prevReedVals);
            read_reed_switches(placeLightReed);
            init_color_map(computer_color_map);
            makeMove(piecesPosition, computer_move, SHOW);
            if (!is_good_game_state(prevReedVals, SHOW)) {
                Serial.println(
                    "ERROR: Reed Switches and piecesPosition mismatch! Game can still continue, but things may not "
                    "work.");
            }
            set_game_state(GAME_READ_BTN);
            break;
        default:
            Serial.println("ERROR: UNREACHABLE");
            exit(1);
    }
    computerMoveVisible = false;
}

void set_color_map_with_moves(int colorMap[BITMAP_SIZE][BITMAP_SIZE], long color, Move moves[], int moves_len) {
    for (int i = 0; i < moves_len; ++i) {
        Move move = moves[i];
        BitmapPosition dest = pieceToBitmapPosition(move.dest);
        colorMap[dest.x][dest.y] = color;
    }
}

void remove_moves_with_piece_at_dest(Move moves[], int* moves_len) {
    Move* temp_moves = (Move*)malloc(*moves_len * sizeof(Move));
    int temp_curr_idx = 0;

    for (int i = 0; i < *moves_len; ++i) {
        Move move = moves[i];
        if (get_piece_at_position(move.dest) == EMPTY) {
            temp_moves[temp_curr_idx++] = move;
        }
    }

    *moves_len = temp_curr_idx;
    for (int i = 0; i < temp_curr_idx; ++i) {
        moves[i] = temp_moves[i];
    }
    free(temp_moves);
}

void read_button_action() {
    read_reed_switches(intermediateReedVals);
    computeDifference(differenceReedVals, prevReedVals, intermediateReedVals);
    PiecePosition change1, change2;
    move_state = check_validity_of_diff(&change1, &change2, differenceReedVals, piecesPosition);
    switch (move_state) {
        case USER_TO_EMPTY: {
            // which kind of move we need to make is available in `move_result` global
            Serial.printf("INFO: Detected a USER_TO_EMPTY move\n");
            Move moves[16];
            int moves_len = 0;
            getValidMovesFor(moves, &moves_len, sizeof(moves) / sizeof(moves[0]), get_piece_at_position(change1));
            remove_moves_with_piece_at_dest(moves, &moves_len);
            set_color_map_with_moves(user_color_map, USR_TO_EMPTY_POSSIBLE_SQUARES_COLOR, moves, moves_len);
            set_game_state(GAME_USER_MOVE_CFM_BTN);
            break;
        }
        case USER_TO_OPPONENT: {
            // which kind of move we need to make is available in `move_result` global
            Serial.printf("INFO: Detected a USER_TO_OPPONENT move\n");
            set_game_state(GAME_USER_MOVE_CFM_BTN);
            break;
        }
        case INVALID: {
            Serial.printf("INFO: Detected an INVALID move\n");
            chessboard_blink(2, RED);
            light_up_chessboard(SHOW, user_color_map);
            set_game_state(GAME_READ_BTN);
            break;
        }
    }
    checked_user_has_valid_move = false;
}

void push_to_saved_computer_moves(Move saved_computer_moves[SAVED_COMPUTER_MOVES], int* saved_computer_moves_idx,
                                  Move move) {
    saved_computer_moves[(*saved_computer_moves_idx)++] = move;
    if (*saved_computer_moves_idx == SAVED_COMPUTER_MOVES) {
        *saved_computer_moves_idx = 0;
    }
}

bool isEqualPiecePos(PiecePosition pos1, PiecePosition pos2) { return pos1.x == pos2.x && pos1.y == pos2.y; }

bool isEqualMove(Move move1, Move move2) {
    return isEqualPiecePos(move1.source, move2.source) && isEqualPiecePos(move1.dest, move2.dest);
}

int modulo(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

Move get_nth_last_computer_move(int n) {
    if (n == 0 || n > SAVED_COMPUTER_MOVES) {
        Serial.printf("WARN: Asking for %d last move when SAVED_COMPUTER_MOVES=%d is invalid\n", n,
                      SAVED_COMPUTER_MOVES);
    }
    return saved_computer_moves[modulo((saved_computer_moves_idx - n), SAVED_COMPUTER_MOVES)];
}

Move get_next_computer_move(Move computer_moves[N_COMPUTER_MOVES], int computer_moves_len) {
    if (computer_moves_len == 0) {
        Serial.printf("ERROR: get_next_computer_move received computer_moves_len = 0\n");
        return (Move){};
    }
    Move nxtMove = computer_moves[0];
    if (computer_moves_len > 1) {
        Move second_last_move = get_nth_last_computer_move(2);
        if (isEqualMove(nxtMove, second_last_move)) {
            nxtMove = computer_moves[random(1, computer_moves_len)];
        }
    }
    push_to_saved_computer_moves(saved_computer_moves, &saved_computer_moves_idx, nxtMove);
    return nxtMove;
}

GameState initiate_computer_move(Piece piecesPosition[BOARD_ROWS][BOARD_COLS]) {
    // TASK
    //
    // set the globals  to the values computed by a "chess engine"
    // of course, do the chess engine stuff
    // call `chess_engine` and `light_up_move`
    int move_found_flag = chess_engine(computer_moves, &computer_moves_len, N_COMPUTER_MOVES, BLACK_PIECE);
    if (!move_found_flag) {
        return GameState::GAME_COMPLETE;
    }
    computer_move = get_next_computer_move(computer_moves, computer_moves_len);
    BitmapPosition source = pieceToBitmapPosition(computer_move.source);
    BitmapPosition dest = pieceToBitmapPosition(computer_move.dest);
    Serial.printf("INFO: Computer move (%d, %d) to (%d, %d)\n", source.x, source.y, dest.x, dest.y);
    set_computer_color_map_with_move(computer_move);
    return game_state;
}

void setup() {
    Serial.begin(9600);
    // buttons
    pinMode(READ_BUTTON, INPUT_PULLUP);
    pinMode(CONFIRM_BUTTON, INPUT_PULLUP);

    // mux pins
    pinMode(S0_MUX_PIN, OUTPUT);
    pinMode(S1_MUX_PIN, OUTPUT);
    pinMode(S2_MUX_PIN, OUTPUT);
    pinMode(S3_MUX_PIN, OUTPUT);

    // mux signal pin
    pinMode(SIG_MUX_PIN, INPUT);

    // set mux to low
    digitalWrite(S0_MUX_PIN, LOW);
    digitalWrite(S1_MUX_PIN, LOW);
    digitalWrite(S2_MUX_PIN, LOW);
    digitalWrite(S3_MUX_PIN, LOW);

    set_game_state(GAME_START);

    // initialize LEDs
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, TOTAL_NUM_LEDS);
    pinMode(LED_PIN, OUTPUT);
    FastLED.show();

    // init user and computer color map
    init_color_map(computer_color_map);
    init_color_map(user_color_map);
}

void dump_bitmap(int bitmap[BITMAP_SIZE][BITMAP_SIZE]) {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        Serial.printf("[");
        for (int j = 0; j < BITMAP_SIZE; j++) {
            Serial.printf("%d, ", bitmap[i][j]);
        }
        Serial.printf("]\n");
    }
    Serial.printf("\n");
}

void print_bitmapPos(BitmapPosition pos) { Serial.printf("INFO: BitmapPos (%d, %d)\n", pos.x, pos.y); }

void sense_placement_of_piece(int placeLightReed[BITMAP_SIZE][BITMAP_SIZE], int currDetect[BITMAP_SIZE][BITMAP_SIZE],
                              ClearAction clearAction, int colorMap[BITMAP_SIZE][BITMAP_SIZE], long colorLiftedPiece) {
    int diff[BITMAP_SIZE][BITMAP_SIZE];
    computeDifference(diff, placeLightReed, currDetect);
    BitmapPosition positions[BITMAP_SIZE * BITMAP_SIZE];
    int count = getActiveBits(positions, diff);
    if (clearAction == CLEAR) {
        clear_leds(NO_SHOW);
    } else {
        light_up_chessboard(NO_SHOW, colorMap);
    }
    for (int i = 0; i < count; ++i) {
        BitmapPosition pos = positions[i];
        light_up_square(bitmapToPiecePosition(pos), colorLiftedPiece, NO_SHOW);
    }
    FastLED.show();
}

// last time for INTERVAL_MILLIS
long last_time = 0;

// start: last button state and times
ulong last_readbtn_click = 0;
int readbtn_prev_state = 0;

ulong last_confirm_btn_click = 0;
int confirm_btn_prev_state = 0;
// end: last button state and times

// how often our game loop runs
#define INTERVAL_MILLIS 100

void sense_piece_movement_in_led(int prevState[BITMAP_SIZE][BITMAP_SIZE], int colorMap[BITMAP_SIZE][BITMAP_SIZE],
                                 long colorLiftedPiece) {
    int currMap[BITMAP_SIZE][BITMAP_SIZE];
    read_reed_switches(currMap);
    sense_placement_of_piece(prevState, currMap, NO_CLEAR, colorMap, colorLiftedPiece);
}

void loop() {
    switch (game_state) {
        case GameState::GAME_START: {
            if (millis() + INTERVAL_MILLIS > last_time) {
                int zeroReedVals[BITMAP_SIZE][BITMAP_SIZE] = {0};
                read_reed_switches(prevReedVals);
                sense_placement_of_piece(zeroReedVals, prevReedVals, CLEAR, NULL, LIFTED_PIECES_COLOR);
                if (is_good_game_state(prevReedVals, NO_SHOW)) {
                    Serial.printf("INFO: Waiting for steady state...\n");
                    delay(750);  // wait for about 1 sec to start the game
                    read_reed_switches(prevReedVals);
                    if (!is_good_game_state(prevReedVals, NO_SHOW)) {
                        return;
                    }
                    Serial.printf("INFO: Game Started!\n");
                    light_up_chessboard(SHOW, user_color_map);
                    read_reed_switches(placeLightReed);
                    set_game_state(GAME_READ_BTN);
                }
                last_time = millis();
            }
            break;
        }
        case GameState::GAME_READ_BTN: {
            if (millis() + INTERVAL_MILLIS > last_time) {
                if (!checked_user_has_valid_move) {
                    Move moves[1] = {};
                    int _x = 0;
                    bool foundValidMove = chess_engine(moves, &_x, 1, WHITE_PIECE);
                    if (!foundValidMove) {
                        set_game_state(GAME_COMPLETE);
                        return;
                    }
                    Serial.printf("INFO: User has a valid move to play\n");
                    checked_user_has_valid_move = true;
                }
                sense_piece_movement_in_led(placeLightReed, user_color_map, LIFTED_PIECES_COLOR);
                button_handler(millis(), READ_BUTTON, read_button_action, &last_readbtn_click, &readbtn_prev_state);
                last_time = millis();
            }
            break;
        }
        case GameState::GAME_USER_MOVE_CFM_BTN: {
            if (millis() + INTERVAL_MILLIS > last_time) {
                sense_piece_movement_in_led(placeLightReed, user_color_map, LIFTED_PIECES_COLOR);
                button_handler(millis(), CONFIRM_BUTTON, confirm_button_action, &last_confirm_btn_click,
                               &confirm_btn_prev_state);
                last_time = millis();
            }
            break;
        }
        case GameState::GAME_COMPUTER_MOVE_CFM_BTN: {
            if (millis() + INTERVAL_MILLIS > last_time) {
                if (get_piece_at_position(computer_move.dest) == EMPTY) {
                    sense_piece_movement_in_led(placeLightReed, computer_color_map, LIFTED_PIECES_COLOR);
                } else {
                    sense_piece_movement_in_led(placeLightReed, computer_color_map, LIFTED_COLOR_FOR_COMPUTER);
                }
                // make sure this function does its stuff only once
                if (!computerMoveVisible) {
                    GameState nxtState = initiate_computer_move(piecesPosition);
                    set_game_state(nxtState);
                    computerMoveVisible = true;
                }
                button_handler(millis(), CONFIRM_BUTTON, confirm_button_action, &last_confirm_btn_click,
                               &confirm_btn_prev_state);
                last_time = millis();
            }
            break;
        }
        case GameState::GAME_IN_INVALID_STATE: {
            if (millis() + INTERVAL_MILLIS > last_time) {
                Serial.println("ERROR: Game reached an INVALID STATE");
                paint_leds(RED, DELAY, SHOW);
                delay(1000);
                last_time = millis();
            }
            break;
        }
        case GameState::GAME_COMPLETE: {
            if (millis() + INTERVAL_MILLIS > last_time) {
                chessboard_blink(4, GAME_COMPLETE_COLOR);
                Serial.println("\nGame complete");
                while (1) {
                    paint_leds(GAME_COMPLETE_COLOR, DELAY, SHOW);
                    FastLED.clear(true);
                }
                last_time = millis();
            }
            break;
        }
        default: {
            paint_leds(RED, DELAY, SHOW);
            Serial.println("ERROR: Unhandled case in main loop");
            last_time = millis();
            break;
        }
    }
}
