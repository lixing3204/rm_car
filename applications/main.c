/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define MOTER_1R0_NUM         8      /* LED PIN引脚编号,查看驱动文件drv_gpio.c确定 */
#define MOTER_1R1_NUM  
#define MOTER_1L0_NUM  
#define MOTER_1L1_NUM  
#define MOTER_2R0_NUM  
#define MOTER_2R1_NUM  
#define MOTER_2L0_NUM  
#define MOTER_2L1_NUM  

#define PWM_DEV_NAME        "pwm1"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */

struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */
/* defined the LED0 pin: PD2 */
#define LED0_PIN    GET_PIN(D, 2)


static void pwm_led_sample(int argc, char *argv[])//int argc, char *argv[]
{
    rt_uint32_t period, pulse, dir;

    period = 36000;     /* 周期0.5ms,单位为纳秒ns */
    dir = 1;            /* PWM脉冲宽度值的增减方向 */
    pulse = 100;          /* PWM脉冲宽度值,单位为纳秒ns */

    /* 设置LED引脚模式为输出 */
    rt_pin_mode(MOTER_1R0_NUM, PIN_MODE_OUTPUT);
    /* 拉高LED设备 */
    rt_pin_write(MOTER_1R0_NUM, PIN_HIGH);
    
    /* 查找设备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        //return RT_ERROR;
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /*使能设备 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
    
    while (1)
    {
        rt_thread_mdelay(50);
        if (dir)
        {
            pulse += 6000;      /* 从0值开始每次增加5000ns */
        }
        else
        {
            pulse -= 6000;      /* 从最大值开始每次减少5000ns */
        }
        if (pulse >= period)
        {
            dir = 0;
        }
        if (0 == pulse)
        {
            dir = 1;
        }
				rt_kprintf("hello world \r\n");
        /* 设置PWM周期和脉冲宽度 */
        rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    }
}


int led1_init(void){
	rt_thread_t thread = rt_thread_create("led", pwm_led_sample, RT_NULL, 256, 29, 10);
	if(thread == RT_NULL) return (-RT_ERROR); 
	rt_thread_startup(thread); 
	return RT_EOK; 
}

/* 导出到 msh 命令列表中 */
 MSH_CMD_EXPORT(pwm_led_sample, pwm sample);

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
		led1_init();

    while (1)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
			  //rt_kprintf("hello world \r\n");
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
   // while(1);
    return RT_EOK;
}
