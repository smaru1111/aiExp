#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define SEED               39 // 乱数の種
#define TRAINING_NUMBER    5 // 訓練データの数
#define TEST_NUMBER      100 // テストデータの数
#define INPUT_NUMBER      15 // 入力層のニューロン数
#define HIDDEN_NUMBER     5 // 隠れ層のニューロン数
#define OUTPUT_NUMBER      5 // 出力層のニューロン数
#define LEARNING_RATE   0.01 // 学習率
// 0.001, 0.002, 0.01, 0.02, 0.1, 0.2 
#define LEARNING_TIME   1000 // 学習回数

void make_test(void);
void init_weight(void);
void learn(void);
double get_test_error(void);

double input_weight[HIDDEN_NUMBER][INPUT_NUMBER];          // 入力層から隠れ層への重み
double hidden_weight[OUTPUT_NUMBER][HIDDEN_NUMBER];        // 隠れ層から出力層への重み

double test_output[TEST_NUMBER][OUTPUT_NUMBER];            // テストデータの教師信号
double training_output[TRAINING_NUMBER][OUTPUT_NUMBER] = { // 訓練データの教師信号
	{ 1, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 0, 1 }
};

double test_input[TEST_NUMBER][INPUT_NUMBER];              // テストデータ
double training_input[TRAINING_NUMBER][INPUT_NUMBER] = {   // 学習データ
	{ 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1 },
	{ 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
	{ 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0 },
	{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1 }
};

int main()
{
	srand(SEED);

	make_test();
	init_weight();
	learn();

	return 0;
}

/* =================================== */
/* テストデータと教師信号の作成        */
/* 学習データを10％の確率で0,1反転する */
/* ことによりテストデータを作成する．  */
/* =================================== */

void make_test(void)
{
	int t, i, o, tn;

	for (t = 0; t < TEST_NUMBER; t++){

		tn = (int)(((double)rand() / (1.0 + RAND_MAX)) * OUTPUT_NUMBER);
		
		for (i = 0; i < INPUT_NUMBER; i++){
			if (((double)rand() / ((double)RAND_MAX + 1)) < 0.1){
				test_input[t][i] = 1 - training_input[tn][i];
			}
			else{
				test_input[t][i] = training_input[tn][i];
			}
		}

		for (o = 0; o < OUTPUT_NUMBER; o++){
			test_output[t][o] = training_output[tn][o];
		}
	}
}

/* ================================= */
/* 重みの初期化                      */
/* input_weightとhidden_weightを全て */
/* -0.1～0.1 の一様乱数で初期化する．*/
/* ================================= */

void init_weight(void){

	int h, i, o;

	// input_weightの初期化
	for (h = 0; h < HIDDEN_NUMBER; h++){
		for (i = 0; i < INPUT_NUMBER; i++){
			input_weight[h][i] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
		}
	}

	// hidden_weightの初期化
	for (o = 0; o < OUTPUT_NUMBER; o++){
		for (h = 0; h < HIDDEN_NUMBER; h++){
			hidden_weight[o][h] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
		}
	}
}

/* ============== */
/* 重みを学習する */
/* ============== */

void learn(void){

	int l, t, h, i, o;

	double error,test_error;
	double wix, whs;
	double sigmoid_wix[TRAINING_NUMBER][HIDDEN_NUMBER];
	double o_whs[TRAINING_NUMBER][OUTPUT_NUMBER];
	double delta_sigmoid_wix[TRAINING_NUMBER][HIDDEN_NUMBER];
	double delta_input_weight[HIDDEN_NUMBER][INPUT_NUMBER];
	double delta_hidden_weight[OUTPUT_NUMBER][HIDDEN_NUMBER];

	for (l = 0; l < LEARNING_TIME; l++){

		// 中間細胞の出力 sigmoid_wix の計算
		// sigmoid関数の代わりに「sigmoid関数を微分した関数」を使用した際の中間細胞の出力 delta_sigmoid_wix の計算
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (h = 0; h < HIDDEN_NUMBER; h++){
				wix = 0;
				for (i = 0; i < INPUT_NUMBER; i++){
					wix += input_weight[h][i] * training_input[t][i];
				}
				sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
				delta_sigmoid_wix[t][h] = sigmoid_wix[t][h] * (1 - sigmoid_wix[t][h]);
			}
		}

		// 出力細胞の出力 whs の計算
		// 教師信号 - 出力細胞 o_whs の計算
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				whs = 0;
				for (h = 0; h < HIDDEN_NUMBER; h++){
					whs += hidden_weight[o][h] * sigmoid_wix[t][h];
				}
				o_whs[t][o] = training_output[t][o] - whs;
			}
		}

		// 訓練データの２乗誤差 error の計算・表示
		// テストデータの２乗誤差 test_error の計算・表示
		error = 0;
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				error += pow(o_whs[t][o], 2);
			}
		}
		test_error = get_test_error();
		// printf("%5.5f    %5.5f\n",error,test_error);
		printf("%d, %5.5f\n", l, error);

		// input_weight による E の微分値の計算
		for (h = 0; h < HIDDEN_NUMBER; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				delta_input_weight[h][i] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					for (o = 0; o < OUTPUT_NUMBER; o++){
						delta_input_weight[h][i] += -2 * delta_sigmoid_wix[t][h] * hidden_weight[o][h] * o_whs[t][o] * training_input[t][i];
					}
				}
			}
		}

		// hidden_weight による E の微分値の計算
		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < HIDDEN_NUMBER; h++){
				delta_hidden_weight[o][h] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					delta_hidden_weight[o][h] += -2 * o_whs[t][o] * sigmoid_wix[t][h];
				}
			}
		}

		// input_weight の更新
		for (h = 0; h < HIDDEN_NUMBER; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				input_weight[h][i] -= LEARNING_RATE * delta_input_weight[h][i];
			}
		}

		// hidden_weight の更新
		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < HIDDEN_NUMBER; h++){
				hidden_weight[o][h] -= LEARNING_RATE * delta_hidden_weight[o][h];
			}
		}
	}
}

/* ============================ */
/* テストデータの２乗誤差の獲得 */
/* ============================ */

double get_test_error(void){

	double test_error;
	double wix, whs;
	double o_whs[TEST_NUMBER][OUTPUT_NUMBER];
	double sigmoid_wix[TEST_NUMBER][HIDDEN_NUMBER];

	// 中間細胞の出力 sigmoid_wix の計算
	for (int t = 0; t < TEST_NUMBER; t++){
		for (int h = 0; h < HIDDEN_NUMBER; h++){
			wix = 0;
			for (int i = 0; i < INPUT_NUMBER; i++){
				wix += input_weight[h][i] * test_input[t][i];
			}
			sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
		}
	}

	// 出力細胞の出力 whs の計算
	// 教師信号 - 出力細胞 o_whs の計算
	for (int t = 0; t < TEST_NUMBER; t++){
		for (int o = 0; o < OUTPUT_NUMBER; o++){
			whs = 0;
			for (int h = 0; h < HIDDEN_NUMBER; h++){
				whs += hidden_weight[o][h] * sigmoid_wix[t][h];
			}
			o_whs[t][o] = test_output[t][o] - whs;
		}
	}

	// テストデータの２乗誤差の計算
	test_error = 0;
	for (int t = 0; t < TEST_NUMBER; t++){
		for (int o = 0; o < OUTPUT_NUMBER; o++){
			test_error += pow(o_whs[t][o], 2);
		}
	}

	return test_error / TEST_NUMBER;
}