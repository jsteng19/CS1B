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
                    while(!isEmpty(operatorStack)) {

                        struct tokenNode* n = pop(operatorStack);
                        if(n->data == '(') {
                            break;
                        }
                        enqueue(outputQueue, n);
                    }

                    if(isEmpty(operatorStack)) {
                        printf("mismatching parenthesis");
                        return 0; // no matching left parenthesis, invalid expression
                    }

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

    return evaluate(outputQueue);   
}

int evaluate(struct deque* outputQueue) {
    struct deque* evalStack = newDeque(); 
    
    struct tokenNode* token;

    while(!isEmpty(outputQueue)) {
        token = pop(outputQueue);
        
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
            else {
                printf("invalid input");
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

    return 0;

}