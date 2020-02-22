#include "random.h"
#include <stdlib.h>

typedef long double __random_t;

__random_t __random(void) {
    return (long double)rand();
}

#define __MAX_RANDOM RAND_MAX
#define __MIN_RANDOM 0

long double ldrandom_range(long double min, long double max) {
    return ((__random() - __MIN_RANDOM)/(__MAX_RANDOM - __MIN_RANDOM)) * (max - min) + min;
}

double drandom_range(double min, double max) {
    return (double)ldrandom_range(min, max);
}

float frandom_range(float min, float max) {
    return (float)ldrandom_range(min, max);
}

unsigned long long ullrandom_range(unsigned long long min, unsigned long long max) {
    return (unsigned long long)ldrandom_range(min, max);
}

long long llrandom_range(long long min, long long max) {
    return (long long)ldrandom_range(min, max);
}

unsigned long ulrange(unsigned long min, unsigned long max) {
    return (unsigned long)ldrandom_range(min, max);
}

long lrandom_range(long min, long max) {
    return (long)ldrandom_range(min, max);
}

unsigned int uirange(unsigned min, unsigned max) {
    return (unsigned int)ldrandom_range(min, max);
}

int irandom_range(int min, int max) {
    return (int)ldrandom_range(min, max);
}