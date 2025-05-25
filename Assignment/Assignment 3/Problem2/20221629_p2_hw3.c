#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 50
typedef enum { HEAD, ENTRY } tagfield; // Tag to decide Node's type
typedef struct
{ // Node to describe the nonzero entry Node
    int row, col;
    int value;
} EntryNode;
typedef struct matrix_node
{ // Node to describe the main Node of matrix
    tagfield tag;
    union
    { // Dependent to tag value
        struct matrix_node* next; // Tag is head
        EntryNode entry; // Tag is entry
    } type;
    struct matrix_node* right, *down;
} MatrixNode;
MatrixNode* hdnode[MAXSIZE], *hdTnode[MAXSIZE]; // Global array for represeting matrices
MatrixNode* mread(void)
{ // Read in a matrix, set up its linked representation and return the head Node
    int rowNum, colNum, termNum, headNum;
    int row, col, value, curRow = 0;
    MatrixNode* temp, *last, *head; // Temporary newNode, last Node, head of hdnode
    FILE* fp = fopen("input.txt", "r");
    fscanf(fp, "%d%d%d", &rowNum, &colNum, &termNum);
    if(rowNum > MAXSIZE || colNum > MAXSIZE || termNum > MAXSIZE * MAXSIZE)
    { // Print error if input exceeds matrix MAXSIZE
        fprintf(stderr, "Exceeding maximum matrix size!\n");
        exit(-1);
    }
    headNum = (rowNum > colNum ? rowNum : colNum); // headNum = max(rowNum, colNum)
    head = (MatrixNode*)malloc(sizeof(MatrixNode));
    head->tag = ENTRY, head->down = NULL, head->right = head; // Initialize the head of header Nodes
    head->type.entry.row = rowNum, head->type.entry.col = colNum, head->type.entry.value = termNum;
    for(int i = 0; i < headNum; i++)
    { // Initialize all header Nodes
        hdnode[i] = (MatrixNode*)malloc(sizeof(MatrixNode));
        hdnode[i]->tag = HEAD, hdnode[i]->type.next = hdnode[i];
        hdnode[i]->down = hdnode[i], hdnode[i]->right = hdnode[i];
    }
    last = hdnode[0]; // Starting from hdnode[0]
    for(int i = 0; i < termNum; i++)
    { // Repeat until every terms are read, increase in row order
        fscanf(fp, "%d%d%d", &row, &col, &value);
        if(row >= MAXSIZE || col >= MAXSIZE)
        { // Print error if input exceeds MAXSIZE
            fprintf(stderr, "Exceeding maximum matrix size!\n");
            exit(-1);
        }
        if(row > curRow)
        { // If current row is over, cycle list and move on to next row
            last->right = hdnode[curRow];
            curRow = row;
            last = hdnode[row];
        }
        temp = (MatrixNode*)malloc(sizeof(MatrixNode));
        temp->tag = ENTRY;
        temp->type.entry.row = row;
        temp->type.entry.col = col;
        temp->type.entry.value = value; // Initialize temp Node
        last->right = temp; // Link to right(column) in increasing order
        last = temp; // Update last(hdnode[row])
        hdnode[col]->type.next->down = temp;
        hdnode[col]->type.next = temp; // Update hdnode[col]
    }
    last->right = hdnode[curRow]; // Closing last hdnode[row]
    for(int i = 0; i < colNum; i++)
        hdnode[i]->type.next->down = hdnode[i]; // Closing every hdnode[col]
    for(int i = 0; i < headNum - 1; i++)
        hdnode[i]->type.next = hdnode[i + 1]; // Linking every head nodes
    hdnode[headNum - 1]->type.next = head; // Cycling header Nodes
    head->right = hdnode[0]; // Final linking to hdnode
    fclose(fp);
    return head;
}
void mwrite(MatrixNode* node)
{ // Print out the matrix in row major form
    FILE* fp = fopen("output.txt", "w");
    MatrixNode* head = node->right; // Initialize head to hdnode[0]
    fprintf(fp, "%d %d %d\n", node->type.entry.row, node->type.entry.col, node->type.entry.value);
    for(int i = 0; i < node->type.entry.row; i++)
    { // Repeat by the number of rows
        for(MatrixNode* colNode = head->right; colNode != head; colNode = colNode->right) // Starting from hdnode[i]
            fprintf(fp, "%d %d %d\n", colNode->type.entry.row, colNode->type.entry.col, colNode->type.entry.value);
        head = head->type.next; // Update head to next row after every column is finished
    }
    fclose(fp);
    return;
}
MatrixNode* mtranspose(MatrixNode* matrix)
{ // Return the transposed matrix without altering original matrix
    int rowNum = matrix->type.entry.col, colNum = matrix->type.entry.row;
    int termNum = matrix->type.entry.value, headNum = (rowNum > colNum ? rowNum : colNum);
    int row, col, value, curRow = 0;
    MatrixNode* temp, *last, *curHead, *ret;
    ret = (MatrixNode*)malloc(sizeof(MatrixNode));
    ret->tag = ENTRY, ret->down = NULL, ret->right = ret; // Initialize the head of transpose header Nodes
    ret->type.entry.row = rowNum, ret->type.entry.col = colNum, ret->type.entry.value = termNum;
    for(int i = 0; i < headNum; i++)
    { // Initialize all transpose header Nodes
        hdTnode[i] = (MatrixNode*)malloc(sizeof(MatrixNode));
        hdTnode[i]->tag = HEAD, hdTnode[i]->type.next = hdTnode[i];
        hdTnode[i]->down = hdTnode[i], hdTnode[i]->right = hdTnode[i];
    }
    last = hdTnode[0], curHead = matrix->right->down;
    for(int i = 0; i < termNum; i++)
    { // Repeat until every terms are read, increase in row order
        if(curHead->tag == HEAD)
        { // If current row is over, cycle list and move on to next row
            last->right = hdTnode[curRow]; // Connect last Node's cycle
            do { // Move to non zero head Node
                curHead = curHead->type.next;
                curRow++;
            } while(curHead->down == curHead);
            curHead = curHead->down;
            last = hdTnode[curRow];
        }
        row = curHead->type.entry.col, col = curHead->type.entry.row, value = curHead->type.entry.value;
        temp = (MatrixNode*)malloc(sizeof(MatrixNode));
        temp->tag = ENTRY;
        temp->type.entry.row = row;
        temp->type.entry.col = col;
        temp->type.entry.value = value; // Initialize temp Node
        last->right = temp; // Link to right(column) in increasing order
        last = temp; // Update last(hdTnode[row])
        hdTnode[col]->type.next->down = temp;
        hdTnode[col]->type.next = temp; // Update hdTnode[col]
        curHead = curHead->down; // Move downwards
    }
    last->right = hdTnode[curRow]; // Closing last hdTnode[row]
    for(int i = 0; i < colNum; i++)
        hdTnode[i]->type.next->down = hdTnode[i]; // Closing every hdTnode[col]
    for(int i = 0; i < headNum - 1; i++)
        hdTnode[i]->type.next = hdTnode[i + 1]; // Linking every head nodes
    hdTnode[headNum - 1]->type.next = ret; // Cycling header Nodes
    ret->right = hdTnode[0]; // Final linking to hdTnode
    return ret;
}
void mfree(MatrixNode* node)
{ // Free every dynamically allocated Nodes from matrix
    MatrixNode* delNode, *start, *curNode = node->right; // Start from hdnode[0]
    while(curNode != node)
    { // Iterate until curNode returns back to starting head node
        start = curNode->down; // Free Nodes downwards
        while(start != curNode)
        { // Erase all row entries
            delNode = start, start = start->down;
            free(delNode);
        }
        delNode = curNode, curNode = curNode->type.next;
        free(delNode); // Free header Node and move on to next header Node
    }
    free(curNode); // Free the first header Node
    return;
}
int main(int argc, char** argv)
{
    MatrixNode* matrix = mread();
    MatrixNode* transMatrix = mtranspose(matrix);
    mwrite(transMatrix);
    mfree(matrix); mfree(transMatrix);
    return 0;
}