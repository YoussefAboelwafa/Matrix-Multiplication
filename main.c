#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define MAX 20

FILE *file_a;
FILE *file_b;

int row_a;
int col_a;
int mat_a[MAX][MAX];

int row_b;
int col_b;
int mat_b[MAX][MAX];

char line_a[MAX];
char line_b[MAX];




void read_inputs()
{
    // READ the first file
    file_a = fopen("a.txt", "r");
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
    file_b = fopen("b.txt", "r");
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

int main()
{
    read_inputs();
    print_array(row_a,col_a,mat_a);
    print_array(row_b,col_b,mat_b);
    return 0;
}
