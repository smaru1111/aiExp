#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include <float.h>

#define SEED               39 // 乱数の種
#define DATA_NUMBER      100 // データの数
#define DATA_DIMENSION     2 // データの次元数

int assignedCluster[DATA_NUMBER];          // 各データが所属するクラスタ番号
int assignedNumber[10];                    // 各クラスタに所属するデータの数
double means[10][DATA_DIMENSION];          // 各クラスタに所属するデータの平均（クラスタ代表点）
double data[DATA_NUMBER][DATA_DIMENSION];  // データ

void generateDataMoG(int gaussNumber);         // データを生成
double generateGauss(void);         // ガウス分布の乱数を生成
void initAssignedCluster(int meanNumber);     // assignedClusterを初期化
void updateMeans(int meanNumber);             // meansを更新
double updateAssignedCluster(int meanNumber); // assignedClusterを更新

int main(){
    srand(SEED);

    for (int gaussNumber = 3; gaussNumber <= 7; gaussNumber += 2) {
        printf("GAUSS_NUMBER: %d\n", gaussNumber);
        for (int meanNumber = 1; meanNumber <= 10; meanNumber++) {
            generateDataMoG(gaussNumber);

            initAssignedCluster(meanNumber);
            updateMeans(meanNumber);
            double priCost = updateAssignedCluster(meanNumber);

            double cost, lastCost;
            int iterationCount = 0;
            do {
                updateMeans(meanNumber);
                cost = updateAssignedCluster(meanNumber);
                if (cost == priCost) {
                    lastCost = cost;
                    break;
                }
                priCost = cost;
            } while (++iterationCount < 1000); // 安全のために最大繰り返し回数を設定

            printf("%.4f\n", lastCost);
        }
        printf("\n");
    }
    return 0;
}

void generateDataMoG(int gaussNumber){
    int n, d, z;
    for (n = 0; n < DATA_NUMBER; n++){
        z = (int)(((double)rand() / ((double)RAND_MAX + 1)) * gaussNumber);
        for (d = 0; d < DATA_DIMENSION; d++){
            data[n][d] = 0.4 * generateGauss() + (double)z;
        }
    }
}

double generateGauss(void){
    double r1;
    double r2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
    while ((r1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0)) == 0.0);
    return sqrt(-2.0 * log(r1)) * sin(2.0 * M_PI * r2);
}

void initAssignedCluster(int meanNumber){
    int n;
    for (n = 0; n < DATA_NUMBER; n++){
        assignedCluster[n] = (int)(((double)rand() / ((double)RAND_MAX + 1)) * meanNumber);
    }
}

void updateMeans(int meanNumber){
    int c, n, d;
    for (c = 0; c < meanNumber; c++){
        assignedNumber[c] = 0;
        for (d = 0; d < DATA_DIMENSION; d++){
            means[c][d] = 0;
        }
    }

    for (n = 0; n < DATA_NUMBER; n++){
        assignedNumber[assignedCluster[n]]++;
        for (d = 0; d < DATA_DIMENSION; d++){
            means[assignedCluster[n]][d] += data[n][d];
        }
    }

    for (c = 0; c < meanNumber; c++){
        if (assignedNumber[c] == 0){
            for (d = 0; d < DATA_DIMENSION; d++){
                means[c][d] = (double)rand() / RAND_MAX; // ゼロ除算を避けるためランダム初期化
            }
        } else {
            for (d = 0; d < DATA_DIMENSION; d++){
                means[c][d] /= assignedNumber[c];
            }
        }
    }
}

double updateAssignedCluster(int meanNumber){
    int n, c, d;
    double cost = 0, min_norm;
    for (n = 0; n < DATA_NUMBER; n++){
        min_norm = DBL_MAX;
        for (c = 0; c < meanNumber; c++){
            double norm = 0;
            for (d = 0; d < DATA_DIMENSION; d++){
                norm += pow(data[n][d] - means[c][d], 2);
            }
            if (min_norm > norm){
                min_norm = norm;
                assignedCluster[n] = c;
            }
        }
        cost += min_norm;
    }
    return cost;
}
