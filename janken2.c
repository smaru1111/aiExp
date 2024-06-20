#include<stdio.h>
#include<limits.h>
#include<time.h>

#define METHOD    0  // ミニマックス法を用いて探索(0)，アルファベータ法を用いて探索(1)
#define COM   1
#define ENEMY 2
#define INFINITY  INT_MAX
#define ACTION_GUU   0
#define ACTION_CHOKI 1
#define ACTION_PAA   2
#define ACTION_LIZARD 3
#define ACTION_SPOCK  4

int actionList[13];  // 最大深さ13に対応するように変更
int reward[5]  = { 0, 2, 5, 1, 4 };
char action[5] = { 'G', 'C', 'P', 'L', 'S' };

int reverse(int);
int minmax(int, int);
int alphabeta(int, int, int, int);
int eval(void);

int MAX_DEPTH = 3;  // 先読みの深さを可変にするために定義

int
main()
{
    clock_t start, end;
    double total_time;
    int depth;
    int i;

    printf("depth, time\n");
    for (depth = 3; depth <= 10; depth++) {
        MAX_DEPTH = depth;
        total_time = 0.0;

        for (i = 0; i < 10; i++) {
            start = clock();
            if (METHOD == 0) {
                // ミニマックス法で最善手を探索
                minmax(COM, 0);
            } else {
                // アルファベータ法で最善手を探索
                alphabeta(COM, 0, -INFINITY, INFINITY);
            }
            end = clock();
            total_time += (double)(end - start) / CLOCKS_PER_SEC;
        }

        printf("%d, %f\n", depth, total_time / 10);
    }

    return 0;
}

/**
 * COMとENEMYを入れ替える関数
 *
 * who    : COM or ENEMY
 * return : COMならENEMY, ENEMYならCOMを返す．
 */
int
reverse(int who){
    if (who == COM) { return ENEMY; }
    else { return COM; }
}

/**
 * ミニマックス法を用いて最善手を探索する関数
 *
 * who    : COMの手番ならCOM, 敵の手番ならENEMY
 * depth  : 読みの深さ
 * return : ルートノードでは最善手，それ以外では評価値
 */
int
minmax(int who, int depth){

    int i,d;
    int evaluation;     // 評価値
    int bestAction;     // 最善手
    int bestEvaluation; // 最善評価値

    // 葉ノードではゲーム結果を評価
    if (depth == MAX_DEPTH) { return eval(); }

    // COMの手番では最大値を見つけたいので最小値をセットしておく
    if (who == COM)  { bestEvaluation = -INFINITY; }

    // ENEMYの手番では最小値を見つけたいので最大値をセットしておく
    if (who == ENEMY){ bestEvaluation = INFINITY; }

    // グー・チョキ・パー・リザード・スポック全て調べる
    for (i = 0; i <= 4; i++){

        actionList[depth] = i;

        // 子ノードの評価値を再帰計算
        evaluation = minmax(reverse(who), depth+1);

        // COMの手番なら最大の評価値を選ぶ
        if (who == COM   && evaluation > bestEvaluation){ bestAction = i; bestEvaluation = evaluation; }
        // ENEMYの手番なら最小の評価値を選ぶ
        if (who == ENEMY && evaluation < bestEvaluation){ bestAction = i; bestEvaluation = evaluation; }

    }

    if (depth == 0) {
        // ルートノードでは最善手
        return bestAction;
    } else{
        // それ以外では評価値
        return bestEvaluation;
    }
}

/**
 * アルファベータ法を用いて最善手を探索する関数
 *
 * who    : COMの手番ならCOM, 敵の手番ならENEMY
 * depth  : 読みの深さ
 * alpha  : ノードの評価値は必ず alpha 以上となる
 * beta   : ノードの評価値は必ず beta  以下となる
 * return : ルートノードでは最善手，それ以外では評価値
 */
int
alphabeta(int who, int depth, int alpha, int beta){

    int i,d;
    int evaluation;     // 評価値
    int bestAction;     // 最善手
    int bestEvaluation; // 最善評価値

    // 葉ノードではゲーム結果を評価
    if (depth == MAX_DEPTH) { return eval(); }

    // COMの手番では最大値を見つけたいので最小値をセットしておく
    if (who == COM)  { bestEvaluation = -INFINITY; }
    // ENEMYの手番では最小値を見つけたいので最大値をセットしておく
    if (who == ENEMY){ bestEvaluation =  INFINITY; }

    // グー・チョキ・パー・リザード・スポック全て調べる
    for (i = 0; i <= 4; i++){

        actionList[depth] = i;

        // 子ノードの評価値を再帰計算
        evaluation = alphabeta(reverse(who), depth + 1, alpha, beta);

        // ベータカット
        if (who == COM   && evaluation >= beta)  { return evaluation; }
        // アルファカット
        if (who == ENEMY && evaluation <= alpha) { return evaluation; }

        // COMの手番なら最大の評価値を選ぶ
        if (who == COM   && evaluation > bestEvaluation){ bestAction = i;  bestEvaluation = evaluation; alpha = evaluation; }
        // ENEMYの手番なら最小の評価値を選ぶ
        if (who == ENEMY && evaluation < bestEvaluation){ bestAction = i;  bestEvaluation = evaluation; beta = evaluation; }

    }

    if (depth == 0) {
        // ルートノードでは最善手
        return bestAction;
    }
    else{
        // それ以外では評価値
        return bestEvaluation;
    }
}

/**
* actionList の評価値を返す
*
* return : 評価値
*/
int
eval(void){

    int d;
    int factor;
    int evaluation = 0;

    for (d = 1; d < MAX_DEPTH; d++){

        if (d % 2 == 0) factor = 1;
        else factor = -1;

        if ((actionList[d] == ACTION_CHOKI && actionList[d - 1] == ACTION_PAA) || (actionList[d] == ACTION_PAA && actionList[d - 1] == ACTION_GUU) ||
            (actionList[d] == ACTION_GUU && actionList[d - 1] == ACTION_CHOKI) || (actionList[d] == ACTION_CHOKI && actionList[d - 1] == ACTION_LIZARD) ||
            (actionList[d] == ACTION_LIZARD && actionList[d - 1] == ACTION_SPOCK) || (actionList[d] == ACTION_SPOCK && actionList[d - 1] == ACTION_GUU) ||
            (actionList[d] == ACTION_GUU && actionList[d - 1] == ACTION_LIZARD) || (actionList[d] == ACTION_LIZARD && actionList[d - 1] == ACTION_PAA) ||
            (actionList[d] == ACTION_PAA && actionList[d - 1] == ACTION_SPOCK) || (actionList[d] == ACTION_SPOCK && actionList[d - 1] == ACTION_CHOKI)) {
            evaluation += factor * reward[actionList[d]];
        } else if ((actionList[d - 1] == ACTION_CHOKI && actionList[d] == ACTION_PAA) || (actionList[d - 1] == ACTION_PAA && actionList[d] == ACTION_GUU) ||
                   (actionList[d - 1] == ACTION_GUU && actionList[d] == ACTION_CHOKI) || (actionList[d - 1] == ACTION_CHOKI && actionList[d] == ACTION_LIZARD) ||
                   (actionList[d - 1] == ACTION_LIZARD && actionList[d] == ACTION_SPOCK) || (actionList[d - 1] == ACTION_SPOCK && actionList[d] == ACTION_GUU) ||
                   (actionList[d - 1] == ACTION_GUU && actionList[d] == ACTION_LIZARD) || (actionList[d - 1] == ACTION_LIZARD && actionList[d] == ACTION_PAA) ||
                   (actionList[d - 1] == ACTION_PAA && actionList[d] == ACTION_SPOCK) || (actionList[d - 1] == ACTION_SPOCK && actionList[d] == ACTION_CHOKI)) {
            evaluation -= factor * reward[actionList[d - 1]];
        }
    }

    return evaluation;
}
