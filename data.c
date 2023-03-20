#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

//data.c is adapted from Assignment 1 by Rohit Sandeep Ambakkat


// skip the header line of .csv file "f"
void SkipHeaderLine(FILE *f) {
    while(fgetc(f) != '\n');
}

//Read the processed data from the file and returns an appropriate data structure 
location_t *fileRead(char data[FIELDS][MAX_LINE_LEN+1]) {
    location_t *s = NULL;
     
    s = malloc(sizeof(*s));     
    assert(s);
    //Read in each field of the data from the array and store it in the structure with its correct data type
    sscanf(data[FOOTPATH_ID], "%d", &s->footpath_id);

    s->address=strdup(data[ADDRESS]);
    assert(s->address);
  
    s->clue_sa=strdup(data[CLUE_SA]);
    assert(s->clue_sa);
    
    s->asset_type=strdup(data[ASSET_TYPE]);
    assert(s->asset_type);

    sscanf(data[DELTAZ], "%lf", &s->deltaz);

    sscanf(data[DISTANCE], "%lf", &s->distance);

    sscanf(data[GRADE1IN], "%lf", &s->grade1in);

    sscanf(data[MCC_ID], "%d", &s->mcc_id);

    sscanf(data[MCC_ID_INT], "%d", &s->mccid_int);

    sscanf(data[RL_MAX], "%lf", &s->rlmax);

    sscanf(data[RL_MIN], "%lf", &s->rlmin);

    s->segside=strdup(data[SEGSIDE]);
    assert(s->segside);

    sscanf(data[STATUS_ID], "%d" ,&s->statusid);

    sscanf(data[STREET_ID], "%d" ,&s->streetid);

    sscanf(data[STREET_GROUP], "%d" ,&s->street_group);
    
    sscanf(data[START_LAT], "%lf" ,&s->start_lat);

    sscanf(data[START_LON], "%lf" ,&s->start_lon);

    sscanf(data[END_LAT], "%lf" ,&s->end_lat);

    sscanf(data[END_LON], "%lf" ,&s->end_lon);  
    return s; 
    
}

//Function to print the data from each row in the csv in a specified format
void printData(location_t *data,FILE *f) {
   fprintf(f, 
            "--> footpath_id: %d ||"
            " address: %s ||" 
            " clue_sa: %s ||" 
            " asset_type: %s ||"
            " deltaz: %.2lf ||" 
            " distance: %.2lf ||"
            " grade1in: %.1lf ||"
            " mcc_id: %d ||" 
            " mccid_int: %d ||" 
            " rlmax: %.2lf ||" 
            " rlmin: %.2lf ||"
            " segside: %s ||" 
            " statusid: %d ||" 
            " streetid: %d ||" 
            " street_group: %d ||" 
            " start_lat: %.6lf ||" 
            " start_lon: %.6lf ||" 
            " end_lat: %.6lf ||" 
            " end_lon: %.6lf || ",
            data->footpath_id,
            data->address,
            data->clue_sa,
            data->asset_type,
            data->deltaz,
            data->distance,
            data->grade1in,
            data->mcc_id,
            data->mccid_int,
            data->rlmax,
            data->rlmin,
            data->segside,
            data->statusid,
            data->streetid,
            data->street_group,
            data->start_lat,
            data->start_lon,
            data->end_lat,
            data->end_lon);
    fprintf(f,"\n");
}


//Free the string elements of the data for which memory was allocated
void dataFree(location_t *s){
    free(s->address);
    free(s->clue_sa);
    free(s->asset_type);
    free(s->segside);
    free(s);
}




