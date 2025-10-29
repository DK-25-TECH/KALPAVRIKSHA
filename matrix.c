#include <stdio.h>
#include <stdlib.h>

int **initialize_matrix(const int size);
void display_matrix(int **matrix, const int size);
void rotate_90_degree(int **matrix, const int size);
void apply_smoothing(int **matrix, const int size);
void free_matrix(int **matrix, const int size);

int **initialize_matrix(const int size)
{
    int **matrix = (int **) malloc(size * sizeof(int *));
    if (matrix == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (int row = 0; row < size; row++) {
        matrix[row] = (int *) malloc(size * sizeof(int));
        if (matrix[row] == NULL) {
            printf("Memory allocation failed\n");
            exit(1);
        }

        for (int col = 0; col < size; col++) {
            matrix[row][col] = rand() % 256;
        }
    }
    return matrix;
}

void display_matrix(int **matrix, const int size)
{
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%3d ", matrix[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

void rotate_90_degree(int **matrix, const int size)
{
    for (int row = 0; row < size; row++) {
        for (int col = row; col < size; col++) {
            int temp = matrix[row][col];
            matrix[row][col] = matrix[col][row];
            matrix[col][row] = temp;
        }
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size / 2; col++) {
            int temp = matrix[row][col];
            matrix[row][col] = matrix[row][size - 1 - col];
            matrix[row][size - 1 - col] = temp;
        }
    }
}

void apply_smoothing(int **matrix, const int size)
{
    int *row_copy = (int *) malloc(size * sizeof(int));
    if (row_copy == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            row_copy[col] = matrix[row][col];
        }

        for (int col = 0; col < size; col++) {
            int sum = 0, count = 0;

            for (int r = row - 1; r <= row + 1; r++) {
                for (int c = col - 1; c <= col + 1; c++) {
                    if (r >= 0 && r < size && c >= 0 && c < size) {
                        sum += (r == row) ? row_copy[c] : matrix[r][c];
                        count++;
                    }
                }
            }
            matrix[row][col] = sum / count;
        }
    }

    free(row_copy);
}

void free_matrix(int **matrix, const int size)
{
    for (int row = 0; row < size; row++) {
        free(matrix[row]);
    }
    free(matrix);
}

int main(void)
{
    int size;

    printf("Enter the size of the matrix: ");
    scanf("%d", &size);

    if (size < 2 || size > 10) {
        printf("Invalid size\n");
        return 1;
    }

    int **matrix = initialize_matrix(size);

    printf("\nOriginal Matrix:\n");
    display_matrix(matrix, size);

    rotate_90_degree(matrix, size);
    printf("Matrix after 90° rotation:\n");
    display_matrix(matrix, size);

    apply_smoothing(matrix, size);
    printf("Matrix after smoothing:\n");
    display_matrix(matrix, size);

    free_matrix(matrix, size);
    return 0;
}
