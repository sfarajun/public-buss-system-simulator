#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bus_system.h"







int main(int argc, char *argv[]) {

    // ----------------------------- 1. open file to get number of bus stops----------------------------------
    FILE *passengers_file;
    passengers_file = fopen(argv[1], "r");
    if (passengers_file == NULL) {
        fprintf(stderr, "Invalid database file nofile\n");
        return -1;
    }


    // ----------------------------- 2. open file to get number of bus stops----------------------------------
    FILE *log_file;
    log_file = fopen(argv[2], "w");
    if (log_file == NULL) {
        fprintf(stderr, "Invalid database file nofile\n");
        return -1;
    }




    // ----------------------------- 3. declare bus_stop and bus variables ----------------------------------
    //create bus_stop array
    int num_of_bus_stops = get_num_bus_stops(passengers_file);
    bus_stop_type *bus_stops = (bus_stop_type *) calloc(num_of_bus_stops, sizeof(bus_stop_type));
    int i;
    for(i = 0; i < num_of_bus_stops; i ++){
        bus_stops[i].ID = i;
    }
    //create bus variable
    bus_type bus;
    bus.current_num_riders = 0;
    bus.current_stop_ID = bus_stops[0].ID;
    bus.riders_head = NULL;






    // ----------------------------- 4. declare simulator loop variable ----------------------------------
    int timestep = 1;
    int current_bus_stop = 0;
    int stop_simulation = 0;
    int no_more_passengers_to_add = 0;
    int number_to_be_added_to_bus = 0;

    //stats
    int total_number_of_passengers = 0;
    int total_passenger_time = 0;
    int total_wait_time = 0;

    //read from file
    int arrival_time;
    int originating_bus_stop_ID;
    int destination_bus_stop_ID;
    //read value of 1st passenger
    fscanf(passengers_file, "%d %d %d", &arrival_time, &originating_bus_stop_ID, &destination_bus_stop_ID);








    // ----------------------------- 5. simulator loop-----------------------------------------------------
    while(stop_simulation == 0) {
        /*
         * ------print log file-----
         */
        print_log_file(log_file, timestep, num_of_bus_stops, &bus, bus_stops);




        /*
         * -----place passenger at bus_stop-----
         */
        passenger *curr;
        //put passengers on bus stop once as they begin
        if(no_more_passengers_to_add == 0){
            while (arrival_time == timestep) {
                //creates new passenger
                curr = (passenger *) calloc(1, sizeof(passenger));
                get_passenger_info(curr, &arrival_time, &originating_bus_stop_ID, &destination_bus_stop_ID);

                //put passenger at head of bus stop
                insert_head(&(bus_stops[originating_bus_stop_ID].waiting_head), curr);

                //update statistics for bus stop
                total_number_of_passengers++;
                bus_stops[originating_bus_stop_ID].num_waiting++;

                //read in next person into values, see if arrived also, or if no more people
                if(fscanf(passengers_file, "%d %d %d", &arrival_time, &originating_bus_stop_ID, &destination_bus_stop_ID) == EOF) {
                    no_more_passengers_to_add = 1;
                    break;
                }
            }
        }




        /*
         * ------unload the bus--------
         */
        passenger *curr_unload = bus.riders_head;
        passenger *previous_unload = bus.riders_head;
        //if there are passengers on the bus
        while (curr_unload != NULL) {
            if (bus.riders_head->destination_ID == current_bus_stop) {
                //update statistics
                bus.current_num_riders--;
                total_passenger_time = total_passenger_time + (timestep - (bus.riders_head->arrival_time));

                //frees head list item if at destination
                curr_unload = delete_head(&(bus.riders_head), curr_unload, previous_unload, current_bus_stop);
                previous_unload = curr_unload;
            }

            else if (curr_unload->destination_ID == current_bus_stop) {
                //update statistics
                bus.current_num_riders--;
                total_passenger_time = total_passenger_time + (timestep - (curr_unload->arrival_time));

                //frees the passengers variable if reached destination
                curr_unload = delete_element(curr_unload, previous_unload, current_bus_stop);
            }

            else {
                //else just go onto next passenger
                previous_unload = curr_unload;
                curr_unload = curr_unload->next;
            }
        }





        /*
         * ------load the bus (and unload bus stop)--------
         */
        //if passengers at bus stop
        while (bus_stops[current_bus_stop].waiting_head != NULL) {
            for(curr = bus_stops[current_bus_stop].waiting_head; curr->next != NULL; curr = curr->next){
                //set curr to end of bus stop list
                number_to_be_added_to_bus++;
                //for stats
                total_wait_time = total_wait_time + (timestep - (curr->arrival_time)) + 1;
            }
            total_wait_time = total_wait_time + (timestep - (curr->arrival_time)) + 1;


            //put passengers on bus
            curr->next = bus.riders_head;
            bus.riders_head = bus_stops[current_bus_stop].waiting_head;
            bus_stops[current_bus_stop].waiting_head = NULL;

            //update stats on number of passengers on bus
            bus.current_num_riders = bus.current_num_riders + number_to_be_added_to_bus + 1;
            number_to_be_added_to_bus = 0;
            bus_stops[current_bus_stop].num_waiting = 0;
        }





        /*
         * --------move_bus---------
         */
        current_bus_stop = (current_bus_stop + 1) % num_of_bus_stops;
        bus.current_stop_ID = current_bus_stop;





        /*
         * ------check if should stop the loop-------
         */
        if(no_more_passengers_to_add == 1){
            if(bus.current_num_riders == 0){
                //check if all the bus stops are empty also
                int j;
                for(j = 0; j < num_of_bus_stops; j++){
                    if(bus_stops[j].num_waiting != 0){
                        stop_simulation = 0;
                        break;
                    }
                    else{
                        stop_simulation = 1;
                    }
                }
            }
        }




        timestep++;
    }





    //-------print final things------------------------------------------
    //bus system ends now 1 second later
    fprintf(log_file, "Total sim time:  %d", timestep);
    fprintf(log_file, "\nTotal passengers:  %d", total_number_of_passengers);
    fprintf(log_file, "\nPassenger throughput:  %.2f", ((float)total_number_of_passengers/timestep));
    fprintf(log_file, "\nAverage passenger time:  %.2f", ((float)total_passenger_time/total_number_of_passengers));
    fprintf(log_file, "\nAverage wait time:  %.2f\n", ((float)total_wait_time/total_number_of_passengers - 1) );


    return 0;
}
