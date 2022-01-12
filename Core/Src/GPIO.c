#include "GPIO.h"

// #include "stm32f401cc_interface.h"

#define _OUTTYPE_MASK ((unsigned char)0x01)
#define _OUTTYPE_SHIFT 0

#define _PUPD_MASK ((unsigned char)0x06)
#define _PUPD_SHIFT 1


//unsigned int PORT[] = {0x40020000 , 0x40020400}; //Port A, Port B

#define GPIO_REG(PORT_ID,REG_ID)  (unsigned int*)(PORT_ID + REG_ID) //The Generic macro

#define MODER  0
#define OTYPER  1
#define OSPEEDR  2
#define PUPDR  3
#define IDR  4
#define ODR  5
#define BSRR  6
#define LCKR  7
#define AFRL  8
#define AFRH  9

#define RCC_AHB1ENR GPIO_REG(0x40023800 , 0x30)
//GPIOA
#define GPIOA_MODER 	GPIO_REG(0x40020000 , 0x00)
#define GPIOA_OTYPER 	GPIO_REG(0x40020000 , 0x04)
#define GPIOA_OSPEEDR   GPIO_REG(0x40020000 , 0x08)
#define GPIOA_PUPDR 	GPIO_REG(0x40020000 , 0x0C)
#define GPIOA_ODR 		GPIO_REG(0x40020000 , 0x10)
#define GPIOA_IDR 		GPIO_REG(0x40020000 , 0x14)
#define GPIOA_BSRR 		GPIO_REG(0x40020000 , 0x18)
#define GPIOA_LCKR 		GPIO_REG(0x40020000 , 0x1C)
#define GPIOA_AFRL 		GPIO_REG(0x40020000 , 0x20)
#define GPIOA_AFRH 		GPIO_REG(0x40020000 , 0x24)

//GPIOB
#define GPIOB_MODER 	GPIO_REG(0x40020400 , 0x00)
#define GPIOB_OTYPER 	GPIO_REG(0x40020400 , 0x04)
#define GPIOB_OSPEEDR   GPIO_REG(0x40020400 , 0x08)
#define GPIOB_PUPDR 	GPIO_REG(0x40020400 , 0x0C)
#define GPIOB_ODR 		GPIO_REG(0x40020400 , 0x10)
#define GPIOB_IDR 		GPIO_REG(0x40020400 , 0x14)
#define GPIOB_BSRR 		GPIO_REG(0x40020400 , 0x18)
#define GPIOB_LCKR 		GPIO_REG(0x40020400 , 0x1C)
#define GPIOB_AFRL 		GPIO_REG(0x40020400 , 0x20)
#define GPIOB_AFRH 		GPIO_REG(0x40020400 , 0x24)

unsigned int* lookup[2][10] = {
    {GPIOA_MODER, GPIOA_OTYPER , GPIOA_OSPEEDR, GPIOA_PUPDR ,GPIOA_ODR , GPIOA_IDR, GPIOA_BSRR  ,GPIOA_LCKR , GPIOA_AFRL, GPIOA_AFRH } ,
    {GPIOB_MODER, GPIOB_OTYPER , GPIOB_OSPEEDR, GPIOB_PUPDR ,GPIOB_ODR , GPIOB_IDR, GPIOB_BSRR  ,GPIOB_LCKR , GPIOB_AFRL, GPIOB_AFRH }
};

void GPIO_EnableClock(unsigned char PORT_ID) {
    *RCC_AHB1ENR |= (0x01 << PORT_ID);
    // to enable the clock of specific port
}

void GPIO_Init(unsigned char PORT_ID, unsigned char PinNum, unsigned char PinMode,
               unsigned char DefaultState)
  {
    *RCC_AHB1ENR |= (0x01 << PORT_ID); // to enable the clock of the port
    // Configure moder register for pin direction
	*lookup[PORT_ID][MODER] &= ~(0x03 << 2 * PinNum);
    *lookup[PORT_ID][MODER] |= PinMode << 2 * PinNum;

    *lookup[PORT_ID][OTYPER]  &= ~(1 << PinNum);
    *lookup[PORT_ID][OTYPER]  |= ((DefaultState & _OUTTYPE_MASK) >> _OUTTYPE_SHIFT) << PinNum;

    *lookup[PORT_ID][PUPDR]  &= ~(0x03 << 2 * PinNum);
    *lookup[PORT_ID][PUPDR]  |= ((DefaultState & _PUPD_MASK) >> _PUPD_SHIFT) << (2 * PinNum);
  }


unsigned char GPIO_WritePin(unsigned char PORT_ID, unsigned char PinNum,
                            unsigned char Data) {
  unsigned char result;
      if (((*lookup[PORT_ID][MODER] & (0x03 << 2 * PinNum)) >> (2 * PinNum)) == 1) {
        if (Data) {
          *lookup[PORT_ID][ ODR] |= (1 << PinNum);
        } else {
          *lookup[PORT_ID][ ODR] &= ~(1 << PinNum);
        }
        result = OK;
      } else {
        result = NOK;
      }
  return result;
}

unsigned char GPIO_ReadPin(unsigned char PORT_ID, unsigned char PinNum) {
  unsigned char data = 0;
    data = ((*lookup[PORT_ID][IDR] & (1 << PinNum)) >> PinNum);

  return data;
}

void delay_ms(int ms)
{
	for(int i=0;i<ms*1000;i++);
}

