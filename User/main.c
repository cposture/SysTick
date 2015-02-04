/*
	����ʹ��SysTick���о�ȷ��ʱ��ʹLed��ÿ��һ��Ԥ���ʱ��������
*/

#include "stm32f10x_rcc.h" 	//ʹ����ʱ����Դ
#include "stm32f10x_gpio.h" //ʹ����GPIO��Դ
#include "stm32f10x_it.h"   //ʹ�����ж���Դ

#define LED_ON 	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_0);

//ȫ�ֵ���ʱ����
static unsigned int g_timedelay;

//����ʱ��
void RCC_conf(void)
{
	//����ϵͳʱ�ӣ�ʹϵͳʱ��Ƶ��Ϊ72MHz
	SystemInit();
	//��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}

//����GPIO����PB0
void GPIO_conf(void)
{
	//�ٶȣ�ģʽ��IO��
	GPIO_InitTypeDef GPIO_Struct;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;			//�����������ΪҪ����LED
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0;						
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_Struct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);							//����͵�ƽ��ʹLed����
}

//��ʱ��������ʱ��λΪ1ms
void Delay(unsigned int delay)
{
	g_timedelay = delay;
	
	while(g_timedelay)
		;
}

//����g_timedelay�Լ�����SysTick�жϺ������ã�ʹ��ʱҪ��stm32f10x_it.h������
void TimingDelay_Decrement(void)
{
	if(g_timedelay)
		g_timedelay--;
}

/*
//SysTick�жϺ���������stm32f10x_it.c�����stm32f10x_it.h������
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}
*/
int main()
{
	RCC_conf();
	GPIO_conf();
	/****************�ؼ�**********/
	SysTick_Config(72000);			//��ʼ��SysTick��ʱ����������ʱ��ԴΪFCLK��72MHz���������������жϵľ�����ʱ��������Ϊ72000
	while(1)
	{
		LED_ON;
		Delay(1000);
		LED_OFF;
		Delay(1000);
	}
}
