#include <stdio.h>
#include <stdlib.h>

void rotate90Degree(int **matrix, int matrixSize);
void displayMatrix(int **matrix, int matrixSize);
void applySmoothing(int **matrix, int matrixSize);

int main() {
    int matrixSize;
    printf("Enter the size of the matrix: \n");
    scanf("%d", &matrixSize);

    if (matrixSize < 2 || matrixSize > 10) {
        printf("Invalid size.\n");
        return 1;
    }

    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    for (int i = 0; i < matrixSize; i++) {
        matrix[i] = (int *)malloc(matrixSize * sizeof(int));
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            matrix[i][j] = rand() % 256;
        }
    }

    displayMatrix(matrix, matrixSize);
    rotate90Degree(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);
    applySmoothing(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);

    for (int i = 0; i < matrixSize; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}

void rotate90Degree(int **matrix, int matrixSize) {
    for (int row = 0; row < matrixSize; row++) {
        for (int col = row; col < matrixSize; col++) {
            int temp = matrix[row][col];
            matrix[row][col] = matrix[col][row];
            matrix[col][row] = temp;
        }
    }

    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize / 2; col++) {
            int temp = matrix[row][col];
            matrix[row][col] = matrix[row][matrixSize - 1 - col];
            matrix[row][matrixSize - 1 - col] = temp;
        }
    }
}

void displayMatrix(int **matrix, int matrixSize) {
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void applySmoothing(int **matrix, int matrixSize) {
    int *previousRow = (int *)malloc(matrixSize * sizeof(int));

    for (int row = 0; row < matrixSize; row++) {
        for (int col = 0; col < matrixSize; col++) {
            previousRow[col] = matrix[row][col];
        }

        for (int col = 0; col < matrixSize; col++) {
            int sum = 0, count = 0;

            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < matrixSize && j >= 0 && j < matrixSize) {
                        sum += (i == row) ? previousRow[j] : matrix[i][j];
                        count++;
                    }
                }
            }
            matrix[row][col] = sum / count;
        }
    }

    free(previousRow);
}