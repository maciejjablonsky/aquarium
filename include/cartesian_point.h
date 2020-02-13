//
// Created by foreverhungry on 13.02.2020.
//

#ifndef AQUARIUM_CARTESIAN_POINT_H
#define AQUARIUM_CARTESIAN_POINT_H

typedef struct {
    long double x;
    long double y;
} cartesian_point_t;

cartesian_point_t * create_cartesian_point(long double x, long double y);
cartesian_point_t * destroy_cartesian_point(cartesian_point_t * this);

long double distance_between(const cartesian_point_t * p1, const cartesian_point_t * p2);

#endif //AQUARIUM_CARTESIAN_POINT_H
