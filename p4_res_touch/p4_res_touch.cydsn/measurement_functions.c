/* 
 * measurement_functions.c
 */

#include <measurement_functions.h>

// Place the measured force sensor results in the result buffer
void store_results(int16 destination[])
{
	uint8 i = 0;
	
	for (i = 0; i < NUM_SENSORS; i++) {
		destination[i] = SarSeq_GetResult16(i);
	
	}
	
}


/* [] END OF FILE */
