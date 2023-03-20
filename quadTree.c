#include "data.h"
#include "linkedList.h"
#include "quadTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <float.h>

//Create a quadtree and initalize its children and data
quadNode *createQuadTree(long double top_right_x, long double top_right_y, long double bottom_left_x, long double bottom_left_y) {
    point *top_right=createPoint(top_right_x,top_right_y);
    point *bottom_left=createPoint(bottom_left_x,bottom_left_y);

    boundary *section=createBoundary(top_right,bottom_left);

    quadNode *tree;
    tree=malloc(sizeof(quadNode));
    assert(tree);
    tree->points=NULL;
    tree->section=section;
    tree->NW=NULL;
    tree->NE=NULL;
    tree->SW=NULL;
    tree->SE=NULL;

    return tree;

}

//Initialize a point which consists of X and Y coordinate
point *createPoint(long double x, long double y) {
    point *newPoint;
    newPoint=malloc(sizeof(point));
    assert(newPoint);

    newPoint->x=x;
    newPoint->y=y;

    return newPoint;

}

//Initialize a boundary for a node in the QuadTree
boundary *createBoundary(point *top_Right, point *bottom_Left) {
    boundary *bound;
    bound=malloc(sizeof(boundary));
    assert(bound);

    bound->topRight=top_Right;
    bound->bottomLeft=bottom_Left;

    return bound;

    
}

//Build data into an appropriate structure and passes it into the correct location in the tree
//Adapted from https://chidiwilliams.com/post/quadtrees/
bool quadTreeAdd(quadNode *tree, long double x, long double y, location_t *data) {
    //If the data is outside the trees section stop the insertion
    if(inRectangle(x,y,tree->section) == 0) {
        return false;
    }

    //Insert the data if the node is empty or it has not yet been subdivded
    if(tree->points == NULL && tree->NW == NULL) {
        point *coordinates=createPoint(x,y);
        tree->points=createDataPoint(coordinates);
        tree->points->data_set->head=addData(tree->points->data_set->head, data);
        tree->points->data_set->size+=1;
        return true;

    }

    //Insert the data if node has notbeen subdivided and coordinates match the already inserted node
    else if(tree->points != NULL && tree->NW == NULL) {
        if(x==tree->points->coordinates->x && y==tree->points->coordinates->y) {
            tree->points->data_set->head=addData(tree->points->data_set->head, data);
            tree->points->data_set->size+=1;
            return true;
        }
    }

    //Subdivide the tree if it has not yet been subdivided and we have reached capacity
    if(tree->NW == NULL) {
        subdivide(tree);
    }

    //Insert the node into the correct child tree
    if(quadTreeAdd(tree->NW, x, y, data)) {
        return true;
    }
    if(quadTreeAdd(tree->NE, x, y, data)) {
        return true;
    }
    if(quadTreeAdd(tree->SW, x, y, data)) {
        return true;
    }
    if(quadTreeAdd(tree->SE, x, y, data)) {
        return true;
    }

    return false;



}

//Check whether a point is inside a rectangle
int inRectangle(long double x, long double y, boundary *section) {
    if(x >section->bottomLeft->x && x < section->topRight->x && y < section->topRight->y && y > section->bottomLeft->y ) {
        return 1;
    }
    return 0;
}

//Create a data point structure that stores the coordinates and the data 
data *createDataPoint(point *coordinates) {
    data *point;
    point=malloc(sizeof(data));
    assert(point);
    point->coordinates=coordinates;
    point->data_set=createList();
    return point;

}

//Creates the children in different quadrants for the tree and reallocates already existing data
//Adapted from https://chidiwilliams.com/post/quadtrees/
void subdivide(quadNode *tree) {
    long double midX=((tree->section->topRight->x)+(tree->section->bottomLeft->x))/2;
    long double midY=((tree->section->topRight->y)+(tree->section->bottomLeft->y))/2;

    long double currtopRightX=tree->section->topRight->x;
    long double currtopRightY=tree->section->topRight->y;
    long double currbottomLeftX=tree->section->bottomLeft->x;
    long double currbottomLeftY=tree->section->bottomLeft->y;

    tree->NE=createQuadTree(currtopRightX,currtopRightY,midX,midY);
    tree->NW=createQuadTree(midX,currtopRightY,currbottomLeftX,midY);
    tree->SE=createQuadTree(currtopRightX,midY,midX,currbottomLeftY);
    tree->SW=createQuadTree(midX,midY,currbottomLeftX,currbottomLeftY);
    
    //Move the points that were in the tree before it was subdivided to its correct location
    node *current=tree->points->data_set->head;
    while(current!=NULL){
        if(quadTreeAdd(tree->NE,tree->points->coordinates->x,tree->points->coordinates->y,current->data)) {
            current=current->next;
            continue;
        }
        if(quadTreeAdd(tree->NW,tree->points->coordinates->x,tree->points->coordinates->y,current->data)) {
            current=current->next;
            continue;
        }
        if(quadTreeAdd(tree->SE,tree->points->coordinates->x,tree->points->coordinates->y,current->data)) {
            current=current->next;
            continue;
        }
        if(quadTreeAdd(tree->SW,tree->points->coordinates->x,tree->points->coordinates->y,current->data)) {
            current=current->next;
            continue;
        }
    }
    //Free the data point in the tree after all data has been moved to the subtrees
    node *curr=tree->points->data_set->head;
    node *prev;
    while(curr != NULL) {
        prev=curr;
        curr=curr->next;
        free(prev);
    
    }
    free(tree->points->coordinates);
    free(tree->points->data_set);
    free(tree->points);
    tree->points=NULL;


    
}

//Determine what quadrant of a specific boundary a coordinate lies in
int determineQuadrant(boundary *bound, point *coordinates) {
    double midX=((bound->topRight->x)+(bound->bottomLeft->x))/2;
    double midY=((bound->topRight->y)+(bound->bottomLeft->y))/2;

    if(coordinates->x <= midX) {
        if(coordinates->y >= midY) {
            return N_W;
        }
        else{
            return S_W;
        }
    }
    else { 
        if(coordinates->y >= midY) {
            return N_E;
        }
        else {
            return S_E;
        }
    }
}

//Read in targets that need to be searched for in the QuadTree
void searchTarget(quadNode *tree, FILE *outFile) {
    char xCoordinate[MAX_STRING_LEN+1],yCoordinate[MAX_STRING_LEN+1];
    double xPosition, yPosition;

    while((scanf("%s %s",xCoordinate,yCoordinate))==2){
        sscanf(xCoordinate, "%lf" ,&xPosition);
        sscanf(yCoordinate, "%lf" ,&yPosition);

        fprintf(outFile,"%s %s\n",xCoordinate,yCoordinate);
        printf("%s %s -->",xCoordinate,yCoordinate);

        point *searchLocation=createPoint(xPosition,yPosition);
        searchTree(searchLocation,tree,outFile);
        free(searchLocation);
          
        
    }
}

//Search for a specific target in the QuadTree
//Adapted from https://chidiwilliams.com/post/quadtrees/
void searchTree(point *target,quadNode *tree,FILE *outFile) {
    //If the point lies outside the current section stop the search
    if(inRectangle(target->x,target->y,tree->section) == 0) {
        return;
    }

    //If the current node of the tree has not been subdivided print the data stored
    if(tree->NW == NULL) {
        list_t *result=sortList(tree->points->data_set);
        node *curr=result->head;
        while(curr!=NULL) {
            printData(curr->data,outFile);
            curr=curr->next;
        }
        printf("\n");
        return;
    }

    //Determine which quadrant of the current section the target it meant to be in to continue recursion
    int quadrant=determineQuadrant(tree->section,target);
    if(quadrant==N_W) {
        printf(" NW");
        searchTree(target,tree->NW,outFile);
    }
    if(quadrant==N_E) {
        printf(" NE");
        searchTree(target,tree->NE,outFile);
    }
    if(quadrant==S_E) {
        printf(" SE");
        searchTree(target,tree->SE,outFile);
    }
    if(quadrant==S_W) {
        printf(" SW");
        searchTree(target,tree->SW,outFile);
    }

}


//Read in a range which is used to search for points in the quadtree
void searchQuery(quadNode *tree, FILE *outFile) {
    char startLon[MAX_STRING_LEN+1], startLat[MAX_STRING_LEN+1], endLon[MAX_STRING_LEN+1], endLat[MAX_STRING_LEN+1];
    long double startX, startY, endX, endY;
    
    while((scanf("%s %s %s %s",startLon,startLat,endLon,endLat)) == 4){
        point *top_Right;
        point *bottom_left;
        startX=strtold(startLon,NULL);
        startY=strtold(startLat,NULL);
        endX=strtold(endLon,NULL);
        endY=strtold(endLat,NULL);

        fprintf(outFile,"%s %s %s %s\n",startLon,startLat,endLon,endLat);
        printf("%s %s %s %s -->",startLon,startLat,endLon,endLat);

        top_Right=createPoint(endX,endY); 
        bottom_left=createPoint(startX,startY);    
        boundary *searchRange=createBoundary(top_Right,bottom_left);

        list_t *result=createList();
        queryRange(searchRange,tree,result,NULL);

        result=sortList(result);
        //Once results have been retrieved use footpath id to print each dataset once
        int currfootPath=-1;
        node *curr= result->head;
        while(curr!=NULL) {
            if(curr->data->footpath_id != currfootPath){
                printData(curr->data,outFile);
                currfootPath=curr->data->footpath_id;
            }
            curr=curr->next;    
        }
        printf("\n");

        //Free the search targets and the nodes for the list used to store the results of the search
        free(top_Right);
        free(bottom_left);
        free(searchRange);
        curr=result->head;
        node *prev;
        while(curr != NULL) {
            prev=curr;
            curr=curr->next;
            free(prev);
    
        }
        free(result);
                  
    }
}

//Search for all points that exist within a certain range of the QuadTree
//Adapted from https://en.wikipedia.org/wiki/Quadtree#:~:text=A%20quadtree%20is%20a%20tree,into%20four%20quadrants%20or%20regions.
int queryRange(boundary *search, quadNode *tree, list_t *output, char *direction) {
    //If the boundary of the search does not intersect with the section of the current node stop search
    if(rectangleOverlap(search,tree->section) == 0) {
        return 0;
    }

    //Print the direction we are searching only if we are not at the root and are search in a leaf or internal node
    if((tree->points != NULL || tree->NW != NULL) && direction != NULL) {
        printf("%s",direction);
    }

    //If the current node is not empty check if it lies within the range of the search    
    if(tree->points != NULL) {
        if(inRectangle(tree->points->coordinates->x,tree->points->coordinates->y,search)) {
            node *curr=tree->points->data_set->head;
            while(curr != NULL){
                output->head=addData(output->head,curr->data);
                curr=curr->next;
            }
        }
    }

    //If we reach a node that has not been subdivided stop the search 
    if(tree->NW == NULL) {
        return 0;
    }
    queryRange(search,tree->SW,output," SW");
    queryRange(search,tree->NW,output," NW");
    queryRange(search,tree->NE,output," NE");
    queryRange(search,tree->SE,output," SE");

    
    return 0;



}

//Check whether two rectangles intersect 
//Adapted from https://www.geeksforgeeks.org/find-two-rectangles-overlap/
int rectangleOverlap(boundary *search, boundary *section) {
    //Check whether one of the rectangles lies on the side of the other 
    if(search->topRight->x < section->bottomLeft->x || section->topRight->x < search->bottomLeft->x) {
        return 0;
    }

    //Check whether one of the rectangles is above or below the other 
    if(search->bottomLeft->y > section->topRight->y || section->bottomLeft->y > search->topRight->y ) {
        return 0;
    }
    return 1;
}

//Free the data in the tree
void freeTree(quadNode *tree) {
    //Stop the free once we reach an empty node
    if(tree == NULL) {
        return;
    }
    //If the tree has not been subdivided and has data free the data
    if(tree->NW == NULL && tree->points != NULL) {
        node *curr=tree->points->data_set->head;
        node *prev;
        while(curr != NULL) {
            prev=curr;
            curr=curr->next;
            free(prev);
    
        }
        free(tree->points->data_set);
        free(tree->points->coordinates);
        free(tree->points);
    }
    //Recurse through the children of the tree and free their data 
    freeTree(tree->NW);
    freeTree(tree->NE);
    freeTree(tree->SE);
    freeTree(tree->SW);

    //Free the current trees boundary 
    free(tree->section->topRight);
    free(tree->section->bottomLeft);
    free(tree->section);
    free(tree);

}











