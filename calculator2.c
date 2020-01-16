#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "deque.c"

int evaluate(struct deque* outputQueue); 
int calculate(char* expression);
bool comparePrecedence(int t1, int t2);

int calculate(char* expression) {
    struct deque* operatorStack = newDeque(); 
    struct deque* outputQueue = newDeque();

    while(*expression != '\0') {
        // printf("%d", *expression);
        if(*expression != ' ') {
            if(isdigit(*expression)) {
                int num = atoi(expression); // atoi converts until it finds a non-digit symbol
                while(isdigit(*expression)) {
                    expression++;
                }
                enqueue(outputQueue, newTokenNode(num, false));
            }
            else {
                if(*expression == '(') {
                    push(operatorStack, newTokenNode(*expression, true));
                }
                else if(*expression == ')') {
                    while(!isEmpty(operatorStack) && peek(operatorStack)->data != '(') {
                        enqueue(outputQueue, pop(operatorStack));
                    }

                    if(isEmpty(operatorStack)) {
                        printf("mismatching parenthesis");
                        return 0; // no matching left parenthesis, invalid expression
                    }

                    pop(operatorStack); // discard both parenthesis
                    //printf("after pop: %d\n", peek(operatorStack)->data);
                }

                else { // assume token is valid operator
                    while(!isEmpty(operatorStack) && comparePrecedence(peek(operatorStack)->data, *expression) && peek(operatorStack)->data != '(') {
                        enqueue(outputQueue, pop(operatorStack));
                    }
                    push(operatorStack, newTokenNode(*expression, true));
                }

                expression++;

            }
        }
        else {
            expression++;
        }     
    }

    while(!isEmpty(operatorStack)) {
        enqueue(outputQueue, pop(operatorStack));
    }
    
    printDeque(outputQueue);
    printf("end of parsing\n");
    return evaluate(outputQueue);   
}

int evaluate(struct deque* outputQueue) {
    struct deque* evalStack = newDeque(); 
    
    struct tokenNode* token;

    while(!isEmpty(outputQueue)) {
        token = pop(outputQueue);
        
        //printf("%d", c);
        if(token->isOperator) {
            int data = token->data;
            int operand2 = pop(evalStack)->data;
            int operand1 = pop(evalStack)->data;
            int res;
            if(data == '+'){
                res = operand1 + operand2;
            }
            else if(data == '-'){
                res = operand1 - operand2;
            }
            else if(data == '*'){
                res = operand1 * operand2;
            }
            else { // symbol not recognized
                return 0;
            }
            push(evalStack, newTokenNode(res, false));
        }

        else {
            push(evalStack, token);
        } 
        
    }
    
    return pop(evalStack)->data;
}

bool comparePrecedence(int t1, int t2) { //return true if t1 greater or equal precedence than t2
    if(t1 == t2 || t1 == '*') {
        return true;
    }
    if(t1 == '+' || t1 == '-') {
        if(t2 == '*') {
            return false;
        }
    }
    return true;
}


int main() {
    char* input;
    while(strcmp(input, "exit") != 0) {
        fgets(input, 1000, stdin);
        strtok(input, "\n"); // removes trailing \n 
        printf("\n%d\n", calculate(input));
    }

    // struct deque* d = newDeque();
    // push(d, newTokenNode('(', true));
    // push(d, newTokenNode('(', true));

    // printf("prev: %p\n", peek(d)->prev);
    // printf("prev data: %d\n", peek(d)->prev->data);
    // printf("%d\n", pop(d)->data);
    
    // printf("%d\n", pop(d)->data);
    // return 0;

}