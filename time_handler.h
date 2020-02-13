//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_TIME_HANDLER_H
#define AQUARIUM_TIME_HANDLER_H

typedef struct {
    long double global;
    long double dtime;
} time_handler_t;

time_handler_t * create_time_handler(void);
time_handler_t * destroy_time_handler(time_handler_t * handler);
void reset_time_handler(time_handler_t * handler);
void update_time_handler(time_handler_t * handler);
long double get_dtime(time_handler_t * handler);
long double get_global_time(time_handler_t * handler);

#endif //AQUARIUM_TIME_HANDLER_H
