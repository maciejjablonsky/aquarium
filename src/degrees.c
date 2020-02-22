#include "degrees.h"

#include <math.h>

long double ldto_degrees(long double radians) {
    return radians * (180/M_PI);
}

long double ldto_radians(long double degrees) {
    return degrees * (M_PI/180);
}

int ito_degrees(long double radians) {
    return (int)ldto_degrees(radians);
}

int ito_radians(long double degrees) {
    return (int)ldto_radians(degrees);
}

double dto_degrees(double radians) {
    return (double)ldto_degrees(radians);
}

double dto_radians(double degrees) {
    return (double)ldto_radians(degrees);
}