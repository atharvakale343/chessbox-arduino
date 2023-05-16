#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

// Defines for chess pieces and spaces
typedef enum Piece {
    NO_PIECE = -1,
    BORDER = 255,
    EMPTY = 0,
    BLACK_PAWN = 1,
    BLACK_ROOK = 5,
    BLACK_QUEEN = 9,
    BLACK_KING = 16,
    WHITE_PAWN = 129,
    WHITE_ROOK = 133,
    WHITE_QUEEN = 137,
    WHITE_KING = 144,
} Piece;

#define BOARD_ROWS 6
#define BOARD_COLS 6

// bitmap
extern Piece piecesPosition[BOARD_ROWS][BOARD_COLS];

typedef struct {
    int x;
    int y;
} PiecePosition;

typedef struct {
    Piece piece;
    PiecePosition source;
    PiecePosition dest;
    int move_value;
} Move;

typedef enum { BLACK_PIECE, WHITE_PIECE, NO_COLOR } PieceColor;

const char *get_piece_name(Piece piece);

#define NUM_OF_BLACK_PIECES 4
extern Piece black_pieces[NUM_OF_BLACK_PIECES];

#define MAX_VALID_MOVES_CAPACITY 100

extern Move valid_moves[MAX_VALID_MOVES_CAPACITY];
extern int valid_moves_len;

bool is_queen(Piece piece, PieceColor pieceColor);

bool is_rook(Piece piece, PieceColor pieceColor);

bool is_pawn(Piece piece, PieceColor pieceColor);

bool is_king(Piece piece, PieceColor pieceColor);

int get_move_value(Move move);

void add_move_into_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Move move);

PieceColor getPieceColor(Piece piece);

bool isOppPieceColor(Piece piece1, Piece piece2);

PieceColor getOppPieceColor(Piece piece);

bool locate_piece_from(PiecePosition *to_return, Piece piece, int startX, int startY);

Move initialize_move(Piece piece, PiecePosition piece_loc);

void add_dest_to_move(Move *move, PiecePosition dest);

Piece get_piece_at_position(PiecePosition position);

void getValidMovesForPawn(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece);

void try_add_delta_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece,
                         PiecePosition original_piece_loc, int dx, int dy);
// also returns if the position is in bounds
bool add_delta_to_pos(PiecePosition *other_piece_pos, int dx, int dy);

bool gen_queen_rook_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Move move, Piece piece,
                          PiecePosition other_piece_pos);

void getValidMovesForRook(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece);

void getValidMovesForQueen(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece);

int kingIsInCheck(Piece kingPiece);

int check_for_certain_piece_at_deltas(int deltaArray[][2], int deltaArrayLength, int (*piece_func)(Piece, PieceColor),
                                      PiecePosition origPiecePos, PieceColor oppPieceColor);

Piece get_king_piece_of_color(PieceColor pieceColor);

int is_valid_pos_for_king(PieceColor kingColor, PiecePosition from_pos, PiecePosition to_pos);

void try_add_delta_king_moves(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece,
                              PiecePosition original_piece_loc, int dx, int dy);

void getValidMovesForKing(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece);

void print_valid_moves(Move valid_moves[], int valid_moves_len);

void getValidMovesFor(Move valid_moves[], int *valid_moves_len, int valid_moves_cap, Piece piece);

Move get_best_move(Move valid_moves[], int *valid_moves_len);

int get_best_moves(Move put_best_into[], int put_best_into_size, Move valid_moves[], int valid_moves_len);

int chess_engine(Move to_return_moves[], int *to_return_moves_len, int to_return_moves_size, PieceColor color);

void visualize_board();

#endif