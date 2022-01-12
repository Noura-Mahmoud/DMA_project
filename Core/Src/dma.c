#include "dma.h"
#include "GPIO.h"
#include <stdint.h>

unsigned int DMA_clk  ;
unsigned int DMA_base  ;
unsigned int stream_number ;
unsigned int SxCR_offset ;
unsigned int * DMA_SxCR ;
unsigned int SxFCR_offset  ;
unsigned int * DMA_SxFCR  ;
unsigned int SxPAR_offset  ;
unsigned int * DMA_SxPAR  ;
unsigned int SxM0AR_offset  ;
unsigned int * DMA_SxM0AR  ;
unsigned int SxNDTR_offset  ;
unsigned int * DMA_SxNDTR  ;
unsigned int * DMA_LIFCR  ;
unsigned int * DMA_HIFCR = 0  ;
unsigned int * DMA_LISR = 0  ;
unsigned int * DMA_HISR  ;

char indication;


void DMA_init(char DMA_NO)
{
//	ISR_global = ISR_local ; //take the address of the function of the interrupt

	if (DMA_NO == 1)
	{
		// ENABLE DMA CLOCK IN RCC
		DMA_clk = DMA1_CLK;
		*RCC_AHB1ENR |= 1<<DMA_clk ;  // enable DMA1_CLK
		DMA_base = 0x40026000;
	}
	else if (DMA_NO == 2)
	{
		// ENABLE DMA CLOCK IN RCC
		DMA_clk = DMA2_CLK;
		*RCC_AHB1ENR |= 1<<DMA_clk ;  // enable DMA2_CLK
		DMA_base = 0x40026400;
	}
}


void stream_init ( char stream_no, const uint32_t *src, const uint32_t *dst, const uint32_t number_of_transfers, char transfer_item_size,
		char transfer_mode, char channel,char priority, char direction, char fifo_threshold, char PINC_mode, char MINC_mode)
{

	stream_number = stream_no;

	SxCR_offset = 0x10 + 0x18 * stream_number;
	DMA_SxCR = DMA_REG (DMA_base, SxCR_offset); // DMA stream x configuration register

	SxFCR_offset = 0x24 + 0x18 * stream_number;
	DMA_SxFCR = DMA_REG (DMA_base, SxFCR_offset ); //DMA stream x FIFO control register

	SxPAR_offset = 0x18 + 0x18 * stream_number;
	DMA_SxPAR = DMA_REG (DMA_base, SxPAR_offset ); // DMA stream x peripheral address register

	SxM0AR_offset = 0x1C + 0x18 * stream_number;
	DMA_SxM0AR = DMA_REG (DMA_base,SxM0AR_offset ); // DMA stream x memory 0 address register

	SxNDTR_offset = 0x14 + 0x18 * stream_number;
	DMA_SxNDTR = DMA_REG (DMA_base, SxNDTR_offset ); // DMA stream x number of data register

	DMA_LIFCR = DMA_REG (DMA_base, 0x08); // DMA low interrupt flag clear register, for streams 0:3
	DMA_HIFCR = DMA_REG (DMA_base, 0x0C); // DMA low interrupt flag clear register, for streams 4:7

	DMA_LISR = DMA_REG (DMA_base, 0x00); // DMA low interrupt status register
	DMA_HISR = DMA_REG (DMA_base, 0x04); // DMA high interrupt status register


	// MAKE SURE THE STREAM IS DISABLED
	*DMA_SxCR &= ~ (1<<EN) ; // make sure DMA stream is disabled
	while (((*DMA_SxCR) & 1) == 1); // Do nothing until EN bit is cleared


	if (direction != 2) // because memory to memory transfer doesn't need channel selection
	{
		// SELECT THE CHANNEL
		*DMA_SxCR |= channel<< CHSEL;
	}

	// SELECT THE STREAM PRIORITY
	*DMA_SxCR |= priority << PL;

	// SELECT THE DIRECTION
	*DMA_SxCR |= direction << DIR;

	// SELECT THE PERIPHIRAL SIZE
	*DMA_SxCR |= transfer_item_size << PSIZE;

	// SELECT THE MEMORY SIZE
	*DMA_SxCR |= transfer_item_size << MSIZE;

	// SELECT transfer MODE
	if (transfer_mode == 2) // FIFO mode
	{
		*DMA_SxFCR |= 1 << DMDIS;

		// SELECT FIFO THRESHOLD LEVEL
		*DMA_SxFCR |= fifo_threshold << FTH;
	}
	else if (transfer_mode == 3) //direct mode
	{
		*DMA_SxFCR &= ~(1 << DMDIS);
	}

	// SELECT PERIPHERAL AND MEMORY INCREMENT MODE
	if (PINC_mode == 1) // incremented
	{
		*DMA_SxCR |= 1 << PINC;
	}
	else if (PINC_mode == 0) //fixed
	{
		*DMA_SxCR &= ~ (1 << PINC);
	}
	if (MINC_mode == 1) // incremented
	{
		*DMA_SxCR |= 1 << MINC;
	}
	else if (MINC_mode == 0) //fixed
	{
		*DMA_SxCR &= ~ (1 << MINC);
	}

	// ENABLE DMA INTERRUPT FROM NVIC
	if ((DMA_base == 0x40026000) & (stream_number != 7)) // DMA 1 with stream 0:6
	{
		*NVIC_ISER0 = 1<< (11 + stream_number);
	}
	else if ((DMA_base == 0x40026000) & (stream_number == 7)) // DMA 1 with stream 7
	{
		*NVIC_ISER1 = 1<< 15;
	}
	else if ((DMA_base == 0x40026400) & (stream_number <5)) // DMA 2 with stream 0:4
	{
		*NVIC_ISER1 = 1<< (24 + stream_number);
	}
	else if ((DMA_base == 0x40026400) & (stream_number >5)) // DMA 2 with stream 5:7
	{
		*NVIC_ISER2 = 1<< (4 + (stream_number-5));
	}

	// ENABLE DMA TRANSFER COMPLETE INTERRUPT
	*DMA_SxCR |= 1 << TCIE;
	// Half transfer interrupt enable
	*DMA_SxCR |= 1 << HTIE;
	// Transfer error interrupt enable
	*DMA_SxCR |= 1 << TEIE;

//	// Direct mode error interrupt enable
//	*DMA_SxCR |= 1 << DMEIE;


	if ((direction == 0) | (direction == 2)) // memory to memory or peripheral to memory
	{
		*DMA_SxPAR |= (uint32_t) src ;
		*DMA_SxM0AR |= (uint32_t) dst ;
	}
	else if (direction == 1) // memory to peripheral
	{
		*DMA_SxM0AR |= (uint32_t) src ;
		*DMA_SxPAR |= (uint32_t) dst ;
	}
	*DMA_SxNDTR = number_of_transfers;
}

char transfer_state(void)
{
	if ( ((*DMA_LISR >> 5)&1) || ((*DMA_LISR >> 11)&1) || ((*DMA_LISR >> 21)&1) || ((*DMA_LISR >> 27)&1) == 1) //TCIFx, Stream x transfer complete interrupt flag (x = 3..0)
	{
		return 1;
	}
	else if ( ((*DMA_LISR >> 4)&1) || ((*DMA_LISR >> 10)&1) || ((*DMA_LISR >> 20)&1) || ((*DMA_LISR >> 26)&1) == 1) //HTIFx, Stream x half transfer interrupt flag (x=3..0)
	{
		return 2;
	}
	else if ( ((*DMA_LISR >> 3)&1) || ((*DMA_LISR >> 9)&1) || ((*DMA_LISR >> 19)&1) || ((*DMA_LISR >> 25)&1) == 1) // TEIFx, Stream x transfer error interrupt flag (x=3..0)
	{
		return 3;
	}

//	if ((*DMA2_LISR >> 5)&1 ==1 ) return 1; // TCIF0, Stream 0 transfer complete interrupt flag
//	if ((*DMA2_LISR >> 4)&1 ==1 ) return 2; // HTIF0, Stream 0 half transfer interrupt flag
//	if ((*DMA2_LISR >> 3)&1 ==1 ) return 3; // TEIF0, Stream 0 transfer error interrupt flag

	else if ( ((*DMA_HISR >> 5)&1) || ((*DMA_HISR >> 11)&1) || ((*DMA_HISR >> 21)&1) || ((*DMA_HISR >> 27)&1) == 1) //TCIFx, Stream x transfer complete interrupt flag (x=7..4)
	{
		return 1;
	}
	else if ( ((*DMA_HISR >> 4)&1) || ((*DMA_HISR >> 10)&1) || ((*DMA_HISR >> 20)&1) || ((*DMA_HISR >> 26)&1) == 1) //HTIFx, Stream x half transfer interrupt flag (x=7..4)
	{
		return 2;
	}
	else if ( ((*DMA_HISR >> 3)&1) || ((*DMA_HISR >> 9)&1) || ((*DMA_HISR >> 19)&1) || ((*DMA_HISR >> 25)&1) == 1) // TEIFx, Stream x transfer error interrupt flag (x=7..4)
	{
		return 3;
	}
	return 0;
}


void DMA_stream_enable()
{
	//ENABLE STREAM AND DMA

	// CLEAR ALL INTERRUPT FLAGS
	if (stream_number == 0)
	{
		*DMA_LIFCR = (1 << CFEIF0) | (1 << CDMEIF0) | (1 << CTEIF0) | (1 << CHTIF0) | (1 << CTCIF0);
	}
	else if (stream_number == 1)
	{
		*DMA_LIFCR = (1 << CFEIF1) | (1 << CDMEIF1) | (1 << CTEIF1) | (1 << CHTIF1) | (1 << CTCIF1);
	}
	else if (stream_number == 2)
	{
		*DMA_LIFCR = (1 << CFEIF2) | (1 << CDMEIF2) | (1 << CTEIF2) | (1 << CHTIF2) | (1 << CTCIF2);
	}
	else if (stream_number == 3)
	{
		*DMA_LIFCR = (1 << CFEIF3) | (1 << CDMEIF3) | (1 << CTEIF3) | (1 << CHTIF3) | (1 << CTCIF3);
	}
	else if (stream_number == 4)
	{
		*DMA_HIFCR = (1 << CFEIF4) | (1 << CDMEIF4) | (1 << CTEIF4) | (1 << CHTIF4) | (1 << CTCIF4);
	}
	else if (stream_number == 5)
	{
		*DMA_HIFCR = (1 << CFEIF5) | (1 << CDMEIF5) | (1 << CTEIF5) | (1 << CHTIF5) | (1 << CTCIF5);
	}
	else if (stream_number == 6)
	{
		*DMA_HIFCR = (1 << CFEIF6) | (1 << CDMEIF6) | (1 << CTEIF6) | (1 << CHTIF6) | (1 << CTCIF6);
	}
	else if (stream_number == 7)
	{
		*DMA_HIFCR = (1 << CFEIF7) | (1 << CDMEIF7) | (1 << CTEIF7) | (1 << CHTIF7) | (1 << CTCIF7);
	}

//	*DMA_LIFCR = (1 << CFEIF0) | (1 << CDMEIF0) | (1 << CTEIF0) | (1 << CHTIF0) | (1 << CTCIF0);

	//ENABLE DMA STREAM x
	*DMA_SxCR |= 1 << EN;

}


void DMA2_Stream0_IRQHandler (void)
{
//	for (int idx = 0; idx < 10 ; idx ++); // why???
	indication = transfer_state();
	callout(indication);

// clear the flag after finishing
	clear_flag (indication);
}

void clear_flag (char indication)
{
	switch (indication)
	{
	case 1:
		*DMA2_LIFCR = 1<< CTCIF0;
	case 2 :
		*DMA2_LIFCR = 1<< CHTIF0;
	case 3:
		*DMA2_LIFCR = 1<< CTEIF0;
	}
}
