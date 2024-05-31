#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES
#include<math.h>

#define SEED               39 // �����̎�
#define DATA_NUMBER      100 // �f�[�^�̐�
#define DATA_DIMENSION     2 // �f�[�^�̎�����
#define GAUSS_NUMBER       3 // �f�[�^�̐��K���z�̐�
#define MEAN_NUMBER        4 // �������ނ���N���X�^�̐�

int assignedCluster[DATA_NUMBER];          // �e�f�[�^����������N���X�^�ԍ�
int assignedNumber[MEAN_NUMBER];           // �e�N���X�^�ɏ�������f�[�^�̐�
double means[MEAN_NUMBER][DATA_DIMENSION]; // �e�N���X�^�ɏ�������f�[�^�̕��ρi�N���X�^��\�_�j
double data[DATA_NUMBER][DATA_DIMENSION];  // �f�[�^

void generateDataMoG(void);         // �f�[�^�𐶐�
double generateGauss(void);         // �K�E�X���z�̗����𐶐�
void initAssignedCluster(void);     // assignedCluster��������
void updateMeans(void);             // means���X�V
double updateAssignedCluster(void); // assignedCluster���X�V

int
main(){
	
	double cost, priCost;
	srand(SEED);

	generateDataMoG();

	initAssignedCluster();
	updateMeans();
	priCost = updateAssignedCluster();

	int c, n, d = 0, cnt = 0;
	
	// �w�K�X�e�b�v���Ƃ�{�����N���X�^,�N���X�^�̑�\�_x, �N���X�^�̑�\�_y}�̌`���ŏo�͂��Ă��I�I�I�I�I
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
	
		// �w�K�X�e�b�v���Ƃ�{�����N���X�^,�f�[�^�̍��Wx, �f�[�^�̍��Wy}�̌`���ŏo��
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

// �f�[�^���������K���z�Ő���
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

// ���K���z�𐶐�
double generateGauss(void){

	double gauss;
	double r1;
	double r2 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
	while ((r1 = ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0)) == 0.0);
	return sqrt(-2.0*log(r1))*sin(2.0*M_PI*r2);
}

// assignedCluster��������
void
initAssignedCluster(void){

	int n;
	for ( n = 0; n < DATA_NUMBER; n++){
        assignedCluster[n] = (int)(((double)rand() / ((double)RAND_MAX + 1)) * MEAN_NUMBER);
	}

}

// means���X�V
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

// assignedCluster���X�V
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

// �N���X�^�f�[�^��CSV�`���ŏo��
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
