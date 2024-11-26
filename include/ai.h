#ifndef AI_H
#define AI_H

typedef struct Game Game;
typedef struct Player Player;
typedef struct Node Node;

#define SIZE (1980*1080)

typedef struct Node
{
    SDL_Point data;
    Node *next;
    Node *parent;
}Node;

typedef struct Queue
{
    Node *first;
    Node *last;
}Queue;

typedef struct HashTable
{
    SDL_Point *table;
    int size;
}HashTable;

Queue *CreateQueue();
Node *Enqueue(Queue *q, SDL_Point a, Node *parent);
SDL_Point Dequeue(Queue* q);
void FreeQueue(Queue *que);

unsigned int Hash(SDL_Point point);
HashTable *CreateHashTable();
int CheckAndAddToHashTable(HashTable *htable, SDL_Point point);
void FreeHashTable(HashTable *htable);

int distance_btw_points(SDL_Point a, SDL_Point b);
Node *ReversePath(Node *path);
void PrintPath(Node *base);
Queue *BSF(Game *game, Player *player);


#endif