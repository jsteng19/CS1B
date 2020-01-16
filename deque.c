#include <stdlib.h>
#include <stdbool.h>


struct tokenNode {
    int data;
    bool isOperator; 
    struct tokenNode *next;
    struct tokenNode *prev;
};

struct tokenNode* newTokenNode(int data, bool isOperator) {
    struct tokenNode* n = (struct tokenNode*) malloc(sizeof(struct tokenNode));
    n->data = data;
    n->isOperator = isOperator;
    n->next = n->prev = NULL;

    return n;
}

struct deque {
    struct tokenNode * head;
    struct tokenNode * tail;
};

struct deque* newDeque() {
    struct deque* d = (struct deque*) malloc(sizeof(struct deque));
    d->head = d->tail = NULL;
    return d;
}

bool isEmpty(struct deque* d) {
    return(!(d->head && d->tail));
}

void push(struct deque* d, struct tokenNode* new) {

    if(d->tail) {
        d->tail->next = new;
        new->prev = d->tail;
    }
    if(!d->head) {
        d->head = new;
    }
    d->tail = new;
}

struct tokenNode* pop(struct deque* d) {
    if(isEmpty(d)) {
        printf("deque is empty\n");
        return newTokenNode('\0', false);
    }

    struct tokenNode* popped = d->tail;
    if(d->tail->prev) {
        d->tail = d->tail->prev;
    }
    else {
        d->head = d->tail = NULL;
    }
    
    return popped;
}

struct tokenNode* peek(struct deque* d) {
    if(isEmpty(d)) {
        return newTokenNode('\0', false);
    }

    return d->tail;
}

void enqueue(struct deque* d, struct tokenNode* new) {

    if(d->head) {
        d->head->prev = new;
        new->next = d->head;
    }
    if(!d->tail) {
        d->tail = new;
    }
    d->head = new;
}

void printDeque(struct deque* d) {
    if(d->head) {
        struct tokenNode* n = d->head;
        while(n) {
            printf("data: %d ",n->data);
            printf("is operator: %d\n", n->isOperator);
            n = n->next;
        }
    }

    printf("tail: %d", d->tail->data);
}