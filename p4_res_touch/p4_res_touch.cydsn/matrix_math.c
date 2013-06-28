/* 
 * matrix_math.c
 */

#include <matrix_math.h>

// Calculate the x, y, and z of the touch using the passed in measurement buffer
void calc_xyz(int16 measurement_array[], int16 xyz[3])
{
	// this is me capturing video!  I hope it works.
	const int16 sensor_x[8] = {0, 8, 16, 16, 16, 8, 0, 0};
	const int16 sensor_y[8] = {0, 0, 0, 8, 16, 16, 16, 8};
	uint8 i = 0;
	
	int16 measurement_array_x[8];
	int16 measurement_array_y[8];
	
	int32 force_x = 0;
	int32 force_y = 0;
	int32 force = 0;
	
	// Scale based on sensor location
	for (i = 0; i < 8; i++) {
		measurement_array_x[i] = (measurement_array[i] * sensor_x[i]);
		measurement_array_y[i] = (measurement_array[i] * sensor_y[i]);
		
	}
	
	// Calculate force of press (Z) and sum of scaled sensor readings
	for (i = 0; i < 8; i++) {
		force += measurement_array[i];
		force_x += measurement_array_x[i] << 4;
		force_y += measurement_array_y[i] << 4;
		
	}
	
	xyz[0] = force_x / force;
	xyz[1] = force_y / force;
	xyz[2] = force >> 4;
		
}

/* [] END OF FILE */
