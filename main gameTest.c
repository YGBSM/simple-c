#include <msp430.h> 
#include <msp430g2231.h>

#define RSTBTN 	BIT1

#define PLAY1	BIT3
#define PLAY2	BIT2
#define PLAY3	BIT4
#define PLAY4	BIT5

#define LED1	BIT0
#define LED2	BIT6
#define LED3	BIT7
#define LED4	BIT8

unsigned char PORT = 0x00;
unsigned char FLAG = 0;
unsigned char cnt1 = 0;
unsigned char cnt2 = 0;
unsigned char cnt3 = 0;
unsigned char cnt4 = 0;

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    //calib DCO
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    //port setup
    P1DIR |= BIT0 + BIT6;
    P1DIR &= ~(PLAY1 + PLAY2);

    //P1REN |= RSTBTN;
    //P1REN |= PLAY1 + PLAY2;

    P1OUT |= 0xFF;

    //timer setup
    TACCTL0 = CCIE;
    TACCR0 = 1000; // 1ms
    TACTL = (TASSEL_2 + ID_0 + MC_1);

    _BIS_SR(LPM0_bits + GIE);
    //_enable_interrupt();

    while(1)
    {
    	//flag is true when one of the buttons was pressed
    	while(FLAG == 1)
    	{
    		_disable_interrupt();
    		//wait until the reset button is pressed...
    		while ((RSTBTN & P1IN) != 0x00)
    		{}
    		//... then reset the game
    		FLAG = 0;
    		PORT = 0x00;
    		P1OUT |= 0xFF;
    		cnt1 = 20;
    		cnt2 = 20;
    		cnt3 = 20;
    		cnt4 = 20;

    		_BIS_SR(GIE + LPM0_bits);
    		//_enable_interrupt();
    	}
    }
}

#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	__disable_interrupt();

	PORT = P1IN;

	//figure out who pushed button first
	//turn on LED
	if(cnt1== 0)
	{
		P1OUT ^= LED1;
		FLAG = 1;
		_BIC_SR(LPM0_EXIT);
	}

	if(cnt2 == 0)
	{
		P1OUT ^= LED2;
		FLAG = 1;
		_BIC_SR(LPM0_EXIT);
	}

	if(cnt3 == 0)
	{
		P1OUT ^= LED3;
		FLAG = 1;
		_BIC_SR(LPM0_EXIT);
	}

	if(cnt4 == 0)
	{
		P1OUT ^= LED4;
		FLAG = 1;
		_BIC_SR(LPM0_EXIT);
	}
	__enable_interrupt();
}

void debounceInputs(int PORT, unsigned char cnt1, unsigned char cnt2, unsigned char cnt3, unsigned char cnt4)
{
	//debounce the buttons
	if ((PORT & PLAY1)==0x00)
	{
		cnt1--;
	}
	else
	{
		cnt1 = 20;
	}

	if ((PORT & PLAY2)==0x00)
	{
		cnt2--;
	}
	else
	{
		cnt2 = 20;
	}

	if ((PORT & PLAY3)==0x00)
	{
		cnt3--;
	}
	else
	{
		cnt3 = 20;
	}

	if ((PORT & PLAY4)==0x00)
	{
		cnt4--;
	}
	else
	{
		cnt4 = 20;
	}//debounce
}

void flashLED(void)
{

}
