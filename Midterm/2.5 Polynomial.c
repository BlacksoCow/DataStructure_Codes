#include <stdio.h>
#include <stdlib.h>
#define MAX_TERMS 100
typedef struct
{
    int exp;
    float coef;
} polynomial;
polynomial terms[MAX_TERMS] = { 0, };
int avail = 0;
int COMPARE(int exp1, int exp2)
{
    if(exp1 == exp2)
        return 0;
    else
        return (exp1 > exp2 ? 1 : -1);
}
void attach(float coef, int exp)
{
    /* add a new term to the polynomial */
    if(avail > MAX_TERMS)
    {
        fprintf(stderr, "Too many terms in the polynomial");
        exit(-1);
    }
    terms[avail].coef = coef;
    terms[avail++].exp = exp;
    return;
}
void padd(int starta, int finisha, int startb, int finishb, int* startd, int* finishd)
{
    /* add A(x) and B(x) to obtain D(x) */
    float coefficient;
    *startd = avail;
    while(starta <= finisha && startb <= finishb)
    {
        switch(COMPARE(terms[starta].exp, terms[startb].exp))
        {
            case -1: // a's exp < b's exp
                attach(terms[startb].coef, terms[startb].exp);
                startb++;
                break;
            case 0: // a's exp == b's exp
                coefficient = terms[starta].coef + terms[startb].coef;
                if(coefficient != 0)
                    attach(coefficient, terms[starta].exp);
                starta++, startb++;
                break;
            case 1: // a's exp > b's exp
                attach(terms[starta].coef, terms[starta].exp);
                starta++;
                break;
        }
    }
    while(starta <= finisha)
    {
        attach(terms[starta].coef, terms[starta].exp);
        starta++;
    }
    while(startb <= finishb)
    {
        attach(terms[startb].coef, terms[startb].exp);
        startb++;
    }
    *finishd = avail - 1;
    return;
}
void printPoly(int start, int finish)
{
    for(int i = start; i <= finish; i++)
    {
        printf("%gx^%d", terms[i].coef, terms[i].exp);
        if(terms[i + 1].coef > 0)
            putchar('+');
    }
    putchar('\n');
    return;
}
int main(int argc, char** argv)
{
    terms[0].coef = 2, terms[0].exp = 2;
    terms[1].coef = -1, terms[1].exp = 1; // A = 2x^2 - x
    terms[2].coef = -3, terms[2].exp = 2;
    terms[3].coef = 1, terms[3].exp = 1; // B = -3x^2 + x
    avail = 4;
    int startd = 0, finishd = 0;
    padd(0, 1, 2, 3, &startd, &finishd);
    printf("startd: %d, finishd: %d\n", startd, finishd);
    printPoly(startd, finishd);
    return 0;
}
