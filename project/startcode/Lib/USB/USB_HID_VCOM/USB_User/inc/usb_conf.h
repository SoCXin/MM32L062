////////////////////////////////////////////////////////////////////////////////
/// @file     usb_conf.h
/// @author   AE TEAM
/// @brief    Custom HID demo configuration file.
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
#ifndef __USB_CONF_H
#define __USB_CONF_H

// Includes ------------------------------------------------------------------
// Exported types ------------------------------------------------------------
// Exported constants --------------------------------------------------------
// Exported macro ------------------------------------------------------------
// Exported functions -------------------------------------------------------
// External variables --------------------------------------------------------
//-------------------------------------------------------------
// EP_NUM
// defines how many endpoints are used by the device
//-------------------------------------------------------------
#define EP_NUM     (2)

//-------------------------------------------------------------
// --------------   Buffer Description Table  -----------------
//-------------------------------------------------------------
// buffer table base address
// buffer table base address
#define BTABLE_ADDRESS      (0x00)

// EP0
// rx/tx buffer base address
#define ENDP0_RXADDR        (0x18)
#define ENDP0_TXADDR        (0x58)

// EP1
// tx buffer base address
#define ENDP1_TXADDR        (0x100)
#define ENDP1_RXADDR        (0x104)

//-------------------------------------------------------------
// -------------------   ISTR events  -------------------------
//-------------------------------------------------------------
// IMR_MSK
// mask defining which events has to be handled
// by the device application software
#define IMR_MSK (CNTR_CTRM  | CNTR_WKUPM | CNTR_SUSPM | CNTR_ERRM  | CNTR_SOFM \
                 | CNTR_ESOFM | CNTR_RESETM )

//CTR service routines
//associated to defined endpoints
//#define  EP1_IN_Callback   NOP_Process
//#define  EP2_IN_Callback   NOP_Process
#define  EP3_IN_Callback   NOP_Process
//#define  EP4_IN_Callback   NOP_Process

#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
//#define  EP3_OUT_Callback   NOP_Process
//#define  EP4_OUT_Callback   NOP_Process

/// @}


/// @}

/// @}


////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////

