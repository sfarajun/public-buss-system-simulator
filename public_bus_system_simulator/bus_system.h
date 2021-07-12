
typedef struct passengers_type{
    int arrival_time;
    int time_in_system;
    int time_waiting_for_bus;
    int start_bus_ID;
    int destination_ID;
    struct passengers_type *next; //points to passenger struct
}passenger;



typedef struct BUS_STOP{
    int ID;
    int num_waiting;
    passenger *waiting_head;  //pointer to head of linked list of passengers waiting
}bus_stop_type;


typedef struct BUS{
    int current_stop_ID;
    int current_num_riders;
    passenger *riders_head;  //pointer to head of linked list of passengers riding
}bus_type;








//----------------------------------- setting stuff up -------------------------------------
int get_num_bus_stops(FILE *);


void get_passenger_info(struct passengers_type *, int *, int *, int *);


void print_log_file(FILE *, int , int , bus_type *, bus_stop_type *);






//------------------------------------ pointer functiions ------------------------------
void insert_head(passenger **head, passenger *p);

passenger * delete_head(passenger **, passenger *, passenger *, int);

passenger * delete_element(passenger *, passenger *, int);

