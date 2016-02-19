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
//  Main program loop.
//
int main(void)
{
	int d;
	
	__disable_interrupt();
	InitialiseSystemClock();
	__enable_interrupt();
	// Configure pins
	PD_DDR = 0x01;
	PD_CR1 = 0x01;
	// Loop
	do {
		PD_ODR ^= 0x01;
		for(d = 0; d < 29000; d++) { }
	} while(1);
}
