#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#include <player.h>
#include <game.h>
#include <ai.h>

Queue *CreateQueue()
{
    Queue *q = malloc(sizeof(Queue));
    if(q==NULL)
    {
        printf("Queue malloc error\t%s",SDL_GetError());
        return NULL;
    }
    q->first = q->last = NULL;
    return q;
}

Node *Enqueue(Queue *q, SDL_Point a, Node *parent)
{
    Node *node = malloc(sizeof(Node));
    if (node==NULL)
    {
        printf("Error allocating memory for node\n");
        return NULL;
    }
    node->data = a;
    node->parent = parent;  // Correctly assign the parent
    node->next = NULL;

    if (q->last)
        q->last->next = node;
    else
        q->first = node;

    q->last = node;
    return node;
}


SDL_Point Dequeue(Queue* q) {
    if (q->first==NULL) {
        SDL_Point invalid = {0,0};
        return invalid;
    }
    Node* temp = q->first;
    SDL_Point value = temp->data;
    q->first = temp->next;
    if (!q->first) {
        q->last = NULL;
    }
    free(temp);
    return value;
}

void FreeQueue(Queue *que)
{
    if(que==NULL)
    {
        printf("Queue is null, cannot free");
        return;
    }
    while(que->first != NULL)
    {
        Node *temp =que->first;
        que->first = temp->next; 
        free(temp);
    }
    free(que);
}

int distance_btw_points(SDL_Point a, SDL_Point b)
{
    return (int)sqrt(pow(a.x - b.x, 2)+ pow(a.y - b.y, 2));
}

unsigned int Hash(SDL_Point point) 
{
    // Prime numbers for better distribution
    const unsigned int prime1 = 73856093;   // Prime multiplier for x
    const unsigned int prime2 = 19349663;   // Prime multiplier for y

    // Use unsigned int directly for all values to avoid negative results
    unsigned int x = (unsigned int)point.x;
    unsigned int y = (unsigned int)point.y;

    // Hash function using multiplication and modulus
    unsigned int hash = (x * prime1 + y * prime2) % SIZE;

    return hash;
}


HashTable *CreateHashTable()
{   
    HashTable *htable = (HashTable*)malloc(sizeof(HashTable));
    if(htable==NULL)
    {
        printf("Malloc error hashtable\t%s",SDL_GetError());
        return NULL;
    }
    htable->size = SIZE;
    htable->table = malloc(sizeof(SDL_Point) * htable->size);
    if(htable->table==NULL)
    {
        printf("Malloc error hashtable->table\t%s",SDL_GetError());
        return NULL;
    }
    for(int i=0;i<htable->size;i++)
    {
        htable->table[i].x = -1;
        htable->table[i].y = -1;
    }
    return htable;
}

int CheckAndAddToHashTable(HashTable *htable, SDL_Point point) {
    unsigned int index = Hash(point);
    if (htable->table[index].x == -1 && htable->table[index].y == -1) {
        htable->table[index] = point;
        return 1; // Successfully added
    }
    return 0; // Already exists
}

void FreeHashTable(HashTable *htable)
{
    if(htable==NULL) return;
    free(htable->table);
    free(htable);
}

SDL_Point *FindAdjacent(SDL_Point base)
{
    SDL_Point adj[8] = {{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
    SDL_Point *return_array = malloc(sizeof(SDL_Point)*8);
    for(int i=0;i<8;i++)
    {
        return_array[i] = (SDL_Point){ adj[i].x+base.x, adj[i].y+base.y };
        //printf("Adjacent elements:(%d,%d)\n",return_array[i].x,return_array[i].y);
    }
    return return_array;
}

Node *ReversePath(Node *path)
{
    if(path == NULL)
    {
        printf("cannot reverse path, returned node is NULL\n");
        return NULL;
    }
    while (path!=NULL)
    {
        
    }
    return NULL;
}

void PrintPath(Node *path) {
    if (path == NULL) {
        printf("Cannot print path, node is null\n");
        return;
    }

    // Count the number of nodes in the path
    int count = 0;
    Node *current = path;
    while (current) {
        count++;
        current = current->parent;
    }

    // Store nodes in an array
    SDL_Point *pathArray = malloc(sizeof(SDL_Point) * count);
    if (pathArray == NULL) {
        printf("Memory allocation failed for path array\n");
        return;
    }

    current = path;
    for (int i = count - 1; i >= 0; i--) {
        pathArray[i] = current->data;
        current = current->parent;
    }

    // Print the path in forward order
    printf("Path from base to target:\n");
    for (int i = 0; i < count; i++) {
        printf("Node: (%d,%d)\n", pathArray[i].x, pathArray[i].y);
    }

    free(pathArray);
}




Queue *BSF(Game *game, Player *player)
{
    /*get player position  
    get target position 
    make a adjacency array to loop and find the adjacent nodes 
    put adjacent nodes in the queue and add them to the hashtable
    dont add a node in the hashtable to the queue 
    keep adding the nodes until you reach the target node. 
    */
    SDL_Point base = (SDL_Point){player->rect.x,player->rect.y};
    SDL_Point target = player->target;
    printf("Target: (%d,%d)\n",target.x,target.y);
    printf("Base: (%d,%d)\n",base.x,base.y);
    int max_iteration = 10000;
    int iteration = 0;

    Queue *que = CreateQueue();
    HashTable *htable = CreateHashTable();
    
    Enqueue(que, base,NULL);

    while(que->last != NULL && iteration<max_iteration)
    {    
        iteration++;

        SDL_Point *adj = FindAdjacent(que->first->data);
        for (int i = 0; i < 8; i++) {
            if (CheckAndAddToHashTable(htable, adj[i])) {
                Node *node = Enqueue(que, adj[i], que->first);
                printf("New Queue Element: (%d,%d)\n",node->data.x,node->data.y);
                if (adj[i].x == target.x && adj[i].y == target.y) 
                {
                    free(adj);
                    FreeHashTable(htable);
                    return que;
                }
            }
        }
        free(adj);
        Dequeue(que);
        if(iteration>=max_iteration)
        {
            printf("Exceeded Max Iteration Count\n");
            break;
        }

    }
    FreeQueue(que);
    FreeHashTable(htable);
    return NULL;
}