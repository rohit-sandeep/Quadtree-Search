/*
* dict3.c and dict4.c
*
* Created by Rohit Sandeep Ambakkat (rsandeepamba@student.unimelb.edu.au)
* 3/09/2022
*
* dict3.c : Reads and stores data from a CSV to a linked list and uses it to build a quadtree
* which is then used to search for specific targets from stdin and if found output is written to an outputfile
*
* To run program type:
* ./dict3 3 inputfile outputfile bottomLeftX bottomRightY topRightX topRightY 
*
* dict4.c : Reads and stores data from a CSV to a linked list and uses it to build a quadtree
* which is then used to search for points within a specific range from stdin and if found 
* output is written to an outputfile
*
* To run program type: 
* ./dict4 4 inputfile outputfile bottomLeftx bottomRightY topRightX topRightY 
*/

// driver.c is adapted from Assignment 1 by Rohit Sandeep Ambakkat 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "linkedList.h"
#include "quadTree.h"

#define STAGE3 "3"
#define INPUTFILE 2
#define OUTPUTFILE 3
#define TOPRIGHTXARGUMENT 6
#define TOPRIGHTYARGUMENT 7
#define BOTTOMLEFTXARGUMENT 4
#define BOTTOMLEFTYARGUMENT 5

int main(int argc, char *argv[]) {
    char line[MAX_LINE_LEN+1];
    char curr;
    int length=0;
    int j=0,n=0,i;
    int quotation=0;
    
    assert(argc >= 7);
    long double top_right_x=strtold(argv[TOPRIGHTXARGUMENT],NULL);
    long double top_right_y=strtold(argv[TOPRIGHTYARGUMENT],NULL);
    long double bottom_left_x=strtold(argv[BOTTOMLEFTXARGUMENT],NULL);
    long double bottom_left_y=strtold(argv[BOTTOMLEFTYARGUMENT],NULL);
 

    FILE *inFile=fopen(argv[INPUTFILE], "r");
    FILE *outFile=fopen(argv[OUTPUTFILE], "w");
    assert(inFile);
    assert(outFile);

    list_t *list=createList();
  
    SkipHeaderLine(inFile);
    location_t *s;

    //Read through each line of the CSV as a string to maintain the formatting and break down the fields
    while( (fgets(line,MAX_LINE_LEN,inFile)) != NULL ) {
        char data[FIELDS][MAX_LINE_LEN+1]={0};
        length=strlen(line);
        //Read through each character of the line that is taken from the CSV to find the elements of each field
        for(i=0;i < length;i++) {
            curr=line[i];
            if(line[i] == ',' && line[i+1] == '"') {
                quotation=1;
                continue;
            }
            else if(line[i] == '"' && line[i+1] == ',') {
                quotation=0;
                continue;
            }
            //If the data for the field being read in is not quotation we store each character into the array of the field until a comma
            if(quotation == 0) {
                if(curr!=',') {
                    data[j][n]=curr;
                    n++;
                }
                //If a comma is encountered we move to the next field and reset the counter for the characters in the input
                else{
                    n=0;
                    j++;
                }
            }
            //If the data for the field is in quotation we store each character into the array until another quotation 
            else {
                if(curr != '"') {
                    data[j][n]=curr;
                    n++;
                }
                else{
                    n=0;
                    j++;
                }
                
            }
        }
        //Pass the array that contains the data of the row after it has been processed into the function so it can be 
        //stored in the structure 
        s=fileRead(data);
        list->head=addData(list->head, s);
        (list->size)++;
        j=0;
        n=0;
    }
    fclose(inFile);

    quadNode *tree=createQuadTree(top_right_x,top_right_y,bottom_left_x,bottom_left_y);

    //Use linked list to store each footpath record into the quad using its start and end coordinates
    node *currNode=list->head;
    while(currNode != NULL) {
        double startX=currNode->data->start_lon;
        double startY=currNode->data->start_lat;
        quadTreeAdd(tree,startX,startY,currNode->data);

        double endX=currNode->data->end_lon;
        double endY=currNode->data->end_lat;
        quadTreeAdd(tree,endX,endY,currNode->data);

        currNode=currNode->next;
        
    }
    if(strcmp(argv[1],STAGE3) == 0){
        searchTarget(tree,outFile);
    }
    else{
        searchQuery(tree,outFile);
    }
    
    
    freeTree(tree);
    fclose(outFile);
    freeList(list->head);
    free(list);


    return 0;
}

