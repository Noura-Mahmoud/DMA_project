/*
 * dma.h
 *
 *  Created on: Jan 8, 2022
 *      Author: noura
 */
#include <stdint.h>
#ifndef INC_DMA_H_
#define INC_DMA_H_

#define DMA1_CLK				   21
#define DMA2_CLK				   22

#define EN						   0 // enable bit
#define CHSEL					   25 // channel selection
#define DIR						   6 // direction
#define PL						   16 // priority level
#define FTH						   0 // FIFO threshold selection

#define PSIZE					   11
#define MSIZE					   13

#define byte 					   0 //00
#define half_word				   1 //01
#define word					   2 //10
#define DMDIS					   2

#define PINC					   9 // peripheral increment
#define MINC					   10 // memory increment
#define TCIE					   4 // Transfer complete interrupt
#define HTIE					   3 // Half transfer interrupt enable
#define TEIE					   2 // Transfer error interrupt enable
#define DMEIE					   1 // Direct mode error interrupt enable

#define fixed					   0
#define incremented				   1

#define CFEIF0					   0 //Stream 0 clear FIFO error interrupt flag
#define CDMEIF0					   2 //Stream 0 clear direct mode error interrupt flag
#define CTEIF0					   3 //Stream 0 clear transfer error interrupt flag
#define CHTIF0					   4 //Stream 0 clear half transfer interrupt flag
#define CTCIF0					   5 //Stream 0 clear transfer complete interrupt flag

#define CFEIF1					   6 //Stream 1 clear FIFO error interrupt flag
#define CDMEIF1					   8 //Stream 1 clear direct mode error interrupt flag
#define CTEIF1					   9 //Stream 1 clear transfer error interrupt flag
#define CHTIF1					   10 //Stream 1 clear half transfer interrupt flag
#define CTCIF1					   11 //Stream 1 clear transfer complete interrupt flag

#define CFEIF2					   16 //Stream 2 clear FIFO error interrupt flag
#define CDMEIF2					   18 //Stream 2 clear direct mode error interrupt flag
#define CTEIF2					   19 //Stream 2 clear transfer error interrupt flag
#define CHTIF2					   20 //Stream 2 clear half transfer interrupt flag
#define CTCIF2					   21 //Stream 2 clear transfer complete interrupt flag

#define CFEIF3					   22 //Stream 3 clear FIFO error interrupt flag
#define CDMEIF3					   24 //Stream 3 clear direct mode error interrupt flag
#define CTEIF3					   25 //Stream 3 clear transfer error interrupt flag
#define CHTIF3					   26 //Stream 3 clear half transfer interrupt flag
#define CTCIF3					   27 //Stream 3 clear transfer complete interrupt flag

#define CFEIF4					   0 //Stream 4 clear FIFO error interrupt flag
#define CDMEIF4					   2 //Stream 4 clear direct mode error interrupt flag
#define CTEIF4					   3 //Stream 4 clear transfer error interrupt flag
#define CHTIF4					   4 //Stream 4 clear half transfer interrupt flag
#define CTCIF4					   5 //Stream 4 clear transfer complete interrupt flag

#define CFEIF5					   6 //Stream 5 clear FIFO error interrupt flag
#define CDMEIF5					   8 //Stream 5 clear direct mode error interrupt flag
#define CTEIF5					   9 //Stream 5 clear transfer error interrupt flag
#define CHTIF5					   10 //Stream 5 clear half transfer interrupt flag
#define CTCIF5					   11 //Stream 5 clear transfer complete interrupt flag

#define CFEIF6					   16 //Stream 6 clear FIFO error interrupt flag
#define CDMEIF6					   18 //Stream 6 clear direct mode error interrupt flag
#define CTEIF6					   19 //Stream 6 clear transfer error interrupt flag
#define CHTIF6					   20 //Stream 6 clear half transfer interrupt flag
#define CTCIF6					   21 //Stream 6 clear transfer complete interrupt flag

#define CFEIF7					   22 //Stream 7 clear FIFO error interrupt flag
#define CDMEIF7					   24 //Stream 7 clear direct mode error interrupt flag
#define CTEIF7					   25 //Stream 7 clear transfer error interrupt flag
#define CHTIF7					   26 //Stream 7 clear half transfer interrupt flag
#define CTCIF7					   27 //Stream 7 clear transfer complete interrupt flag

#define one_quarter_FIFO		   0 //00 // 1/4 full fifo
#define half_FIFO			   	   1 //01 // 1/2 full fifo
#define three_quarters_FIFO		   2 //10 // 3/4 full fifo
#define full_FIFO			  	   3 //11

#define FIFO_mode				   2
#define direct_mode				   3

#define channel_0				   0 //000
#define channel_1				   1 //001
#define channel_2				   2 //010
#define channel_3				   3 //011
#define channel_4				   4 //100
#define channel_5				   5 //101
#define channel_6				   6 //110
#define channel_7				   7 //111

#define PL_Low					   0 //00
#define PL_Medium				   1 //01
#define PL_High					   2 //10
#define PL_Very_high			   3 //11

#define Peripheral_to_memory	   0 //00
#define Memory_to_peripheral	   1 //01
#define Memory_to_memory		   2 //10



#define DMA_REG(PORT_ID,REG_ID)   (unsigned int*)(PORT_ID + REG_ID) //The Generic macro
#define RCC_AHB1ENR                DMA_REG(0x40023800 ,0x30) //RCC AHB1 peripheral clock enable register

#define NVIC_ISER0				   DMA_REG(0xE000E100 , 0x00) //Interrupt set-enable register 0
#define NVIC_ISER1				   DMA_REG(0xE000E100 , 0x04) //Interrupt set-enable register 1
#define NVIC_ISER2				   DMA_REG(0xE000E100 , 0x08) //Interrupt set-enable register 2

//DMA2 stream 0
#define DMA2_S0CR       	  	   DMA_REG(0x40026400 ,0x10) //DMA2 stream 0 configuration register
#define DMA2_S0FCR       	  	   DMA_REG(0x40026400 ,0x24) //DMA2 stream 0 FIFO control register
#define DMA2_S0PAR				   DMA_REG(0x40026400 ,0x18) //DMA2 stream 0 peripheral address register
#define DMA2_S0M0AR				   DMA_REG(0x40026400 ,0x1C) //DMA2 stream 0 memory 0 address register
#define DMA2_S0M1AR				   DMA_REG(0x40026400 ,0x20) //DMA2 stream 0 memory 1 address register, in case of double buffer mode
#define DMA2_S0NDTR				   DMA_REG(0x40026400 ,0x14) //DMA2 stream 0 number of data register, it determines no. of transfers
#define DMA2_LIFCR				   DMA_REG(0x40026400 ,0x08) //DMA2 stream 0 low interrupt flag clear register
#define DMA2_LISR				   DMA_REG(0x40026400 ,0x00) //DMA low interrupt status register

//
////DMA1 stream 0
//#define DMA1_S0CR       	  	   DMA_REG(0x40026000 ,0x10) //DMA1 stream 0 configuration register
//#define DMA1_S0FCR       	  	   DMA_REG(0x40026000 ,0x24) //DMA1 stream 0 FIFO control register
//#define DMA1_S0PAR				   DMA_REG(0x40026000 ,0x18) //DMA1 stream 0 peripheral address register
//#define DMA1_S0M0AR				   DMA_REG(0x40026000 ,0x1C) //DMA1 stream 0 memory 0 address register
//#define DMA1_S0M1AR				   DMA_REG(0x40026000 ,0x20) //DMA1 stream 0 memory 1 address register, in case of double buffer mode
//#define DMA1_S0NDTR				   DMA_REG(0x40026000 ,0x14) //DMA1 stream 0 number of data register, it determines no. of transfers
//#define DMA1_LIFCR				   DMA_REG(0x40026000 ,0x08) //DMA1 stream 0 low interrupt flag clear register


void stream_init (char stream_no, const uint32_t *src, const uint32_t *dst, const uint32_t number_of_transfers, char transfer_item_size,
		char transfer_mode, char channel,char priority, char direction, char fifo_threshold, char PINC_mode, char MINC_mode);

void DMA_init(char DMA_NO);

char transfer_state(void);
void DMA_stream_enable();

void isr_DMA2_stream0(void);
void callout(char state);
void clear_flag (char indication);

#endif /* INC_DMA_H_ */
