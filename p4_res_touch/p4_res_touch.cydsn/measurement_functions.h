/* 
 * measurement_functions.h
 */

#include <device.h>

// The number of force sensors used
#define NUM_SENSORS 8

// Place the measured force sensor results in the result buffer
void store_results(int16 destination[]);

//[] END OF FILE
