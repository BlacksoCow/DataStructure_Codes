#include <stdio.h>
#include <stdlib.h>
typedef char Element; // Stack's data type is Element. Modify here to change data type.
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
int main(int argc, char** argv)
{
    Stack* stack = CreateS(100);
    char bracket, string[100] = { '\0', };
    scanf("%s", string);
    for(int i = 0; string[i] != '\0'; i++)
    {
        bracket = string[i];
        if(bracket == '(' || bracket == '{' || bracket == '[')
            push(stack, bracket); // If left bracket, push into stack
        else if(bracket == ')')
        {
            if(!IsEmpty(stack) && stack->arr[stack->top] == '(')
                pop(stack); // If matching bracket exists, pop from stack
            else
            { // If right bracket doesn't match stack's top, impossible to be T
                puts("F"); FreeS(stack);
                return 0;
            }
        }
        else if(bracket == '}')
        {
            if(!IsEmpty(stack) && stack->arr[stack->top] == '{')
                pop(stack); // If matching bracket exists, pop from stack
            else
            { // If right bracket doesn't match stack's top, impossible to be T
                puts("F"); FreeS(stack);
                return 0;
            }
        }
        else if(bracket == ']')
        {
            if(!IsEmpty(stack) && stack->arr[stack->top] == '[')
                pop(stack); // If matching bracket exists, pop from stack
            else
            { // If right bracket doesn't match stack's top, impossible to be T
                puts("F"); FreeS(stack);
                return 0;
            }
        }
    }
    if(!IsEmpty(stack))
        puts("F"); // Some left brackets have no pair
    else
        puts("T"); // Only possible when all brackets have pairs and stack is empty
    FreeS(stack);
    return 0;
}