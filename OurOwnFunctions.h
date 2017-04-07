uint16_t dioda_zielona = GPIO_Pin_12;//D
uint16_t dioda_pomaranczowa = GPIO_Pin_13;//D
uint16_t dioda_czerwona = GPIO_Pin_14;//D
uint16_t dioda_niebieska = GPIO_Pin_15;//D

void stmkonf_diod_wbudowanych()
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

void stmkonf_wbudowanego_przycisku()
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

void stmkonf_pinu_jako_wejscie_up(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{
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

void stmkonf_pinu_jako_wejscie_down(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{
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
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_DOWN; // podciaganie vcc masa lub nic
	GPIO_Init(GPIOx, &GPIO_struct);
}

void stmkonf_pinu_jako_wyjscie(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin_x)
{
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

void stmkonf_timera(TIM_TypeDef* TIMx, uint16_t prescaler, uint32_t period)
{//dziala
	if(TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // konfiguracja zegara
	}
	else if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // konfiguracja zegara
	}
	else if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // konfiguracja zegara
	}
	else if(TIMx == TIM5)
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

void stmwlacz_timer(TIM_TypeDef* TIMx)
{//dziala
	TIM_Cmd(TIMx, ENABLE);  // komenda uruchamiania timera
}

void stmwylacz_timer(TIM_TypeDef* TIMx)
{//dziala
	TIM_Cmd(TIMx, DISABLE);  // komenda uruchamiania timera
}

void stmkonf_NVIC_timera(TIM_TypeDef* TIMx, uint8_t TIMx_IRQn)
{//dziala
	// ustawienie trybu pracy priorytetów przerwañ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct. NVIC_IRQChannel = TIMx_IRQn;// numer przerwania//w stm32f4xx.h on sam znajduje numery
	NVIC_InitStruct. NVIC_IRQChannelPreemptionPriority = 0x00;// priorytet g³ówny
	NVIC_InitStruct. NVIC_IRQChannelSubPriority = 0x00;// subpriorytet
	NVIC_InitStruct. NVIC_IRQChannelCmd = ENABLE;// uruchom dany kana³
	NVIC_Init(&NVIC_InitStruct);// zapisz wype³nion¹ strukturê do rejestrów

	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);// wyczyszczenie przerwania od timera (wyst¹pi³o przy konfiguracji timera)
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);// zezwolenie na przerwania od przepe³nienia dla timera
}

void stmkonf_EXTI(uint8_t EXTIx_IRQn, uint32_t EXTI_Linex, uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
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

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
}
