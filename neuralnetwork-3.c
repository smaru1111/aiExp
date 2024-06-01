#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SEED               39 // �����̎�
#define TRAINING_NUMBER    10 // �P���f�[�^�̐�
#define TEST_NUMBER      100 // �e�X�g�f�[�^�̐�
#define INPUT_NUMBER      15 // ���͑w�̃j���[������

#define OUTPUT_NUMBER      10 // �o�͑w�̃j���[������

#define HIDDEN_NUMBER     10 // �B��w�̃j���[������
#define LEARNING_RATE   0.01 // �w�K��
#define LEARNING_TIME   1000 // �w�K��

void make_test(void);
void init_weight(void);
void learn(void);
double get_test_error(void);

double input_weight[HIDDEN_NUMBER][INPUT_NUMBER];          // ���͑w����B��w�ւ̏d��
double hidden_weight[OUTPUT_NUMBER][HIDDEN_NUMBER];        // �B��w����o�͑w�ւ̏d��

double test_output[TEST_NUMBER][OUTPUT_NUMBER];            // �e�X�g�f�[�^�̋��t�M��
double training_output[TRAINING_NUMBER][OUTPUT_NUMBER] = {     // �P���f�[�^�̋��t�M��
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

double test_input[TEST_NUMBER][INPUT_NUMBER];									 // �e�X�g�f�[�^
double training_input[TRAINING_NUMBER][INPUT_NUMBER] = {       // �w�K�f�[�^
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

double total_test_error = 0;
double total_error = 0;
double average_test_errors[LEARNING_TIME]; // �e�w�K�X�e�b�v�̗ݐό덷
double average_errors[LEARNING_TIME]; // �e�w�K�X�e�b�v�̗ݐό덷

int main()
{
    srand(SEED);

    for (int iteration = 0; iteration < 100; iteration++) {
        make_test();
        init_weight();
        learn();
    }

    // �e�w�K�X�e�b�v�̕��ϓ��덷��\��
    for (int l = 0; l < LEARNING_TIME; l++) {
        average_test_errors[l] /= 100;
        printf("%d, %5.5f\n", l, average_test_errors[l]);
    }
		// average_test_errors[LEARNING_TIME-1] /= 100;
		// printf("%5.5f\n",  average_test_errors[LEARNING_TIME-1]);

    return 0;
}

/* =================================== */
/* �e�X�g�f�[�^�Ƌ��t�M���̍쐬        */
/* �w�K�f�[�^��10���̊m����0,1���]���� */
/* ���Ƃɂ��e�X�g�f�[�^���쐬����D  */
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
/* �d�݂̏�����                      */
/* input_weight��hidden_weight��S�� */
/* -0.1�`0.1 �̈�l�����ŏ���������D*/
/* ================================= */

void init_weight(void){

    int h, i, o;

    // input_weight�̏�����
    for (h = 0; h < HIDDEN_NUMBER; h++){
        for (i = 0; i < INPUT_NUMBER; i++){
            input_weight[h][i] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
        }
    }

    // hidden_weight�̏�����
    for (o = 0; o < OUTPUT_NUMBER; o++){
        for (h = 0; h < HIDDEN_NUMBER; h++){
            hidden_weight[o][h] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
        }
    }
}

/* ============== */
/* �d�݂��w�K���� */
/* ============== */

void learn(void){

    int l, t, h, i, o;

    double error, test_error;
    double wix, whs;
    double sigmoid_wix[TRAINING_NUMBER][HIDDEN_NUMBER];
    double o_whs[TRAINING_NUMBER][OUTPUT_NUMBER];
    double delta_sigmoid_wix[TRAINING_NUMBER][HIDDEN_NUMBER];
    double delta_input_weight[HIDDEN_NUMBER][INPUT_NUMBER];
    double delta_hidden_weight[OUTPUT_NUMBER][HIDDEN_NUMBER];

    for (l = 0; l < LEARNING_TIME; l++){

        // ���ԍזE�̏o�� sigmoid_wix �̌v�Z
        // sigmoid�֐��̑���Ɂusigmoid�֐�����������֐��v���g�p�����ۂ̒��ԍזE�̏o�� delta_sigmoid_wix �̌v�Z
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

        // �o�͍זE�̏o�� whs �̌v�Z
        // ���t�M�� - �o�͍זE o_whs �̌v�Z
        for (t = 0; t < TRAINING_NUMBER; t++){
            for (o = 0; o < OUTPUT_NUMBER; o++){
                whs = 0;
                for (h = 0; h < HIDDEN_NUMBER; h++){
                    whs += hidden_weight[o][h] * sigmoid_wix[t][h];
                }
                o_whs[t][o] = training_output[t][o] - whs;
            }
        }

        // �P���f�[�^�̂Q��덷 error �̌v�Z�E�\��
        // �e�X�g�f�[�^�̂Q��덷 test_error �̌v�Z�E�\��
        error = 0;
        for (t = 0; t < TRAINING_NUMBER; t++){
            for (o = 0; o < OUTPUT_NUMBER; o++){
                error += pow(o_whs[t][o], 2);
            }
        }
        test_error = get_test_error();
        
        // �e�w�K�X�e�b�v���̗ݐό덷�̍X�V
        average_test_errors[l] += test_error;

        // printf("%d, %5.5f\n", l, test_error);

        // input_weight �ɂ�� E �̔����l�̌v�Z
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

        // hidden_weight �ɂ�� E �̔����l�̌v�Z
        for (o = 0; o < OUTPUT_NUMBER; o++){
            for (h = 0; h < HIDDEN_NUMBER; h++){
                delta_hidden_weight[o][h] = 0;
                for (t = 0; t < TRAINING_NUMBER; t++){
                    delta_hidden_weight[o][h] += -2 * o_whs[t][o] * sigmoid_wix[t][h];
                }
            }
        }

        // input_weight �̍X�V
        for (h = 0; h < HIDDEN_NUMBER; h++){
            for (i = 0; i < INPUT_NUMBER; i++){
                input_weight[h][i] -= LEARNING_RATE * delta_input_weight[h][i];
            }
        }

        // hidden_weight �̍X�V
        for (o = 0; o < OUTPUT_NUMBER; o++){
            for (h = 0; h < HIDDEN_NUMBER; h++){
                hidden_weight[o][h] -= LEARNING_RATE * delta_hidden_weight[o][h];
            }
        }
    }
}

/* ============================ */
/* �e�X�g�f�[�^�̂Q��덷�̊l�� */
/* ============================ */

double get_test_error(void){

    double test_error;
    double wix, whs;
    double o_whs[TEST_NUMBER][OUTPUT_NUMBER];
    double sigmoid_wix[TEST_NUMBER][HIDDEN_NUMBER];

    // ���ԍזE�̏o�� sigmoid_wix �̌v�Z
    for (int t = 0; t < TEST_NUMBER; t++){
        for (int h = 0; h < HIDDEN_NUMBER; h++){
            wix = 0;
            for (int i = 0; i < INPUT_NUMBER; i++){
                wix += input_weight[h][i] * test_input[t][i];
            }
            sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
        }
    }

    // �o�͍זE�̏o�� whs �̌v�Z
    // ���t�M�� - �o�͍זE o_whs �̌v�Z
    for (int t = 0; t < TEST_NUMBER; t++){
        for (int o = 0; o < OUTPUT_NUMBER; o++){
            whs = 0;
            for (int h = 0; h < HIDDEN_NUMBER; h++){
                whs += hidden_weight[o][h] * sigmoid_wix[t][h];
            }
            o_whs[t][o] = test_output[t][o] - whs;
        }
    }

    // �e�X�g�f�[�^�̂Q��덷�̌v�Z
    test_error = 0;
    for (int t = 0; t < TEST_NUMBER; t++){
        for (int o = 0; o < OUTPUT_NUMBER; o++){
            test_error += pow(o_whs[t][o], 2);
        }
    }

    return test_error / TEST_NUMBER;
}
