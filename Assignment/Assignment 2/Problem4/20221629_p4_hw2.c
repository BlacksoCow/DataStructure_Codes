#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef char* Element; // Stack's data type is Element. Modify here to change data type.
typedef struct
{
    Element* arr; // Element array
    int top; // Top of stack
} Stack;
Stack* CreateS(int maxStackSize)
{ // Create stack dynamically and return pointer
    Stack* stack = (Stack*)malloc(sizeof(Stack)); // Creating stack
    stack->arr = (Element*)malloc(maxStackSize * sizeof(Element));
    for(int i = 0; i < maxStackSize; i++)
        stack->arr[i] = (char*)calloc(100, sizeof(char));
    stack->top = -1; // Initializing stacks members
    return stack;
}
void FreeS(Stack* stack, int maxStackSize)
{ // Free dynamically allocated memories
    for(int i = 0; i < maxStackSize; i++)
        free(stack->arr[i]);
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
    strcpy((stack->arr)[++(stack->top)], item);
    return;
}
int IsEmpty(Stack* stack)
{ // Check whether stack is empty
    if(stack->top < 0)
        return 1;
    return 0;
}
Element* pop(Stack* stack)
{ // Pop an item from stack. If stack is empty, exit program
    if(IsEmpty(stack))
    {
        puts("Stack is empty!");
        exit(-1);
    }
    return &(stack->arr)[(stack->top)--]; // Return the string pointer
}
int main(int argc, char** argv)
{
    char str[100] = { '\0', }, directory[100] = { '\0', };
    scanf("%s", directory);
    if(directory[0] != '/')
    { // If input is not normalized, output same string and exit
        printf("%s\n", directory);
        return 0;
    }
    Stack* stack = CreateS(100);
    int sIdx = 0, dIdx = 0;
    while(directory[dIdx] != '\0')
    { // Repeat until input string reaches end
        if(directory[dIdx] == '/')
        { // If character is '/', push the directory name into stack
            if(sIdx != 0)
            { // Only when directory name is not empty
                str[sIdx] = '\0'; // Update last character as null
                if(strcmp(str, "..") == 0)
                { // If directory is ".." remove previous directory when stack is not empty
                    if(!IsEmpty(stack))
                    pop(stack);
                }
                else if(strcmp(str, ".") != 0)
                    push(stack , str); // If directory is not ".." and ".", push into stack
                sIdx = 0;
            }
        }
        else
            str[sIdx++] = directory[dIdx]; // Concatenate characters into directory name
        dIdx++;
    }
    if(sIdx != 0)
    { // Push or pop the rest of directory name into stack
        str[sIdx] = '\0'; // Update last character as null
        if(strcmp(str, "..") == 0)
            pop(stack); // If directory is ".." remove previous directory
        else if(strcmp(str, ".") != 0)
            push(stack , str); // If directory is not ".." and ".", push into stack
        sIdx = 0;
    }
    if(IsEmpty(stack))
        putchar('/');
    else
    { // Only print when stack is not empty
        for(int i = 0; i <= stack->top; i++)
            printf("/%s", stack->arr[i]);
    }
    putchar('\n');
    FreeS(stack, 100);
    return 0;
}