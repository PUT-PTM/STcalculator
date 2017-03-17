
/*Zawsze dodajemy GPIO i RCC, TIM do timerow
// zawsze podmienic na #define HSE_VALUE    ((uint32_t)8000000)// z 25 na 8
#include "stm32f4xx_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4xx_syscfg.h" //kontroler przerwan
#include "n126803.h"


int main(void)
{
	SystemInit(); // pierwsza komenda niezbedna do dzialania

	// GPIOD Periph clock enable, takotwanie, niezbedne do dzialania
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	TIM3 TIM4 do APB1
	nie wiem jakie timery do APB2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE );//podlaczenie timera do szyny - niezbedne do dzialania
	TIM_TimeBaseInitTypeDef TIM_Timerx;// struktura timera
	TIM_Cmd(TIMx, ENABLE);// komenda uruchamiania timera

	//ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE); // zegar dla modu³u ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 , ENABLE); // zegar dla modu³u ADC2

	auto counter = TIMx->CNT; //aktualna wartosc licznika powiazana z periodem
	if (TIM_GetFlagStatus(TIMx, TIM_FLAG_Update ))
	{
		TIM_ClearFlag(TIMx, TIM_FLAG_Update );
		GPIO_ResetBits(GPIOD, GPIO_Pin_x);
		GPIO_SetBits(GPIOD, GPIO_Pin_x);
	}
}

timery:
Timery 2-5 oraz 9-14 - timery ogólnego przeznaczenia
Timery 6 i 7 - podstawowe peryferia o najprostszych funkcjach
Timery 1 i 8 - zaawansowane timery do najbardziej z³o¿onych zadañ

Maksymalna czêstotliwoœæ inkrementacji/dekrementacji timera zale¿y od tego
do której szyny APB zosta³ pod³¹czony.
W przypadku timerów: TIM1, TIM8, TIM9, TIM10 i TIM11 ftim = fcpu = 168 MHz (szyna APB2).
W przypadku timerów: TIM2 - TIM7, TIM12 - TIM14 ftim = fcpu/2 = 84 MHz (szyna APB1).

piny wolne:
PA0 - przycisk wbudowany
PA1 - ADC1
PA2 - ADC2
PA3 - DAC?
PA6 - TIM3_CH1
PA8, 15
PB0 - TIM3_CH3
PB1 - TIM3_CH4
PB2, 8, 11-15
PC1, 2, 4-6, 8, 9, 11, 13-15
PC7 - TIM3_CH2 i jakies MCLK jakby cos nie dzialalo ok
PD0-3, 6-11
PD12-15 diody wbudowane
PE2, 4-6
PE7-14 klawiatura 8-przyciskowa
PE15

zegar 8 segmentowy
PB3 - h
PB4 - 4
PB5 - 3
PB6 - 2
PB7 - 1
PD1 - a
PD2 - b
PD3 - c
PD4 - d
PD5 - e
PD6 - f
PD7 - g

if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_x)==RESET)
{
	GPIO_SetBits(GPIOD, GPIO_Pin_x);
	//GPIO_ResetBits(GPIOD, GPIO_Pin_x);
}
*/

uint16_t dioda_zielona = GPIO_Pin_12;
uint16_t dioda_pomaranczowa = GPIO_Pin_13;
uint16_t dioda_czerwona = GPIO_Pin_14;
uint16_t dioda_niebieska = GPIO_Pin_15;

//do generowania sinusa
float alfa = 0; //k¹t <0;360>
float sinusek = 0;//wynik f-cji sinus <-1;1>
int digitAmplituda = 0;//zmienna w programie
int digital_napiecie_sinus = 0;//zmienna w programie
int ADC_Resultsinus = 0;//zmienna w programie

//do generowania pily
float liczbaalfapily = 0.0;
int digitAmplitudapily = 0;
int digital_Napieciepily = 0;
int ADC_Resultpila=0;

void nkonf_diod_wbudowanych()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_Diody;
	// Configure PD12, PD13, PD14 and PD15 in output pushpull mode
	GPIO_Diody.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Diody.GPIO_Mode = GPIO_Mode_OUT; // tryb wyjscia
	GPIO_Diody.GPIO_OType = GPIO_OType_PP; // typ wyjscia - push pull
	GPIO_Diody.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Diody.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub tu nic
	GPIO_Init(GPIOD, &GPIO_Diody);
}

void nkonf_klawiatury_8_klawiszowej()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_klawiatura;
	GPIO_klawiatura.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_klawiatura.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_klawiatura.GPIO_OType = GPIO_OType_PP; // wypelic jesli pin jest wejsciem
	GPIO_klawiatura.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_klawiatura.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_klawiatura);
}
void nkonf_wyswietlacza_LED()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_wyswietlacz_Port_B;
	//piny wyjsciowe do zegara
	GPIO_wyswietlacz_Port_B.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_wyswietlacz_Port_B.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_wyswietlacz_Port_B.GPIO_OType = GPIO_OType_PP;
	GPIO_wyswietlacz_Port_B.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_wyswietlacz_Port_B.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_wyswietlacz_Port_B);

	GPIO_InitTypeDef GPIO_wyswietlacz_Port_D;
	//piny wyjsciowe do zegara
	GPIO_wyswietlacz_Port_D.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_wyswietlacz_Port_D.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_wyswietlacz_Port_D.GPIO_OType = GPIO_OType_PP;
	GPIO_wyswietlacz_Port_D.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_wyswietlacz_Port_D.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_wyswietlacz_Port_D);
}

void nwyswietlaj_cyfre_na_wyswietlaczu_na_pozycji(int pozycja)
{//dziala
	if(pozycja == 1 )
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
	}
	else if(pozycja == 2 )
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);//4
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);//3
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);//1
		GPIO_SetBits(GPIOB, GPIO_Pin_6);//2
	}
	else if(pozycja == 3 )
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
	}
	else if(pozycja == 0 || pozycja == 4)//pozycja 4
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
	}
}

void nwyswietl_kropke(int czy_tak)
{//dziala
	switch (czy_tak)
	{
	case 0:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		break;
	}
	case 1:
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);//kropka
		break;
	}
	default:
		{break;}
	}
}

void nwyswietl_srodkowy_segment(int czy_tak)
{//dziala
	switch (czy_tak)
	{
	case 0:
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 1:
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	default:
		{break;}
	}
}

void nwygas_zegar ()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);//4
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);//3
	GPIO_ResetBits(GPIOB, GPIO_Pin_6);//2
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);//1
	GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment
	GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy
	GPIO_SetBits(GPIOD, GPIO_Pin_3);//dolny prawy
	GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny
	GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
	GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy
	GPIO_SetBits(GPIOD, GPIO_Pin_7);//srodkowy
}

void nwyswietl_cyfre(int cyfra)
{//dziala
	switch (cyfra)
	{
	case 0:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_ResetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 1:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 2:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_SetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_ResetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 3:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 4:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 5:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 6:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_ResetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 7:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 8:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_ResetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	case 9:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_ResetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	default:
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_3);//kropka
		GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment
		GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy
		GPIO_SetBits(GPIOD, GPIO_Pin_3);//dolny prawy
		GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny
		GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy
		GPIO_SetBits(GPIOD, GPIO_Pin_7);//srodkowy
		break;
	}
	}
}

void nwirujace_swiatlo_na_jednej_cyfrze_zegara(unsigned int liczba)//liczba inkrementowana lub dekrementowana
{//mozna podawac liczbe malejaca jak i rosnaca
	switch (liczba%6)
	{
		case 0:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_6);
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment
			GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy
			break;
		}
		case 1:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment
			GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy
			GPIO_SetBits(GPIOD, GPIO_Pin_3);
			break;
		}
		case 2:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy
			GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy
			GPIO_SetBits(GPIOD, GPIO_Pin_4);
			break;

		}
		case 3:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_3);
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny
			GPIO_SetBits(GPIOD, GPIO_Pin_5);
			break;
		}
		case 4:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_4);
			GPIO_ResetBits(GPIOD, GPIO_Pin_5);//dolny lewy
			GPIO_SetBits(GPIOD, GPIO_Pin_6);
			break;
		}
		case 5:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_5);
			GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy
			GPIO_SetBits(GPIOD, GPIO_Pin_1);
			break;
		}
		default:
		{break;}
	}
}

void nwirujace_swiatlo_na_calym_zegarze(unsigned int liczba)//liczba inkrementowana lub dekrementowana
{//mozna podawac liczbe malejaca jak i rosnaca
	/*zegar 8 segmentowy
	PB3 - h
	PB4 - 4
	PB5 - 3
	PB6 - 2
	PB7 - 1
	PD1 - a
	PD2 - b
	PD3 - c
	PD4 - d
	PD5 - e
	PD6 - f
	PD7 - g*/
	switch (liczba%12)
	{
		case 0:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy zgas
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);//2 zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_7);//1 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment zapal !!!
			break;
		}
		case 1:
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_7);//1 zgas
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);//3 zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_6);//2 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment zapal !!!
			break;
		}
		case 2:
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);//2 zgas
			GPIO_ResetBits(GPIOB, GPIO_Pin_4);//4 zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_5);//3 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment zapal !!!
			break;
		}
		case 3:
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);//3 zgas
			GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_4);//4 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_1);//gorny segment zapal !!!
			break;
		}

		case 4:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment zgas
			GPIO_SetBits(GPIOD, GPIO_Pin_3);//dolny prawy zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_4);//4 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_2);//gorny prawy zapal!!!
			break;

		}
		case 5:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_2);//gorny prawy zgas
			GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_4);//4 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_3);//dolny prawy zapal!!!
			break;
		}
		case 6:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_3);//dolny prawy zgas
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);//3 zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_4);//4 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny zapal!!!
			break;
		}
		case 7:
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_4);//4 zgas
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);//2 zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_5);//3 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny zapal!!!
			break;
		}
		case 8:
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);//3 zgas
			GPIO_ResetBits(GPIOB, GPIO_Pin_7);//1 zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_6);//2
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny zapal!!!
			break;
		}
		case 9:
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);//2 zgas
			GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_7);//1 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);//dolny zapal!!!
			break;
		}
		case 10:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_4);//dolny zgas
			GPIO_SetBits(GPIOD, GPIO_Pin_6);//gorny lewy zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_7);//1 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_5);//dolny lewy zapal!!!
			break;
		}
		case 11:
		{
			GPIO_SetBits(GPIOD, GPIO_Pin_5);//dolny lewy zgas
			GPIO_SetBits(GPIOD, GPIO_Pin_1);//gorny segment zgas
			GPIO_SetBits(GPIOB, GPIO_Pin_7);//1 zapal
			GPIO_ResetBits(GPIOD, GPIO_Pin_6);//gorny lewy zapal!!!
			break;
		}
		default:
		{break;}
	}
}

void nkonf_wbudowanego_przycisku()
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_przycisk;
	GPIO_przycisk.GPIO_Pin = GPIO_Pin_0;
	GPIO_przycisk.GPIO_Mode = GPIO_Mode_IN; // tryb wejscia
	//GPIO_przycisk.GPIO_OType = GPIO_OType_PP;
	GPIO_przycisk.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_przycisk.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub tu nic
	GPIO_Init(GPIOA, &GPIO_przycisk);
}

void nkonf_pinu_jako_wejscie(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{//jeszcze nie przetestowane
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Pin = GPIO_Pin_x;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN; // tryb dzialania
	//GPIO_struct.GPIO_OType = GPIO_OType_PP;//tryb jesli wyjscie
	GPIO_struct.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOx, &GPIO_struct);
}

void nkonf_pinu_jako_wyjscie(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{//jeszcze nie przetestowane
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Pin = GPIO_Pin_x;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT; // tryb dzialania
	GPIO_struct.GPIO_OType = GPIO_OType_PP;// tryb jesli wyjscie
	GPIO_struct.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	//GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOx, &GPIO_struct);
}

void nkonf_timera(TIM_TypeDef* TIMx, uint16_t prescaler, uint32_t period)
{//dziala
	if(TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // konfiguracja zegara
	}
	if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // konfiguracja zegara
	}
	if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // konfiguracja zegara
	}
	if(TIMx == TIM5)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); // konfiguracja zegara
	}

	TIM_TimeBaseInitTypeDef TIM_TIMx;
	// Time base configuration
	TIM_TIMx.TIM_Period = period;
	TIM_TIMx.TIM_Prescaler = prescaler;
	TIM_TIMx.TIM_ClockDivision = TIM_CKD_DIV1; // wartosc domyslna
	TIM_TIMx.TIM_CounterMode = TIM_CounterMode_Up; // lub opcja zliacznia w dol - DOWN
	TIM_TimeBaseInit(TIMx, &TIM_TIMx);
	//int licznik = TIM3->CNT; //TIM3->CNT jest powiazany z periodem!!!
}

void nwlacz_timer(TIM_TypeDef* TIMx)
{//dziala
	TIM_Cmd(TIMx, ENABLE);  // komenda uruchamiania timera
}

void nwylacz_timer(TIM_TypeDef* TIMx)
{//dziala
	TIM_Cmd(TIMx, DISABLE);  // komenda uruchamiania timera
}

void nkonf_NVIC_timera(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn)
{//dziala
	// ustawienie trybu pracy priorytetów przerwañ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStruct;
	// numer przerwania
	NVIC_InitStruct. NVIC_IRQChannel = TIMx_IRQn;//w stm32f4xx.h on sam znajduje numery
	// priorytet g³ówny
	NVIC_InitStruct. NVIC_IRQChannelPreemptionPriority = 0x00;
	// subpriorytet
	NVIC_InitStruct. NVIC_IRQChannelSubPriority = 0x00;
	// uruchom dany kana³
	NVIC_InitStruct. NVIC_IRQChannelCmd = ENABLE;
	// zapisz wype³nion¹ strukturê do rejestrów
	NVIC_Init(&NVIC_InitStruct);

	// wyczyszczenie przerwania od timera (wyst¹pi³o przy konfiguracji timera)
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	// zezwolenie na przerwania od przepe³nienia dla timera
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
}

void nkonf_PWM_i_PA3_dla_TIM2_CH4(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration: */
	TIM_OCInitStructure. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure. TIM_Pulse = 0;
	TIM_OCInitStructure. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_3;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	//GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOA, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM2->CCR4=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_PA6_dla_TIM3_CH1(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration: */
	TIM_OCInitStructure. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure. TIM_Pulse = 0;
	TIM_OCInitStructure. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_6;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOA, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM3->CCR1=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_PC7_dla_TIM3_CH2(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration: */
	TIM_OCInitStructure. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure. TIM_Pulse = 0;
	TIM_OCInitStructure. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_7;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOC, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM3->CCR2=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_PB0_dla_TIM3_CH3(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration: */
	TIM_OCInitStructure. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure. TIM_Pulse = 0;
	TIM_OCInitStructure. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_0;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOB, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM3->CCR3=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_PB1_dla_TIM3_CH4(uint32_t period, int duty)
{//nie przetestowane
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* PWM1 Mode configuration: */
	TIM_OCInitStructure. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure. TIM_Pulse = 0;
	TIM_OCInitStructure. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_1;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOB, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM3->CCR4=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}
////////////////////////////////////////////////////
void nkonf_PWM_i_diody_zielonej_PD12_dla_TIM4_CH1(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	TIM_OCInitTypeDef TIM_OC_PWM;
	/* PWM1 Mode configuration: */
	TIM_OC_PWM. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_PWM. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_PWM. TIM_Pulse = 0;
	TIM_OC_PWM. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OC_PWM);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_12;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOD, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM4->CCR1=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_diody_czerwonej_PD13_dla_TIM4_CH2(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	TIM_OCInitTypeDef TIM_OC_PWM;
	/* PWM1 Mode configuration: */
	TIM_OC_PWM. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_PWM. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_PWM. TIM_Pulse = 0;
	TIM_OC_PWM. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM4, &TIM_OC_PWM);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_13;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOD, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM4->CCR2=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_diody_pomaranczowej_PD14_dla_TIM4_CH3(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	TIM_OCInitTypeDef TIM_OC_PWM;
	/* PWM1 Mode configuration: */
	TIM_OC_PWM. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_PWM. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_PWM. TIM_Pulse = 0;
	TIM_OC_PWM. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &TIM_OC_PWM);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_14;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOD, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM4->CCR3=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

void nkonf_PWM_i_diody_niebieskiej_PD15_dla_TIM4_CH4(uint32_t period, int duty)
{//dziala
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	TIM_OCInitTypeDef TIM_OC_PWM;
	/* PWM1 Mode configuration: */
	TIM_OC_PWM. TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_PWM. TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_PWM. TIM_Pulse = 0;
	TIM_OC_PWM. TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OC_PWM);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitTypeDef GPIO_Pin_Wyjsciowy_Timera_PWM;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Pin = GPIO_Pin_15;
	GPIO_Pin_Wyjsciowy_Timera_PWM. GPIO_Mode = GPIO_Mode_AF;//funkcja alternatywna dla PWM
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_OType = GPIO_OType_PP;
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_Pin_Wyjsciowy_Timera_PWM.GPIO_PuPd = GPIO_PuPd_NOPULL; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOD, &GPIO_Pin_Wyjsciowy_Timera_PWM);

	TIM4->CCR4=duty*(period+1)/100; //ten rejestr mozna zmieniac dynamicznie w programie
}

//void nkonf_EXTI(uint8_t EXTIx_IRQn, uint32_t EXTI_Linex, uint8_t GPIOy, uint8_t EXTI_PinSourcex)
void nkonf_EXTI(uint8_t EXTIx_IRQn, uint32_t EXTI_Linex, GPIO_TypeDef* GPIOy, uint8_t EXTI_PinSourcex) //stara wersja powodowala error
{//dziala
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//uruchomiæ zasilanie systemu przerwañ
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct. NVIC_IRQChannel = EXTIx_IRQn;// numer przerwania
	NVIC_InitStruct. NVIC_IRQChannelPreemptionPriority = 0x00;// priorytet g³ówny
	NVIC_InitStruct. NVIC_IRQChannelSubPriority = 0x00;// subpriorytet
	NVIC_InitStruct. NVIC_IRQChannelCmd = ENABLE;// uruchom dany kana³
	NVIC_Init(&NVIC_InitStruct);// zapisz wype³nion¹ strukturê do rejestrów
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct. EXTI_Line = EXTI_Linex;// wybór numeru aktualnie konfigurowanej linii przerwañ
	EXTI_InitStruct. EXTI_Mode = EXTI_Mode_Interrupt;// wybór trybu - przerwanie b¹dŸ zdarzenie
	EXTI_InitStruct. EXTI_Trigger = EXTI_Trigger_Rising;// wybór zbocza, na które zareaguje przerwanie
	EXTI_InitStruct. EXTI_LineCmd = ENABLE;// uruchom dan¹ liniê przerwañ
	EXTI_Init(&EXTI_InitStruct);// zapisz strukturê konfiguracyjn¹ przerwañ zewnêtrznych do rejestrów
	
	// pod³¹czenie danego pinu portu do kontrolera przerwañ
	SYSCFG_EXTILineConfig(GPIOy, EXTI_PinSourcex);
}

/*
void nkonf_pin_wyjsciowy_i_ADC(ADC_TypeDef* ADCx)
{//niesprawdzone dla ADC2 ani dla dwuch jednoczesnie
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // zegar dla modu³u ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); // zegar dla modu³u ADC1

	GPIO_InitTypeDef GPIO_InitStruct;
	//inicjalizacja wejœcia ADC
	GPIO_InitStruct. GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStruct. GPIO_Mode = GPIO_Mode_AN;
	//GPIO_InitStruct. GPIO_Speed = GPIO_Speed_100MHz; // max prekosc przelaczania wyprowadzen
	GPIO_InitStruct. GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	ADC_CommonInitTypeDef ADC_CommonInitStruct;//Wspólna konfiguracja dla wszystkich uk³adów ADC:

	ADC_CommonInitStruct. ADC_Mode = ADC_Mode_Independent;// niezale¿ny tryb pracy przetworników

	ADC_CommonInitStruct. ADC_Prescaler = ADC_Prescaler_Div2;// zegar g³ówny podzielony przez 2

	ADC_CommonInitStruct. ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;// opcja istotna tylko dla trybu multi ADC

	ADC_CommonInitStruct. ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;// czas przerwy pomiêdzy kolejnymi konwersjami
	ADC_CommonInit(&ADC_CommonInitStruct);


	ADC_InitTypeDef ADC_InitStruct;//Konfiguracja danego przetwornika ADC:

	ADC_InitStruct. ADC_Resolution = ADC_Resolution_12b;//ustawienie rozdzielczoœci przetwornika na maksymaln¹ (12 bitów)
	//wy³¹czenie trybu skanowania (odczytywaæ bêdziemy jedno wejœcie ADC
	//w trybie skanowania automatycznie wykonywana jest konwersja na wielu
	//wejœciach/kana³ach)
	ADC_InitStruct. ADC_ScanConvMode = DISABLE;

	ADC_InitStruct. ADC_ContinuousConvMode = ENABLE;//w³¹czenie ci¹g³ego trybu pracy
	//wy³¹czenie zewnêtrznego wyzwalania
	//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w
	//dokumentacji)
	ADC_InitStruct. ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct. ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//wartoœæ binarna wyniku bêdzie podawana z wyrównaniem do prawej
	//funkcja do odczytu stanu przetwornika ADC zwraca wartoœæ 16-bitow¹
	//dla przyk³adu, wartoœæ 0xFF wyrównana w prawo to 0x00FF, w lewo 0x0FF0
	ADC_InitStruct. ADC_DataAlign = ADC_DataAlign_Right;

	ADC_InitStruct. ADC_NbrOfConversion = 1;//liczba konwersji równa 1, bo 1 kana³

	ADC_Init(ADCx, &ADC_InitStruct);// zapisz wype³nion¹ strukturê do rejestrów przetwornika numer 1

	if(ADCx == ADC1)
	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_84Cycles);
	}
	if(ADCx == ADC2)
	{
		ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_84Cycles);
	}

	ADC_Cmd(ADCx, ENABLE);

	//Odczyt wartoœci poprzez odpytywanie flagi zakoñczenia konwersji
	//wykonujemy nastêpuj¹co:
	//ADC_SoftwareStartConv(ADC1);
	//while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	//ADC_Result1 = ADC_GetConversionValue(ADC1);
	//float Vout = nDigital_to_Voltage(ADC_Result1);

}

float nDigital_to_Voltage(int ADC_Result)
{//dziala
	float Vout = ((float)ADC_Result/(float)4095)*2.95;
	return Vout;
}

void nkonf_pin_wejsciowy_i_DAC()
{//nietestowane
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	//inicjalizacja wyjœcia DAC
	GPIO_InitStructure. GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure. GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure. GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure. GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitTypeDef DAC_InitStructure;
	//wy³¹czenie zewnêtrznego wyzwalania
	//konwersja mo¿e byæ wyzwalana timerem, stanem wejœcia itd. (szczegó³y w dokumentacji)
	DAC_InitStructure. DAC_Trigger = DAC_Trigger_None;
	//nast. 2 linie - wy³¹czamy generator predefiniowanych przebiegów
	//wyjœciowych (wartoœci zadajemy sami, za pomoc¹ odpowiedniej funkcji)
	DAC_InitStructure. DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure. DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	//w³¹czamy buforowanie sygna³u wyjœciowego
	DAC_InitStructure. DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	//Mo¿na teraz uruchomiæ przetwornik DAC:
	DAC_Cmd(DAC_Channel_1, ENABLE);

	//DAC_SetChannel1Data(DAC_Align_12b_R, 0xFFF);//mo¿liwa jest zmiana wartoœci generowanej przez modu³ DAC
}
*/
/*
float ngeneruj_sinusoide (float Amplituda, float deltaalfa)
{
	alfa +=deltaalfa;//k¹t zwiekszany od 0 do 360//alfa = 360/czestotliwosc timera //0.09 kiedys bylo
	if(alfa >= 360.0)
		alfa=0.0;

	sinusek = sin(alfa * 3.14/180.0);//wartosc f-cji sinus
	//2.95/4095=amplituda,ktora chcemy / Digital
	//Digital = Amplituda*4095/2.95//wazny wzor
	digitAmplituda = Amplituda*4095.0/2.95;// chyba cyfrowo amplituda
	digital_napiecie_sinus = (int)((sinusek+1.0)*digitAmplituda);// przesuwamy o jeden w gore by nie podawac liczb ujemnych
	DAC_SetChannel1Data(DAC_Align_12b_R, digital_napiecie_sinus);

	//DAC wlasnie wygenerowalo napiecie. Teraz trzeba odczytac je poprzez ADC
	ADC_SoftwareStartConv(ADC1);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_Resultsinus = ADC_GetConversionValue(ADC1);
	float Voutsinus = nDigital_to_Voltage(ADC_Resultsinus);
	return Voutsinus;
}
*/
/*float ngeneruj_pile (float Amplitudapily, float deltapily)
{
	liczbaalfapily += deltapily;//wynik = czestotliwosc timera//zaleznie ile wykresow chcemy w danym okresie
	if(liczbaalfapily >= 1)
		liczbaalfapily = 0;

	digitAmplitudapily = Amplitudapily*4095.0/2.95;// chyba cyfrowo amplituda

	digital_Napieciepily = (int)(liczbaalfapily * digitAmplitudapily);
	DAC_SetChannel1Data(DAC_Align_12b_R, digital_Napieciepily);

	//DAC wlasnie wygenerowalo napiecie. Teraz trzeba odczytac je poprzez ADC
	ADC_SoftwareStartConv(ADC1);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_Resultpila = ADC_GetConversionValue(ADC1);
	float Voutpily = nDigital_to_Voltage(ADC_Resultpila);
	return Voutpily;
}
*/

/*
void njeden_przycisk_zalaczanie_diod_po_koleifor(GPIO_TypeDef *port1, GPIO_InitTypeDef *pin1, GPIO_TypeDef *port2, GPIO_InitTypeDef *pin2, GPIO_TypeDef *port3, GPIO_InitTypeDef *pin3, GPIO_TypeDef *port4, GPIO_InitTypeDef *pin4, GPIO_TypeDef *port5, GPIO_InitTypeDef *pin5)
{//do tego dobrze by bylo miec timer i przerwania od stykow
	int t2 = 0;
	int t1 = 0;
	int number = 0;
	for(;;)
	{
		t2=t1;
		t1 = GPIO_ReadInputDataBit(port1, pin1);
		if ( t2 == 0 && t1 == 1)
			number+=1;
		if(number%4 == 0)
		{
			GPIO_ResetBits(port5, pin5);
			GPIO_SetBits(port2, pin2);
		}
		else if(number%4 == 1)
		{
			GPIO_ResetBits(port2, pin2);
			GPIO_SetBits(port3, pin3);
		}
		else if(number%4 == 2)
		{
			GPIO_ResetBits(port3, pin3);
			GPIO_SetBits(port4, pin4);
		}
		else if(number%4 == 3)
		{
			GPIO_ResetBits(port4, pin4);
			GPIO_SetBits(port5, pin5);
		}
		else if(number >100)
			number -= 80;
	}
}

void TIM3_IRQHandler ( void )
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		// miejsce na kod wywo³ywany w momencie wyst¹pienia przerwania
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);// wyzerowanie flagi wyzwolonego przerwania
	}
}

void EXTI0_IRQHandler ( void )
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		// miejsce na kod wywo³ywany w momencie wyst¹pienia przerwania
		// wyzerowanie flagi wyzwolonego przerwania
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
*/

