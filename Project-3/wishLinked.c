//LinkedList implementation for wish shell
#include <stdlib.h>
#include <string.h>
#include "wish.h"

//add value to end of list
Node_t* addToList(Node_t* head, char* value){
    Node_t* newNode = (Node_t*)malloc(sizeof(Node_t));
    newNode->value = value;
    newNode->next = NULL;
    if(head == NULL){
        head = newNode;
    } else {
        Node_t* current = head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = newNode;
    }
    return head;
}

//get value at index
char* getValue(Node_t* head, int index){
    Node_t* current = head;
    for(int i=0; i<index; i++){
        current = current->next;
        if(current == NULL){
            return NULL;
        }

    }
    return current->value;
}

//form array of strings from linked list for n tokens
void getArray(Node_t* head, char* argv[], int index){
    Node_t* current = head;
    for(int i=0; i<index; i++){
        if(current == NULL) return;
        
        argv[i] = current->value;
        current = current->next;
    }
    return;
}


//remove value from list
void removeFromList(Node_t* head, char* value){
    Node_t* current = head;
    Node_t* previous = NULL;
    while(current != NULL){
        if(strcmp(current->value, value) == 0){
            if(previous == NULL){
                head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

//free list memory
Node_t* freeList(Node_t* head, Node_t* previous){
    Node_t* current = head;

    if(previous != NULL){
        previous->next = NULL;
    }
    
    while(current != NULL){
        Node_t* temp = current;
        current = current->next;
        free(temp);
    }
    return NULL;
}