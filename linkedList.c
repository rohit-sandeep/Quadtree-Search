#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "linkedList.h"

//linkedList.c is adapted from Assignment 1 by Rohit Sandeep Ambakkat 

//Creates an empty list that consists of a head to which data is added
list_t *createList() {
    list_t *list;
    list=malloc(sizeof(list_t));
    assert(list);

    list->head=NULL;
    list->size=0;

    return list;
}

//Creates an empty node to which data and other nodes can be added
node *createnode() {
    node *buffer = malloc(sizeof(node));
    assert(buffer);

    buffer->next= NULL;

    return buffer;
}

//Adds data to the linked list and returns the updated head node of the list
node *addData(node *head, location_t *data) {
    node *buffer, *iteration;
    buffer= createnode();
    buffer->data=data;
    //Check whether the list is empty and if it is then the data passed through will become the first element of the list
    if(head == NULL) {
        head = buffer;
    }
    else{
        iteration=head;
        //Iterate until the end of the list is found and append new node 
        while(iteration->next != NULL) {
            iteration=iteration->next;
        }
        iteration->next=buffer;
    }
    return head;
}

//Check linkedlist for specific data inputs from stdin in a linear manner
void searchData(node *head, FILE *outFile) {
    char search[MAX_STRING_LEN+1];

    while(scanf(" %[^\n]", search) == 1) {
        fprintf(outFile,"%s\n", search);
        node *current=head;
        int n=0;

        while(current != NULL) {
            if(strcmp(search, current->data->address)==0) {
                printData(current->data, outFile);
                n+=1;
            }
            current=current->next;
        }

        if(n > 0) {
            printf("%s --> %d\n", search, n);
            
        }
        else {
            printf("%s --> NOTFOUND\n", search);

        }
    }

}

//Function that frees all elements of the linked list
void freeList(node *head) {
    node *prev;
    while(head != NULL) {
        prev=head;
        head=head->next;
        dataFree(prev->data);
        free(prev);
    
    }
}

//Sort the element of a linked list 
//Adapted from https://www.javatpoint.com/program-to-sort-the-elements-of-the-singly-linked-list
list_t *sortList(list_t *list){
    node *curr=NULL;
    node *temp=NULL;
    location_t *tempdata;
    curr=list->head;
    
    while(curr != NULL){
        temp=curr->next;
        while(temp != NULL) {
            if(curr->data->footpath_id > temp->data->footpath_id) {
                tempdata=curr->data;
                curr->data=temp->data;
                temp->data=tempdata;
            }
            temp=temp->next;
        }
        curr=curr->next;
    }
    return list;
}


