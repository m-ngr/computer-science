/*
    Binary Tree Printer Coded By Mahmoud Elnagar
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//tree definition
typedef struct tree{
    int data;
    struct tree *left;
    struct tree *right;
}tree;
//list node definition
typedef struct node{
    int data;
    struct node *next;
}node;
//insert a number to tree , No duplication
void insertTree(tree **root, int value){
    //if free space found then set new node at it
    if (*root == NULL){
        *root = malloc(sizeof(tree));
        if (*root == NULL){return;}
        (*root)->data = value;
        (*root)->left = NULL;
        (*root)->right = NULL;
    }else{
    //find path for valid free space
        if ((*root)->data > value){
            insertTree(&((*root)->left), value);
        }
        if((*root)->data < value){
            insertTree(&((*root)->right), value);
        }
    }
}

int depth(tree *root){
    //if empty tree return 0
    if(root == NULL){return 0;}
    //if only one node and no child then return 1
    if (root->left == NULL && root->right == NULL){return 1;}
    //get right and left child depth
    int left = depth(root->left);
    int right = depth (root->right);
    //return the largest child depth + 1 (current level)
    if (left > right){
        return left + 1;
    }else{
        return right + 1;
    }
}

tree *searchTree(tree *root, int value){
    //if tree is empty return NULL
    if (root == NULL){return NULL;}
    //if value if found return node
    if (root->data == value){return root;}
    //search for value in left child
    tree *result = searchTree(root->left, value);
    //if not found in left child, search in right child
    if (result == NULL){result = searchTree(root->right, value);}
    //return final result
    return result;
}
//destroy a root and its childs
void destroyTree(tree *root){
    if (root == NULL){return;}
    destroyTree(root->left);
    destroyTree(root->right);
    free(root);
}
//delete the root of a specific value
void deleteTree(tree **root, int value){
    if (*root == NULL){return;}
    if ((*root)->data == value){
        *root = NULL;
        destroyTree(*root);
    }else{
        deleteTree(&((*root)->left), value);
        deleteTree(&((*root)->right), value);
    }
}
//return length of integer number
int intLenght(int number){
    int length = 1;
    while(number > 9){
        number /= 10;
        length++;
    }
    return length;
}
//delete node from List
void deleteNode(node **head, int value){
    if (*head == NULL){return;}
    //if head = value then delete head and set head pointer to next node
    if ((*head)->data == value){
        node *tmp = *head;
        *head = (*head)->next;
        free(tmp);
        return;
    }
    //walk through the list without changing the head pointer
    node *current = *head;
    while(current->next != NULL){
        //if next node data = value then delete next node
        if (current->next->data == value){
            node *tmp = current->next;
            current->next = current->next->next;
            free(tmp);
            return;
        }
        current = current->next;
    }
}
//insert node to list
void insertNode(node **head, int value){
    //create new node
    node* newNode = malloc(sizeof(node));
    if (newNode == NULL){return;}
    newNode->data = value;
    newNode->next = NULL;
    //if List is empty set pointer to the new Node
    if (*head == NULL){*head = newNode;return;}
    //else find the last node of the list without changing the pointer head
    node *currentNode = *head;
    while(currentNode->next != NULL){
        currentNode = currentNode->next;
    }
    //attach new node to the last node
    currentNode->next = newNode;
}
//check if value is in the list
int inList(node *currentNode, int value){
    while (currentNode != NULL){
        if (value == currentNode->data){
            return 1;//found
        }
        currentNode = currentNode->next;
    }
    return 0;//not found
}

void printList(node *currentNode){
    while(currentNode != NULL){
        printf("%d --> ", currentNode->data);
        currentNode = currentNode->next;
    }
    puts("NULL");
}

void printTree(tree *root, node **list, int space){
    if (root != NULL){
        //print the right child
        printTree(root->right, list, space + 3 + intLenght(root->data));
        //print space or link
        for (int i = 0 ; i < space ; i++){
            if (inList(*list, i)){
                printf("%c", '|');
            }else{
                printf("%c", ' ');
            }
        }
        //print current value
        printf("%d", root->data);
        //print branch if root have child
        if (root->left != NULL || root->right != NULL){
            printf("%s", "---|");
        }
        //start/stop printing vertical lines
        if (inList(*list, space)){
            deleteNode(list, space);
        }else{
           if (space != 0){insertNode(list, space);}
        }
        //new line
        puts("");
        //print the left child
        printTree(root->left, list, space + 3 + intLenght(root->data));
    }else{
        //start/stop printing vertical lines
        if (inList(*list, space)){
            deleteNode(list, space);
        }else{
           if (space != 0){insertNode(list, space);}
        }
    }
}

int main(void){
    srand(time(NULL));
    tree *root = NULL;
    node *head = NULL;
    //insert pre-designed tree
    insertTree(&root, 50);
    insertTree(&root, 20);
    insertTree(&root, 30);
    insertTree(&root, 40);
    insertTree(&root, 70);
    insertTree(&root, 80);
    insertTree(&root, 60);
    insertTree(&root, 10);
    insertTree(&root, 20);
    insertTree(&root, 00);
    insertTree(&root, 90);
    //print pre-designed tree
    puts("Pre-designed Tree:");
    printf("depth = %d\nGraph:\n", depth(root));
    printTree(root, &head, 0);
    //insert random tree
    root = NULL;
    head = NULL;
    for(int i = 1 ; i <= 20 ; i++){
        insertTree(&root, rand()%200);
    }
    //print random tree
    puts("RANDOM Tree:");
    printf("depth = %d\nGraph:\n", depth(root));
    printTree(root, &head, 0);
}
