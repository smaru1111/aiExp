#include<stdio.h>
#include<limits.h>
#include<time.h>

#define MAX_DEPTH 3  // 先読みの深さ
#define METHOD    0  // ミニマックス法を用いて探索(0)，アルファベータ法を用いて探索(1)

#define COM   1
#define ENEMY 2

#define INFINITY  INT_MAX

#define ACTION_GUU   0
#define ACTION_CHOKI 1
#define ACTION_PAA   2

int actionList[MAX_DEPTH];
int reward[3]  = { 0, 2, 5 };
char action[3] = { 'G', 'C', 'P' };

int reverse(int);
int minmax(int, int);
int alphabeta(int, int, int, int);
int eval(void);

int
main()
{
	clock_t start, end;

	start = clock();

	if (METHOD == 0) {
		// ミニマックス法で最善手を探索
		printf("bestAction = %c\n", action[minmax(COM, 0)]);
	}
	else{
		// アルファベータ法で最善手を探索
		printf("bestAction = %c\n", action[alphabeta(COM, 0, -INFINITY, INFINITY)]);
	}

	end = clock();
	printf("処理時間:%f秒\n", (double)(end - start)/ CLOCKS_PER_SEC);

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
	
	// グー・チョキ・パー全て調べる
	for (i = 0; i <= 2; i++){

		actionList[depth] = i;

		// 子ノードの評価値を再帰計算
		evaluation = minmax(reverse(who), depth+1);

		// 評価値を表示
		for (d = 0; d < MAX_DEPTH; d++){
			if (d <= depth) { printf("%c", action[actionList[d]]); }
			else { printf(" "); }
		}
		printf(" : %3d\n", evaluation);
		
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
	
	// グー・チョキ・パー全て調べる
	for (i = 0; i <= 2; i++){

		actionList[depth] = i;

		// 子ノードの評価値を再帰計算
		evaluation = alphabeta(reverse(who), depth + 1, alpha, beta);
		
		// 評価値を表示
		for (d = 0; d < MAX_DEPTH; d++){
			if (d <= depth) { printf("%c", action[actionList[d]]); }
			else { printf(" "); }
		}
		printf(" : %3d\n", evaluation);
		
		// ベータカット
		if (who == COM   && evaluation >= beta)  { printf("beta cut\n"); return evaluation; }
		// アルファカット
		if (who == ENEMY && evaluation <= alpha) { printf("alpha cut\n"); return evaluation; }

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
		
		if (actionList[d] == ACTION_CHOKI && actionList[d - 1] == ACTION_PAA) { evaluation += factor * reward[ACTION_CHOKI]; }
		if (actionList[d] == ACTION_PAA   && actionList[d - 1] == ACTION_GUU) { evaluation += factor * reward[ACTION_PAA]; }
	}

	return evaluation;
}
