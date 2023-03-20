#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>

//data.h is adapted from Assignment 1 by Rohit Sandeep Ambakkat 

// constant defnitions
#define MAX_STRING_LEN 128
#define MAX_LINE_LEN 512
#define FIELDS 19
#define FOOTPATH_ID 0
#define ADDRESS 1
#define CLUE_SA 2
#define ASSET_TYPE 3
#define DELTAZ 4
#define DISTANCE 5
#define GRADE1IN 6
#define MCC_ID 7
#define MCC_ID_INT 8
#define RL_MAX 9
#define RL_MIN 10
#define SEGSIDE 11
#define STATUS_ID 12
#define STREET_ID 13
#define STREET_GROUP 14
#define START_LAT 15
#define START_LON 16
#define END_LAT 17
#define END_LON 18

typedef struct location location_t;

// data definitions
struct location{
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon; 
    double end_lat;
    double end_lon;
};






void SkipHeaderLine(FILE *f);


location_t *fileRead(char data[FIELDS][MAX_LINE_LEN+1]);

void printData(location_t *data, FILE *f);


void dataFree(location_t *s);

#endif
