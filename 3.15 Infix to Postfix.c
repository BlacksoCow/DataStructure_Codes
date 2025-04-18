#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100
typedef enum
{
    lparen, rparen, plus, minus, times, divide, mod, eos, operand
} precedence;
precedence stack[MAX_SIZE];
int top = -1;
char expr[MAX_SIZE];
int icp[] = {20, 19, 12, 12, 13, 13, 13, 0};
int isp[] = {0, 19, 12, 12, 13, 13, 13, 0};
void push(precedence value)
{
    stack[++top] = value;
    return;
}
precedence pop(void)
{
    return stack[top--];
}
precedence getToken(char* symbol, int* n)
{
    /* get the next token, symbol is the character representation, which is returned,
    the token is represeted by its enumerated value */
    *symbol = expr[(*n)++];
    switch (*symbol)
    {
        case '(': return lparen;
        case ')': return rparen;
        case '+': return plus;
        case '-': return minus;
        case '*': return times;
        case '/': return divide;
        case '%': return mod;
        case '\0': return eos;
        default: return operand;
    }
}
void printToken(precedence token)
{
    switch(token)
    {
        case plus:
            putchar('+');
            break;
        case minus:
            putchar('-');
            break;
        case times:
            putchar('*');
            break;
        case divide:
            putchar('/');
            break;
        case mod:
            putchar('%');
            break;
    }
    return;
}
void postfix(void)
{
    /* output the postfix of the expression */
    char symbol;
    int n = 0;
    precedence token;
    push(eos);
    for(token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n))
    {
        if(token == operand)
            printf("%c", symbol);
        else if(token == rparen)
        {
            while(stack[top] != lparen)
                printToken(pop()); // Unstack until left parenthesis appears
            pop(); // Discard the left parenthesis
        }
        else
        {
            while(isp[stack[top]] >= icp[token])
                printToken(pop()); // Pop and print until input's precedence is higher
            push(token);
        }
    }
    while((token = pop()) != eos)
        printToken(token);
    putchar('\n');
    return;
}
int main(int argc, char** argv)
{
    strcpy(expr, "a*(b+c/d)");
    postfix();
    return 0;
}