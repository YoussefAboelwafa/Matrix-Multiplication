#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <pthread.h>

#define MAX 20

#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define reset "\e[0m"

FILE *file_a;
FILE *file_b;
FILE *file_c_matrix;
FILE *file_c_row;
FILE *file_c_element;

int row_a;
int col_a;
int mat_a[MAX][MAX];
int row_b;
int col_b;
int mat_b[MAX][MAX];

char line_a[MAX];
char line_b[MAX];

int row_c;
int col_c;
int mat_c_per_matrix[MAX][MAX];
int mat_c_per_row[MAX][MAX];
int mat_c_per_element[MAX][MAX];

char *input1;
char *input2;
char *output_matrix;
char *output_row;
char *output_element;

struct timeval stop1, stop2, stop3, start1, start2, start3;

struct Data
{
    int row;
    int col;
};

void read_inputs()
{
    // READ the first file
    file_a = fopen(input1, "r");
    // ERROR if file is empty
    if (file_a == NULL)
    {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    fgets(line_a, MAX, file_a);
    // line_a contains the row and col of the first matrix
    sscanf(line_a, "row=%d col=%d", &row_a, &col_a);

    for (int i = 0; i < row_a; i++)
    {
        for (int j = 0; j < col_a; j++)
        {
            fscanf(file_a, "%d", &mat_a[i][j]);
        }
    }
    fclose(file_a);

    //------------------------------------------------------------

    // READ the second file
    file_b = fopen(input2, "r");
    // ERROR if file is empty
    if (file_b == NULL)
    {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    fgets(line_b, MAX, file_b);
    // line_b contains the row and col of the second matrix
    sscanf(line_b, "row=%d col=%d", &row_b, &col_b);

    for (int i = 0; i < row_b; i++)
    {
        for (int j = 0; j < col_b; j++)
        {
            fscanf(file_b, "%d", &mat_b[i][j]);
        }
    }
    fclose(file_b);
}

void *matrix_mult(void *arg)
{
    int sum = 0;
    for (int i = 0; i < row_c; i++)
    {
        for (int j = 0; j < col_c; j++)
        {
            for (int k = 0; k < col_a; k++)
            {
                sum += mat_a[i][k] * mat_b[k][j];
            }
            mat_c_per_matrix[i][j] = sum;
            sum = 0;
        }
    }
    // write in matrix file
    file_c_matrix = fopen(output_matrix, "w");
    fprintf(file_c_matrix, "Method: A thread per matrix\n");
    fprintf(file_c_matrix, "row=%d col=%d\n", row_c, col_c);

    for (int i = 0; i < row_c; i++)
    {
        for (int j = 0; j < col_c; j++)
        {
            fprintf(file_c_matrix, "%d ", mat_c_per_matrix[i][j]);
        }
        fprintf(file_c_matrix, "\n");
    }
    fclose(file_c_matrix);
    return NULL;
}

void *row_mult(void *arg)
{
    int *prow_number = (int *)arg;
    int row_number = *prow_number;
    int sum;
    for (int i = 0; i < col_b; i++)
    {
        sum = 0;
        for (int j = 0; j < row_b; j++)
        {
            sum += mat_a[row_number][j] * mat_b[j][i];
        }
        mat_c_per_row[row_number][i] = sum;
    }
    // write in row file
    file_c_row = fopen(output_row, "w");
    fprintf(file_c_row, "Method: A thread per row\n");
    fprintf(file_c_row, "row=%d col=%d\n", row_c, col_c);

    for (int i = 0; i < row_c; i++)
    {
        for (int j = 0; j < col_c; j++)
        {
            fprintf(file_c_row, "%d ", mat_c_per_row[i][j]);
        }
        fprintf(file_c_row, "\n");
    }
    fclose(file_c_row);
    return NULL;
}

void *element_mult(void *arg)
{
    struct Data *my_data = (struct Data *)arg;
    int row = my_data->row;
    int col = my_data->col;
    int sum = 0;
    for (int i = 0; i < col_a; i++)
    {
        sum += mat_a[row][i] * mat_b[i][col];
    }
    mat_c_per_element[row][col] = sum;

    // write in element file
    file_c_element = fopen(output_element, "w");
    fprintf(file_c_element, "Method: A thread per element\n");
    fprintf(file_c_element, "row=%d col=%d\n", row_c, col_c);

    for (int i = 0; i < row_c; i++)
    {
        for (int j = 0; j < col_c; j++)
        {
            fprintf(file_c_element, "%d ", mat_c_per_element[i][j]);
        }
        fprintf(file_c_element, "\n");
    }
    fclose(file_c_element);
    return NULL;
}
void print_array(int row, int col, int arr[][MAX])
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 4 && argc != 1)
    {
        errno = ENOENT;
        perror("ERROR:Wrong input parameters");
        exit(EXIT_FAILURE);
    }

    // Default arguments
    if (argc == 1)
    {
        input1 = "a.txt";
        input2 = "b.txt";
        output_matrix = "c_per_matrix.txt";
        output_row = "c_per_row.txt";
        output_element = "c_per_element.txt";
    }

    // Custom arguments
    if (argc == 4)
    {
        input1 = malloc(strlen(argv[1]) + 5);
        strcpy(input1, argv[1]); // Copy the original string to the new string
        strcat(input1, ".txt");  // Concatenate the extension to the new string

        input2 = malloc(strlen(argv[2]) + 5);
        strcpy(input2, argv[2]); // Copy the original string to the new string
        strcat(input2, ".txt");  // Concatenate the extension to the new string

        output_matrix = malloc(strlen(argv[3]) + 16);
        strcpy(output_matrix, argv[3]);           // Copy the original string to the new string
        strcat(output_matrix, "_per_matrix.txt"); // Concatenate the extension to the new string

        output_row = malloc(strlen(argv[3]) + 16);
        strcpy(output_row, argv[3]);        // Copy the original string to the new string
        strcat(output_row, "_per_row.txt"); // Concatenate the extension to the new string

        output_element = malloc(strlen(argv[3]) + 16);
        strcpy(output_element, argv[3]);            // Copy the original string to the new string
        strcat(output_element, "_per_element.txt"); // Concatenate the extension to the new string
    }

    read_inputs();

    // check if valid dimensions
    if (col_a != row_b)
    {
        perror("ERROR in matrices dimensions");
        exit(EXIT_FAILURE);
    }

    row_c = row_a;
    col_c = col_b;

    pthread_t t_matrix;
    pthread_t t_rows[row_c];
    pthread_t t_elements[row_c * col_c];

    // matrix thread
    gettimeofday(&start1, NULL); // start checking matrix time
    pthread_create(&t_matrix, NULL, &matrix_mult, NULL);
    pthread_join(t_matrix, NULL);
    gettimeofday(&stop1, NULL); // end checking matrix time
    // row threads
    gettimeofday(&start2, NULL); // start checking matrix time
    int row_number[row_c];
    for (int i = 0; i < row_c; i++)
    {
        row_number[i] = i;
        pthread_create(&t_rows[i], NULL, &row_mult, &row_number[i]);
    }
    for (int i = 0; i < row_c; i++)
    {
        pthread_join(t_rows[i], NULL);
    }
    gettimeofday(&stop2, NULL); // end checking matrix time

    // elements threads
    gettimeofday(&start3, NULL); // start checking matrix time

    int k = 0;
    for (int i = 0; i < row_c; i++)
    {
        for (int j = 0; j < col_c; j++)
        {
            struct Data *data = malloc(sizeof(struct Data));
            data->row = i;
            data->col = j;
            pthread_create(&t_elements[k], NULL, &element_mult, (void *)data);
            k++;
        }
    }
    for (int i = 0; i < (row_c * col_c); i++)
    {
        pthread_join(t_elements[i], NULL);
    }

    gettimeofday(&stop3, NULL); // end checking matrix time

    print_array(row_c, col_c, mat_c_per_matrix);
    print_array(row_c, col_c, mat_c_per_row);
    print_array(row_c, col_c, mat_c_per_element);

    printf(BGRN "Number of threads taken for multiplication per matrix: (%d)\n" reset, 1);
    printf(BYEL "Number of threads taken for multiplication per row: (%d)\n" reset, row_c);
    printf(BRED "Number of threads taken for multiplication per element: (%d)\n" reset, row_c * col_c);
    printf(BGRN "\nMicroseconds taken for multiplication per matrix: (%lu)\n" reset, stop1.tv_usec - start1.tv_usec);
    printf(BYEL "Microseconds taken for multiplication per row: (%lu)\n" reset, stop2.tv_usec - start2.tv_usec);
    printf(BRED "Microseconds taken for multiplication per element: (%lu)\n\n" reset, stop3.tv_usec - start3.tv_usec);

    return 0;
}
