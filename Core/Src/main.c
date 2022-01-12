#include "GPIO.h"
#include "dma.h"
#include <stdint.h>

void callout(char condition);
void DMA2_Stream0_IRQHandler (void);

#define buffer_size ((uint32_t)100)
static uint32_t src_buffer [buffer_size];
static uint32_t dst_buffer [buffer_size] = {};
char state = 0;
unsigned FT_state = 0;


int main(void)
{
	GPIO_Init(0, 1, OUTPUT, PUSH_PULL); // to set pin high to check if destination equals the source after transferring // checking from the for loop

	GPIO_Init(0, 7, OUTPUT, PUSH_PULL); // for full transfer
	GPIO_Init(0, 8, OUTPUT, PUSH_PULL); // for half transfer
	GPIO_Init(0, 9, OUTPUT, PUSH_PULL); // for transfer error

	for (int idx = 0; idx < buffer_size ; idx ++)
	{
		src_buffer[idx] = (idx + 1) * 100; //array of integers of size 100 elements initialized with numbers of 100,200, …etc.
		dst_buffer[idx] = 0;
	}

	DMA_init(2);
	stream_init (0, src_buffer , dst_buffer, buffer_size /*100*/ , word, FIFO_mode , channel_0 , PL_Very_high , Memory_to_memory, half_FIFO , incremented, incremented);
	DMA_stream_enable();

	while(1)
	{
		if (FT_state == 1)
		{
		//	MAKING SURE THE TRANSFER IS COMPLETE WITHOUT THE INTERRUPT FLAGS
				for (int idx = 0; idx < buffer_size ; idx ++)
				{
					if( dst_buffer[idx] == src_buffer[idx])
					{
						GPIO_WritePin(0, 1, 1);
					}
					else
					{
						GPIO_WritePin(0, 1, 0);
						break;
					}
				}
				FT_state = 0;
		}

	}
	return 0;
}

void callout(char condition)
{
	switch (condition)
	{
		case 1:
			GPIO_WritePin(0, 7, 1);// for full transfer
			GPIO_WritePin(0, 8, 0);
			GPIO_WritePin(0, 9, 0);

			FT_state = 1;
			break;
		case 2:
			GPIO_WritePin(0, 8, 1);// for half transfer
			GPIO_WritePin(0, 7, 0);
			GPIO_WritePin(0, 9, 0);
			break;
		case 3:
			GPIO_WritePin(0, 9, 1);// for transfer error
			GPIO_WritePin(0, 7, 0);
			GPIO_WritePin(0, 8, 0);
			break;
	}
}
