


#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
void Timer1_Init(){
	//set first 4 pins of Port C to output
	DDRC |= 0x0F;
	PORTC &= ~(0x0F);

	//Non-PWM Mode
	TCCR1A = (1<<FOC1A);

	/* CTC Mode
	 * Prescaler = 1024
	 */
	TCCR1B |= (1<<WGM12) | (1<<CS10) | (1<<CS12);

	//Set initial value to 0
	TCNT1 = 0;

	//Set top value to 1000 (1 sec)
	OCR1A =1000 ;

	//Enable global interrupt
	SREG |=(1<<7);

	//Enable Interrupts
	TIMSK |= (1<<OCIE1A);
}

void INT0_Init(){

	//Set PD2 as input for button 1 with internal pull-up
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);

	// Falling edge generates interrupt
	MCUCR |= (1<<ISC01);

	// Enable external interrupt
	GICR |= (1<<INT0);

	//Enable global interrupt
	SREG |=(1<<7);
}

void INT1_Init(){

	//SET PD3 as input for button 2
	DDRD &= ~(1<<PD3);

	// Falling edge generates interrupt
	MCUCR |= (1<<ISC11) | (1<<ISC10);

	// Enable external interupt
	GICR |= (1<<INT1);

	//Enable global interrupt
	SREG |=(1<<7);
}

void INT2_Init(){

	//Set PD2 as input for button 3 with internal pull-up
	DDRD &= ~(1<<PB2);
	PORTD |= (1<<PB2);

	// Falling edge generates interrupt
	MCUCSR &= ~(1<<ISC2);

	// Enable external interupt
	GICR |= (1<<INT2);

	//Enable global interrupt
	SREG |=(1<<7);
}





unsigned char sec = 0;
unsigned char min = 0;
unsigned char hour = 0;

int main(){

	//Set first 6 pins of Port A as output
	//for 7 segments enable/disable
	DDRA |= 0x3F;
	PORTA = 0;

	Timer1_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	while(1){
		PORTA = (1<<5);
		PORTC = sec%10;
		_delay_ms(5);

		PORTA = (1<<4);
		PORTC = sec/10;
		_delay_ms(5);

		PORTA = (1<<3);
		PORTC = min%10;
		_delay_ms(5);

		PORTA = (1<<2);
		PORTC = min/10;
		_delay_ms(5);

		PORTA = (1<<1);
		PORTC = hour%10;
		_delay_ms(5);

		PORTA = (1<<0);
		PORTC = hour/10;
		_delay_ms(5);

	}
}

ISR(TIMER1_COMPA_vect){

	sec++;
	if(sec == 60){
		sec = 0;
		min++;
	}
	if(min == 60){
		min = 0;
		hour++;
	}

}

ISR(INT0_vect){

	sec = 0;
	min = 0;
	hour = 0;
}

ISR(INT1_vect)//ISR To pause stopwatch
{

	TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);
}

ISR(INT2_vect)//ISR To resume stopwatch
{

	TCCR1B |= (1<<CS12) | (1<<CS10);
}


