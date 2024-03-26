//Dante Villarreal
//ID: 1001580411

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>



// Node for the doubly linked list
typedef struct Node 
{
    int key;
    int value;
    struct Node* next;
    struct Node* prev;
} Node;

// Hash table
typedef struct HashTable 
{
    int size;
    int count;
    Node** items;
} HashTable;

//prototype for rehash fn
void rehash(HashTable* hashTable, int newSize);


Node* createNode(int key, int value) 
{
    //allocate memory for new node
    Node* node = (Node*) malloc(sizeof(Node));
    //set key and value
    node->key = key;
    node->value = value;
    
    node->next = NULL;
    node->prev = NULL;
    printf("Node created with key=%d, value=%d\n", key, value);
    return node;
}

HashTable* createHashTable(int size) 
{
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    hashTable->size = size;
    hashTable->count = 0;
    //calloc because need to initialize memory to zero
    //      so I can check if a particular index is empty
    //      by comparing it to NULL.
    hashTable->items = (Node**) calloc(hashTable->size, sizeof(Node*));
    printf("\n\nHash table created with size=%d\n", size);
    return hashTable;
}

int hashFunction(int key, int size) 
{
    int method = rand() % 2; // Randomly choose 0 or 1
    int A = 5;
    int hashIndex;

    if (method == 0) {
        // Multiplication method
        hashIndex = (key * A) % size;
    } else {
        // Division method
        hashIndex = key % size;
    }

    printf("Hash function called with key=%d, size=%d, method=%d, hashIndex=%d\n", key, size, method, hashIndex);
    return hashIndex;
}

void insert(HashTable* hashTable, int key, int value) 
{
    //compute has of key
    int index = hashFunction(key, hashTable->size);
    //create new node
    Node* node = createNode(key, value);
    if (!hashTable->items[index]) 
    {
        //if spot empty, insert node
        hashTable->items[index] = node;
    } 
    else 
    {
        //else append it to end of spot's linked list
        Node* temp = hashTable->items[index];
        while (temp->next) 
        {
            temp = temp->next;
        }
        temp->next = node;
        node->prev = temp;
    }
    hashTable->count++;
    printf("Inserted key=%d, value=%d at index=%d, new count=%d\n", key, value, index, hashTable->count);
    if (hashTable->count == hashTable->size) 
    {
        printf("Hash table full, rehashing...\n");
        rehash(hashTable, hashTable->size * 2);
    }
}

void rehash(HashTable* hashTable, int newSize) 
{
    printf("\n\nRehashing started, old size=%d, new size=%d\n", hashTable->size, newSize);
    //calloc because need to initialize memory to zero
    //      so I can check if a particular index is empty
    //      by comparing it to NULL.
    //new items are pointer to a pointer to a Node.
    Node** newItems = (Node**) calloc(newSize, sizeof(Node*));
    int oldSize = hashTable->size;
    Node** oldItems = hashTable->items;
    hashTable->size = newSize;
    hashTable->items = newItems;
    hashTable->count = 0;
    for (int i = 0; i < oldSize; i++) 
    {
        Node* node = oldItems[i];
        while (node) 
        {
            insert(hashTable, node->key, node->value);
            Node* oldNode = node;
            node = node->next;
            free(oldNode);
        }
    }
    free(oldItems);
    printf("Rehashing completed, new size=%d\n", hashTable->size);
}
void delete(HashTable* hashTable, int key) 
{
    //compute hash
    int index = hashFunction(key, hashTable->size);
    Node* node = hashTable->items[index];
    if (node) 
    {
        //checks of key of current node matches key want to delete
        if (node->key == key) 
        {
            if (node->next) 
            {
                //removes current node from list
                node->next->prev = NULL;
                hashTable->items[index] = node->next;
            }
            else 
            {
                //updates items array of hash table to point to next node
                //. basically removes current node from hash table
                hashTable->items[index] = NULL;
            }
            free(node);
            hashTable->count--;
            printf("Deleted key=%d, new count=%d\n", key, hashTable->count);

            if (hashTable->count == hashTable->size / 4) 
            {
                printf("Hash table 1/4 full, rehashing...\n");
                rehash(hashTable, hashTable->size / 2);
            }
        } 
        else 
        {
            while (node->next) 
            {
                if (node->next->key == key) 
                {
                    Node* temp = node->next;
                    node->next = temp->next;
                    if (temp->next) 
                    {
                        temp->next->prev = node;
                    }
                    free(temp);
                    hashTable->count--;
                    printf("Deleted key=%d, new count=%d\n", key, hashTable->count);
                    if (hashTable->count == hashTable->size / 4) 
                    {
                        printf("Hash table 1/4 full, rehashing...\n");
                        rehash(hashTable, hashTable->size / 2);
                    }
                    break;
                }
                node = node->next;
            }
        }
    }
}


int main() {
    srand(time(NULL)); // Seed for the random number generator
    HashTable* hashTable = createHashTable(10);
    
    // rehash when 1/4 empty
		//insert(hashTable, key, value)
        insert(hashTable, 1, 10);
        insert(hashTable, 2, 21);
        insert(hashTable, 3, 29);
        delete(hashTable, 2);
    
    //this will rehash into double the size
	    HashTable* hashTable1 = createHashTable(10);
	    for (int i = 0; i < 15; i++) {
	        insert(hashTable1, i, i * 3);
	    }
    return 0;
}
