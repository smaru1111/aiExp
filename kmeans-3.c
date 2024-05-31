#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DATA_NUMBER 47
#define DATA_DIMENSION 4
#define MEAN_NUMBER 6
#define SEED 39

int assignedCluster[DATA_NUMBER];
int assignedNumber[MEAN_NUMBER];
double means[MEAN_NUMBER][DATA_DIMENSION];
double data[DATA_NUMBER][DATA_DIMENSION] = {
    {0.217, 0.916, -0.191, 0.736}, {0.652, 0.991, 0.535, -0.099}, {0.401, 0.812, 0.711, -0.061},
    {-0.031, 0.463, 0.545, 0.442}, {0.430, 0.760, 1.952, -0.287}, {0.430, -0.226, 1.392, 0.052},
    {0.065, 0.083, 1.179, -0.294}, {-0.450, -1.020, -0.113, -0.400}, {-0.232, -0.812, -0.145, -0.106},
    {0.147, -1.035, -0.020, -0.159}, {-0.269, -1.049, -0.691, 0.209}, {-0.474, -0.512, -0.599, 0.345},
    {0.081, 3.106, 0.077, 3.743}, {-0.405, -0.434, -0.752, 0.721}, {-0.304, 0.900, 3.577, 0.052},
    {-0.693, 0.506, 1.017, -0.452}, {-0.720, 0.486, 1.026, -0.144}, {-0.752, 0.298, 0.623, -0.715},
    {-0.098, -0.914, -0.196, 4.547}, {-0.181, -0.222, 1.026, 0.525}, {-0.816, -1.388, 0.133, -0.497},
    {-0.253, -0.888, -0.261, -0.167}, {-0.811, -0.181, -0.793, -0.106}, {-0.677, -0.939, -0.201, -0.625},
    {-0.960, -1.259, -0.178, -0.633}, {-0.880, 1.020, 0.109, 0.796}, {-0.517, 1.752, -0.534, 0.848},
    {-0.723, 0.212, -0.298, 0.059}, {-0.981, -1.365, -0.279, -0.618}, {-0.573, -0.072, 0.216, 0.089},
    {-0.320, 0.227, 0.790, -0.715}, {0.163, -0.401, 1.026, 0.029}, {-0.434, -0.902, -0.256, -0.708},
    {-0.176, 0.491, -0.219, -0.144}, {-0.010, -0.712, -0.266, -0.753}, {-0.461, -0.513, -0.270, -0.708},
    {-0.589, -0.264, -0.298, -0.625}, {-0.293, -0.113, -0.242, -0.760}, {-0.122, 2.116, 0.184, -0.693},
    {0.318, 0.187, -0.631, 0.217}, {-0.109, -0.371, 0.239, -0.881}, {0.380, -0.341, -0.599, -0.648},
    {0.953, -0.798, -1.321, -0.369}, {1.047, -0.415, -0.682, -0.272}, {3.243, 0.170, -1.756, -0.204},
    {4.681, -0.921, -2.256, -0.768}, {1.105, 2.572, -2.307, 0.202}
};

void initMeans(void) {
    for (int i = 0; i < MEAN_NUMBER; i++) {
        for (int j = 0; j < DATA_DIMENSION; j++) {
            means[i][j] = data[rand() % DATA_NUMBER][j];
        }
    }
}

void updateMeans(void) {
    int count[MEAN_NUMBER] = {0};
    double sum[MEAN_NUMBER][DATA_DIMENSION] = {0};

    for (int i = 0; i < DATA_NUMBER; i++) {
        int cluster = assignedCluster[i];
        count[cluster]++;
        for (int j = 0; j < DATA_DIMENSION; j++) {
            sum[cluster][j] += data[i][j];
        }
    }

    for (int i = 0; i < MEAN_NUMBER; i++) {
        if (count[i] > 0) {
            for (int j = 0; j < DATA_DIMENSION; j++) {
                means[i][j] = sum[i][j] / count[i];
            }
        }
    }
}

double updateAssignedCluster(void) {
    double totalDistance = 0;

    for (int i = 0; i < DATA_NUMBER; i++) {
        double minDistance = INFINITY;
        int minIndex = -1;

        for (int j = 0; j < MEAN_NUMBER; j++) {
            double distance = 0;
            for (int k = 0; k < DATA_DIMENSION; k++) {
                distance += pow(data[i][k] - means[j][k], 2);
            }
            if (distance < minDistance) {
                minDistance = distance;
                minIndex = j;
            }
        }
        assignedCluster[i] = minIndex;
        totalDistance += minDistance;
    }

    return totalDistance;
}

int main() {
    srand(SEED);
    initMeans();
    double lastCost = updateAssignedCluster();
    double cost;

    while (1) {
        updateMeans();
        cost = updateAssignedCluster();
        if (fabs(lastCost - cost) < 1e-4) {
            break;
        }
        lastCost = cost;
    }

    // Œ‹‰Ê‚Ìo—Í
    for (int i = 0; i < DATA_NUMBER; i++) {
        printf("%d\n", assignedCluster[i]);
    }

    return 0;
}