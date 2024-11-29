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
    unsigned int original_index = index; 
    int attempt = 0;

    while (htable->table[index].x != -1 || htable->table[index].y != -1) {
        if (htable->table[index].x == point.x && htable->table[index].y == point.y) {
            return 0;
        }

        // Linear probing
        index = (index + 1) % htable->size;

        if (index == original_index) {
            printf("Hash table is full, cannot add point (%d, %d)\n", point.x, point.y);
            return -1; 
        }

        attempt++;
    }

    
    htable->table[index] = point;
    return 1;
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

void PrintPath(Node *path) {
    if (path == NULL) {
        printf("Cannot print path, node is null\n");
        return;
    }

    printf("Path from base to target:\n");
    Node *current = path;
    while (current) {
        printf("Node: (%d,%d)\n", current->data.x, current->data.y);
        current = current->parent;
    }
}





Queue *BFS(Game *game, Player *player)
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
    int max_iteration = SIZE;
    int iteration = 0;

    Queue *que = CreateQueue();
    HashTable *htable = CreateHashTable();
    
    Enqueue(que, base,NULL);
    CheckAndAddToHashTable(htable, base);
    while(que->last != NULL && iteration<max_iteration)
    {    
        iteration++;

        SDL_Point *adj = FindAdjacent(que->first->data);
        for (int i = 0; i < 8; i++) 
        {
            if (CheckAndAddToHashTable(htable, adj[i])) 
            {
                Enqueue(que, adj[i], que->first);
                printf("New Queue Element: (%d,%d)\n",adj[i].x,adj[i].y);
                if (adj[i].x == target.x && adj[i].y == target.y) 
                {
                    printf("Found target at: (%d,%d)\n",adj[i].x,adj[i].y);
                    free(adj);
                    FreeHashTable(htable);
                    return que;
                }
            }
            if (CheckAndAddToHashTable(htable, adj[i]) == -1)
            {
                FreeQueue(que);
                FreeHashTable(htable);
                return NULL;
                break;
            }
        }
        free(adj);
        //printf("(%d,%d)\n",que->first->data.x,que->first->data.y);
        Dequeue(que);
        if(iteration>=max_iteration)
        {
            printf("Exceeded Max Iteration Count\n");
            FreeQueue(que);
            FreeHashTable(htable);
            return NULL;
            break;
        }

    }
    FreeQueue(que);
    FreeHashTable(htable);
    return NULL;
}