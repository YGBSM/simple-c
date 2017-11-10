#include <msp430.h> 
#include <msp430g2231.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * main.c
 */

#define RSTBTN 	BIT1
#define PLAY1	BIT3
#define PLAY2	BIT2
#define LED1	BIT0
#define LED2	BIT6
#define BUTTONS	BIT3 + BIT2

// Delay in ms
#define DLY		1

unsigned char PORT = 0x00;
int FLAG = 0;
int i = 0;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    //calib DCO
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //port setup
    P1DIR |= BIT0 + BIT6;
    P1DIR &= ~(PLAY1+PLAY2);

    P1REN |= RSTBTN;

    P1OUT &= 0x00;
	
    P1IE |= PLAY1 + PLAY2;
    P1IES |= PLAY1 + PLAY2;
    P1IFG &= ~PLAY1 + PLAY2;

    //timer setup
    TACTL |= (TASSEL_2 + ID_0 + MC_1 + TACLR);
    TACTL &= ~(TAIFG);
    TACCTL0 |= (CCIE);
    TACCR0 = 1000; // 1ms

    int keyPressed();
    int debounceSwitch();

    _enable_interrupt();

    while(1)
    {

    	/*
    	PORT = P1IN;
    	if (((PORT & PLAY1) == 0x00) && !FLAG)
    	{
    		P1OUT ^= LED1;
    		FLAG = 0x01;
    		//__delay_cycles(500000);
    	}

    	//PORT = P1IN;
    	if (((PORT & PLAY2) == 0x00) && !FLAG)
    	{
    		P1OUT ^= LED2;
    		FLAG = 1;
    	}

    	if ((RSTBTN & PORT) == 0x00)
    	{
    		P1OUT = 0x00;
    		FLAG = 0x00;
    	}*/
 }
}
    int keyPressed()
    {
    	PORT = P1IN;
    	if((PORT & PLAY1)==0x00)
    	{
    		return 1;
    	}
    	return 0;
    }

    int debounceSwitch()
    {
    	static uint16_t State = 0; //current debounce status
    	State = (State<<1) | !keyPressed() | 0xE000;
    	if (State == 0xF000)
    	{
    		return 1;
    	}
    	return 0;
    }


#pragma vector = TIMERA0_VECTOR
__interrupt void CCRO_ISR(void)
{

	if(++i == DLY)
	{
		int conditionedPress =0;
		debounceSwitch();
		if (conditionedPress)
		{
			P1OUT ^= LED1;
		}
		i = 0;
	}
}


