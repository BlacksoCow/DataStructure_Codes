#include <stdio.h>
void SWAP(char* c1, char* c2)
{
    char temp = *c1;
    *c1 = *c2;
    *c2 = temp;
    return;
}
void perm(char* list, int i, int n)
{
    /* generate all the permutation of list[i] to list[n] */
    if(i == n)
    {
        for(int j = 0; j < n; j++)
            printf("%c", list[j]);
        putchar('\n');
    }
    else
    {
        for(int j = i; j < n; j++)
        {
            SWAP(&list[i], &list[j]);
            perm(list, i + 1, n);
            SWAP(&list[i], &list[j]);
        }
    }
    return;
}
int main(int argc, char** argv)
{
    char list[] = {'a', 'b', 'c', 'd'};
    perm(list, 0, sizeof(list));
    return 0;
}