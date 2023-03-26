#ifndef __TEST_RANDOM_H__
#define __TEST_RANDOM_H__

#include <stdint.h>
#include <stdlib.h>

static int16_t random_i(int16_t min, int16_t max){
	return (rand()/(RAND_MAX/(max-min)))+min;
}

#endif