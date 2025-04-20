#include <stdio.h>
#define MAX_TERMS 1000
typedef struct
{
	double coef;
	int exp;
} Polynomial;
Polynomial terms[MAX_TERMS] = { 0, };
int curIdx = 0;
int readPoly(int startIdx)
{ // Read polynomial as input
	int termNum, exp, FLAG = 0;
	double coef;
	puts("Input polynomial:");
	scanf("%d", &termNum);
	while (curIdx + termNum >= MAX_TERMS)
	{ // If size overflows array, resize polynomial
		puts("Polynomial size is too big!");
		scanf("%d", &termNum);
	}
	for (int i = 0; i < termNum; i++)
	{
		FLAG = 0;
		scanf("%lf%d", &coef, &exp);
		for (int i = startIdx; i < curIdx; i++)
		{ // If same exp exists, conflate them
			if (terms[i].exp == exp)
			{
				terms[i].coef += coef; FLAG = 1;
				break;
			}
		}
		if (FLAG == 0)
		{ // Fill in terms array if not conflated
			terms[curIdx].coef = coef;
			terms[curIdx++].exp = exp;
		}
	}
	return curIdx;
}
int pmult(int startA, int startB, int startRes)
{ // Multiply two polynomials inputted
	int FLAG = 0;
	Polynomial res = { 0, };
	for (int i = startA; i < startB; i++)
	{
		for (int j = startB; j < startRes; j++)
		{
			FLAG = 0;
			res.coef = terms[i].coef * terms[j].coef;
			res.exp = terms[i].exp + terms[j].exp;
			for (int k = startRes; k < curIdx; k++)
			{ // If same exp exists, conflate them
				if (terms[k].exp == res.exp)
				{
					terms[k].coef += res.coef; FLAG = 1;
					break;
				}
			}
			if (FLAG == 0)
			{ // Fill in terms if not conflated
				terms[curIdx++] = res;
				if (curIdx >= MAX_TERMS)
					return 0;
			}
		}
	}
	return 1;
}
void printPoly(int startRes)
{ // Prints polynomial result
	while (terms[startRes].coef == 0)
		startRes++; // Move startRes pos until coef != 0
	if (startRes >= curIdx)
	{ // If result is empty, print 0
		putchar('0');
		return;
	}
	for(int i = startRes; i < curIdx; i++)
	{
		double coef = terms[i].coef;
		int exp = terms[i].exp;
		if (coef == 1 || coef == -1)
		{ // Coef == 1 -> Print when exp == 0, dont when exp != 1
			if (exp == 0)
				printf("%g", coef);
			else
			{ // exp != 0 -> print exp
				if (coef == 1)
					printf("x^%d", exp);
				else if (coef == -1)
					printf("-x^%d", exp);
			}
		}
		else if (coef != 0)
		{ // Coef != 0, 1 -> 3 cases when exp == 0, 1, else
			if (exp == 0)
				printf("%g", coef);
			else
				printf("%gx^%d", coef, exp);
		}
		if (terms[i + 1].coef > 0)
			putchar('+');
	}
	putchar('\n');
	return;
}
void BubbleSort(int startIdx, int endIdx)
{
	Polynomial temp = { 0, };
	for (int i = startIdx; i < endIdx - 1; i++)
	{
		for (int j = i + 1; j < endIdx; j++)
		{
			if (terms[i].exp < terms[j].exp)
			{ // If latter term's exp is greater, swap terms
				temp = terms[i];
				terms[i] = terms[j];
				terms[j] = temp;
			}
		}
	}
	return;
}
int main(int argc, char** argv)
{
	int startA = 0;
	int startB = readPoly(startA);
	int startRes = readPoly(startB);
	if (pmult(startA, startB, startRes))
	{ // If multiplication succeeds, print result
		BubbleSort(startRes, curIdx);
		printPoly(startRes);
	}
	else
		puts("Multiplication out of bounds! Exit program!");
	return 0;
}
