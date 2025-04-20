#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100
typedef enum
{
    lparen, rparen, plus, minus, times, divide, mod, eos, operand
} precedence;
int stack[MAX_SIZE], top = -1;
char expr[MAX_SIZE];
void push(int value)
{
    stack[++top] = value;
    return;
}
int pop(void)
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
int eval(void)
{
    precedence token;
    char symbol;
    int op1, op2, n = 0; // Index for the expr string
    token = getToken(&symbol, &n);
    while(token != eos)
    {
        if(token == operand)
            push(symbol - '0'); // Push operand as integer type
        else
        {
            op2 = pop(), op1 = pop();
            switch(token)
            {
                case plus:
                    push(op1 + op2); break;
                case minus:
                    push(op1 - op2); break;
                case times:
                    push(op1 * op2); break;
                case divide:
                    push(op1 / op2); break;
                case mod:
                    push(op1 % op2); break;
            }
        }
        token = getToken(&symbol, &n);
    }
    return pop();
}
int main(int argc, char** argv)
{
    strcpy(expr, "62/34-*");
    printf("%s = %d\n", expr, eval());
    return 0;
}