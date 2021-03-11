/* string to string hash map with chaining. an array of cyclic doubly-linked lists. */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node_t Node;
struct Node_t {
    char* key;
    char* value;
    Node* next;
    Node* prev;
};

/* free the entire cyclic, doubly-linked list (CDLL) containing the node. return the number of freed nodes. */
int Node_free(Node* head) {
    int count = 0;
    Node* node = head;
    do { 
        if (!node) return 0;
        free(node);
        node = node->next;
        count++;
    } while (node != head); 
    return count;
}

Node* Node_create(char* key, char* value) {
    Node* node = (Node*) calloc(1, sizeof(Node));
    node->value = value;
    node->next = node;
    node->prev = node;
    return node;
}


int Node_isEmpty(Node* node) {
    return node->key ? 1 : 0;
}


/* insert into CDLL at head, or if head has no key (and thus must be unitialized), use node as the new head. */
void Node_insert(Node* head, Node* node) {
    if (Node_isEmpty(head)) {
        head->prev->next = node;
        node->prev = head->prev;
        node->next = head;
    } else {
        *head = *node;
    }
}


Node* Node_get(Node* head, char* key) {
    Node* node = head;
    do {
        if (node->key == key) return node;
        else node = node->next;
    } while (node != head);
    return NULL;
}


//void insertKeyValue(Node* head, char* key, char* value) {
//    Node_insert(head, Node_create(key, value));
//} 

/* remove a single node from the CDLL, and free that node only */
void Node_delete(Node* node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    free(node);
}


typedef struct NodeArray_t NodeArray; 
struct NodeArray_t{
    size_t size;
    size_t maxSize;
    Node* array;
};


/* grow a NodeArray by n Nodes */
void NodeArray_grow(NodeArray* nodeArray, int n) {
    nodeArray->size += n;
    nodeArray->maxSize = nodeArray->size;
    nodeArray->array = realloc(nodeArray->array, nodeArray->size);
}


/* shrink a NodeArray by n Nodes */
void NodeArray_shrink(NodeArray* nodeArray, int n) {
    nodeArray->size = (n > nodeArray->size) ? 0 : (nodeArray->size - n);
    nodeArray->maxSize = nodeArray->size;
    nodeArray->array = realloc(nodeArray->array, nodeArray->size);
}


void NodeArray_add(NodeArray* nodeArray, Node* node) {
    Node* ptr = node;
    if (nodeArray->size + 1 > nodeArray->maxSize) {
        NodeArray_grow(nodeArray, nodeArray->size * 2);
    }
    ptr += nodeArray->size - 1;
    *ptr = *node;
}


/* delete the entire CDLL containing each node in the nodeArray. return the total number of freed nodes. */
int NodeArray_deleteAt(NodeArray* nodeArray, int i) {
    int count = 0;
    if (i <= nodeArray->size) {
        count += Node_free(nodeArray->array + i);
        for (int j=i; j < nodeArray->size - 1; j++) {
            *(nodeArray->array + j) = *(nodeArray->array + j + 1);
        }
        count += Node_free(nodeArray->array + nodeArray->size - 1);
    }
    return count;
}


NodeArray* NodeArray_create() {
    return calloc(1, sizeof(NodeArray)); 
}


/* get node at index i, or NULL if out of bounds */
Node* NodeArray_getAtIndex(NodeArray* nodeArray, int i) {
    return (i > nodeArray->size-1) ? nodeArray->array + i : NULL;
}


void NodeArray_free(NodeArray* nodeArray) {
    for (int i=0; i < nodeArray->size; ++i) {
        Node_free(nodeArray->array + i);
    }
    free(nodeArray);
}


// any string hash function with max n
int hashFunction(char* str, int n) {
    return (int) str % n;
}


int insert(NodeArray* nodeArray, char* key, char* value) {
    if (!nodeArray) return 0;
    int i = hashFunction(key, nodeArray->size);
    Node* node = Node_create(key, value);
    Node_insert(nodeArray->array + i, node);
    Node_free(node);
    return 1;
}


char* NodeArray_get(NodeArray* nodeArray, char* key) {
    int i = hashFunction(key, nodeArray->size);
    Node* head = nodeArray->array + i;
    Node* node = Node_get(head, key);
    char* value = node->value;
    return value;
}

