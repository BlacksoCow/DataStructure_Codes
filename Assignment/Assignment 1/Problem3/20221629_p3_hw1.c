#include <stdio.h>
#include <stdlib.h>
#define MAX_COL 1000
#define MAX_TERMS 1000
typedef struct
{
	int row;
	int col;
	int value;
} term;
void readMatrix(term mat[], FILE* fp)
{
	int row, col, value, idx = 0, cnt = 0;
	fscanf(fp, "%d%d", &row, &col);
	mat[idx].row = row; mat[idx++].col = col;
	for (int i = 1; i <= row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			fscanf(fp, "%d", &value);
			if (value != 0)
			{
				mat[idx].row = i - 1;
				mat[idx].col = j;
				mat[idx++].value = value;
				mat[0].value++;
			}
		}
		
	}
	return;
}
void printMatrix(term mat[], FILE* fp)
{
	fprintf(fp, "%d %d %d\n", mat[0].row, mat[0].col, mat[0].value);
	for (int i = 1; i <= mat[0].value; i++)
		fprintf(fp, "%d %d %d\n", mat[i].row, mat[i].col, mat[i].value);
	return;
}
int COMPARE(int a, int b)
{
	if (a < b)
		return -1;
	else if (a == b)
		return 0;
	else
		return 1;
}
void fast_transpose(term a[], term b[])
{ /* the transpose of a is placed in b */
	int row_terms[MAX_COL] = { 0, }, starting_pos[MAX_COL];
	int i, j, num_cols = a[0].col, num_terms = a[0].value;
	b[0].row = num_cols; b[0].col = a[0].row;
	b[0].value = num_terms;
	if (num_terms > 0) { /* nonzero matrix */
		for (i = 1; i <= num_terms; i++) row_terms[a[i].col]++;
		starting_pos[0] = 1;
		for (i = 1; i < num_cols; i++)
			starting_pos[i] = starting_pos[i - 1] + row_terms[i - 1];
		for (i = 1; i <= num_terms; i++) {
			j = starting_pos[a[i].col]++;
			b[j].row = a[i].col; b[j].col = a[i].row;
			b[j].value = a[i].value;
		}
	}
	return;
}
void storesum(term d[], int* totald, int row, int column, int* sum)
{ /* if *sum != 0, then it along with its row and column position is stored
  as the *totald+1 entry in d */
	if (*sum)
		if (*totald < MAX_TERMS) {
			d[++*totald].row = row;
			d[*totald].col = column;
			d[*totald].value = *sum;
			*sum = 0;
		}
		else {
			fprintf(stderr, "Numbers of terms exceeds % d", MAX_TERMS);
			exit(-1);
		}
	return;
}
void mmult(term a[], term b[], term d[])
/* multiply two sparse matrices */
{
	int i, j, column, totalb = b[0].value, totald = 0;
	int rows_a = a[0].row, cols_a = a[0].col, totala = a[0].value;
	int cols_b = b[0].col;
	int row_begin = 1, row = a[1].row, sum = 0;
	term new_b[MAX_TERMS];
	if (cols_a != b[0].row) {
		fprintf(stderr, "Incompatible matrices\n");
		exit(1);
	}
	fast_transpose(b, new_b);
	/* set boundary condition */
	a[totala + 1].row = rows_a;
	new_b[totalb + 1].row = cols_b; new_b[totalb + 1].col = -1;
	for (i = 1; i <= totala; ) {
		column = new_b[1].row;
		for (j = 1; j <= totalb + 1; ) {
			/* multiply row of a by column of b */
			if (a[i].row != row) {
				storesum(d, &totald, row, column, &sum);
				i = row_begin;
				for (; new_b[j].row == column; j++);
				column = new_b[j].row;
			}
			else if (new_b[j].row != column) {
				storesum(d, &totald, row, column, &sum);
				i = row_begin;
				column = new_b[j].row;
			}
			else
			{
				switch (COMPARE(a[i].col, new_b[j].col))
				{
					case -1: /* go to next term in a */
						i++; break;
					case 0: /* add terms, go to next term in a and b */
						sum += (a[i++].value * new_b[j++].value);
						break;
					case 1: /* go to next term in b */
						j++;
				}
			}
		} /* end of for j <= totalb+1 */
		for (; a[i].row == row; i++);
		row_begin = i; row = a[i].row;
	} /* end of for i <= totala */
	d[0].row = rows_a; d[0].col = cols_b;
	d[0].value = totald;
	return;
}
int main(int argc, char** argv)
{
	FILE* fpA, * fpB, * fpW;
	fpA = fopen("inputA.txt", "r");
	fpB = fopen("inputB.txt", "r");
	fpW = fopen("output.txt", "w");
	term matA[MAX_TERMS] = { 0, }, matB[MAX_TERMS] = { 0, };
	term new_B[MAX_TERMS] = { 0, }, resmat[MAX_TERMS] = { 0, }; // Resulting matrix
	readMatrix(matA, fpA);
	readMatrix(matB, fpB);
	mmult(matA, matB, resmat);
	printMatrix(resmat, fpW);
	return 0;
}
