#include <stdio.h>
#include <stdlib.h>
void transpose(int** a, int** b, int rows, int cols)
{
	for (int i = 0; i < cols; i++)
		for (int j = 0; j < rows; j++)
			b[i][j] = a[j][i];
	return;
}
int main(int argc, char** argv)
{
	int row, col;
	FILE* fpI = fopen("input.txt", "r"), *fpO = fopen("output.txt", "w");
	if (fpI == NULL || fpO == NULL)
	{
		printf("File open failed.");
		return -1;
	}
	fscanf(fpI, "%d%d", &row, &col);
	int** mat1 = (int**)malloc(row * sizeof(int*));
	if (mat1 == NULL) return -1;
	for (int i = 0; i < row; i++)
	{ // Initialize original matrix
		mat1[i] = (int*)malloc(col * sizeof(int));
		if (mat1[i] == NULL) return -1;
	}
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			fscanf(fpI, "%d", &(mat1[i][j])); // Input complete

	int** mat2 = (int**)malloc(col * sizeof(int*));
	if (mat2 == NULL) exit(-1);
	for (int i = 0; i < col; i++)
	{ // Initialize transpose matrix
		mat2[i] = (int*)malloc(row * sizeof(int));
		if (mat2[i] == NULL) exit(-1);
	}
	transpose(mat1, mat2, row, col);
	fprintf(fpO, "%d %d\n", col, row);
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
			fprintf(fpO, "%d ", mat2[i][j]);
		fputc('\n', fpO);
	}
	for (int i = 0; i < row; i++)
		free(mat1[i]);
	free(mat1);
	for (int i = 0; i < col; i++)
		free(mat2[i]);
	free(mat2);
	fclose(fpI);
	fclose(fpO);
	return 0;
}