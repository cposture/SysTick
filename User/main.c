/*
	程序使用SysTick进行精确延时，使Led灯每隔一段预设的时间亮和灭
*/

#include "stm32f10x_rcc.h" 	//使用了时钟资源
#include "stm32f10x_gpio.h" //使用了GPIO资源
#include "stm32f10x_it.h"   //使用了中断资源

#define LED_ON 	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_0);

//全局的延时参数
static unsigned int g_timedelay;

//配置时钟
void RCC_conf(void)
{
	//配置系统时钟，使系统时钟频率为72MHz
	SystemInit();
	//打开GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
}

//配置GPIO——PB0
void GPIO_conf(void)
{
	//速度，模式，IO口
	GPIO_InitTypeDef GPIO_Struct;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出，因为要驱动LED
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0;						
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_Struct);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);							//输出低电平，使Led灯亮
}

//延时函数，延时单位为1ms
void Delay(unsigned int delay)
{
	g_timedelay = delay;
	
	while(g_timedelay)
		;
}

//用于g_timedelay自减，供SysTick中断函数调用，使用时要在stm32f10x_it.h中声明
void TimingDelay_Decrement(void)
{
	if(g_timedelay)
		g_timedelay--;
}

/*
//SysTick中断函数，放在stm32f10x_it.c里，并在stm32f10x_it.h中声明
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}
*/
int main()
{
	RCC_conf();
	GPIO_conf();
	/****************关键**********/
	SysTick_Config(72000);			//初始化SysTick定时器，并设置时钟源为FCLK（72MHz）设置两次两次中断的经过的时钟周期数为72000
	while(1)
	{
		LED_ON;
		Delay(1000);
		LED_OFF;
		Delay(1000);
	}
}
