#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define SEED  0
#define TRUE  1
#define FALSE 0

#define EMPTY 0 // �F�i��j
#define BLACK 1 // �F�i���΁j
#define WHITE 2 // �F�i���΁j
#define INFINITY  INT_MAX

#define NUMBER_OF_GAMES 1000 // �ΐ퐔
#define BOARD_SIZE_X 3 // �ՖʃT�C�Y��
#define BOARD_SIZE_Y 3 // �ՖʃT�C�Y�c
#define MAX_EMPTY (BOARD_SIZE_X * BOARD_SIZE_Y - 4) // ��̍ő吔

typedef struct {
    int x;
    int y;
} pos_t; // �ꏊ

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

    pos_t your_pos, enemy_pos; // ���E�G�̏ꏊ
    int win_number = 0; // ���̏�����
    int your_color = BLACK; // ���F�͍�
    int enemy_color = WHITE; // �G�F�͔�
    int you_can_put_anywhere, enemy_can_put_anywhere; // ���E�G�͂ǂ����ɐ΂�u���邩
    int lose_flag = FALSE; // �u����̂ɒu���Ȃ������ꍇ�A���̎��_�ŕ���
    int board[BOARD_SIZE_X][BOARD_SIZE_Y]; // �Ֆ�

    srand(SEED);
    for (int i = 0; i < NUMBER_OF_GAMES; i++) {

        lose_flag = FALSE;
        init_board(board);

        do
        {
            // ���̔�
            you_can_put_anywhere = can_put_stone_anywhere(your_color, board);
            if (you_can_put_anywhere == TRUE) {
                get_your_position(&your_pos, your_color, board);
                if (can_put_stone(your_pos, your_color, board) == TRUE) {
                    put_stone(your_pos, your_color, board);
                }
                else {
                    // �u����̂ɒu���Ȃ������̂ŕ���
                    lose_flag = TRUE;
                    break;
                }
            }

            // �G�̔�
            enemy_can_put_anywhere = can_put_stone_anywhere(enemy_color, board);
            if (enemy_can_put_anywhere == TRUE) {
                get_random_position(&enemy_pos, enemy_color, board);
                put_stone(enemy_pos, enemy_color, board);
            }

            // ���E�G���ɐ΂��u���Ȃ��Ȃ�ΏI��
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
 * ���̒u���ꏊ��Ԃ�
 * ret_pos : �ꏊ
 * color  : �F
 * board  : �Ֆ�
 */
void
get_your_position(pos_t* ret_pos, int color, int board[][BOARD_SIZE_Y])
{
    minmax(ret_pos, BLACK, 0, board);
    // get_random_position(ret_pos, color, board);
}

/**
 * �u����ꏊ���烉���_���ȏꏊ��Ԃ�
 * retPos : �ꏊ
 * color  : �F
 * board  : �Ֆ�
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
 * �F�𔽓]���ĕԂ�
 * color : �F�iBLACK/WHITE/EMPTY�j
 */
int
reverse_color(int color) {
    if (color == WHITE) return BLACK;
    if (color == BLACK) return WHITE;
    return EMPTY;
}

/**
 * �Ֆʂ�����������
 * board : �Ֆ�
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
 * �΂�u��
 * pos   : �ꏊ
 * color : �F
 * board : �Ֆ�
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
 * �΂��u���邩��Ԃ��i�u����FTRUE�A�u���Ȃ��FFALSE�j
 * pos   : �ꏊ
 * color : �F
 * board : �Ֆ�
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
 * �Ֆʂ̂ǂ����ɐ΂��u���邩��Ԃ��i�ǂ����ɒu����FTRUE�A�ǂ��ɂ��u���Ȃ��FFALSE�j
 * color : �F
 * board : �Ֆ�
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
 * ���΂̐��]���΂̐���Ԃ�
 * board : �Ֆ�
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
 * �Ֆʂ�\������
 * board : �Ֆ�
 */
void
print_board(int board[][BOARD_SIZE_Y])
{
    pos_t pos;
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (board[pos.x][pos.y] == BLACK) printf("�a");
            if (board[pos.x][pos.y] == WHITE) printf("�v");
            if (board[pos.x][pos.y] == EMPTY) printf("�E");
        }
        printf("\n");
    }
}

/**
 * �~�j�}�b�N�X�@��p���čőP���T������֐�
 * ���[�g�m�[�h�ł�retBestPos�ɍőP��������ĕԂ��B����ȊO�ł͕]���l��Ԃ��B
 *
 * color  : �F
 * depth  : �ǂ݂̐[��
 * board  : �Ֆ�
 */
int
minmax(pos_t* retBestPos, int color, int depth, int board[][BOARD_SIZE_Y])
{
    pos_t pos, bestPos;
    int evaluation;     // �]���l
    int bestEvaluation; // �őP�]���l

    // �t�m�[�h�ł͕]���l��Ԃ�
    if (can_put_stone_anywhere(color, board) == FALSE && can_put_stone_anywhere(reverse_color(color), board) == FALSE) return evaluate_board(board);

    // ���Ԃ̃p�X�ł͓G�Ԃɂ��ĒT��
    if (can_put_stone_anywhere(color, board) == FALSE && can_put_stone_anywhere(reverse_color(color), board) == TRUE) color = reverse_color(color);

    // ���Ԃł͍ő�l�����������̂ōŏ��l���Z�b�g����
    // �G�Ԃł͍ŏ��l�����������̂ōő�l���Z�b�g����
    if (color == BLACK) { bestEvaluation = -INFINITY; }
    else { bestEvaluation = INFINITY; }

    // �u����}�X��S�Ē��ׂ�
    for (pos.y = 0;pos.y < BOARD_SIZE_Y;pos.y++) {
        for (pos.x = 0;pos.x < BOARD_SIZE_X;pos.x++) {
            if (can_put_stone(pos, color, board) == TRUE) {

                // �q�m�[�h�̔Ֆʊm�ۂƍ쐬
                int next_board[BOARD_SIZE_X][BOARD_SIZE_Y];
                memcpy(next_board, board, sizeof(int) * BOARD_SIZE_X * BOARD_SIZE_Y);
                put_stone(pos, color, next_board);

                // �q�m�[�h�̕]���l���ċA�v�Z
                evaluation = minmax(NULL,reverse_color(color), depth + 1, next_board);

                // ���ԂȂ�ő�̕]���l��I��
                if (color == BLACK && evaluation > bestEvaluation) { bestPos = pos; bestEvaluation = evaluation; }
                // �G�ԂȂ�ŏ��̕]���l��I��
                if (color == WHITE && evaluation < bestEvaluation) { bestPos = pos; bestEvaluation = evaluation; }
            }
        }
    }

    // ���[�g�m�[�h�ł�retBestPos�ɍőP��������ĕԂ��B����ȊO�ł͕]���l��Ԃ��B
    if (depth == 0) { *retBestPos = bestPos; }
    return bestEvaluation;
}
