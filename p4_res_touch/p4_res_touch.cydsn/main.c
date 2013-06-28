/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <device.h>
#include <matrix_math.h>
#include <measurement_functions.h>

int16 measurement_array[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int16 xyz[3] = {0, 0, 0};
uint8 measurement_done = 0;

// Let the system know that a cycle of measurements has been complete
void ScanDoneRoutine() {
	measurement_done = 1;

	ScanDoneISR_ClearPending();

}

// Respond to message from computer
void PcUartRxInt() {

	uint8 buff_size = PC_UART_SpiUartGetRxBufferSize();
	char rxChar = PC_UART_UartGetChar();
	
	if (rxChar == 't')
	{
		PC_UART_SpiUartPutArray((uint8*)xyz, sizeof(xyz));
		PC_UART_SpiUartPutArray((uint8*)measurement_array, sizeof(measurement_array));
	}
	
	PC_UART_ClearRxInterruptSource(PC_UART_GetRxInterruptSource());
	UartRxISR_ClearPending();
	
}

void main()
{

    SarSeq_Start();
	ScanDoneISR_Start();
	UartRxISR_Start();
	LED_PWM_Start();
	PC_UART_Start();
	
	ScanDoneISR_SetVector(ScanDoneRoutine);
	UartRxISR_SetVector(PcUartRxInt);

    CyGlobalIntEnable;
	
	SarSeq_StartConvert();
	
    for(;;)
    {
     	if (measurement_done != 0) {
		
			store_results(measurement_array);
			
			calc_xyz(measurement_array, xyz);
			
			if (xyz[2] < 0) 
			{
				xyz[2] = -xyz[2];
				
			}
			
			LED_PWM_WriteCompare(xyz[2]);
			
			measurement_done = 0;
		
		}
		
    }
}

/* [] END OF FILE */
