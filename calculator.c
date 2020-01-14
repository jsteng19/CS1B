#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_SIZE 1000

struct stack {
    int topIndex;
    int* arr;
};

struct stack* newStack() { 
    struct stack* stack = (struct stack*)malloc(sizeof(struct stack)); 
    stack->topIndex = -1; 
    stack->arr = (int*)malloc(MAX_SIZE * sizeof(int)); 
    return stack; 
} 
  
int isStackFull(struct stack* stack) { 
    return stack->topIndex == MAX_SIZE - 1; 
} 
  
int isStackEmpty(struct stack* stack) { 
    return stack->topIndex == -1; 
} 
  
void push(struct stack* stack, int new) { 
    if (isStackFull(stack)) {
        printf("stack is full");
        return; 
    }
    
    stack->topIndex++;
    stack->arr[stack->topIndex] = new; 
    printf("%d pushed to stack\n", new); 

} 
  

int pop(struct stack* stack) { 
    if (isStackEmpty(stack)) {
        printf("stack is empty\n"); 
        return 'E'; 
    }
    stack->topIndex--;
    return stack->arr[stack->topIndex + 1]; 
    
} 

struct queue {
    int topIndex;
    int bottomIndex;
    int* arr;
};

struct queue* newQueue() { 
    struct queue* queue = (struct queue*)malloc(sizeof(struct queue)); 
    queue->topIndex = 0; 
    queue->bottomIndex = 0;
    queue->arr = (int*)malloc(MAX_SIZE * sizeof(int)); 
    return queue; 
} 

int isQueueFull(struct queue* queue) {  // not a good queue, it can be full even if nothing is queued
    return queue->topIndex == MAX_SIZE - 1; 
} 
  
int isQueueEmpty(struct queue* queue) { 
    return queue->topIndex == queue->bottomIndex; 
} 
  
void enqueue(struct queue* queue, int new) { 
    if (isQueueFull(queue)) {
        printf("queue is full");
        return; 
    }
    
    queue->arr[queue->topIndex] = new; 
    queue->topIndex++;
    printf("%d added to queue\n", new); 
} 
  
int dequeue(struct queue* queue) { 
    if (isQueueEmpty(queue)) {
        printf("queue is empty\n"); 
        return 'E'; 
    }
    queue->bottomIndex++;
    return queue->arr[queue->bottomIndex-1];
    
} 

int evaluate(struct queue* outputQueue) {
    struct stack* evalStack = newStack(); 
    
    int c;

    while(!isQueueEmpty(outputQueue)) {
        c = dequeue(outputQueue);
        printf("%d", c);

        if(c == '+'){
            push(evalStack, pop(evalStack) + pop(evalStack));
        }
        else if(c == '-'){
            push(evalStack, pop(evalStack) - pop(evalStack));
        }
        else if(c == 'x' || c == '*'){
            push(evalStack, pop(evalStack) * pop(evalStack));
        }
        else {
            push(evalStack, c);
        } 
    }
    
    return pop(evalStack);
}

int calculate(char* expression) {
    struct stack* operatorStack = newStack(); 
    struct queue* outputQueue = newQueue();

    while(*expression != '\0') {
        // printf("%d", *expression);
        if(*expression != ' ') {
            if(isdigit(*expression)) {
                int num = atoi(expression); // atoi converts until it finds a non-digit symbol
                while(isdigit(*expression)) {
                    expression++;
                }
                enqueue(outputQueue, num);
                //enqueue(outputQueue, *expression);
            }
            else {
                push(operatorStack, *expression);
                expression++;
            }
        }
        else {
            expression++;
        }
        
    }

    while(!isStackEmpty(operatorStack)) {
        enqueue(outputQueue, pop(operatorStack));
    }


    printf("end of parsing\n");
    return evaluate(outputQueue);
    
}

int main() {
    char* input;
    while(strcmp(input, "exit") != 0) {
        fgets(input, 1000, stdin);
        strtok(input, "\n"); // removes trailing \n 
        printf("\n%d\n", calculate(input));
    }

    // struct stack* s = newStack(); 

    // push(s, '1');
    // push(s, 'A');
    // push(s, 'B');
    // push(s, 'C');

    // printf("popped %c \n", pop(s));
    // printf("popped %c \n", pop(s));
    // printf("popped %c \n", pop(s));
    // printf("popped %c \n", pop(s));
    // printf("popped %c \n", pop(s));

    // struct queue* q = newQueue(); 
    // enqueue(q, '1');
    // enqueue(q, 'A');
    // enqueue(q, 'B');
    // enqueue(q, 'C');

    // printf("dequeued %c \n", dequeue(q));
    // printf("dequeued %c \n", dequeue(q));
    // printf("dequeued %c \n", dequeue(q));
    // printf("dequeued %c \n", dequeue(q));
    // printf("dequeued %c \n", dequeue(q));
    // printf("dequeued %c \n", dequeue(q));

    

    return 0;
}