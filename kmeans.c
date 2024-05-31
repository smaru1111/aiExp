#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES
#include<math.h>

#define SEED               39 // 乱数の種
#define DATA_NUMBER      100 // データの数
#define DATA_DIMENSION     2 // データの次元数
#define GAUSS_NUMBER       3 // データの正規分布の数
#define MEAN_NUMBER        4 // 自動分類するクラスタの数

int assignedCluster[DATA_NUMBER];          // 各データが所属するクラスタ番号
int assignedNumber[MEAN_NUMBER];           // 各クラスタに所属するデータの数
double means[MEAN_NUMBER][DATA_DIMENSION]; // 各クラスタに所属するデータの平均（クラスタ代表点）
double data[DATA_NUMBER][DATA_DIMENSION];  // データ

void generateDataMoG(void);         // データを生成
double generateGauss(void);         // ガウス分布の乱数を生成
void initAssignedCluster(void);     // assignedClusterを初期化
void updateMeans(void);             // meansを更新
double updateAssignedCluster(void); // assignedClusterを更新

int
main(){
	
	double cost, priCost;
	srand(SEED);

	generateDataMoG();

	initAssignedCluster();
	updateMeans();
	priCost = updateAssignedCluster();

	int c, n, d = 0, cnt = 0;
	
	// 学習ステップごとの{所属クラスタ,クラスタの代表点x, クラスタの代表点y}の形式で出力してるよ！！！！！
	// while (1){
	// 			// printf("Step cost: %f\n", priCost);
	// 			// printf("%f\n", priCost);
  //       // printf("Cluster means:\n");
	// 			cnt += 1;
	// 			printf("step%d\n", cnt);
  //       for (c = 0; c < MEAN_NUMBER; c++){
  //           printf("%d,", c);
  //           for (d = 0; d < DATA_DIMENSION; d++){
  //               	printf("%f ", means[c][d]);
	// 								if (d < DATA_DIMENSION - 1) {
	// 									printf(",");
	// 								}
  //           }
  //           printf("\n");
  //       }
	// 			printf("\n");

  //       updateMeans();
  //       cost = updateAssignedCluster();

  //       if (cost == priCost) { break; }
  //       priCost = cost;
	// }
	
		// 学習ステップごとに{所属クラスタ,データの座標x, データの座標y}の形式で出力
	while (1){
		cnt += 1;
		printf("step%d\n", cnt);
		outputClusterData(cnt);

        updateMeans();
        cost = updateAssignedCluster();

        if (cost == priCost) { break; }
        priCost = cost;
	}

	return 0;
}

// データを混合正規分布で生成
void
generateDataMoG(void){

	int n,d,z;
	for ( n = 0; n < DATA_NUMBER; n++){
		z = (int)(((double)rand() / ((double)RAND_MAX + 1)) * GAUSS_NUMBER);
		for ( d = 0; d < DATA_DIMENSION; d++){
			data[n][d] = 0.4 * generateGauss() + (double)z;
		}
	}
	printf("data_x = \n");
	d = 0;
	for ( n = 0; n < DATA_NUMBER; n++){
		// for ( d = 0; d < DATA_DIMENSION; d++){
			printf("%f", data[n][d]);
			printf("\n");
		// }
	}
	printf("---------------\n");
	d = 1;
	printf("data_y = \n");
	for ( n = 0; n < DATA_NUMBER; n++){
		// for ( d = 0; d < DATA_DIMENSION; d++){
			printf("%f", data[n][d]);
			printf("\n");
		// }
	}
	printf("---------------\n");
}

// 正規分布を生成
double generateGauss(void){

	double gauss;
	double r1;
	double r2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
	while ((r1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0)) == 0.0);
	return sqrt(-2.0*log(r1))*sin(2.0*M_PI*r2);
}

// assignedClusterを初期化
void
initAssignedCluster(void){

	int n;
	for ( n = 0; n < DATA_NUMBER; n++){
        assignedCluster[n] = (int)(((double)rand() / ((double)RAND_MAX + 1)) * MEAN_NUMBER);
	}

}

// meansを更新
void
updateMeans(void){

	int c, n, d;
	for ( c = 0; c < MEAN_NUMBER; c++){
		assignedNumber[c] = 0;
	}

	for ( n = 0; n < DATA_NUMBER; n++){
		assignedNumber[assignedCluster[n]] ++;
	}

	for ( d = 0; d < DATA_DIMENSION; d++){
		for ( c = 0; c < MEAN_NUMBER; c++){
			means[c][d] = 0;
		}
	
		for ( n = 0; n < DATA_NUMBER; n++){
			means[assignedCluster[n]][d] += data[n][d];
		}

		for ( c = 0; c < MEAN_NUMBER; c++){
			if (assignedNumber[c] == 0){ means[c][d] = (double)rand() / RAND_MAX; }
			else {
				means[c][d] /= assignedNumber[c];
			}
		}
	}

}

// assignedClusterを更新
double
updateAssignedCluster(void){

	int n, c, d;
	double cost;
	double min_norm;
	double norm[DATA_NUMBER][MEAN_NUMBER];

	for ( n = 0; n < DATA_NUMBER; n++){
		for ( c = 0; c < MEAN_NUMBER; c++){
			norm[n][c] = 0;
			for ( d = 0; d < DATA_DIMENSION; d++){
				norm[n][c] += pow(data[n][d] - means[c][d], 2);
			}
		}
	}

	cost = 0;
	for ( n = 0; n < DATA_NUMBER; n++){
		assignedCluster[n] = 0;
		min_norm = norm[n][0];
		for ( c = 1; c < MEAN_NUMBER; c++){
			if (min_norm > norm[n][c]){
				min_norm = norm[n][c];
				assignedCluster[n] = c;
			}
		}
		cost += norm[n][assignedCluster[n]];
	}

	return cost;
}

// クラスタデータをCSV形式で出力
void
outputClusterData(int step){

	int n, d;
	printf("step %d: {cluster, x, y}\n", step);
	for (n = 0; n < DATA_NUMBER; n++){
		printf("%d", assignedCluster[n]);
		for (d = 0; d < DATA_DIMENSION; d++){
			printf(", %f", data[n][d]);
		}
		printf("\n");
	}
}
