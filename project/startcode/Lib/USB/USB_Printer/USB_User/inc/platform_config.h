////////////////////////////////////////////////////////////////////////////////
/// @file     platform_config.h
/// @author   AE TEAM
/// @brief    Evaluation board specific configuration file.
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

// Define to prevent recursive inclusion  --------------------------------------
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

// Includes ------------------------------------------------------------------

// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Uncomment the line corresponding to the MM32 evaluation board
// used to run the example


#define USB_DISCONNECT                      GPIOA
#define USB_DISCONNECT_PIN                  GPIO_Pin_11|GPIO_Pin_12
#define RCC_AHBPeriph_GPIO_DISCONNECT      RCC_AHBPeriph_GPIOA

#define RCC_AHBPeriph_GPIO_KEY             RCC_AHBPeriph_GPIOC
#define RCC_AHBPeriph_GPIO_TAMPER          RCC_AHBPeriph_GPIOA
#define RCC_AHBPeriph_GPIO_IOAIN           RCC_AHBPeriph_GPIOA
#define RCC_AHBPeriph_GPIO_LED             RCC_AHBPeriph_GPIOA

#define GPIO_KEY                            GPIOC
#define GPIO_TAMPER                         GPIOA
#define GPIO_IOAIN                          GPIOA
#define GPIO_LED1                           GPIOA
#define GPIO_LED2                           GPIOD

#define GPIO_KEY1_PIN                       GPIO_Pin_5   // PC.05  
#define GPIO_KEY2_PIN                       GPIO_Pin_1   // PC.01  
#define GPIO_TAMPER_PIN                     GPIO_Pin_0   // PA.00  
#define GPIO_IOAIN_PIN                      GPIO_Pin_1   // PA.01  

#define GPIO_LED1_PIN                       GPIO_Pin_8   // PA.08  
#define GPIO_LED2_PIN                       GPIO_Pin_2   // PC.07   

#define GPIO_KEY1_PORTSOURCE                GPIO_PortSourceGPIOC
#define GPIO_KEY1_PINSOURCE                 GPIO_PinSource5
#define GPIO_KEY1_EXTI_Line                 EXTI_Line5

#define GPIO_KEY2_PORTSOURCE                GPIO_PortSourceGPIOC
#define GPIO_KEY2_PINSOURCE                 GPIO_PinSource1
#define GPIO_KEY2_EXTI_Line                 EXTI_Line1

#define GPIO_TAMPER_PORTSOURCE              GPIO_PortSourceGPIOA
#define GPIO_TAMPER_PINSOURCE               GPIO_PinSource0
#define GPIO_TAMPER_EXTI_Line               EXTI_Line0

#define ADC_AIN_CHANNEL                     ADC_Channel_1

// Exported macro ------------------------------------------------------------
// Exported functions -------------------------------------------------------

#endif // __PLATFORM_CONFIG_H  

/// @}

/// @}

/// @}
