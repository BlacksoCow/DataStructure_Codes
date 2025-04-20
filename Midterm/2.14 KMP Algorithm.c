#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100
int failure[MAX_SIZE + 1];
void fail(char* pat)
{
    /* Evaluate the pattern's failure function */
    int i = 0, j = 0, lenp = strlen(pat);
    failure[0] = -1;
    for (int j = 1; j < lenp; j++)
    {
        i = failure[j - 1];
        while (pat[j] != pat[i + 1] && (i >= 0))
            i = failure[i];
        if (pat[j] == pat[i + 1])
            failure[j] = i + 1;
        else
            failure[j] = -1;
    }
    return;
}
int pmatch(char* string, char* pat)
{
    /* KMP Algorithm */
    int i = 0, j = 0;
    int lens = strlen(string), lenp = strlen(pat);
    fail(pat);
    while(i < lens && j < lenp)
    {
        if (string[i] == pat[j])
            i++, j++;
        else if (j == 0)
            i++;
        else
            j = failure[j - 1] + 1;
    }
    return ((j == lenp) ? (i - lenp) : -1);
}
int main(int argc, char** argv)
{
    char string[MAX_SIZE + 1] = "Fucking data structure";
    char pat[MAX_SIZE + 1] = "ata str";
    printf("First starting index: %d\n", pmatch(string, pat));
    return 0;
}