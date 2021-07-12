#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bus_system.h"




//gets number of stops from file
int get_num_bus_stops(FILE *passengers_file){
    int num_of_bus_stops;
    fscanf(passengers_file, "%d", &num_of_bus_stops);
    return num_of_bus_stops;
}


//gets passenger info from file
void get_passenger_info(struct passengers_type *curr, int *arrival_time, int *originating_bus_stop_ID, int *destination_bus_stop_ID){
    curr -> arrival_time = *arrival_time;
    curr -> start_bus_ID = *originating_bus_stop_ID;
    curr -> destination_ID = *destination_bus_stop_ID;
}


//prints log file
void print_log_file(FILE *log_file, int timestep, int num_of_bus_stops, bus_type *bus, bus_stop_type *bus_stops){
    fprintf(log_file, "*** TIME STEP = %d ***\n", timestep);
    fprintf(log_file, "Bus: Bus Stop %d, passengers %d\n", bus->current_stop_ID, bus->current_num_riders);

    int i;
    for(i = 0; i < num_of_bus_stops; i ++){
        fprintf(log_file, "Bus Stop %d:  waiting %d\n", bus_stops[i].ID, bus_stops[i].num_waiting);
    }
}

