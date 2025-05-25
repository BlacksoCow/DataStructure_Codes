#include <stdio.h>
#include <stdlib.h>
typedef struct _node
{ // Node for a polynomial
    int coef, exp;
    struct _node* next;
} Node;
Node* avail = NULL; // Available space list
int COMPARE(int n1, int n2)
{ // Return the value by comparing two integers
    if(n1 == n2) return 0;
    return (n1 > n2 ? 1 : -1);
}
Node* getNode(void)
{ // Provide a node for use
    Node* retNode;
    if(avail != NULL)
    { // If avail is not empty, set retNode as avail
        retNode = avail;
        avail = avail->next;
    }
    else
    { // If no spare nodes exist, create a new Node
        retNode = (Node*)malloc(sizeof(Node));
        if(retNode == NULL)
        { // Error message for allocation failure
            fprintf(stderr, "Memory is full! Allocation failed");
            exit(-1);
        }
    }
    return retNode;
}
void perase(Node** clist)
{ // Erase the circular list clist
    if(*clist != NULL)
    { // Return the clist to avail list including the header node
        Node* temp = (*clist)->next;
        (*clist)->next = avail;
        avail = temp;
        *clist = NULL;
    }
    return;
}
void attach(int coefficient, int exponent, Node** poly)
{ // Create and attach newNode to poly in exp decreasing order
    Node* term, *curNode = *poly;
    while(curNode->next != *poly)
    { // Repeat until iteration reaches end
        if(curNode->next->exp < exponent)
        { // Link terms in exp decreasing order
            term = getNode(); // Try to get Node from avail
            term->coef = coefficient, term->exp = exponent;
            term->next = curNode->next;
            curNode->next = term;
            return; // Immediately return after linking
        }
        else if(curNode->next->exp == exponent)
        { // If same exp exists, only update coef
            curNode->next->coef += coefficient;
            if(curNode->next->coef == 0)
            { // If coefficient becomes 0, delete Node in list
                Node* delNode = curNode->next;
                curNode->next = curNode->next->next;
                free(delNode);
            }
            return; // Immediately return after updating
        }
        curNode = curNode->next; // Move until curNode becomes last
    }
    term = getNode(); // Terms gets linked at end of list
    term->coef = coefficient, term->exp = exponent;
    term->next = curNode->next; // terms->next becomes *poly
    curNode->next = term;
    return;
}
Node* pread(int termNums)
{ // Read in a polynomial and convert it to its circular represenation
    int coefficient, expon;
    Node* head = (Node*)malloc(sizeof(Node));
    head->coef = 0, head->exp = -1, head->next = head; // Initialize head Node
    for(int i = 0; i < termNums; i++)
    { // Create and attach each terms
        scanf("%d%d", &coefficient, &expon);
        if(coefficient == 0 || expon < 0) continue;
        attach(coefficient, expon, &head); // Attach terms into head
    }
    return head;
}
void pwrite(Node* poly)
{ // Output the polynomial using a form that clearly displays it
    if(poly->next == poly)
    { // Return when poly is empty
        putchar('0'); putchar('\n');
        return;
    }
    Node* curNode = poly->next; // Term's starting position
    do    
    { // Repeat until curNode returns back to start
        int coef = curNode->coef, exp = curNode->exp;
        if(exp != 0) // Case when term x exists
            printf("%dx^%d", coef, exp);
        else // Case when term x doesn't exist
            printf("%d", coef);
        if((curNode->next != poly) && (curNode->next->coef > 0))
            putchar('+'); // Print + sign when next coef is positive
        curNode = curNode->next; // Move to the next term
    } while(curNode != poly);
    putchar('\n');
    return;
}
void padd(Node* A, Node* B)
{ // Print a polynomial which is the sum of A and B
    Node* curA = A, *curB = B, *retNode = getNode();
    int sum, done = 0; // Check whether one polynomial reached the end
    curA = curA->next, curB = curB->next; // Move to the starting term
    retNode->exp = -1, retNode->next = retNode; // Initialize retNode's head
    do
    { // Repeat until done becomes 1
        switch(COMPARE(curA->exp, curB->exp))
        { // If one reaches the end, exp is -1 so no leftovers
            case -1: // curA->exp < curB->exp
                attach(curB->coef, curB->exp, &retNode);
                curB = curB->next; break;
            case 0: // curA->exp == curB->exp
                if(curA == A) done = 1; // Both reached end
                else
                { // Calculate the sum and attach to retNode
                    sum = curA->coef + curB->coef;
                    if(sum != 0)
                        attach(sum, curA->exp, &retNode);
                    curA = curA->next, curB = curB->next;
                } break;
            case 1: // curA->exp > curB->exp
                attach(curA->coef, curA->exp, &retNode);
                curA = curA->next; break;
        }
    } while (done != 1);
    printf("Result: "); pwrite(retNode); // Print out the output
    perase(&retNode); // Return the used list into avail
    return;
}
void psub(Node* A, Node* B)
{ // Print a polynomial which is the sum of A and B
    Node* curA = A, *curB = B, *retNode = getNode();
    int sub, done = 0; // Check whether one polynomial reached the end
    curA = curA->next, curB = curB->next; // Move to the starting term
    retNode->exp = -1, retNode->next = retNode; // Initialize retNode's head
    do
    { // Repeat until done becomes 1
        switch(COMPARE(curA->exp, curB->exp))
        { // If one reaches the end, exp is -1 so no leftovers
            case -1: // curA->exp < curB->exp
                attach(-(curB->coef), curB->exp, &retNode);
                curB = curB->next; break;
            case 0: // curA->exp == curB->exp
                if(curA == A) done = 1; // Both reached end
                else
                { // Calculate the sum and attach to retNode
                    sub = curA->coef - curB->coef;
                    if(sub != 0) 
                        attach(sub, curA->exp, &retNode);
                    curA = curA->next, curB = curB->next;
                } break;
            case 1: // curA->exp > curB->exp
                attach(curA->coef, curA->exp, &retNode);
                curA = curA->next; break;
        }
    } while (done != 1);
    printf("Result: "); pwrite(retNode); // Print out the output
    perase(&retNode); // Return the used list into avail
    return;
}
void pmult(Node* A, Node* B)
{ // Print a polynomial which is the multiple of A and B
    Node* curA = A, *curB = B, *retNode = getNode();
    int sub, done = 0; // Check whether one polynomial reached the end
    retNode->exp = -1, retNode->next = retNode; // Initialize retNode's head
    int coef, exp;
    for(Node* curA = A->next; curA != A; curA = curA->next)
    { // Repeat until curA reaches end
        for(Node* curB = B->next; curB != B; curB = curB->next)
        { // Repeat until curB reaches end
            coef = curA->coef * curB->coef; // Result coef
            exp = curA->exp + curB->exp; // Result exp
            attach(coef, exp, &retNode);
        }
    }
    printf("Result: "); pwrite(retNode); // Print out the output
    perase(&retNode); // Return the used list into avail
    return;
}
void eval(Node* A, Node* B)
{ // Evaluate the value of A(x) and B(x) where x is input
    double x;
    printf("Enter x value: ");
    scanf("%lf", &x);
    Node* curNode = A->next; // Move to starting postion
    double ret = 0, input = 1;
    do
    { // Calculate A(x)
        input = 1;
        for(int i = 0; i < curNode->exp; i++)
            input *= x; // Power of a by curNode->exp
        ret += curNode->coef * input;
        curNode = curNode->next;
    } while(curNode != A);
    printf("A(%g) = %g\n", x, ret);
    curNode = B->next, ret = 0, input = 1; // Move to starting position
    do
    { // Calculate B(x)
        input = 1;
        for(int i = 0; i < curNode->exp; i++)
            input *= x; // Power of a by curNode->exp
        ret += curNode->coef * input;
        curNode = curNode->next;
    } while(curNode != B);
    printf("B(%g) = %g\n", x, ret);
    return;
}
void freePoly(Node* poly)
{ // Free dynamically allocated polynomial
    if(poly == NULL) return;
    Node* curNode = poly->next;
    Node* delNode; // Node to be deleted
    while(curNode != poly)
    { // Free until it comes back to the start
        delNode = curNode;
        curNode = curNode->next;
        delNode->next = NULL;
        free(delNode);
    }
    curNode->next = NULL;
    free(curNode);
    return;
}
void freeAvail(Node* ptr)
{ // Free available list's Nodes
    Node* delNode, *curNode = ptr;
    while(curNode != NULL)
    { // Freeing singly linked list
        delNode = curNode;
        curNode = curNode->next;
        delNode->next = NULL;
        free(delNode);
    }
    return;
}
int main(int argc, char** argv)
{
    int command, terms, option;
    Node* A, *B;
    printf("Enter number of terms in polynomial A: ");
    scanf("%d", &terms);
    puts("Enter terms for A (coef exp):");
    A = pread(terms);
    printf("\nEnter number of terms in polynomial B: ");
    scanf("%d", &terms);
    puts("Enter terms for B (coef exp):");
    B = pread(terms);
    while(1)
    { // Infinite loop
        puts("\nChoose operation:");
        puts("1. Addition");
        puts("2. Subtraction");
        puts("3. Multiplication");
        puts("4. Evaluation");
        puts("5. Quit");
        printf("Enter choice: ");
        scanf("%d", &option);
        if(option == 5) break;
        switch(option)
        {
            case 1: padd(A, B); break;
            case 2: psub(A, B); break;
            case 3: pmult(A, B); break;
            case 4: eval(A, B); break;
            default: puts("Wrong input!"); break; // Do nothing on wrong command
        }
    }
    freePoly(A); freePoly(B); freeAvail(avail);
    puts("Good Bye");
    return 0;
}