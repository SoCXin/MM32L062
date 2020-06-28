////////////////////////////////////////////////////////////////////////////////
/// @file    rtc.c
/// @author  AE TEAM
/// @brief    In window comparator mode,The transformation results are detected
///           Set the threshold value from 0 to 3V, and connect PB6 and PA0 with
///           jumper cap to see the effect.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////
// Define to prevent recursive inclusion
#define _RTC_C_

// Files includes
#include "delay.h"
#include "uart.h"
#include "rtc.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_REALTIME
/// @{
#define RTC_IRQHandler RTC_BKP_IRQHandler
////////////////////////////////////////////////////////////////////////////////
/// @addtogroup RTC_Exported_Functions
/// @{
Calendar_TypeDef calendar;
s8 printBuf[100];
const u8 table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};
const u8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize RTC clock and check whether it works properly
/// @note    It must be careful of the Chip Version.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 RTC_Init(void)
{
    //Check if the clock is configured for the first time
    u8 temp = 0;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    //Enabling backup register access
    PWR_BackupAccessCmd(ENABLE);
    //Read data from a specified backup register: Read data that is not identical
    //to the specified data written
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050) {
        //Reset backup area
        BKP_DeInit();
        if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {
            RCC_HSEConfig(RCC_HSE_ON);

            DELAY_Ms(2000);
            //Check whether the specified RCC marker is set or not, and wait for the
            //low-speed crystal oscillator to be ready
            while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {
                temp++;
                if(temp >= 250)
                    return 1;
                DELAY_Ms(10);
            }
        }

        //Setting RTC Clock
        RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);
        RCC_RTCCLKCmd(ENABLE);
        //Waiting for the last write to RTC registers to complete
        RTC_WaitForLastTask();
        //Waiting for RTC register synchronization
        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        //Allow configuration
        RTC_WaitForLastTask();
        //Setting the value of RTC pre-frequency Division
        RTC_SetPrescaler(8000000 / 128 - 1);
        RTC_WaitForLastTask();
        RTC_WaitForSynchro();
        //Setup time
        RTC_Set(2020, 3, 30, 10, 30, 55);
        //Write user program data to a specified backup register
        BKP_WriteBackupRegister(BKP_DR1, 0X5050);
        RTC_WaitForLastTask();
    }
    //System Continuation Timing
    else {

        RTC_WaitForSynchro();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
    //Update time
    RTC_NVIC_Config();
    return 0;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief  RTC Clock Interruption
/// @note    None.
/// @param  bound: Baud rate
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_BKP_IRQHandler(void)
{
    //Seconds interrupt
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET) {
        //Update time
        RTC_Get();
    }
    //Alarm clock interrupt
    if(RTC_GetITStatus(RTC_IT_ALR) != RESET) {
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
    RTC_ClearITPendingBit((RTC_IT_TypeDef)(RTC_IT_SEC | RTC_IT_OW));
    RTC_WaitForLastTask();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Judging whether it is a leap year function
/// @note    None.
/// @param  year
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 Is_Leap_Year(u16 year)
{
    if(year % 4 == 0) {
        if(year % 100 == 0) {
            if(year % 400 == 0)return 1;
            else return 0;
        }
        else return 1;
    }
    else return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Monthly data sheet
/// @note    None.
/// @param  year
/// @retval None.
////////////////////////////////////////////////////////////////////////////////

u8 RTC_Set(u16 syear, u8 smon, u8 sday, u8 hour, u8 min, u8 sec)
{
    u16 t;
    u32 seccount = 0;
    if(syear < 1970 || syear > 2099)return 1;
    //Add up the seconds of all the years
    for(t = 1970; t < syear; t++) {
        //The number of seconds in leap years
        if(Is_Leap_Year(t))seccount += 31622400;
        else seccount += 31536000;
    }
    smon -= 1;
    //Add up the seconds of the previous month
    for(t = 0; t < smon; t++) {
        seccount += (u32)mon_table[t] * 86400;
        if(Is_Leap_Year(syear) && t == 1)seccount += 86400;
    }
    //Add up the seconds of the previous date
    seccount += (u32)(sday - 1) * 86400;
    seccount += (u32)hour * 3600;
    seccount += (u32)min * 60;
    seccount += sec;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    //Enabling RTC and backup register access
    PWR_BackupAccessCmd(ENABLE);
    //Setting the value of RTC counter
    RTC_SetCounter(seccount);

    RTC_WaitForLastTask();
    RTC_Get();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the current time
/// @note    None.
/// @param  year
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 RTC_Get(void)
{
    static u16 daycnt = 0;
    u32 timecount = 0;
    u32 temp = 0;
    u16 temp1 = 0;
    timecount = RTC_GetCounter();
    //day
    temp = timecount / 86400;
    if(daycnt != temp) {
        daycnt = temp;
        temp1 = 1970;
        while(temp >= 365) {
            if(Is_Leap_Year(temp1)) {
                if(temp >= 366)temp -= 366;
                else {
                    temp1++;
                    break;
                }
            }
            else temp -= 365;
            temp1++;
        }
        calendar.w_year = temp1;
        temp1 = 0;
        while(temp >= 28) {
            if(Is_Leap_Year(calendar.w_year) && temp1 == 1) {
                if(temp >= 29)temp -= 29;
                else break;
            }
            else {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1];
                else break;
            }
            temp1++;
        }
        calendar.w_month = temp1 + 1;
        calendar.w_date = temp + 1;
    }
    temp = timecount % 86400;
    calendar.hour = temp / 3600;
    calendar.min = (temp % 3600) / 60;
    calendar.sec = (temp % 3600) % 60;
    calendar.week = RTC_Get_Week(calendar.w_year, calendar.w_month, calendar.w_date);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the current time week
/// @note    None.
/// @param  year
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
u8 RTC_Get_Week(u16 year, u8 month, u8 day)
{
    u16 temp2;
    u8 yearH, yearL;

    yearH = year / 100;
    yearL = year % 100;

    if (yearH > 19)yearL += 100;

    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];
    if (yearL % 4 == 0 && month < 3)temp2--;
    return(temp2 % 7);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Tamper test
/// @note   None.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void RTC_HSE_Init(void)
{

    if(1 == RTC_Init()) {
        printf("RTC ERROR!");
        DELAY_Ms(1000);
        printf("RTC Trying...");
    }
    else {
        printf("RTC Init finish!");
    }

}

/// @}

/// @}

/// @}
