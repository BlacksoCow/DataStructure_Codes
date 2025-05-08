#include <stdio.h>
#include <stdlib.h>
typedef enum
{ lparen, rparen, plus, minus, times, divide,
mod, unary_minus, eos, operand } precedence;
typedef int Element; // Stack's data type is Element. Modify here to change data type.
typedef struct
{
    Element* arr; // Element array
    int top; // Top of stack
} Stack;
Stack* CreateS(int maxStackSize)
{ // Create stack dynamically and return pointer
    Stack* stack = (Stack*)malloc(sizeof(Stack)); // Creating stack
    stack->arr = (Element*)malloc(maxStackSize * sizeof(Element));
    stack->top = -1; // Initializing stack's members
    return stack;
}
void FreeS(Stack* stack)
{ // Free dynamically allocated memories
    free(stack->arr);
    free(stack);
    return;
}
int IsFull(Stack* stack)
{ // Check whether stack is full
    if(stack->top >= 99)
        return 1;
    return 0;
}
void push(Stack* stack, Element item)
{ // Push item into stack. If stack is full, exit program
    if (IsFull(stack))
    {
        puts("Stack is full!");
        exit(-1);
    }
    (stack->arr)[++(stack->top)] = item;
    return;
}
int IsEmpty(Stack* stack)
{ // Check whether stack is empty
    if(stack->top < 0)
        return 1;
    return 0;
}
Element pop(Stack* stack)
{ // Pop an item from stack. If stack is empty, exit program
    if(IsEmpty(stack))
    {
        puts("Stack is empty!");
        exit(-1);
    }
    return (stack->arr)[(stack->top)--];
}
precedence getToken(char expr[], char* symbol, int* n)
{ // Return the symbol as precedence enumerated number
    *symbol = expr[(*n)++];
    switch(*symbol)
    {
        case '(': return lparen;
        case ')': return rparen;
        case '+': return plus;
        case '-': return minus;
        case '*': return times;
        case '/': return divide;
        case '%': return mod;
        case '#': return unary_minus;
        case '\0': return eos;
        default: return operand;
    }
}
char returnToken(precedence token)
{ // Return the character type of token base on precedence
    switch(token)
    {
        case plus:
            return '+';
        case minus:
            return '-';
        case times:
            return '*';
        case divide:
            return '/';
        case mod:
            return '%';
        case unary_minus:
            return '#';
    }
}
void postfix(Stack* stack, char expr[])
{ // Output the postfix version of the infix expression
    char symbol, temp[100] = { '\0', };
    int i, n = 0, idx = 0;
    int isp[] = {0, 19, 12, 12, 13, 13, 13, 14, 0}, icp[] = {20, 19, 12, 12, 13, 13, 13, 14, 0};
    precedence token, prev = eos;
    stack->top = 0;
    (stack->arr)[0] = eos;
    for(token = getToken(expr, &symbol, &n); token != eos; token = getToken(expr, &symbol, &n))
    {
        if(token == operand)
            temp[idx++] = symbol;
        else if(token == rparen)
        { // Unstack symbols until lparen appears
            while((stack->arr)[stack->top] != lparen)
                temp[idx++] = returnToken(pop(stack));
            pop(stack);
        }
        else
        { // Print token until input token exceeds top's precedence
            if(token == minus && (prev != operand &&  prev != rparen))
                token = unary_minus; // Case when minus function as unary minus
            while(isp[(stack->arr)[stack->top]] >= icp[token])
                temp[idx++] = returnToken(pop(stack));
            push(stack, token);
        }
        prev = token; // Updating previous token
    }
    while((token = pop(stack)) != eos)
        temp[idx++] = returnToken(token);
    for(i = 0; i < idx; i++)
    { // Switching expr into postfix expression
        printf("%c", temp[i]);
        expr[i] = temp[i];
    }
    expr[i] = '\0';
    putchar('\n');
    return;
}
int eval(Stack* stack, char expr[])
{ // Evaluate the postfix expression and return the result
    char symbol;
    int op1, op2, n = 0;
    precedence token;
    while((token = getToken(expr, &symbol, &n)) != eos)
    {
        if(token == operand)
            push(stack, symbol - '0'); // Pushing value as integer
        else if(token == unary_minus)
            push(stack, (-1) * pop(stack));
        else
        {
            op2 = pop(stack), op1 = pop(stack);
            switch(token)
            {
                case plus: push(stack, op1 + op2); break;
                case minus: push(stack, op1 - op2); break;
                case times: push(stack, op1 * op2); break;
                case divide: push(stack, op1 / op2); break;
                case mod: push(stack, op1 % op2); break;
            }
        }
    }
    return pop(stack);
}
int main(int argc, char** argv)
{
    Stack* stack = CreateS(100);
    char expr[100] = { '\0', }; // Expression array
    printf("Input: "); scanf("%s", expr);
    printf("Postfix: "); postfix(stack, expr);
    FreeS(stack);
    stack = CreateS(100); // Create new stack
    printf("Result: %d\n", eval(stack, expr));
    FreeS(stack);
    return 0;
}