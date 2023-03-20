#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

//linkedList.h is adapted from Assignment 1 by Rohit Sandeep Ambakkat

#include "data.h"

typedef struct list node;
typedef struct linkedlist list_t;

struct list{
    location_t *data;
    node *next;

};


struct linkedlist{
    node *head;
    int size;
};


list_t *createList();

node *createnode();

node *addData(node *head, location_t *data);

void searchData(node *head, FILE *outFile);

void freeList(node *head);

list_t *sortList(list_t *list);
#endif
