#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bus_system.h"



//inserts passenger at the head of linked list
void insert_head(passenger **head, passenger *curr){
    curr->next = *(head);
    *head = curr;
}


//deletes passenger at the head of linked list
passenger * delete_head(passenger **head, passenger *curr_two, passenger *previous_two, int current_bus_stop){
    *head = (*head)->next;  //now skips over first passenger
    free(previous_two);  //frees first passenger
    return (*head);
}


//deletes passenger in middle of linked list
passenger * delete_element(passenger *curr_unload, passenger *previous_unload, int current_bus_stop){
    previous_unload->next = curr_unload->next;  //skips over the current passenger
    passenger *free_temp = curr_unload;
    free(free_temp);  //gets rid of current passenger
    return (previous_unload->next);  //curr now points to next passenger
}

