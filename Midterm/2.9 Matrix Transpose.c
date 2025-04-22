#include <stdio.h>
#define MAX_TERMS 100
#define MAX_COL 100
typedef struct
{
    int row;
    int col;
    int value;
} term;
term a[MAX_TERMS], b[MAX_TERMS]; // 0 index is size of matrix
void fast_transpose(term a[], term b[])
{
    /* the transpose of a is placed in b */
    int j, row_terms[MAX_COL] = { 0, }, starting_pos[MAX_COL] = { 0, };
    int num_cols = a[0].col, num_terms = a[0].value;
    b[0].row = num_cols, b[0].col = a[0].row;
    b[0].value = num_terms; // Initialize b's size and value

    if(num_terms > 0) // If non zero matrix
    {
        for(int i = 1; i <= num_terms; i++)
            row_terms[a[i].col]++; // Initialize row_terms
    }
    row_terms[num_cols - 1] = 1 + num_terms - row_terms[num_cols - 1];
    for(int i = num_cols - 1; i > 0; i--)
        row_terms[i - 1] = row_terms[i] - row_terms[i - 1];

    /*starting_pos[0] = 1;
    for(int i = 1; i <= num_cols; i++) // Initialize starting_pos
        starting_pos[i] = starting_pos[i - 1] + row_terms[i - 1]; */
    for(int i = 1; i <= num_terms; i++)
    {
        j = row_terms[a[i].col]++; // j = index of b to be implemented
        b[j].row = a[i].col;
        b[j].col = a[i].row;
        b[j].value = a[i].value;
    }
    return;
}
void input(term a[], int idx, int row, int col, int value)
{
    a[idx].row = row;
    a[idx].col = col;
    a[idx].value = value;
    return;
}
void printMatrix(term a[])
{
    for(int i = 0; i <= a[0].value; i++)
        printf("%d %d %d\n", a[i].row, a[i].col, a[i].value);
    return;
}
int main(int argc, char** argv)
{
    input(a, 0, 2, 3, 4);
    input(a, 1, 0, 0, 9);
    input(a, 2, 0, 1, 6);
    input(a, 3, 1, 0, 10);
    input(a, 4, 1, 2, 7);
    fast_transpose(a, b);
    printMatrix(b);
    return 0;
}
