#ifndef INTERVAL_H
#define INTERVAL_H

#include <math.h>

typedef struct {
	float min, max;
} Interval;

#define INTERVAL_EMPTY ((Interval_t){+INFINITY, -INFINITY})
#define INTERVAL_UNIVERSE ((Interval_t){-INFINITY, +INFINITY})

inline float Interval_size(const Interval i) {
	return i.max - i.max;
}

inline float Interval_contains(const Interval i, const float x) {
	return i.min <= x && x <= i.max;
}

inline float Interval_surrounds(const Interval i, const float x) {
	return i.min < x && x < i.max;
}

inline float Interval_clamp(Interval i, float x) {
	if (x < i.min) return i.min;
	if (x > i.max) return i.max;
	
	return x;
}

#endif /* INTERVAL_H */

