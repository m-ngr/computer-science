/*
    POSTFIX COMPILER EVALUATION CODED BY MAHMOUD ELNAGAR
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
//nodes for stack
typedef struct node{
    int data;
    struct node *next;
}node;
//view stack top node
int stackTop(node *stack){
    if (stack !=NULL){
        return stack->data;
    }
}
//pop stack top node
int pop(node **stack){
    if (*stack != NULL){
        node *tmp = *stack;
        int value = tmp->data;
        *stack = (*stack)->next;
        free(tmp);
        return value;
    }
}
//push a value to the stack
void push(node **stack, int value){
    node* newNode = malloc(sizeof(node));
    if (newNode == NULL){return;}
    newNode->data = value;
    newNode->next = *stack;
    *stack = newNode;
}
//check operators precedence
int precedence(char operator1, char operator2){
    //rank operator 1
    switch(operator1){
    case '+':
    case '-':
        operator1 = 0;
        break;
    case '*':
    case '/':
    case '%':
        operator1 = 1;
        break;
    case '^':
        operator1 = 2;
        break;
    default:
        return -1;
        break;
    }
    //rank operator 2
    switch(operator2){
    case '+':
    case '-':
        operator2 = 0;
        break;
    case '*':
    case '/':
    case '%':
        operator2 = 1;
        break;
    case '^':
        operator2 = 2;
        break;
    default:
        return -1;
        break;
    }
    if (operator1 == operator2){return 0;}
    if (operator1 > operator2){return 1;}
    if (operator1 < operator2){return -1;}
}

bool isOperator(char c){
    switch(c){
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '%':
        return true;
        break;
    default:
        return false;
        break;
    }
}
//perform operations
int calculate(int op1, int op2, char ops){
    if (ops == '+'){ return op1 + op2;}
    if (ops == '-'){ return op1 - op2;}
    if (ops == '*'){ return op1 * op2;}
    if (ops == '/'){ return op1 / op2;}
    if (ops == '%'){ return op1 % op2;}
    if (ops == '^'){
        int result = 1;
        for (int i = 0 ; i < op2 ; i++){
            result *= op1;
        }
        return result;
    }
}
//convert expression from infix to postfix form
void convertToPostfix(char *infix, char *postfix){
    //define new stack
    node* stack = NULL;
    push(&stack, '(');
    //append ")" to the end of infix to indicate the end of conversion process
    char append[2] = ")";
    strcat(infix, append);
    //process the conversion
    while (stack != NULL){
        //if infix char is digit then copy it to postfix
        if (isdigit(*infix)){
            *postfix = *infix;
            postfix++;
        }
        //if infix char in '(' then push it to stack
        if ('(' == *infix){
            push(&stack, '(');
        }
        //if infix char in ')' then...
        if (')' == *infix){
            //set marker ',' to postfix
            *postfix = ',';
            postfix++;
            //copy operators from stack to postfix until reach '('
            while (stackTop(stack) != '('){
                *postfix = pop(&stack);
                postfix++;
            }
            //delete '(' from stack
            pop(&stack);
        }
        //if infix char is an operator then...
        if (isOperator(*infix)){
            //set marker ',' to postfix
            *postfix = ',';
            postfix++;
            //copy operators of high (1) or equal (0) precedence from stack to postfix (if exist)
            while(precedence(stackTop(stack), *infix) != -1){
                *postfix = pop(&stack);
                postfix++;
            }
            //push operator from infix to stack
            push(&stack, *infix);
        }
        //get next char from infix expression
        infix++;
    }
    //end marker
    *postfix = '\0';
}

int evaluatePostfix(char *postfix){
    //create empty stack
    node *stack = NULL;
    //process the postfix evaluation
    while (*postfix != '\0'){
        // if postfix char is digit then read the complete number and push it to stack
        if (isdigit(*postfix)){
            int number = 0;
            //read as a single number until the marker ','
            while(*postfix != ','){
                number *= 10;
                number += *postfix - '0';
                postfix++;
            }
            push(&stack, number);
        }
        //if postfix char is an operator then...
        if (isOperator(*postfix)){
            //pop the two numbers from stack (in reverse) and perform calculation
            int op1,op2;
            op2 = pop(&stack);
            op1 = pop(&stack);
            //push the result of the calculation to stack
            push(&stack, calculate(op1, op2, *postfix));
        }
        //get next char from postfix expression
        postfix++;
    }
    //return result
    return pop(&stack);
}

int main(void){
    char infix[50] = {"(60 + 20) * 50 - 80 / 40 + 2"};// 4000
    char postfix[50];
    printf("Infix: %s\n", infix);
    convertToPostfix(infix,postfix);
    printf("Postfix: %s\n", postfix);
    printf("Evaluation: %d\n", evaluatePostfix(postfix));
}
