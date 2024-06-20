#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SEED               39 // 乱数の種
#define TRAINING_NUMBER    10 // 訓練データの数
#define TEST_NUMBER      100 // テストデータの数
#define INPUT_NUMBER      15 // 入力層のニューロン数
#define OUTPUT_NUMBER      10 // 出力層のニューロン数

int HIDDEN_NUMBER; // 隠れ層のニューロン数
double LEARNING_RATE; // 学習率
int LEARNING_TIME; // 学習回数

void make_test(void);
void init_weight(void);
double learn(void);
double get_test_error(void);

double **input_weight;          // 入力層から隠れ層への重み
double **hidden_weight;        // 隠れ層から出力層への重み

double test_output[TEST_NUMBER][OUTPUT_NUMBER];            // テストデータの教師信号
double training_output[TRAINING_NUMBER][OUTPUT_NUMBER] = { // 訓練データの教師信号
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }
};

double test_input[TEST_NUMBER][INPUT_NUMBER];              // テストデータ
double training_input[TRAINING_NUMBER][INPUT_NUMBER] = {   // 学習データ
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },
    { 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1 },
    { 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1 },
    { 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1 },
    { 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
    { 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1 },
    { 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1 },
    { 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1 }
};

int index_of_main = 0;
double total_test_error = 0;
double *average_test_errors; // 各学習ステップの累積誤差

void free_weights() {
    for (int i = 0; i < HIDDEN_NUMBER; i++) {
        free(input_weight[i]);
    }
    free(input_weight);

    for (int i = 0; i < OUTPUT_NUMBER; i++) {
        free(hidden_weight[i]);
    }
    free(hidden_weight);
}

void allocate_weights() {
    input_weight = (double **)malloc(HIDDEN_NUMBER * sizeof(double *));
    for (int i = 0; i < HIDDEN_NUMBER; i++) {
        input_weight[i] = (double *)malloc(INPUT_NUMBER * sizeof(double));
    }

    hidden_weight = (double **)malloc(OUTPUT_NUMBER * sizeof(double *));
    for (int i = 0; i < OUTPUT_NUMBER; i++) {
        hidden_weight[i] = (double *)malloc(HIDDEN_NUMBER * sizeof(double));
    }

    average_test_errors = (double *)malloc(LEARNING_TIME * sizeof(double));
}

int main() {
    srand(SEED);

    int best_hidden_number = 0;
    double best_learning_rate = 0.0;
    int best_learning_time = 0;
    double min_average_error = 1e9;

    for (int hidden_number = 38; hidden_number <= 39; hidden_number += 3) {
        printf("------------------------------------\n");
        printf("Hidden number: %d\n", hidden_number);
        for (double learning_rate = 0.02; learning_rate <= 0.022; learning_rate += 0.001) {
            for (int learning_time = 710; learning_time <= 740; learning_time += 10) {

                HIDDEN_NUMBER = hidden_number;
                LEARNING_RATE = learning_rate;
                LEARNING_TIME = learning_time;

                allocate_weights();

                // average_test_errorsの初期化
                for (int l = 0; l < LEARNING_TIME; l++) {
                    average_test_errors[l] = 0.0;
                }

                // 各学習ステップの平均二乗誤差を表示
                double average_test_error = 0.0;
                total_test_error = 0;

                for (int iteration = 0; iteration < 100; iteration++) {
                    make_test();
                    init_weight();
                    average_test_error = learn();
                }


                printf("%5.5f\n", average_test_error);

                if (average_test_error < min_average_error) {
                    min_average_error = average_test_error;
                    best_hidden_number = hidden_number;
                    best_learning_rate = learning_rate;
                    best_learning_time = learning_time;
                    printf("Best hidden number: %d\n", best_hidden_number);
                    printf("Best learning rate: %f\n", best_learning_rate);
                    printf("Best learning time: %d\n", best_learning_time);
                    printf("Minimum average test error: %5.5f\n", min_average_error);
                }
                
                index_of_main++;

                free_weights();
            }
        }
    }

    printf("Best hidden number: %d\n", best_hidden_number);
    printf("Best learning rate: %f\n", best_learning_rate);
    printf("Best learning time: %d\n", best_learning_time);
    printf("Minimum average test error: %5.5f\n", min_average_error);

    return 0;
}

/* =================================== */
/* テストデータと教師信号の作成        */
/* 学習データを10％の確率で0,1反転する */
/* ことによりテストデータを作成する．  */
/* =================================== */

void make_test(void) {
    int t, i, o, tn;

    for (t = 0; t < TEST_NUMBER; t++) {

        tn = (int)(((double)rand() / (1.0 + RAND_MAX)) * OUTPUT_NUMBER);
        
        for (i = 0; i < INPUT_NUMBER; i++) {
            if (((double)rand() / ((double)RAND_MAX + 1)) < 0.1) {
                test_input[t][i] = 1 - training_input[tn][i];
            } else {
                test_input[t][i] = training_input[tn][i];
            }
        }

        for (o = 0; o < OUTPUT_NUMBER; o++) {
            test_output[t][o] = training_output[tn][o];
        }
    }
}

/* ================================= */
/* 重みの初期化                      */
/* input_weightとhidden_weightを全て */
/* -0.1～0.1 の一様乱数で初期化する．*/
/* ================================= */

void init_weight(void) {
    int h, i, o;

    // input_weightの初期化
    for (h = 0; h < HIDDEN_NUMBER; h++) {
        for (i = 0; i < INPUT_NUMBER; i++) {
            input_weight[h][i] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
        }
    }

    // hidden_weightの初期化
    for (o = 0; o < OUTPUT_NUMBER; o++) {
        for (h = 0; h < HIDDEN_NUMBER; h++) {
            hidden_weight[o][h] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
        }
    }
}

/* ============== */
/* 重みを学習する */
/* ============== */

double learn(void) {
    int l, t, h, i, o;

    double error, test_error;
    double wix, whs;
    double **sigmoid_wix = (double **)malloc(TRAINING_NUMBER * sizeof(double *));
    double **o_whs = (double **)malloc(TRAINING_NUMBER * sizeof(double *));
    double **delta_sigmoid_wix = (double **)malloc(TRAINING_NUMBER * sizeof(double *));
    double **delta_input_weight = (double **)malloc(HIDDEN_NUMBER * sizeof(double *));
    double **delta_hidden_weight = (double **)malloc(OUTPUT_NUMBER * sizeof(double *));

    for (t = 0; t < TRAINING_NUMBER; t++) {
        sigmoid_wix[t] = (double *)malloc(HIDDEN_NUMBER * sizeof(double));
        o_whs[t] = (double *)malloc(OUTPUT_NUMBER * sizeof(double));
        delta_sigmoid_wix[t] = (double *)malloc(HIDDEN_NUMBER * sizeof(double));
    }

    for (h = 0; h < HIDDEN_NUMBER; h++) {
        delta_input_weight[h] = (double *)malloc(INPUT_NUMBER * sizeof(double));
    }

    for (o = 0; o < OUTPUT_NUMBER; o++) {
        delta_hidden_weight[o] = (double *)malloc(HIDDEN_NUMBER * sizeof(double));
    }

    for (l = 0; l < LEARNING_TIME; l++) {
        // 中間細胞の出力 sigmoid_wix の計算
        // sigmoid関数の代わりに「sigmoid関数を微分した関数」を使用した際の中間細胞の出力 delta_sigmoid_wix の計算
        for (t = 0; t < TRAINING_NUMBER; t++) {
            for (h = 0; h < HIDDEN_NUMBER; h++) {
                wix = 0;
                for (i = 0; i < INPUT_NUMBER; i++) {
                    wix += input_weight[h][i] * training_input[t][i];
                }
                sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
                delta_sigmoid_wix[t][h] = sigmoid_wix[t][h] * (1 - sigmoid_wix[t][h]);
            }
        }

        // 出力細胞の出力 whs の計算
        // 教師信号 - 出力細胞 o_whs の計算
        for (t = 0; t < TRAINING_NUMBER; t++) {
            for (o = 0; o < OUTPUT_NUMBER; o++) {
                whs = 0;
                for (h = 0; h < HIDDEN_NUMBER; h++) {
                    whs += hidden_weight[o][h] * sigmoid_wix[t][h];
                }
                o_whs[t][o] = training_output[t][o] - whs;
            }
        }

        // 訓練データの２乗誤差 error の計算・表示
        error = 0;
        for (t = 0; t < TRAINING_NUMBER; t++) {
            for (o = 0; o < OUTPUT_NUMBER; o++) {
                error += pow(o_whs[t][o], 2);
            }
        }

        // テストデータの２乗誤差 test_error の計算
        test_error = get_test_error();
        // total_test_error += test_error; // 累積テスト誤差の更新
        average_test_errors[l] += test_error;

        // printf("%d, %5.5f\n", l, test_error);

        // input_weight による E の微分値の計算
        for (h = 0; h < HIDDEN_NUMBER; h++) {
            for (i = 0; i < INPUT_NUMBER; i++) {
                delta_input_weight[h][i] = 0;
                for (t = 0; t < TRAINING_NUMBER; t++) {
                    for (o = 0; o < OUTPUT_NUMBER; o++) {
                        delta_input_weight[h][i] += -2 * delta_sigmoid_wix[t][h] * hidden_weight[o][h] * o_whs[t][o] * training_input[t][i];
                    }
                }
            }
        }

        // hidden_weight による E の微分値の計算
        for (o = 0; o < OUTPUT_NUMBER; o++) {
            for (h = 0; h < HIDDEN_NUMBER; h++) {
                delta_hidden_weight[o][h] = 0;
                for (t = 0; t < TRAINING_NUMBER; t++) {
                    delta_hidden_weight[o][h] += -2 * o_whs[t][o] * sigmoid_wix[t][h];
                }
            }
        }

        // input_weight の更新
        for (h = 0; h < HIDDEN_NUMBER; h++) {
            for (i = 0; i < INPUT_NUMBER; i++) {
                input_weight[h][i] -= LEARNING_RATE * delta_input_weight[h][i];
            }
        }

        // hidden_weight の更新
        for (o = 0; o < OUTPUT_NUMBER; o++) {
            for (h = 0; h < HIDDEN_NUMBER; h++) {
                hidden_weight[o][h] -= LEARNING_RATE * delta_hidden_weight[o][h];
            }
        }
    }

    for (t = 0; t < TRAINING_NUMBER; t++) {
        free(sigmoid_wix[t]);
        free(o_whs[t]);
        free(delta_sigmoid_wix[t]);
    }
    free(sigmoid_wix);
    free(o_whs);
    free(delta_sigmoid_wix);

    for (h = 0; h < HIDDEN_NUMBER; h++) {
        free(delta_input_weight[h]);
    }
    free(delta_input_weight);

    for (o = 0; o < OUTPUT_NUMBER; o++) {
        free(delta_hidden_weight[o]);
    }
    free(delta_hidden_weight);
    
    return average_test_errors[LEARNING_TIME - 1] / 100;
}

/* ============================ */
/* テストデータの２乗誤差の獲得 */
/* ============================ */

double get_test_error(void) {
    double test_error = 0.0;
    double wix, whs;
    double **o_whs = (double **)malloc(TEST_NUMBER * sizeof(double *));
    double **sigmoid_wix = (double **)malloc(TEST_NUMBER * sizeof(double *));

    for (int t = 0; t < TEST_NUMBER; t++) {
        o_whs[t] = (double *)malloc(OUTPUT_NUMBER * sizeof(double));
        sigmoid_wix[t] = (double *)malloc(HIDDEN_NUMBER * sizeof(double));
    }

    // 中間細胞の出力 sigmoid_wix の計算
    for (int t = 0; t < TEST_NUMBER; t++) {
        for (int h = 0; h < HIDDEN_NUMBER; h++) {
            wix = 0;
            for (int i = 0; i < INPUT_NUMBER; i++) {
                wix += input_weight[h][i] * test_input[t][i];
            }
            sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
        }
    }

    // 出力細胞の出力 whs の計算
    // 教師信号 - 出力細胞 o_whs の計算
    for (int t = 0; t < TEST_NUMBER; t++) {
        for (int o = 0; o < OUTPUT_NUMBER; o++) {
            whs = 0;
            for (int h = 0; h < HIDDEN_NUMBER; h++) {
                whs += hidden_weight[o][h] * sigmoid_wix[t][h];
            }
            o_whs[t][o] = test_output[t][o] - whs;
        }
    }

    // テストデータの２乗誤差の計算
    test_error = 0;
    for (int t = 0; t < TEST_NUMBER; t++) {
        for (int o = 0; o < OUTPUT_NUMBER; o++) {
            test_error += pow(o_whs[t][o], 2);
        }
    }

    for (int t = 0; t < TEST_NUMBER; t++) {
        free(o_whs[t]);
        free(sigmoid_wix[t]);
    }
    free(o_whs);
    free(sigmoid_wix);

    return test_error / TEST_NUMBER;
}