#include <stdio.h>
#include <stdlib.h>

void rotate90Degree(int **matrix, int matrixSize);
void displayMatrix(int **matrix, int matrixSize);
void applySmoothing(int **matrix, int matrixSize);

int main()
{
    int matrixSize;
    printf("Enter the size of the matrix: \n");
    scanf("%d", &matrixSize);
    if (matrixSize < 2 || matrixSize > 10)
    {
        printf("Invalid size.\n");
        return 1;
    }

    int **matrix = (int **)malloc(matrixSize * sizeof(int *));
    for (int row = 0; row < matrixSize; row++)
    {
        *(matrix + row) = (int *)malloc(matrixSize * sizeof(int));
    }

    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            *(*(matrix + row) + column) = rand() % 256;
        }
    }

    displayMatrix(matrix, matrixSize);
    rotate90Degree(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);
    applySmoothing(matrix, matrixSize);
    displayMatrix(matrix, matrixSize);
}

void rotate90Degree(int **matrix, int matrixSize)
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = row; column < matrixSize; column++)
        {
            int temp = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(*(matrix + column) + row);
            *(*(matrix + column) + row) = temp;
        }
    }

    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize / 2; column++)
        {
            int temp = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(*(matrix + row) + (matrixSize - 1 - column));
            *(*(matrix + row) + (matrixSize - 1 - column)) = temp;
        }
    }
}

void displayMatrix(int **matrix, int matrixSize)
{
    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            printf("%d ", *(*(matrix + row) + column));
        }
        printf("\n");
    }
    printf("\n");
}

void applySmoothing(int **matrix, int matrixSize)
{
    int *tempMatrix = (int *)malloc(matrixSize * matrixSize * sizeof(int));

    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            *(tempMatrix + row * matrixSize + column) = *(*(matrix + row) + column);
        }
    }

    for (int row = 0; row < matrixSize; row++)
    {
        for (int column = 0; column < matrixSize; column++)
        {
            int sum = 0;
            int count = 0;

            for (int i = row - 1; i <= row + 1; i++)
            {
                for (int j = column - 1; j <= column + 1; j++)
                {
                    if (i >= 0 && i < matrixSize && j >= 0 && j < matrixSize)
                    {
                        sum += *(tempMatrix + i * matrixSize + j);
                        count++;
                    }
                }
            }
            *(*(matrix + row) + column) = sum / count;
        }
    }
    free(tempMatrix);
}
