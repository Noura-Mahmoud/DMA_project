//#ifndef INC_GPIO_H_
//#define INC_GPIO_H_
//
//#include "stm32f401xc.h"
//
////write result
//#define OK  ((unsigned char)0)
//#define NOK ((unsigned char)1)
//
////mode types
//#define INPUT ((unsigned int)0x00)
//#define OUTPUT ((unsigned int)0x01)
//#define ALTERNATE_FUN ((unsigned int)0x02)
//#define ANALOG ((unsigned int)0x03)
//
////output modes
//#define PUSH_PULL ((unsigned int)0x00)
//#define OPEN_DRAIN ((unsigned int)0x01)
//
////input modes
//#define PULL_UP	 ((unsigned int)0x01)
//#define PULL_DOWN ((unsigned int)0x02)
//
////value
//#define LOW 0
//#define HIGH 1
//
//
//void GPIO_Init( char Port, unsigned int PIN_NO, unsigned int PIN_Dir, unsigned int Default_State);
//unsigned char GPIO_WritePin(char Port, unsigned int PIN_NO, unsigned int Data);
//unsigned char GPIO_ReadPin(char Port, unsigned int PIN_NO);
//
//#endif /* INC_GPIO_H_ */


#ifndef GPIO_H
#define GPIO_H

// #include "stm32f401xc.h"

// Result Status
#define OK ((unsigned char)0)
#define NOK ((unsigned char)1)

// Mode Types
#define INPUT ((unsigned char)0x00)
#define OUTPUT ((unsigned char)0x01)
#define ALTERNATE_FUN ((unsigned char)0x02)
#define ANALOG ((unsigned char)0x03)

// Output modes
#define PUSH_PULL ((unsigned char)0x00)
#define OPEN_DRAIN ((unsigned char)0x01)

// Resistor modes
#define NO_PULL_UP_DOWN ((unsigned char)0x00)
#define PULL_UP ((unsigned char)0x02)
#define PULL_DOWN ((unsigned char)0x04)

//value
#define LOW 0
#define HIGH 1


void GPIO_EnableClock(unsigned char PORT_ID);

void GPIO_Init(unsigned char PORT_ID, unsigned char PinNum, unsigned char PinDir,
               unsigned char DefaultState);
unsigned char GPIO_WritePin(unsigned char PORT_ID, unsigned char PinNum,
                            unsigned char Data);

unsigned char GPIO_ReadPin(unsigned char PORT_ID, unsigned char PinNum);
void delay_ms(int ms);

#endif /* GPIO_H */
