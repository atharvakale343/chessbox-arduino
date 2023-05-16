#include "main.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines for chess pieces and spaces

// bitmap
// Original
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, BLACK_ROOK, BORDER},
//     {BORDER, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BORDER},
//     {BORDER, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, BORDER},
//     {BORDER, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, WHITE_ROOK, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };
// clang-format off
// check if it checks more than taking a pawn
Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
    {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
    {BORDER, EMPTY, BLACK_KING, EMPTY, EMPTY, BORDER},
    {BORDER, BLACK_PAWN, EMPTY, BLACK_QUEEN, EMPTY, BORDER},
    {BORDER, EMPTY, WHITE_ROOK, EMPTY, EMPTY, BORDER},
    {BORDER, EMPTY, WHITE_KING, EMPTY, EMPTY, BORDER},
    {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
};
// clang-format on
// clang-format off
// invalidate too close kings test case
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, EMPTY, WHITE_KING, EMPTY, EMPTY, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, EMPTY, EMPTY, BLACK_KING, EMPTY, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };
// clang-format on
// kingWrongMoveCheck
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, BLACK_ROOK, BORDER},
//     {BORDER, BLACK_PAWN, BLACK_PAWN, WHITE_PAWN, BLACK_PAWN, BORDER},
//     {BORDER, WHITE_PAWN, EMPTY, WHITE_PAWN, WHITE_PAWN, BORDER},
//     {BORDER, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, WHITE_ROOK, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };
// checkmate fast check
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, BLACK_ROOK, BORDER},
//     {BORDER, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, WHITE_PAWN, BORDER},
//     {BORDER, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_PAWN, BORDER},
//     {BORDER, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, WHITE_ROOK, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };
// Pawn testing
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, BLACK_ROOK, BLACK_QUEEN, BLACK_KING, BLACK_ROOK, BORDER},
//     {BORDER, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BORDER},
//     {BORDER, WHITE_PAWN, WHITE_PAWN, EMPTY, WHITE_PAWN, BORDER},
//     {BORDER, WHITE_ROOK, WHITE_QUEEN, WHITE_KING, WHITE_ROOK, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };

// Rook testing
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, EMPTY, BLACK_ROOK, EMPTY, EMPTY, BORDER},
//     {BORDER, WHITE_QUEEN, BLACK_ROOK, EMPTY, WHITE_KING, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, EMPTY, WHITE_PAWN, EMPTY, EMPTY, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };

// Queen Test 1
// clang-format off
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, EMPTY,  EMPTY,       EMPTY,  EMPTY,  BORDER},
//     {BORDER, EMPTY,  EMPTY,       EMPTY,  EMPTY,  BORDER},
//     {BORDER, EMPTY,  BLACK_QUEEN, EMPTY,  EMPTY,  BORDER},
//     {BORDER, EMPTY,  EMPTY,       EMPTY,  EMPTY,  BORDER},
//     {BORDER, BORDER, BORDER,      BORDER, BORDER, BORDER},
// };
// clang-format on

// Queen Test 2
// clang-format off
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER,     BORDER,       BORDER,     BORDER, BORDER},
//     {BORDER, EMPTY,      BLACK_PAWN,   EMPTY, EMPTY,  BORDER},
//     {BORDER, WHITE_PAWN,      EMPTY,        EMPTY,      EMPTY,  BORDER},
//     {BORDER, BLACK_PAWN, BLACK_QUEEN,  BLACK_PAWN, EMPTY,  BORDER},
//     {BORDER, BLACK_PAWN, BLACK_PAWN,   BLACK_PAWN, BLACK_KING,  BORDER},
//     {BORDER, BORDER,     BORDER,       BORDER,     BORDER, BORDER},
// };
// clang-format on

// kingInCheck Test 1
// clang-format off
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER,       BORDER,      BORDER, BORDER,       BORDER},
//     {BORDER, WHITE_ROOK,   EMPTY,       EMPTY,  WHITE_QUEEN,  BORDER},
//     {BORDER, EMPTY,        EMPTY,       EMPTY,  EMPTY,        BORDER},
//     {BORDER, EMPTY,        EMPTY,       EMPTY,  WHITE_PAWN,   BORDER},
//     {BORDER, EMPTY,        BLACK_KING,  EMPTY,  EMPTY,        BORDER},
//     {BORDER, BORDER,       BORDER,      BORDER, BORDER,       BORDER},
// };
// clang-format on

// kingInCheck Test 2
// clang-format off
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER,       BORDER,      BORDER, BORDER,       BORDER},
//     {BORDER, WHITE_QUEEN,  EMPTY,       EMPTY,  WHITE_QUEEN,  BORDER},
//     {BORDER, EMPTY,        EMPTY,       EMPTY,  EMPTY,        BORDER},
//     {BORDER, EMPTY,        EMPTY,       EMPTY,  WHITE_PAWN,   BORDER},
//     {BORDER, EMPTY,        BLACK_KING,  EMPTY,  EMPTY,        BORDER},
//     {BORDER, BORDER,       BORDER,      BORDER, BORDER,       BORDER},
// };
// clang-format on

// kingInCheck Test 3
// clang-format off
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER,       BORDER,      BORDER,      BORDER,       BORDER},
//     {BORDER, WHITE_QUEEN,  EMPTY,       EMPTY,       WHITE_QUEEN,  BORDER},
//     {BORDER, EMPTY,        EMPTY,       EMPTY,       EMPTY,        BORDER},
//     {BORDER, EMPTY,        EMPTY,       WHITE_PAWN,  WHITE_PAWN,   BORDER},
//     {BORDER, EMPTY,        BLACK_KING,  EMPTY,       EMPTY,        BORDER},
//     {BORDER, BORDER,       BORDER,      BORDER,      BORDER,       BORDER},
// };
// clang-format on

// Pin Test
// clang-format off
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER,       BORDER,      BORDER,      BORDER,       BORDER},
//     {BORDER, WHITE_QUEEN,  EMPTY,       EMPTY,       WHITE_QUEEN,  BORDER},
//     {BORDER, EMPTY,        EMPTY,       EMPTY,       EMPTY,        BORDER},
//     {BORDER, BLACK_PAWN,   BLACK_PAWN,  EMPTY,       EMPTY,        BORDER},
//     {BORDER, BLACK_KING,   EMPTY,       BLACK_ROOK,  WHITE_QUEEN,  BORDER},
//     {BORDER, BORDER,       BORDER,      BORDER,      BORDER,       BORDER},
// };
// clang-format on

// Empty boards
// Piece piecesPosition[BOARD_ROWS][BOARD_COLS] = {
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, EMPTY, EMPTY, EMPTY, EMPTY, BORDER},
//     {BORDER, BORDER, BORDER, BORDER, BORDER, BORDER},
// };

#define NUM_OF_BLACK_PIECES 4
Piece black_pieces[NUM_OF_BLACK_PIECES] = {BLACK_KING, BLACK_QUEEN, BLACK_ROOK, BLACK_PAWN};

Move valid_moves[MAX_VALID_MOVES_CAPACITY] = {};
int valid_moves_len = 0;

const char *get_piece_name(Piece piece) {
    switch (piece) {
        case BLACK_PAWN:
            return " BLACK_PAWN";
            break;
        case BLACK_ROOK:
            return " BLACK_ROOK";
            break;
        case BLACK_QUEEN:
            return "BLACK_QUEEN";
            break;
        case BLACK_KING:
            return " BLACK_KING";
            break;
        case WHITE_PAWN:
            return " WHITE_PAWN";
            break;
        case WHITE_ROOK:
            return " WHITE_ROOK";
            break;
        case WHITE_QUEEN:
            return "WHITE_QUEEN";
            break;
        case WHITE_KING:
            return " WHITE_KING";
            break;
        case EMPTY:
            return "   EMPTY   ";
            break;
        case BORDER:
            return "   BORDER  ";
            break;
        case NO_PIECE:
            printf("ERROR: NO_PIECE SHOULD NOT BE REACHABLE IN SWITCH CASE\n");
            return "  NO_PIECE ";
            break;
        default:
            printf("ERROR: UNREACHABLE SWITCH CASE\n");
            return "INVALID PIECE";
            break;
    }
}

bool is_queen(Piece piece, PieceColor pieceColor) {
    if (pieceColor == WHITE_PIECE) {
        return piece == WHITE_QUEEN;
    } else if (pieceColor == BLACK_PIECE) {
        return piece == BLACK_QUEEN;
    } else {
        return piece == WHITE_QUEEN || piece == BLACK_QUEEN;
    }
}

bool is_rook(Piece piece, PieceColor pieceColor) {
    if (pieceColor == WHITE_PIECE) {
        return piece == WHITE_ROOK;
    } else if (pieceColor == BLACK_PIECE) {
        return piece == BLACK_ROOK;
    } else {
        return piece == WHITE_ROOK || piece == BLACK_ROOK;
    }
}

bool is_pawn(Piece piece, PieceColor pieceColor) {
    if (pieceColor == WHITE_PIECE) {
        return piece == WHITE_PAWN;
    } else if (pieceColor == BLACK_PIECE) {
        return piece == BLACK_PAWN;
    } else {
        return piece == WHITE_PAWN || piece == BLACK_PAWN;
    }
}

bool is_king(Piece piece, PieceColor pieceColor) {
    if (pieceColor == WHITE_PIECE) {
        return piece == WHITE_KING;
    } else if (pieceColor == BLACK_PIECE) {
        return piece == BLACK_KING;
    } else {
        return piece == WHITE_KING || piece == BLACK_KING;
    }
}

int get_move_value(Move move) {
    Piece destPiece = get_piece_at_position(move.dest);
    int move_value = 0;
    if (is_pawn(destPiece, NO_COLOR)) {
        move_value += 1;
    } else if (is_rook(destPiece, NO_COLOR)) {
        move_value += 2;
    } else if (is_queen(destPiece, NO_COLOR)) {
        move_value += 3;
    }
    if (getPieceColor(move.piece) == NO_COLOR) {
        printf("ERROR: move.piece was not initialized\n");
    }
    if (!is_valid_pos_for_king(getOppPieceColor(move.piece), move.source, move.dest)) {
        move_value += 2;
    }
    return move_value;
}

void add_move_into_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Move move) {
    if (*valid_moves_len < valid_moves_cap) {
        move.move_value = get_move_value(move);
        valid_moves[(*valid_moves_len)++] = move;
    } else {
        printf("WARN: valid_moves hit max size %d. Generated move was not added!\n", MAX_VALID_MOVES_CAPACITY);
    }
}

PieceColor getPieceColor(Piece piece) {
    if (piece == EMPTY || piece == BORDER) {
        return NO_COLOR;
    }
    for (int i = 0; i < NUM_OF_BLACK_PIECES; ++i) {
        if (black_pieces[i] == piece) {
            return BLACK_PIECE;
        }
    }
    return WHITE_PIECE;
}

bool isOppPieceColor(Piece piece1, Piece piece2) {
    PieceColor p1c = getPieceColor(piece1);
    PieceColor p2c = getPieceColor(piece2);
    return p1c == WHITE_PIECE ? p2c == BLACK_PIECE : p2c == WHITE_PIECE;
}

PieceColor getOppPieceColor(Piece piece) {
    PieceColor pc = getPieceColor(piece);
    if (pc == WHITE_PIECE) {
        return BLACK_PIECE;
    } else if (pc == BLACK_PIECE) {
        return WHITE_PIECE;
    }
    return NO_COLOR;
}

bool locate_piece_from(PiecePosition *to_return, Piece piece, int startX, int startY) {
    for (int i = startX; i < BOARD_ROWS; ++i) {
        for (int j = startY; j < BOARD_COLS; ++j) {
            Piece curr_piece = piecesPosition[i][j];
            if (piece == curr_piece) {
                to_return->x = i;
                to_return->y = j;
                return 1;
            }
        }
        startY = 0;
    }
    return 0;
}

Move initialize_move(Piece piece, PiecePosition piece_loc) {
    return (Move){
        .piece = piece, .source = {.x = piece_loc.x, .y = piece_loc.y}, .dest = {.x = -1, .y = -1}, .move_value = -1};
}

void add_dest_to_move(Move *move, PiecePosition dest) {
    move->dest.x = dest.x;
    move->dest.y = dest.y;
}

Piece get_piece_at_position(PiecePosition position) { return piecesPosition[position.x][position.y]; }

void getValidMovesForPawn(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece) {
    PieceColor pieceColor = getPieceColor(piece);
    PiecePosition piece_loc = {.x = -1, .y = -1};
    while (locate_piece_from(&piece_loc, piece, piece_loc.x, piece_loc.y + 1)) {
        int x = piece_loc.x, y = piece_loc.y;
        Move move = initialize_move(piece, piece_loc);

        PiecePosition other_piece_pos;
        Piece other_piece;

        // up left position
        switch (pieceColor) {
            case WHITE_PIECE:
                other_piece_pos = (PiecePosition){.x = x - 1, .y = y - 1};
                break;
            case BLACK_PIECE:
                other_piece_pos = (PiecePosition){.x = x + 1, .y = y - 1};
                break;
            default:
                printf("ERROR: Not a valid pieceColor\n");
                break;
        }
        other_piece = get_piece_at_position(other_piece_pos);
        add_dest_to_move(&move, other_piece_pos);
        if (isOppPieceColor(piece, other_piece) && is_valid_pos_for_king(pieceColor, move.source, move.dest)) {
            add_move_into_moves(valid_moves, valid_moves_len, valid_moves_cap, move);
        }

        // up right position
        switch (pieceColor) {
            case WHITE_PIECE:
                other_piece_pos = (PiecePosition){.x = x - 1, .y = y + 1};
                break;
            case BLACK_PIECE:
                other_piece_pos = (PiecePosition){.x = x + 1, .y = y + 1};
                break;
            default:
                printf("ERROR: Not a valid pieceColor\n");
                break;
        }
        other_piece = get_piece_at_position(other_piece_pos);
        add_dest_to_move(&move, other_piece_pos);
        if (isOppPieceColor(piece, other_piece) && is_valid_pos_for_king(pieceColor, move.source, move.dest)) {
            add_move_into_moves(valid_moves, valid_moves_len, valid_moves_cap, move);
        }

        // up position
        switch (pieceColor) {
            case WHITE_PIECE:
                other_piece_pos = (PiecePosition){.x = x - 1, .y = y};
                break;
            case BLACK_PIECE:
                other_piece_pos = (PiecePosition){.x = x + 1, .y = y};
                break;
            default:
                printf("ERROR: Not a valid pieceColor\n");
                break;
        }
        other_piece = get_piece_at_position(other_piece_pos);
        add_dest_to_move(&move, other_piece_pos);
        if (other_piece == EMPTY && is_valid_pos_for_king(pieceColor, move.source, move.dest)) {
            add_move_into_moves(valid_moves, valid_moves_len, valid_moves_cap, move);
        }
    }
    // if (*valid_moves_len == 0) {
    //     printf("WARN: Did not find a move for %s\n", get_piece_name(piece));
    // }
}

// also returns if the position is in bounds
bool add_delta_to_pos(PiecePosition *other_piece_pos, int dx, int dy) {
    other_piece_pos->x += dx;
    other_piece_pos->y += dy;
    Piece piece = get_piece_at_position(*other_piece_pos);
    if (piece == BORDER) {
        return 0;
    }
    return 1;
}

bool gen_queen_rook_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Move move, Piece piece,
                          PiecePosition other_piece_pos) {
    add_dest_to_move(&move, other_piece_pos);
    Piece other_piece = get_piece_at_position(other_piece_pos);
    PieceColor pieceColor = getPieceColor(piece);
    if (other_piece == EMPTY && is_valid_pos_for_king(pieceColor, move.source, move.dest)) {
        add_move_into_moves(valid_moves, valid_moves_len, valid_moves_cap, move);
    } else if (isOppPieceColor(piece, other_piece) && is_valid_pos_for_king(pieceColor, move.source, move.dest)) {
        if (other_piece == WHITE_KING || other_piece == BLACK_KING) {
            printf("WARN: A move involving taking over a king was generated\n");
        }
        add_move_into_moves(valid_moves, valid_moves_len, valid_moves_cap, move);
        // we cannot move any further this way
        return 1;
    } else {
        // if same color piece
        return 1;
    }
    return 0;
}

void try_add_delta_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece,
                         PiecePosition original_piece_loc, int dx, int dy) {
    Move move = initialize_move(piece, original_piece_loc);
    PiecePosition other_piece_pos = original_piece_loc;
    while (add_delta_to_pos(&other_piece_pos, dx, dy)) {
        int should_stop =
            gen_queen_rook_moves(valid_moves, valid_moves_len, valid_moves_cap, move, piece, other_piece_pos);
        if (should_stop) {
            break;
        }
    }
}

void getValidMovesForRook(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece) {
    PiecePosition piece_loc = {.x = -1, .y = -1};
    while (locate_piece_from(&piece_loc, piece, piece_loc.x, piece_loc.y + 1)) {
        // go up
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, 0);
        // go down
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, 0);
        // go left
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, 0, -1);
        // go right
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, 0, +1);
    }
    // if (*valid_moves_len == 0) {
    //     printf("WARN: Did not find a move for %s\n", get_piece_name(piece));
    // }
}

void getValidMovesForQueen(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece) {
    PiecePosition piece_loc = {.x = -1, .y = -1};
    while (locate_piece_from(&piece_loc, piece, piece_loc.x, piece_loc.y + 1)) {
        // go up
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, 0);
        // go down
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, 0);
        // go left
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, 0, -1);
        // go right
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, 0, +1);
        // go up-right diagonal
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, +1);
        // go up-left diagonal
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, -1);
        // go down-right diagonal
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, +1);
        // go down-left diagonal
        try_add_delta_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, -1);
    }
    // if (*valid_moves_len == 0) {
    //     printf("WARN: Did not find a move for %s\n", get_piece_name(piece));
    // }
}

int check_is_in_check_delta(bool (*check_funcs[2])(Piece, PieceColor), int check_funcs_len, PiecePosition kingPosition,
                            PieceColor oppKingColor, int dx, int dy) {
    PiecePosition deltaPosition = kingPosition;
    Piece other_piece;
    while (add_delta_to_pos(&deltaPosition, dx, dy)) {
        other_piece = get_piece_at_position(deltaPosition);
        for (int f_idx = 0; f_idx < check_funcs_len; f_idx++) {
            if (check_funcs[f_idx](other_piece, oppKingColor)) {
                return 1;
            }
        }
        if (other_piece != EMPTY) {
            break;
        }
    }
    return 0;
}

int check_for_certain_piece_at_deltas(int deltaArray[][2], int deltaArrayLength, bool (*piece_func)(Piece, PieceColor),
                                      PiecePosition origPiecePos, PieceColor oppPieceColor) {
    for (int i = 0; i < deltaArrayLength; ++i) {
        int delX = deltaArray[i][0];
        int delY = deltaArray[i][1];
        PiecePosition deltaPosition = origPiecePos;
        if (add_delta_to_pos(&deltaPosition, delX, delY)) {
            Piece other_piece = get_piece_at_position(deltaPosition);
            if (piece_func(other_piece, oppPieceColor)) {
                return 1;
            }
        }
    }
    return 0;
}

int kingIsInCheck(Piece kingPiece) {
    PiecePosition kingPosition;
    locate_piece_from(&kingPosition, kingPiece, 0, 0);
    PieceColor oppKingColor = getOppPieceColor(kingPiece);
    bool (*check_funcs[2])(Piece piece, PieceColor pieceColor) = {is_queen, is_rook};
    // up: check for queen or rook
    if (check_is_in_check_delta(check_funcs, 2, kingPosition, oppKingColor, +1, 0)) {
        return 1;
    }

    // down: check for queen or rook
    if (check_is_in_check_delta(check_funcs, 2, kingPosition, oppKingColor, -1, 0)) {
        return 1;
    }

    // left: check for queen or rook
    if (check_is_in_check_delta(check_funcs, 2, kingPosition, oppKingColor, 0, -1)) {
        return 1;
    }

    // right: check for queen or rook
    if (check_is_in_check_delta(check_funcs, 2, kingPosition, oppKingColor, 0, +1)) {
        return 1;
    }

    bool (*check_funcs2[1])(Piece piece, PieceColor pieceColor) = {is_queen};

    // go up-right diagonal: check for queen
    if (check_is_in_check_delta(check_funcs2, 1, kingPosition, oppKingColor, +1, +1)) {
        return 1;
    }

    // go up-left diagonal: check for queen
    if (check_is_in_check_delta(check_funcs2, 1, kingPosition, oppKingColor, +1, -1)) {
        return 1;
    }

    // go down-right diagonal: check for queen
    if (check_is_in_check_delta(check_funcs2, 1, kingPosition, oppKingColor, -1, +1)) {
        return 1;
    }

    // go down-left diagonal: check for queen
    if (check_is_in_check_delta(check_funcs2, 1, kingPosition, oppKingColor, -1, -1)) {
        return 1;
    }

    // look for pawns and opp king
    PiecePosition deltaPosition = kingPosition;
    Piece other_piece;
    switch (oppKingColor) {
        case BLACK_PIECE: {
            // black pawns can only attack white king from -1, [-1, 1]
            // go up-left
            deltaPosition = kingPosition;
            if (add_delta_to_pos(&deltaPosition, -1, -1)) {
                other_piece = get_piece_at_position(deltaPosition);
                if (is_pawn(other_piece, oppKingColor)) {
                    return 1;
                }
            }

            // go up-right
            deltaPosition = kingPosition;
            if (add_delta_to_pos(&deltaPosition, -1, +1)) {
                other_piece = get_piece_at_position(deltaPosition);
                if (is_pawn(other_piece, oppKingColor)) {
                    return 1;
                }
            }
            // check too close black king to current position of white king
            int deltaArray[8][2] = {{-1, -1}, {-1, 0}, {-1, +1}, {0, +1}, {+1, +1}, {+1, 0}, {+1, -1}, {0, -1}};
            int deltaArrayLength = sizeof(deltaArray) / sizeof(deltaArray[0]);

            if (check_for_certain_piece_at_deltas(deltaArray, deltaArrayLength, is_king, kingPosition, oppKingColor)) {
                return 1;
            }

            break;
        }
        case WHITE_PIECE: {
            // white pawns can only attack black king from +1, [-1, 1]
            deltaPosition = kingPosition;
            // go down left
            if (add_delta_to_pos(&deltaPosition, +1, -1)) {
                other_piece = get_piece_at_position(deltaPosition);
                if (is_pawn(other_piece, oppKingColor)) {
                    return 1;
                }
            }

            deltaPosition = kingPosition;
            // go down-right
            if (add_delta_to_pos(&deltaPosition, +1, +1)) {
                other_piece = get_piece_at_position(deltaPosition);
                if (is_pawn(other_piece, oppKingColor)) {
                    return 1;
                }
            }

            // check too close black king to current position of white king
            int deltaArray[8][2] = {{-1, -1}, {-1, 0}, {-1, +1}, {0, +1}, {+1, +1}, {+1, 0}, {+1, -1}, {0, -1}};
            int deltaArrayLength = sizeof(deltaArray) / sizeof(deltaArray[0]);

            if (check_for_certain_piece_at_deltas(deltaArray, deltaArrayLength, is_king, kingPosition, oppKingColor)) {
                return 1;
            }

            break;
        }
        default:
            printf("ERROR: UNREACHABLE CASE STATEMENT\n");
            break;
    }
    return 0;
}

Piece get_king_piece_of_color(PieceColor pieceColor) {
    if (pieceColor == WHITE_PIECE) {
        return WHITE_KING;
    }
    return BLACK_KING;
}

int is_valid_pos_for_king(PieceColor kingColor, PiecePosition from_pos, PiecePosition to_pos) {
    // temporarily move the piece from old position to new position
    Piece piece = piecesPosition[from_pos.x][from_pos.y];
    piecesPosition[from_pos.x][from_pos.y] = EMPTY;
    Piece saved_piece = piecesPosition[to_pos.x][to_pos.y];
    piecesPosition[to_pos.x][to_pos.y] = piece;

    // check if this is a valid position for the king to be in
    Piece kingPiece = get_king_piece_of_color(kingColor);
    int is_valid = !kingIsInCheck(kingPiece);

    // restore the piece to the old position
    piecesPosition[from_pos.x][from_pos.y] = piece;
    piecesPosition[to_pos.x][to_pos.y] = saved_piece;

    return is_valid;
}

void try_add_delta_king_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece,
                              PiecePosition original_piece_loc, int dx, int dy) {
    Move move = initialize_move(piece, original_piece_loc);
    PiecePosition other_piece_pos = original_piece_loc;
    PieceColor pieceColor = getPieceColor(piece);
    if (add_delta_to_pos(&other_piece_pos, dx, dy)) {
        if (is_valid_pos_for_king(pieceColor, original_piece_loc, other_piece_pos)) {
            gen_queen_rook_moves(valid_moves, valid_moves_len, valid_moves_cap, move, piece, other_piece_pos);
        }
    }
}

void getValidMovesForKing(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece) {
    PiecePosition piece_loc = {.x = -1, .y = -1};
    while (locate_piece_from(&piece_loc, piece, piece_loc.x, piece_loc.y + 1)) {
        // go up
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, 0);
        // go down
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, 0);
        // go left
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, 0, -1);
        // go right
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, 0, +1);
        // go up-right diagonal
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, +1);
        // go up-left diagonal
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, +1, -1);
        // go down-right diagonal
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, +1);
        // go down-left diagonal
        try_add_delta_king_moves(valid_moves, valid_moves_len, valid_moves_cap, piece, piece_loc, -1, -1);
    }
    // if (*valid_moves_len == 0) {
    //     printf("WARN: Did not find a move for %s\n", get_piece_name(piece));
    // }
}

void print_valid_moves(Move valid_moves[], int valid_moves_len) {
    if (valid_moves_len == 0) {
        printf("\tNo valid moves found\n");
        return;
    }
    for (int i = 0; i < valid_moves_len; ++i) {
        Move move = valid_moves[i];
        printf("\tSource: (%d, %d)\n", move.source.x, move.source.y);
        printf("\t  Dest: (%d, %d)\n", move.dest.x, move.dest.y);
        printf("\t Value: %d\n", move.move_value);
        printf("\t Piece: %s\n", get_piece_name(move.piece));
        printf("\t-----\n");
    }
}

void getValidMovesFor(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece) {
    // reset to zero moves
    *valid_moves_len = 0;
    switch (piece) {
        case BLACK_PAWN:
            getValidMovesForPawn(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case BLACK_ROOK:
            getValidMovesForRook(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case BLACK_QUEEN:
            getValidMovesForQueen(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case BLACK_KING:
            getValidMovesForKing(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case WHITE_PAWN:
            getValidMovesForPawn(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case WHITE_ROOK:
            getValidMovesForRook(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case WHITE_QUEEN:
            getValidMovesForQueen(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        case WHITE_KING:
            getValidMovesForKing(valid_moves, valid_moves_len, valid_moves_cap, piece);
            break;
        default:
            printf("ERROR: %s UNREACHABLE SWITCH CASE\n", get_piece_name(piece));
            break;
    }
    // printf("INFO: Valid Moves for %s:\n", get_piece_name(piece));
    print_valid_moves(valid_moves, *valid_moves_len);
}

int move_comparator(const void *cmp1, const void *cmp2) {
    Move move1 = *((Move *)cmp1);
    Move move2 = *((Move *)cmp2);

    return move2.move_value - move1.move_value;
}

void cpy_moves_into(Move dest[], Move src[], int how_many_moves) {
    memcpy(dest, src, sizeof(dest[0]) * how_many_moves);
}

Move get_best_move(Move valid_moves[], int *valid_moves_len) {
    if (*valid_moves_len == 0) {
        printf("ERROR: No best move to find with zero moves\n");
    }
    int best_move_idx = 0;
    for (int i = 0; i < *valid_moves_len; ++i) {
        if (valid_moves[i].move_value > valid_moves[best_move_idx].move_value) {
            best_move_idx = i;
        }
    }
    return valid_moves[best_move_idx];
}

int get_best_moves(Move put_best_into[], int put_best_into_size, Move valid_moves[], int valid_moves_len) {
    if (valid_moves_len == 0) {
        printf("ERROR: No best move to find with zero moves\n");
        return 0;
    }
    qsort(valid_moves, valid_moves_len, sizeof(valid_moves[0]), move_comparator);
    int copy_moves = fmin(put_best_into_size, valid_moves_len);
    cpy_moves_into(put_best_into, valid_moves, copy_moves);
    return copy_moves;
}

int chess_engine(Move to_return_moves[], int *to_return_moves_len, int to_return_moves_size, PieceColor color) {
    // TASK
    // Compute a valid move for "BLACK"
    // set the globals sourcex, soucey, destx, desty
    // TODO
    PiecePosition kingPos;
    if (locate_piece_from(&kingPos, BLACK_KING, 0, 0) == 0) {
        printf("WARN: Could not locate the BLACK_KING\n");
    }
    Move moves[4] = {};
    int moves_len = 0;

    switch (color) {
        case BLACK_PIECE: {
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, BLACK_KING);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, BLACK_PAWN);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, BLACK_QUEEN);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, BLACK_ROOK);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }

            if (moves_len == 0) {
                printf("INFO: No moves for Black found. Checkmate\n");
                return 0;
            }
            break;
        }
        case WHITE_PIECE: {
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, WHITE_KING);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, WHITE_PAWN);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, WHITE_QUEEN);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            getValidMovesFor(valid_moves, &valid_moves_len, MAX_VALID_MOVES_CAPACITY, WHITE_ROOK);
            if (valid_moves_len > 0) {
                moves[moves_len++] = get_best_move(valid_moves, &valid_moves_len);
            }
            if (moves_len == 0) {
                printf("INFO: No moves for White found. Checkmate\n");
                return 0;
            }
            break;
        }
        default:
            break;
    }
    *to_return_moves_len = get_best_moves(to_return_moves, to_return_moves_size, moves, moves_len);
    return 1;
}

void visualize_board() {
    printf(
        "------------------------------------------------------------------------------------------"
        "-----------------------------------------------------\n");
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int line = 0; line < 2; ++line) {
            for (int j = 0; j < BOARD_COLS; ++j) {
                Piece curr_piece = piecesPosition[i][j];
                if (line == 0) {
                    printf("|        (%d, %d)       | ", i, j);
                } else if (line == 1) {
                    printf("|    [%s]    | ", get_piece_name(curr_piece));
                }
            }
            printf("\n");
        }
        printf(
            "--------------------------------------------------------------------------------------"
            "---------------------------------------------------------\n");
    }
}

Move computer_move;

#define N_COMPUTER_MOVES 3

int main() {
    Move computer_moves[N_COMPUTER_MOVES];
    int len = 0;
    if (chess_engine(computer_moves, &len, N_COMPUTER_MOVES, BLACK_PIECE)) {
        printf("Computer moves returned:\n");
        assert(len > 0);
        print_valid_moves(computer_moves, len);
    } else {
        assert(len == 0);
    }
    visualize_board();
    return 0;
}