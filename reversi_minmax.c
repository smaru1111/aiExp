#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define SEED  0
#define TRUE  1
#define FALSE 0

#define EMPTY 0 // 色（空）
#define BLACK 1 // 色（黒石）
#define WHITE 2 // 色（白石）
#define INFINITY  INT_MAX

#define NUMBER_OF_GAMES 1000 // 対戦数
#define BOARD_SIZE_X 3 // 盤面サイズ横
#define BOARD_SIZE_Y 3 // 盤面サイズ縦
#define MAX_EMPTY (BOARD_SIZE_X * BOARD_SIZE_Y - 4) // 空の最大数

typedef struct {
    int x;
    int y;
} pos_t; // 場所

void get_your_position(pos_t*, int, int[][BOARD_SIZE_Y]);
void get_random_position(pos_t*, int, int[][BOARD_SIZE_Y]);
int reverse_color(int);
void init_board(int[][BOARD_SIZE_Y]);
void put_stone(pos_t, int, int[][BOARD_SIZE_Y]);
int can_put_stone(pos_t, int, int[][BOARD_SIZE_Y]);
int can_put_stone_anywhere(int, int[][BOARD_SIZE_Y]);
int evaluate_board(int[][BOARD_SIZE_Y]);
void print_board(int[][BOARD_SIZE_Y]);

int minmax(pos_t*, int, int, int[][BOARD_SIZE_Y]);

int
main(void) {

    pos_t your_pos, enemy_pos; // 自・敵の場所
    int win_number = 0; // 自の勝ち数
    int your_color = BLACK; // 自色は黒
    int enemy_color = WHITE; // 敵色は白
    int you_can_put_anywhere, enemy_can_put_anywhere; // 自・敵はどこかに石を置けるか
    int lose_flag = FALSE; // 置けるのに置かなかった場合、その時点で負け
    int board[BOARD_SIZE_X][BOARD_SIZE_Y]; // 盤面

    srand(SEED);
    for (int i = 0; i < NUMBER_OF_GAMES; i++) {

        lose_flag = FALSE;
        init_board(board);

        do
        {
            // 自の番
            you_can_put_anywhere = can_put_stone_anywhere(your_color, board);
            if (you_can_put_anywhere == TRUE) {
                get_your_position(&your_pos, your_color, board);
                if (can_put_stone(your_pos, your_color, board) == TRUE) {
                    put_stone(your_pos, your_color, board);
                }
                else {
                    // 置けるのに置かなかったので負け
                    lose_flag = TRUE;
                    break;
                }
            }

            // 敵の番
            enemy_can_put_anywhere = can_put_stone_anywhere(enemy_color, board);
            if (enemy_can_put_anywhere == TRUE) {
                get_random_position(&enemy_pos, enemy_color, board);
                put_stone(enemy_pos, enemy_color, board);
            }

            // 自・敵共に石が置けなくなれば終了
        } while (lose_flag == FALSE && (you_can_put_anywhere == TRUE || enemy_can_put_anywhere == TRUE));

        if (lose_flag == FALSE && evaluate_board(board) > 0) {
            win_number++;
            printf("o");
        }
        else {
            printf("x");
        }
    }

    printf("win rate = %4.1f\n", (double)win_number * 100 / NUMBER_OF_GAMES);
}

/**
 * 自の置く場所を返す
 * ret_pos : 場所
 * color  : 色
 * board  : 盤面
 */
void
get_your_position(pos_t* ret_pos, int color, int board[][BOARD_SIZE_Y])
{
    minmax(ret_pos, BLACK, 0, board);
    // get_random_position(ret_pos, color, board);
}

/**
 * 置ける場所からランダムな場所を返す
 * retPos : 場所
 * color  : 色
 * board  : 盤面
 */
void
get_random_position(pos_t* ret_pos, int color, int board[][BOARD_SIZE_Y])
{
    int r, can_put_stone_number;
    pos_t pos, posList[MAX_EMPTY];

    can_put_stone_number = 0;
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (can_put_stone(pos, color, board) == TRUE) {
                can_put_stone_number++;
                posList[can_put_stone_number - 1] = pos;
            }
        }
    }

    if (can_put_stone_number >= 1) {
        r = (int)((rand() / (RAND_MAX + 1.0)) * can_put_stone_number);
        *ret_pos = posList[r];
    }
}

/**
 * 色を反転して返す
 * color : 色（BLACK/WHITE/EMPTY）
 */
int
reverse_color(int color) {
    if (color == WHITE) return BLACK;
    if (color == BLACK) return WHITE;
    return EMPTY;
}

/**
 * 盤面を初期化する
 * board : 盤面
 */
void
init_board(int board[][BOARD_SIZE_Y])
{
    pos_t pos;
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            board[pos.x][pos.y] = EMPTY;
        }
    }

    board[1][BOARD_SIZE_Y / 2 - 1] = WHITE; board[2][BOARD_SIZE_Y / 2 - 1] = BLACK;
    board[1][BOARD_SIZE_Y / 2] = BLACK; board[2][BOARD_SIZE_Y / 2] = WHITE;
}

/**
 * 石を置く
 * pos   : 場所
 * color : 色
 * board : 盤面
 */
void
put_stone(pos_t pos, int color, int board[][BOARD_SIZE_Y])
{
    int r, reverse_num;
    int rev_color = reverse_color(color);

    board[pos.x][pos.y] = color;

    for (int dir_x = -1;dir_x < 2;dir_x++) {
        for (int dir_y = -1;dir_y < 2;dir_y++) {
            if (dir_x == 0 && dir_y == 0) continue;
            for (reverse_num = 1;;reverse_num++) {
                if (pos.x + dir_x * reverse_num < 0 || pos.x + dir_x * reverse_num >= BOARD_SIZE_X ||
                    pos.y + dir_y * reverse_num < 0 || pos.y + dir_y * reverse_num >= BOARD_SIZE_Y ||
                    board[pos.x + dir_x * reverse_num][pos.y + dir_y * reverse_num] != rev_color) break;
            }
            if (pos.x + dir_x * reverse_num >= 0 && pos.x + dir_x * reverse_num < BOARD_SIZE_X &&
                pos.y + dir_y * reverse_num >= 0 && pos.y + dir_y * reverse_num < BOARD_SIZE_Y &&
                board[pos.x + dir_x * reverse_num][pos.y + dir_y * reverse_num] == color) {
                for (r = 1;r < reverse_num;r++) {
                    board[pos.x + dir_x * r][pos.y + dir_y * r] = color;
                }
            }
        }
    }
}

/**
 * 石が置けるかを返す（置ける：TRUE、置けない：FALSE）
 * pos   : 場所
 * color : 色
 * board : 盤面
 */
int
can_put_stone(pos_t pos, int color, int board[][BOARD_SIZE_Y])
{
    int reverse_num;
    int rev_color = reverse_color(color);

    if (board[pos.x][pos.y] != EMPTY) return FALSE;

    for (int dir_x = -1;dir_x < 2;dir_x++) {
        for (int dir_y = -1;dir_y < 2;dir_y++) {
            if (dir_x == 0 && dir_y == 0) continue;
            for (reverse_num = 1;;reverse_num++) {
                if (pos.x + dir_x * reverse_num < 0 || pos.x + dir_x * reverse_num >= BOARD_SIZE_X ||
                    pos.y + dir_y * reverse_num < 0 || pos.y + dir_y * reverse_num >= BOARD_SIZE_Y ||
                    board[pos.x + dir_x * reverse_num][pos.y + dir_y * reverse_num] != rev_color) break;
            }
            if (pos.x + dir_x * reverse_num >= 0 && pos.x + dir_x * reverse_num < BOARD_SIZE_X &&
                pos.y + dir_y * reverse_num >= 0 && pos.y + dir_y * reverse_num < BOARD_SIZE_Y &&
                reverse_num >= 2 && board[pos.x + dir_x * reverse_num][pos.y + dir_y * reverse_num] == color) return TRUE;
        }
    }

    return FALSE;
}

/**
 * 盤面のどこかに石が置けるかを返す（どこかに置ける：TRUE、どこにも置けない：FALSE）
 * color : 色
 * board : 盤面
 */
int
can_put_stone_anywhere(int color, int board[][BOARD_SIZE_Y])
{
    pos_t pos;
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (can_put_stone(pos, color, board) == TRUE) return TRUE;
        }
    }
    return FALSE;
}

/**
 * 黒石の数‐白石の数を返す
 * board : 盤面
 */
int
evaluate_board(int board[][BOARD_SIZE_Y])
{
    int res = 0;
    pos_t pos;
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (board[pos.x][pos.y] == BLACK) res++;
            if (board[pos.x][pos.y] == WHITE) res--;
        }
    }
    return res;
}

/**
 * 盤面を表示する
 * board : 盤面
 */
void
print_board(int board[][BOARD_SIZE_Y])
{
    pos_t pos;
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (board[pos.x][pos.y] == BLACK) printf("Ｂ");
            if (board[pos.x][pos.y] == WHITE) printf("Ｗ");
            if (board[pos.x][pos.y] == EMPTY) printf("・");
        }
        printf("\n");
    }
}

/**
 * ミニマックス法を用いて最善手を探索する関数
 * ルートノードではretBestPosに最善手を代入して返す。それ以外では評価値を返す。
 *
 * color  : 色
 * depth  : 読みの深さ
 * board  : 盤面
 */
int
minmax(pos_t* retBestPos, int color, int depth, int board[][BOARD_SIZE_Y])
{
    pos_t pos, bestPos;
    int evaluation;     // 評価値
    int bestEvaluation; // 最善評価値

    // 葉ノードでは評価値を返す
    if (can_put_stone_anywhere(color, board) == FALSE && can_put_stone_anywhere(reverse_color(color), board) == FALSE) return evaluate_board(board);

    // 自番のパスでは敵番にして探索
    if (can_put_stone_anywhere(color, board) == FALSE && can_put_stone_anywhere(reverse_color(color), board) == TRUE) color = reverse_color(color);

    // 自番では最大値を見つけたいので最小値をセットする
    // 敵番では最小値を見つけたいので最大値をセットする
    if (color == BLACK) { bestEvaluation = -INFINITY; }
    else { bestEvaluation = INFINITY; }

    // 置けるマスを全て調べる
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (can_put_stone(pos, color, board) == TRUE) {

                // 子ノードの盤面確保と作成
                int next_board[BOARD_SIZE_X][BOARD_SIZE_Y];
                memcpy(next_board, board, sizeof(int) * BOARD_SIZE_X * BOARD_SIZE_Y);
                put_stone(pos, color, next_board);

                // 子ノードの評価値を再帰計算
                evaluation = minmax(NULL,reverse_color(color), depth + 1, next_board);

                // 自番なら最大の評価値を選ぶ
                if (color == BLACK && evaluation > bestEvaluation) { bestPos = pos; bestEvaluation = evaluation; }
                // 敵番なら最小の評価値を選ぶ
                if (color == WHITE && evaluation < bestEvaluation) { bestPos = pos; bestEvaluation = evaluation; }
            }
        }
    }

    // ルートノードではretBestPosに最善手を代入して返す。それ以外では評価値を返す。
    if (depth == 0) { *retBestPos = bestPos; }
    return bestEvaluation;
}
