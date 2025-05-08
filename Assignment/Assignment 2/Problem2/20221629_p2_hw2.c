#include <stdio.h>
#include <stdlib.h>
typedef struct
{
    int ver, hor;
} Offset;
void InitOffset(Offset* move)
{ // Initialize offset array
    move[0].ver = -1, move[0].hor = 0;
    move[1].ver = -1, move[1].hor = 1;
    move[2].ver = 0, move[2].hor = 1;
    move[3].ver = 1, move[3].hor = 1;
    move[4].ver = 1, move[4].hor = 0;
    move[5].ver = 1, move[5].hor = -1;
    move[6].ver = 0, move[6].hor = -1;
    move[7].ver = -1, move[7].hor = -1;
    return;
}
typedef struct
{
    int row, col; // Current location
} Element; // Queue's data type is Element. Modify here to change data type.
typedef struct
{
    Element* arr; // Array of elements
    int front, rear; // Index of front and rear
} Queue;
Queue* CreateQ(int maxQueueSize)
{ // Create queue dynamically and return pointer
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->arr = (Element*)malloc(maxQueueSize * sizeof(Element));
    queue->front = 0, queue->rear = 0; // Queue is empty when front == rear
    return queue;
}
void FreeQ(Queue* queue)
{ // Free dynamically allocated memories
    free(queue->arr);
    free(queue);
    return;
}
int IsFull(Queue* queue)
{ // Check whether queue is full
    if((queue->rear + 1) % 100 == queue->front)
        return 1;
    return 0;
}
void AddQ(Queue* queue, Element item)
{ // Add item at the rear of queue. If queue is empty, exit program
    if(IsFull(queue))
    {
        puts("Queue is full!");
        exit(-1);
    }
    queue->rear = (queue->rear + 1) % 100;
    queue->arr[queue->rear] = item;
    return;
}
int IsEmpty(Queue* queue)
{ // Check whether queue is empty
    if(queue->front == queue->rear)
        return 1;
    return 0;
}
Element DeleteQ(Queue* queue)
{ // Delete an item from stack. If queue is empty, exit program
    if(IsEmpty(queue))
    {
        puts("Queue is empty!");
        exit(-1);
    }
    queue->front = (queue->front + 1) % 100;
    return queue->arr[queue->front];
}
Element** MakePathMaze(int row, int col)
{ // Create and return maze pointer that records the path
    Element** maze = (Element**)malloc((row + 2) * sizeof(Element*));
    for(int i = 0; i < row + 2; i++)
        maze[i] = (Element*)calloc((col + 2), sizeof(Element)); // Initialize size
    return maze;
}
void FreePathMaze(Element** maze, int row, int col)
{ // Free the dynamically allocated path maze
    for(int i = 0; i < row + 2; i++)
        free(maze[i]);
    free(maze);
    return;
}
int** MakeMaze(int row, int col)
{ // Create, initialize and return maze pointer
    int** maze = (int**)malloc((row + 2) * sizeof(int*));
    for(int i = 0; i < row + 2; i++)
        maze[i] = (int*)calloc((col + 2), sizeof(int)); // Initialize size
    for(int i = 0; i < row + 2; i++)
        maze[i][0] = 1, maze[i][col + 1] = 1;
    for(int i = 0; i < col + 2; i++)
        maze[0][i] = 1, maze[row + 1][i] = 1; // Make border around the maze
    return maze;
}
void FillMaze(FILE* fp, int** maze, int row, int col)
{ // Fill in the values into maze
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            fscanf(fp, "%d", &maze[i + 1][j + 1]);
    return;
}
void FreeMaze(int** maze, int row, int col)
{ // Free dynamically allocated maze
    for(int i = 0; i < row + 2; i++)
        free(maze[i]);
    free(maze);
    return;
}
void PrintPath(Element* path, int size)
{ // Print the path of path array in reverse order since path array is reversed
    puts("The path is:\nrow    col");
    for(int i = size; i >= 0; i--)
        printf(" %d\t%d\n", path[i].row, path[i].col);
    return;
}
void FindPath(int** maze, int** mark, int row, int col)
{ // Find the path of the maze and save it into path array
    if(maze[row][col] == 1)
    { // If target location is not 0 return immediately
        puts("Target location is unreachable!");
        return;
    }
    Element** pathMaze = MakePathMaze(row, col); // Temporary maze for recording the path
    Element* path = (Element*)malloc(row * col * sizeof(Element)); // Final path array
    Element pos, nextPos;
    pos.row = 1, pos.col = 1;
    Queue* queue = CreateQ(101);
    AddQ(queue, pos); // Queue for saving available positions
    mark[pos.row][pos.col] = 1; // Initial visit
    Offset* move = (Offset*)malloc(8 * sizeof(Offset)); // 8 directions available
    InitOffset(move);
    int nextRow, nextCol, found = 0, pathidx = 0;
    while(!IsEmpty(queue))
    { // Repeat until queue becomes empty
        pos = DeleteQ(queue);
        for(int i = 0; i < 8; i++)
        {
            nextRow = pos.row + move[i].ver;
            nextCol = pos.col + move[i].hor;
            if(nextRow == row && nextCol == col)
            { // Found the target location
                pathMaze[nextRow][nextCol] = pos; // Record the previous space
                pos.row = nextRow, pos.col = nextCol; // Update the current position
                found = 1; break;
            }
            else if(maze[nextRow][nextCol] == 0 && mark[nextRow][nextCol] == 0) // Able to move to next position
            { // mark's value will be 1 if next space is visited or outside map
                mark[nextRow][nextCol] = 1; // Update to visited
                pathMaze[nextRow][nextCol] = pos; // Record the previous space
                nextPos.row = nextRow, nextPos.col = nextCol; // Update the current position
                AddQ(queue, nextPos); // Push the next position
            }
        }
        if(found == 1)
            break;
    }
    if(found == 0)
        puts("Path doesn't exist!"); // No path in maze
    else
    { // Copy the path into path array
        pos.row = row, pos.col = col; // Update pos to target location
        path[pathidx++] = pos;
        while(!(pos.row == 1 && pos.col == 1))
        { // Repeat until pos reaches starting point
            path[pathidx++] = pathMaze[pos.row][pos.col];
            pos = pathMaze[pos.row][pos.col]; // Update position
        }
        pathidx--;// Include staring point to path array
        PrintPath(path, pathidx);
    }
    FreePathMaze(pathMaze, row, col);
    free(path);
    free(move);
    FreeQ(queue);
    return;
}
int main(int argc, char** argv)
{
    FILE* fp = fopen("Problem2/input.txt", "r");
    if(fp == NULL)
    { // If file open failed crash the program
        printf("File open failed");
        exit(-1);
    }
    int row, col;
    fscanf(fp, "%d%d", &row, &col); // Get the size of maze
    int** maze = MakeMaze(row, col); // Create maze
    FillMaze(fp, maze, row, col); // Fill in maze
    int** mark = MakeMaze(row, col); // Check whether the space was visited
    FindPath(maze, mark, row, col); // Find and print the path in maze
    FreeMaze(maze, row, col);
    FreeMaze(mark, row, col);
    fclose(fp);
    return 0;
}