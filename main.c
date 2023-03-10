#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 150

FILE *file_a;
FILE *file_b;
int row_a = 3;
int col_a = 5;
int row_b = 3;
int col_b = 3;
char line_a[MAX];
char line_b[MAX];
int array_a[MAX][MAX];
int array_b[MAX][MAX];

char *extract_after_equal(char *input)
{
    char *equal_ptr = strstr(input, "="); // find first occurrence of '='
    if (equal_ptr == NULL)
    {
        printf("Equal sign not found in input string\n");
        return NULL;
    }

    // extract substring after '='
    equal_ptr++; // move pointer past '=' character
    int len = strlen(equal_ptr);
    char *output = malloc(len + 1);  // allocate memory for output string
    strncpy(output, equal_ptr, len); // copy substring to output string
    output[len] = '\0';              // add null terminator to output string
    return output;
}

void read_inputs()
{
    // READ the first file
    file_a = fopen("a.txt", "r");
    fgets(line_a, MAX, file_a);
    for (int i = 0; i < row_a; i++)
    {
        for (int j = 0; j < col_a; j++)
        {
            fscanf(file_a, "%d", &array_a[i][j]);
        }
    }
    fclose(file_a);

    // READ the second file
    file_b = fopen("b.txt", "r");
    fgets(line_b, MAX, file_b);
    for (int i = 0; i < row_b; i++)
    {
        for (int j = 0; j < col_b; j++)
        {
            fscanf(file_b, "%d", &array_b[i][j]);
        }
    }
    fclose(file_b);
}

int main()
{
    read_inputs();
    printf("%s", line_a);
    for (int i = 0; i < row_a; i++)
    {
        for (int j = 0; j < col_a; j++)
        {
            printf("%d ", array_a[i][j]);
        }
        printf("\n");
    }

    printf("%s", line_b);
    for (int i = 0; i < row_b; i++)
    {
        for (int j = 0; j < col_b; j++)
        {
            printf("%d ", array_b[i][j]);
        }
        printf("\n");
    }

    return 0;
}
