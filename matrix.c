#include <stdio.h>
#include <stdlib.h>

int **initializeMatrix(const int size)
{
    int **matrix = (int **) malloc(size * sizeof(int *));
    if (matrix == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (int row = 0; row < size; row++)
    {
        matrix[row] = (int *) malloc(size * sizeof(int));
        if (matrix[row] == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }

        for (int col = 0; col < size; col++)
        {
            matrix[row][col] = rand() % 256;
        }
    }

    return matrix;
}

void displayMatrix(int **matrix, const int size)
{
    if (matrix != NULL)
    {
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                printf("%3d ", matrix[row][col]);
            }
            printf("\n");
        }
        printf("\n");
    }
    else
    {
        printf("Matrix is NULL. Cannot display.\n");
    }
}

void rotateClockwise(int **matrix, const int size)
{
    if (matrix != NULL)
    {
        for (int row = 0; row < size; row++)
        {
            for (int col = row; col < size; col++)
            {
                int temp = matrix[row][col];
                matrix[row][col] = matrix[col][row];
                matrix[col][row] = temp;
            }
        }

        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size / 2; col++)
            {
                int temp = matrix[row][col];
                matrix[row][col] = matrix[row][size - 1 - col];
                matrix[row][size - 1 - col] = temp;
            }
        }
    }
    else
    {
        printf("Matrix is NULL. Cannot rotate.\n");
    }
}

void applySmoothing(int **matrix, const int size)
{
    if (matrix != NULL)
    {
        int *rowCopy = (int *) malloc(size * sizeof(int));
        if (rowCopy == NULL)
        {
            printf("Memory allocation failed\n");
            exit(1);
        }

        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                rowCopy[col] = matrix[row][col];
            }

            for (int col = 0; col < size; col++)
            {
                int sum = 0;
                int count = 0;

                for (int r = row - 1; r <= row + 1; r++)
                {
                    for (int c = col - 1; c <= col + 1; c++)
                    {
                        if (r >= 0 && r < size && c >= 0 && c < size)
                        {
                            sum += (r == row) ? rowCopy[c] : matrix[r][c];
                            count++;
                        }
                    }
                }

                matrix[row][col] = sum / count;
            }
        }

        free(rowCopy);
    }
    else
    {
        printf("Matrix is NULL. Cannot apply smoothing.\n");
    }
}

void freeMatrix(int **matrix, const int size)
{
    if (matrix != NULL)
    {
        for (int row = 0; row < size; row++)
        {
            free(matrix[row]);
        }
        free(matrix);
    }
}

int main(void)
{
    int size;
    printf("Enter the size of the matrix: ");
    scanf("%d", &size);

    if (size < 2 || size > 10)
    {
        printf("Invalid size\n");
        return 1;
    }

    int **matrix = initializeMatrix(size);

    printf("\nOriginal Matrix:\n");
    displayMatrix(matrix, size);

    rotateClockwise(matrix, size);
    printf("Matrix after 90° rotation:\n");
    displayMatrix(matrix, size);

    applySmoothing(matrix, size);
    printf("Matrix after smoothing:\n");
    displayMatrix(matrix, size);

    freeMatrix(matrix, size);
    return 0;
}