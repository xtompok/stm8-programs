#include "stm8s.h"

//
//  Setup the system clock to run at 16MHz using the internal oscillator.
//
void InitialiseSystemClock()
{
CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
CLK_ICKR = CLK_ICKR_HSIEN;                 //  Enable the HSI.
CLK_ECKR = 0;                       //  Disable the external clock.
while (!(CLK_ICKR & CLK_ICKR_HSIRDY));       //  Wait for the HSI to be ready for use.
CLK_CKDIVR = 0;                     //  Ensure the clocks are running at full speed.
CLK_PCKENR1 = 0xff;                 //  Enable all peripheral clocks.
CLK_PCKENR2 = 0xff;                 //  Ditto.
CLK_CCOR = 0;                       //  Turn off CCO.
CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.
CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
CLK_SWCR = 0;                       //  Reset the clock switch control register.
CLK_SWCR = CLK_SWCR_SWEN;                  //  Enable switching.
while (CLK_SWCR & CLK_SWCR_SWBSY);        //  Pause while the clock switch is busy.
}

//
//  Setup the USART to run at 115200 baud, no parity, one stop bit, 8 data bits.
//
//  Important: This relies upon the system clock being set to run at 16 MHz.
//
void InitialiseUSART()
{
    //
    //  Clear the Idle Line Detected bit in the status register by a read
    //  to the USART1_SR register followed by a Read to the USART1_DR register.
    //
    unsigned char tmp = USART1_SR;
    tmp = USART1_DR;
    //
    //  Reset the USART registers to the reset values.
    //
    USART1_CR1 = 0;
    USART1_CR2 = 0;
    USART1_CR4 = 0;
    USART1_CR3 = 0;
    USART1_CR5 = 0;
    USART1_GTR = 0;
    USART1_PSCR = 0;
    //
    //  Now setup the port to 115200,n,8,1.
    //
    USART1_CR1 &= ~USART_CR1_M;        //  8 Data bits.
    USART1_CR1 &= ~USART_CR1_PCEN;     //  Disable parity.
    USART1_CR3 &= ~(USART_CR3_STOP1|USART_CR3_STOP2) ;     //  1 stop bit.
    USART1_BRR2 = 0x0b;      //  Set the baud rate registers to 115200 baud
    USART1_BRR1 = 0x08;      //  based upon a 16 MHz system clock.
    //
    //  Disable the transmitter and receiver.
    //
    USART1_CR2 &= ~(USART_CR2_TEN|USART_CR2_REN);
    //
    //  Set the clock polarity and lock phase.
    //
    USART1_CR3 = (USART_CR3_CPOL|USART_CR3_CPHA);
    //
    //  Turn on the USART transmit and receive.
    //
    USART1_CR2 |= (USART_CR2_TEN|USART_CR2_REN);
}

//
//  Send a message to the debug port (USART1).
//
void USARTPrintf(char *message)
{
    char *ch = message;
    while (*ch)
    {
        USART1_DR = (unsigned char) *ch;     //  Put the next character into the data transmission register.
        while (!(USART1_SR & USART_SR_TXE));          //  Wait for transmission to complete.
        ch++;                               //  Grab the next character.
    }
}

 
//
//  Main program loop.
//
int main(void)
{
	int d;
	
	__disable_interrupt();
	InitialiseSystemClock();
	InitialiseUSART();
	__enable_interrupt();
	// Configure pins
	PD_DDR = 0x01;
	PD_CR1 = 0x01;
	// Loop
	do {
		PD_ODR ^= 0x01;
		for(d = 0; d < 29000; d++) { }
		USARTPrintf("Hello from my microcontroller....\n\r");
	} while(1);
}
