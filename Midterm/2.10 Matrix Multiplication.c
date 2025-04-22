#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 100
#define MAX_COL 100
typedef struct
{
    int row;
    int col;
    int value;
} term;
int COMPARE(int n1, int n2)
{
    if(n1 == n2)
        return 0;
    else
        return (n1 > n2 ? 1 : -1);
}
term a[MAX_TERMS], b[MAX_TERMS], d[MAX_TERMS]; // 0 index is size of matrix
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
    for(int i = num_cols - 1; i > 0; i--) // Using only one extra array
        row_terms[i - 1] = row_terms[i] - row_terms[i - 1];
    for(int i = 1; i <= num_terms; i++)
    {
        j = row_terms[a[i].col]++; // j = index where b's data is implemented
        b[j].row = a[i].col;
        b[j].col = a[i].row;
        b[j].value = a[i].value;
    }
    return;
}
void storesum(term d[], int* totald, int row, int column, int* sum)
{
    /* if *sum != 0, then it along with its row and column position
    is stored as *totald + 1 entry in d */
    if(*sum != 0)
    {
        if(*totald < MAX_TERMS)
        {
            d[++(*totald)].row = row;
            d[*totald].col = column;
            d[*totald].value = *sum;
        }
        else
        {
            fprintf(stderr, "Numbers of terms exceed %d!", MAX_TERMS);
            exit(-1);
        }
        *sum = 0;
    }
    return;
}
void mmult(term a[], term b[], term d[])
{
    /* multiply two matrices */
    int column, totalb = b[0].value, totald = 0;
    int rows_a = a[0].row, columns_a = a[0].col, totala = a[0].value;
    int rows_b = b[0].row, columns_b = b[0].col; totalb = b[0].value;
    int row_begin = 1, row = a[1].row, sum = 0;
    term new_b[MAX_TERMS] = { 0, };
    if(columns_a != rows_b)
    {
        fprintf(stderr, "Incompatible matrices!\n");
        exit(-1);
    }
    fast_transpose(b, new_b);
    /* set boundary condition */
    a[totala + 1].row = rows_a;
    new_b[totalb + 1].row = columns_b;
    new_b[totalb + 1].col = -1;
    for(int i = 1; i <= totala;)
    {
        column = new_b[1].row;
        for(int j = 1; j <= totalb + 1;)
        {
            /* multiply rows of a by columns of b */
            if(a[i].row != row)
            {
                storesum(d, &totald, row, column, &sum);
                i = row_begin; // Return i back to starting point of row
                for(; new_b[j].row == column; j++);
                column = new_b[j].row;
            }
            else if(new_b[j].row != column)
            {
                storesum(d, &totald, row, column, &sum);
                i = row_begin; // Return i back to starting point of row
                column = new_b[j].row;
            }
            else
            {
                switch(COMPARE(a[i].col, new_b[j].col))
                {
                    case -1: // a[i].col < new_b[j].col
                        i++; break;
                    case 0: // a[i].col == new_b[j].col
                        sum += a[i].value * new_b[j].value;
                        i++; j++; break;
                    case 1: // a[i].col > new_b[j].col
                        j++; break;
                }
            }
        }
        for(; a[i].row == row; i++);
        row_begin = i; row = a[i].row;
    }
    d[0].row = rows_a, d[0].col = columns_b;
    d[0].value = totald; // Initialize d's size
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
    input(a, 1, 0, 0, 4);
    input(a, 2, 0, 2, 5);
    input(a, 3, 1, 1, 1);

    input(b, 0, 3, 2, 3);
    input(b, 1, 0, 0, 1);
    input(b, 2, 1, 1, 2);
    input(b, 3, 2, 0, 1);

    mmult(a, b, d);
    printMatrix(d);
    return 0;
}
