#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100
void strnins(char* s, char* t, int i)
{
    /* insert string t into string s at ith index */
    char temp[MAX_SIZE] = { 0, };
    if(i < 0 || i > strlen(s))
    {
        fprintf(stderr, "Index is out of bounds!\n");
        exit(-1);
    }
    else if(strlen(s) == 0)
        strcpy(s, t);
    else
    {
        strncpy(temp, s, i);
        strcat(temp, t);
        strcat(temp, (s + i));
        strcpy(s, temp);
    }
    return;
}
int main(int argc, char** argv)
{
    char src[MAX_SIZE] = "Source string";
    char dst[MAX_SIZE] = "Destination string";
    strnins(src, dst, 3);
    printf("%s\n", src);
    return 0;
}