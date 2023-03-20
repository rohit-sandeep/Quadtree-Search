#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "data.h"
#include "linkedList.h"
#include <stdbool.h>

#define S_W 0
#define N_W 1
#define N_E 2
#define S_E 3

typedef struct point2D point;
typedef struct Rectangle boundary;
typedef struct quadTreeNode quadNode;
typedef struct dataPoint data;


struct point2D{
    long double x;
    long double y;

};

struct Rectangle{
    point *topRight;
    point *bottomLeft;
};

struct dataPoint{
    list_t *data_set;
    point *coordinates;

};

struct quadTreeNode{
    boundary *section;
    data *points;
    quadNode *NW;
    quadNode *NE;
    quadNode *SW;
    quadNode *SE;

};


quadNode *createQuadTree(long double top_right_x, long double top_right_y, long double bottom_left_x, long double bottom_left_y);

point *createPoint(long double x, long double y);

boundary *createBoundary(point *top_Right, point *bottom_Left);

bool quadTreeAdd(quadNode *tree, long double x, long double y, location_t *data);

int inRectangle(long double x, long double y, boundary *section);

data *createDataPoint(point *coordinates);

void subdivide(quadNode *tree); 

int determineQuadrant(boundary *bound, point *coordinates);

void searchTarget(quadNode *tree, FILE *outFile);

void searchTree(point *target,quadNode *tree, FILE *outFile);

void freeTree(quadNode *tree);

int rectangleOverlap(boundary *search, boundary *section);

int queryRange(boundary *search, quadNode *tree,list_t *output, char *direction);

void searchQuery(quadNode *tree, FILE *outFile);


#endif
