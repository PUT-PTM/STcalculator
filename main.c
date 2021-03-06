#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"

#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h"
#include "OurOwnFunctions.h"
#include "interpreter.h"

//keypad
//without interrupts, columns
uint16_t pin1= GPIO_Pin_4; //right keyboard pin
GPIO_TypeDef* port1= GPIOB;

uint16_t pin2= GPIO_Pin_5;
GPIO_TypeDef* port2= GPIOB;

uint16_t pin3= GPIO_Pin_7;
GPIO_TypeDef* port3= GPIOD;

uint16_t pin4= GPIO_Pin_6;
GPIO_TypeDef* port4= GPIOD;


//with interrupts, rows
uint16_t pin5= GPIO_Pin_3;
GPIO_TypeDef* port5= GPIOA;

uint16_t pin6= GPIO_Pin_2;
GPIO_TypeDef* port6= GPIOD;

uint16_t pin7= GPIO_Pin_1;
GPIO_TypeDef* port7= GPIOD;

uint16_t pin8= GPIO_Pin_0;//left keyboard pin
GPIO_TypeDef* port8= GPIOD;


int display_period = 99;
int contrast_period = 39;
uint8_t zasilany_pin = 0;
char znak = 0;
char napis_shift[2]="  ";
char napis_display[29]="1+20.1*2^3+4";
signed int dlugoscnapis_display = 12;
//char napis_display[29]="2*sin30+cos60-tg45*ctg45";
//signed int dlugoscnapis_display = 24;
int shift=0;

int main(void)
{
    //Initialize system
    SystemInit();

	stmkonf_pinu_jako_wyjscie(port1, pin1);
	stmkonf_pinu_jako_wyjscie(port2, pin2);
	stmkonf_pinu_jako_wyjscie(port3, pin3);
	stmkonf_pinu_jako_wyjscie(port4, pin4);
	stmkonf_pinu_jako_wejscie_down(port5, pin5);
	stmkonf_pinu_jako_wejscie_down(port6, pin6);
	stmkonf_pinu_jako_wejscie_down(port7, pin7);
	stmkonf_pinu_jako_wejscie_down(port8, pin8);


	stmkonf_timera(TIM3, 8399, 199);//timer przelaczajacy zasilanie pomiedzy pin1 2 3 4
	stmkonf_NVIC_timera(TIM3, TIM3_IRQn);
	stmwlacz_timer(TIM3);

	stmkonf_timera(TIM4, 839, display_period);//timer od regulacji jasnosci wyswietlacza
	stmkonf_PWM_i_PB8_dla_TIM4_CH3(display_period, 50);
	stmwlacz_timer(TIM4);

	stmkonf_timera(TIM3, 839, contrast_period);//timer od regulacji kontrastu znakow
	stmkonf_PWM_i_PB0_dla_TIM3_CH3(contrast_period, 0);
	stmwlacz_timer(TIM3);

	stmkonf_timera(TIM5, 8399, 1000);//redukcja drgan stykow
	stmkonf_NVIC_timera(TIM5, TIM5_IRQn);

	//po zmianie EXTI3 z GPIOD na GPIOA, albo po usunieciu - wyswietlacz przestaje dzialac
	stmkonf_EXTI(EXTI3_IRQn, EXTI_Line3, EXTI_PortSourceGPIOA, EXTI_PinSource3);//przerwanie czekajace na dane z pin5
	stmkonf_EXTI(EXTI2_IRQn, EXTI_Line2, EXTI_PortSourceGPIOD, EXTI_PinSource2);//przerwanie czekajace na dane z pin6
	stmkonf_EXTI(EXTI1_IRQn, EXTI_Line1, EXTI_PortSourceGPIOD, EXTI_PinSource1);//przerwanie czekajace na dane z pin7
	stmkonf_EXTI(EXTI0_IRQn, EXTI_Line0, EXTI_PortSourceGPIOD, EXTI_PinSource0);//przerwanie czekajace na dane z pin8

    //Initialize LCD 20 cols x 4 rows
    TM_HD44780_Init(16, 2);

	TM_HD44780_Clear();//Clear Display
    //Put string to LCD
    TM_HD44780_Puts(0, 0, napis_shift);
    TM_HD44780_Puts(2, 0, napis_display);

    //Show cursor
    TM_HD44780_CursorOn();

    //Enable cursor blinking
    TM_HD44780_BlinkOff();

    while (1)
    {}
}

void EXTI3_IRQHandler ( void )//przerwanie czekajace na dane z pin5
{
	if (EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		EXTI_ClearITPendingBit(EXTI_Line2);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line1);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void EXTI2_IRQHandler ( void )//przerwanie czekajace na dane z pin6
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		EXTI_ClearITPendingBit(EXTI_Line3);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line1);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void EXTI1_IRQHandler ( void )//przerwanie czekajace na dane z pin7
{
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		EXTI_ClearITPendingBit(EXTI_Line3);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line2);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void EXTI0_IRQHandler ( void )//przerwanie czekajace na dane z pin8
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		stmwylacz_timer(TIM3);//wylacza przelaczanie zasilania na piny
		EXTI_ClearITPendingBit(EXTI_Line3);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line2);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line1);// wyzerowanie flagi wyzwolonego przerwania
		stmwlacz_timer(TIM5);//czeka 1/8 sekundy i dopiero sprawdza stan pinu
	}
}

void TIM5_IRQHandler ( void )//czeka 1/8 sekundy i dopiero sprawdza stan pinu// timer do redukcji drgan stykow
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if(GPIO_ReadInputDataBit(port5, pin5) != RESET)//rzad najnizszy
		{
			switch(zasilany_pin%4)
			{
			case 0:
			{
				shift^=1;// * na klawiaturze, ma byc shift
				if(shift)
				{
					napis_shift[0]='s';
				}
				else
				{napis_shift[0]=' ';}//usun S z napis_displayu bo wylaczylismy shift
				break;
			}
			case 1:
			{
				if(dlugoscnapis_display < n)
				{
					if(shift)
					{
						znak = pierwiastek; //symbol pierwiastkowania
						shift=0;
						napis_shift[0]=' ';//usun S z napis_displayu bo wylaczylismy shift
					}
					else
					{
						znak = '/';//na klawiaturze D
					}
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			case 2:
			{
				//ma byc =, na klawiaturze #
				dlugoscnapis_display = interpreter(napis_display, dlugoscnapis_display);
				break;
			}
			case 3:
			{
				if(dlugoscnapis_display < n)
				{
					if(shift)
					{
						znak = '.';
						shift=0;
						napis_shift[0]=' ';//usun S z napis_displayu bo wylaczylismy shift
					}
					else
					{
						znak = '0';
					}
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			default:
				break;
			}
			TM_HD44780_Clear();//Clear Display
		    TM_HD44780_Puts(0, 0, napis_shift);
		    TM_HD44780_Puts(2, 0, napis_display);//put text on display
		}
		else if(GPIO_ReadInputDataBit(port6, pin6) != RESET && dlugoscnapis_display<n)
		{
			switch(zasilany_pin%4)
			{
			case 0:
			{
				znak = '7';
				break;
			}
			case 1:
			{
				if(shift)
				{
					znak = '^';
					shift=0;
					napis_shift[0]=' ';//usun s z napis_shift bo wylaczylismy shift
				}
				else
				{
					znak = '*';//mnozenie, na klawiaturze C
				}
				break;
			}
			case 2:
			{
				znak = '9';
				break;
			}
			case 3:
			{
				znak = '8';
				break;
			}
			default:
				break;
			}
			napis_display[dlugoscnapis_display] = znak;
			dlugoscnapis_display++;
			TM_HD44780_Clear();//Clear Display
		    TM_HD44780_Puts(0, 0, napis_shift);
		    TM_HD44780_Puts(2, 0, napis_display);//put text on display
		}
		else if(GPIO_ReadInputDataBit(port7, pin7) != RESET && dlugoscnapis_display<n)
		{
			switch(zasilany_pin%4)
			{
			case 0:
			{
				if(shift)
				{
					napis_display[dlugoscnapis_display] = 's';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 'i';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 'n';
					dlugoscnapis_display++;
					shift=0;
					napis_shift[0]=' ';//usun s z napis_shift bo wylaczylismy shift
				}
				else
				{
					znak = '4';
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			case 1:
			{
				if(shift)
				{
					napis_display[dlugoscnapis_display] = 'c';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 't';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 'g';
					dlugoscnapis_display++;
					shift=0;
					napis_shift[0]=' ';//usun s z napis_shift bo wylaczylismy shift
				}
				else
				{
					znak = '-';//na klawiaturze B
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			case 2:
			{
				if(shift)
				{
					napis_display[dlugoscnapis_display] = 't';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 'g';
					dlugoscnapis_display++;
					shift=0;
					napis_shift[0]=' ';//usun s z napis_shift bo wylaczylismy shift
				}
				else
				{
					znak = '6';
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			case 3:
			{
				if(shift)
				{
					napis_display[dlugoscnapis_display] = 'c';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 'o';
					dlugoscnapis_display++;
					napis_display[dlugoscnapis_display] = 's';
					dlugoscnapis_display++;
					shift=0;
					napis_shift[0]=' ';//usun s z napis_shift bo wylaczylismy shift
				}
				else
				{
					znak = '5';
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			default:
				break;
			}
			TM_HD44780_Clear();//Clear Display
		    TM_HD44780_Puts(0, 0, napis_shift);
		    TM_HD44780_Puts(2, 0, napis_display);//put text on display
		}
		else if(GPIO_ReadInputDataBit(port8, pin8) != RESET)//////////////////////////////////////////
		{
			switch(zasilany_pin%4)
			{
			case 0:
			{
				if(shift)
				{//kasuj ostatni znak
					dlugoscnapis_display--;

					if(napis_display[dlugoscnapis_display]>='a' && napis_display[dlugoscnapis_display]<='z')
					{
						while(napis_display[dlugoscnapis_display]>='a' && napis_display[dlugoscnapis_display]<='z')
						{
							napis_display[dlugoscnapis_display]='\0';
							dlugoscnapis_display--;
						}
						dlugoscnapis_display++;
					}
					else
					{
						napis_display[dlugoscnapis_display]='\0';
					}
					shift=0;
					napis_shift[0]=' ';//usun S z napis_displayu bo wylaczylismy shift
				}
				else if(dlugoscnapis_display < n)
				{
					znak = '1';
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}

				break;
			}
			case 1:
			{
				if(shift)
				{
					int duty = simple_charstring_to_int(napis_display, dlugoscnapis_display);
					TIM3->CCR3=duty*(contrast_period+1)/100;//timer od regulacji kontrastu znakow
					shift=0;
					napis_shift[0]=' ';//usun S z napis_displayu bo wylaczylismy shift
				}
				else if(dlugoscnapis_display < n)
				{
					znak = '+';//na klawiaturze A
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			case 2:
			{
				if(shift)
				{
					int duty = simple_charstring_to_int(napis_display, dlugoscnapis_display);
					TIM4->CCR3=duty*(display_period+1)/100;//timer od regulacji jasnosci wyswietlacza
					shift=0;
					napis_shift[0]=' ';//usun S z napis_displayu bo wylaczylismy shift
				}
				else if(dlugoscnapis_display < n)
				{
					znak = '3';
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			case 3:
			{
				if(shift)
				{//kasuj wszystkie
					for(int c=0; c<n; c++)
					{napis_display[c] = '\0';}
					dlugoscnapis_display=0;
					shift=0;
					napis_shift[0]=' ';//usun S z napis_displayu bo wylaczylismy shift
				}
				else if(dlugoscnapis_display < n)
				{
					znak = '2';
					napis_display[dlugoscnapis_display] = znak;
					dlugoscnapis_display++;
				}
				break;
			}
			default:
				break;
			}
				   
	   
			TM_HD44780_Clear();//Clear Display
		    TM_HD44780_Puts(0, 0, napis_shift);
		    TM_HD44780_Puts(2, 0, napis_display);//put text on display
		}
		EXTI_ClearITPendingBit(EXTI_Line3);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line2);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line1);// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);// wyzerowanie flagi wyzwolonego przerwania
		stmwylacz_timer(TIM5);
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
		stmwlacz_timer(TIM3);//z powrotem wlacza przelaczanie zasilania na piny
	}
}

void TIM3_IRQHandler ( void )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		zasilany_pin++;
		switch(zasilany_pin%4)
		{
		case 0:
		{
			GPIO_ResetBits(port3, pin3);
			GPIO_SetBits(port4, pin4);
			break;
		}
		case 1:
		{
			GPIO_ResetBits(port4, pin4);
			GPIO_SetBits(port1, pin1);
			break;
		}
		case 2:
		{
			GPIO_ResetBits(port1, pin1);
			GPIO_SetBits(port2, pin2);
			break;
		}
		case 3:
		{
			GPIO_ResetBits(port2, pin2);
			GPIO_SetBits(port3, pin3);
			break;
		}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}


