#include <stdio.h>
#include <stdlib.h>

#define SEED               0 // 乱数の種
#define LEARNING_RATE   0.01 // 学習率
#define DISCOUNT_RATE    0.9 // 割引率
#define LEARN_TIME      1000 // 最大更新回数
#define ENEMY_STRATEGY    16 // 敵の戦略(カード合計が ENEMY_STRATEGY 以下ならヒット)
#define REWARD_WIN       100 // 報酬（勝った時）
#define REWARD_DRAW        0 // 報酬（引き分けた時）
#define REWARD_LOSE     -100 // 報酬（負けた時）

#define STATE_NUMBER      22 // カード合計1～21とバースト(22)
#define ACTION_NUMBER      2 // ヒットとステイ

#define HIT                0
#define STAY               1

#define BATTLE             0
#define COM_WIN            1
#define COM_DRAW           2
#define COM_LOSE           3

double q[STATE_NUMBER][ACTION_NUMBER];
double reward_table[4] = { 0, REWARD_WIN, REWARD_DRAW, REWARD_LOSE }; // 勝負中，勝ち，引分け，負け

void init_q(void);
void init_card(int *,int *);
void learn(void);
void print_q(void);
int get_card(void);
int get_com_action(int);
int get_enemy_action(int);
int get_enemy_total(int, int);
int judge(int, int, int);
int minimum(int, int);
double maximum(double, double);

int
main(){

	srand(SEED);
	init_q();
	learn();
	
	return 0;
}

void
init_q(void){
	for (int s = 0; s < STATE_NUMBER; s++){
		for (int a = 0; a < ACTION_NUMBER; a++){
			q[s][a] = 0;
		}
	}
}

void
learn(void){

	int com_state, com_action;
	int next_com_state;
	int enemy_total;
	int result;
	int win_num=0, draw_num=0, lose_num=0;
	double reward;
	double max_q;

	init_card(&com_state,&enemy_total);
	for (int t = 0; t < LEARN_TIME; t++){

		com_action = get_com_action(com_state);
		if (com_action == HIT) { next_com_state = minimum(22,com_state + get_card()); }
		else { next_com_state = com_state; }

		enemy_total = get_enemy_total(com_action, enemy_total);
		result = judge(com_state, com_action, enemy_total);
		reward = reward_table[result];
		if (result == COM_WIN)  { win_num++; }
		if (result == COM_DRAW) { draw_num++; }
		if (result == COM_LOSE) { lose_num++; }
		if (win_num + draw_num + lose_num != 0) {
			printf("%f\n", (double)win_num / (win_num + draw_num + lose_num)*100);
		}

		max_q = maximum(q[next_com_state - 1][HIT], q[next_com_state - 1][STAY]);
		q[com_state-1][com_action] += LEARNING_RATE * (reward + DISCOUNT_RATE * max_q - q[com_state-1][com_action]);
		
		com_state = next_com_state;
		if (result != BATTLE) {
			init_card(&com_state, &enemy_total);
		}
	}
	
	print_q();
}

void
init_card(int *com_state, int *enemy_total){
	*com_state   = get_card() + get_card();
	*enemy_total = get_card() + get_card();
}

int
get_card(void){
	return minimum(10,1 + (int)((double)rand() * 13 / (1.0 + RAND_MAX)));
}

int
get_com_action(int com_state){
	if (q[com_state-1][HIT] > q[com_state-1][STAY]) { return HIT; }
	else { return STAY; }
}

int
get_enemy_total(int com_action,int enemy_total){
	
	int enemy_action;

	if (com_action == HIT) {
		enemy_action = get_enemy_action(enemy_total);
		if (enemy_action == HIT) { enemy_total = minimum(22, enemy_total + get_card()); }
	} else {
		enemy_action = get_enemy_action(enemy_total);
		while (enemy_action == HIT && enemy_total != 22) {
			enemy_total = minimum(22, enemy_total + get_card());
			enemy_action = get_enemy_action(enemy_total);
		}
	}
	return enemy_total;
}

int
get_enemy_action(int enemy_total){
	if (enemy_total <= ENEMY_STRATEGY) { return HIT; }
	else { return STAY; }
}

int
judge(int com_state, int com_action, int enemy_total){
	if (com_state == 22) { return COM_LOSE; }
	if (enemy_total == 22) { return COM_WIN; }
	if (com_action == STAY) { 
		if (com_state > enemy_total) return COM_WIN;
		if (com_state < enemy_total) return COM_LOSE;
		return COM_DRAW;
	}
	return BATTLE;
}

void
print_q(void){
	for (int a = 0; a < ACTION_NUMBER; a++){
		if (a == 0) printf("HIT:\n");
		else printf("STAY:\n");
		for (int s = 0; s < STATE_NUMBER; s++){
			printf("%2d : %6.2f\n", s+1, q[s][a]);
		}
	}
}

int
minimum(int a, int b){
	if (a > b) return b;
	return a;
}

double
maximum(double a, double b){
	if (a < b) return b;
	return a;
}