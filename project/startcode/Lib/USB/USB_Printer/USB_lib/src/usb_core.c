////////////////////////////////////////////////////////////////////////////////
/// @file     uab_core.c
/// @author   AE TEAM
/// @brief    Standard protocol processing (USB v2.0).
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
#define _USB_CORE_C_
// Files includes


#include "usb_lib.h"

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup MM32_Hardware_Abstract_Layer
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_CORE
/// @{

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup USB_Exported_Functions
/// @{

#define ValBit(VAR,Place)    (VAR & (1 << Place))
#define SetBit(VAR,Place)    (VAR |= (1 << Place))
#define ClrBit(VAR,Place)    (VAR &= ((1 << Place) ^ 255))

#define Send0LengthData()   if(!(_GetUSB_CTRL0()&EP0_CTRL_TRANEN))\
        _SetUSB_CTRL0(EP0_CTRL_TRANEN|0);



#define USB_StatusIn() Send0LengthData()
#define USB_StatusOut() 0

#define StatusInfo0 StatusInfo.bw.bb1 // Reverse bb0 & bb1  
#define StatusInfo1 StatusInfo.bw.bb0


uint16_t_uint8_t StatusInfo;
bool Data_Mul_MaxPacketSize = false;
// Private function prototypes
static void DataStageOut(void);
static void DataStageIn(void);
static void NoData_Setup0(void);
static void Data_Setup0(void);


////////////////////////////////////////////////////////////////////////////////
/// @brief  Return the current configuration variable address.
/// @param  Length - How many bytes are needed.
/// @retval Return 1 , if the request is invalid when "Length" is 0.
///                Return "Buffer" if the "Length" is not 0.
////////////////////////////////////////////////////////////////////////////////
u8* Standard_GetConfiguration(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength =
            sizeof(pInformation->Current_Configuration);
        return 0;
    }
    pUser_Standard_Requests->User_GetConfiguration();
    return (u8*)&pInformation->Current_Configuration;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  This routine is called to set the configuration value
///                Then each class should configure device themself.
/// @param  None.
/// @retval Return USB_SUCCESS, if the request is performed.
///         Return USB_UNSUPPORT, if the request is invalid
////////////////////////////////////////////////////////////////////////////////
RESULT Standard_SetConfiguration(void)
{

    if ((pInformation->USBwValue0 <=
            Device_Table.Total_Configuration) && (pInformation->USBwValue1 == 0)
            && (pInformation->USBwIndex == 0)) { //call Back usb spec 2.0//
        pInformation->Current_Configuration = pInformation->USBwValue0;
        pUser_Standard_Requests->User_SetConfiguration();
        return USB_SUCCESS;
    }
    else {
        return USB_UNSUPPORT;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Return the Alternate Setting of the current interface.
/// @param  Length - How many bytes are needed.
/// @retval Return 0, if the request is invalid when "Length" is 0.
///               Return "Buffer" if the "Length" is not 0.
////////////////////////////////////////////////////////////////////////////////
u8* Standard_GetInterface(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength =
            sizeof(pInformation->Current_AlternateSetting);
        return 0;
    }
    pUser_Standard_Requests->User_GetInterface();
    return (u8*)&pInformation->Current_AlternateSetting;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  This routine is called to set the interface.
///               Then each class should configure the interface them self.
/// @param  None.
/// @retval - Return USB_SUCCESS, if the request is performed.
///         - Return USB_UNSUPPORT, if the request is invalid.
////////////////////////////////////////////////////////////////////////////////
RESULT Standard_SetInterface(void)
{
    RESULT Re;
    //Test if the specified Interface and Alternate Setting are supported by
    //  the application Firmware//
    Re = (*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, pInformation->USBwValue0);

    if (pInformation->Current_Configuration != 0) {
        if ((Re != USB_SUCCESS) || (pInformation->USBwIndex1 != 0)
                || (pInformation->USBwValue1 != 0)) {
            return  USB_UNSUPPORT;
        }
        else if (Re == USB_SUCCESS) {
            pUser_Standard_Requests->User_SetInterface();
            pInformation->Current_Interface = pInformation->USBwIndex0;
            pInformation->Current_AlternateSetting = pInformation->USBwValue0;
            return USB_SUCCESS;
        }

    }

    return USB_UNSUPPORT;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Copy the device request data to "StatusInfo buffer".
/// @param  None.
/// @retval Return 0, if the request is at end of data block,
///                or is invalid when "Length" is 0.
////////////////////////////////////////////////////////////////////////////////
u8* Standard_GetStatus(u16 Length)
{
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength = 2;
        return 0;
    }

    StatusInfo.w = 0;
    // Reset Status Information //

    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)) {
        //Get Device Status //
        u8 Feature = pInformation->Current_Feature;

        // Remote Wakeup enabled //
        if (ValBit(Feature, 5)) {
            SetBit(StatusInfo0, 1);
        }

        // Bus-powered //
        if (ValBit(Feature, 6)) {
            ClrBit(StatusInfo0, 0);
        }
        else { // Self-powered //
            SetBit(StatusInfo0, 0);
        }
    }
    //Interface Status//
    else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) {
        return (u8*)&StatusInfo;
    }
    //Get EndPoint Status//
    else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT)) {
        u8 Related_Endpoint;
        u8 wIndex0 = pInformation->USBwIndex0;

        Related_Endpoint = (wIndex0 & 0x0f);
        if (ValBit(wIndex0, 7)) {
            // IN endpoint //

            if(_GetUSB_HALT() & (1 << Related_Endpoint)) {
                SetBit(StatusInfo0, 0); // IN Endpoint stalled //
            }
        }
        else {
            // OUT endpoint //

            if(_GetUSB_HALT() & (1 << Related_Endpoint)) {
                SetBit(StatusInfo0, 0); // OUT Endpoint stalled //
            }
        }

    }
    else {
        return NULL;
    }
    pUser_Standard_Requests->User_GetStatus();
    return (u8*)&StatusInfo;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Clear or disable a specific feature.
/// @param  None.
/// @retval - Return USB_SUCCESS, if the request is performed.
///         - Return USB_UNSUPPORT, if the request is invalid.
////////////////////////////////////////////////////////////////////////////////
RESULT Standard_ClearFeature(void)
{
    u32     Type_Rec = Type_Recipient;
    u32     Status;


    if (Type_Rec == (STANDARD_REQUEST | DEVICE_RECIPIENT)) {
        //Device Clear Feature//
        ClrBit(pInformation->Current_Feature, 5);
        return USB_SUCCESS;
    }
    else if (Type_Rec == (STANDARD_REQUEST | ENDPOINT_RECIPIENT)) {
        //EndPoint Clear Feature//
        DEVICE* pDev;
        u32 Related_Endpoint;
        u32 wIndex0;
        u32 rEP;

        if ((pInformation->USBwValue != ENDPOINT_STALL)
                || (pInformation->USBwIndex1 != 0)) {
            return USB_UNSUPPORT;
        }

        pDev = &Device_Table;
        wIndex0 = pInformation->USBwIndex0;
        rEP = wIndex0 & ~0x80;
        Related_Endpoint = ENDP0 + rEP;

        if (ValBit(pInformation->USBwIndex0, 7)) {
            //Get Status of endpoint & stall the request if the related_ENdpoint
            //is Disabled//

            Status = (_GetUSB_HALT() >> Related_Endpoint) & 0x01;
        }
        else {

            Status = (_GetUSB_HALT() >> Related_Endpoint) & 0x01;
        }

        if ((rEP >= pDev->Total_Endpoint) || (Status == 0)
                || (pInformation->Current_Configuration == 0)) {
            return USB_UNSUPPORT;
        }


        if (wIndex0 & 0x80) {
            // IN endpoint //

            if(_GetUSB_HALT() & (1 >> Related_Endpoint)) {
                _ClrUSB_HALT((1 >> Related_Endpoint)) ;
            }
        }
        else {
            // OUT endpoint //

            if(_GetUSB_HALT() & (1 >> Related_Endpoint)) {
                _ClrUSB_HALT((1 >> Related_Endpoint)) ;
            }
        }
        pUser_Standard_Requests->User_ClearFeature();
        return USB_SUCCESS;
    }

    return USB_UNSUPPORT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set or enable a specific feature of EndPoint.
/// @param  None.
/// @retval - Return USB_SUCCESS, if the request is performed.
///         - Return USB_UNSUPPORT, if the request is invalid.
////////////////////////////////////////////////////////////////////////////////
RESULT Standard_SetEndPointFeature(void)
{
    u32    wIndex0;
    u32    Related_Endpoint;
    u32    rEP;
    u32   Status;

    wIndex0 = pInformation->USBwIndex0;
    rEP = wIndex0 & ~0x80;
    Related_Endpoint = ENDP0 + rEP;

    Status = (_GetUSB_HALT() >> Related_Endpoint) & 0x01;
    if (Related_Endpoint >= Device_Table.Total_Endpoint
            || pInformation->USBwValue != 0 || Status == 0
            || pInformation->Current_Configuration == 0) {
        return USB_UNSUPPORT;
    }
    else {
        _SetUSB_HALT(1 << Related_Endpoint);
    }
    pUser_Standard_Requests->User_SetEndPointFeature();
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set or enable a specific feature of Device..
/// @param  None.
/// @retval - Return USB_SUCCESS, if the request is performed.
///         - Return USB_UNSUPPORT, if the request is invalid.
////////////////////////////////////////////////////////////////////////////////
RESULT Standard_SetDeviceFeature(void)
{
    SetBit(pInformation->Current_Feature, 5);
    pUser_Standard_Requests->User_SetDeviceFeature();
    return USB_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Standard_GetDescriptorData is used for descriptors transfer.
///              : This routine is used for the descriptors resident in Flash
///                 or RAM
///                pDesc can be in either Flash or RAM
///                The purpose of this routine is to have a versatile way to
///                response descriptors request. It allows user to generate
///                certain descriptors with software or read descriptors from
///                external storage part by part..
/// @param  - Length - Length of the data in this transfer.
///                - pDesc - A pointer points to descriptor struct.
///                The structure gives the initial address of the descriptor and
///                its original size.
/// @retval Address of a part of the descriptor pointed by the Usb_
///                 wOffset The buffer pointed by this address contains at least
///                Length bytes.
////////////////////////////////////////////////////////////////////////////////
u8* Standard_GetDescriptorData(u16 Length, ONE_DESCRIPTOR* pDesc)
{
    u32  wOffset;

    wOffset = pInformation->Ctrl_Info.Usb_wOffset;
    if (Length == 0) {
        pInformation->Ctrl_Info.Usb_wLength = pDesc->Descriptor_Size - wOffset;
        return 0;
    }

    return pDesc->Descriptor + wOffset;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  Data stage of a Control Write Transfer.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DataStageOut(void)
{
    ENDPOINT_INFO* pEPinfo = &pInformation->Ctrl_Info;
    u32 save_rLength;

    save_rLength = pEPinfo->Usb_rLength;

    if (pEPinfo->CopyData && save_rLength) {
        u8* Buffer;
        u32 Length;

        Length = pEPinfo->PacketSize;
        if (Length > save_rLength) {
            Length = save_rLength;
        }

        Buffer = (*pEPinfo->CopyData)(Length);
        pEPinfo->Usb_rLength -= Length;
        pEPinfo->Usb_rOffset += Length;

        PMAToUserBufferCopy(Buffer, ENDP0, Length);//EP0 receive OUT_PACK data
    }

    if (pEPinfo->Usb_rLength != 0) {

    }
    // Set the next State//
    if (pEPinfo->Usb_rLength >= pEPinfo->PacketSize) {
        pInformation->ControlState = OUT_DATA;
    }
    else {
        if (pEPinfo->Usb_rLength > 0) {
            pInformation->ControlState = LAST_OUT_DATA;
        }
        else if (pEPinfo->Usb_rLength == 0) {
            pInformation->ControlState = WAIT_STATUS_IN;
            USB_StatusIn();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Data stage of a Control Read Transfer.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void DataStageIn(void)
{
    ENDPOINT_INFO* pEPinfo = &pInformation->Ctrl_Info;
    u32 save_wLength = pEPinfo->Usb_wLength;
    u32 ControlState = pInformation->ControlState;

    u8* DataBuffer;
    u32 Length;

    if ((save_wLength == 0) && (ControlState == LAST_IN_DATA)) {
        if(Data_Mul_MaxPacketSize == true) {
            // No more data to send and empty packet //
            // Send0LengthData();
            if(!(_GetUSB_CTRL0()&EP0_CTRL_TRANEN)) {
                _SetUSB_CTRL0(EP0_CTRL_TRANEN | 0);
                ControlState = LAST_IN_DATA;
                Data_Mul_MaxPacketSize = false;
            }
        }
        else {
            // No more data to send so STALL the TX Status//
            ControlState = WAIT_STATUS_OUT;

        }
        goto Expect_Status_Out;
    }

    Length = pEPinfo->PacketSize;
    ControlState = (save_wLength <= Length) ? LAST_IN_DATA : IN_DATA;

    if (Length > save_wLength) {
        Length = save_wLength;
    }

    DataBuffer = (*pEPinfo->CopyData)(Length);

//  if(Length == 1)
//  {
//    Length = 2;
//    pEPinfo->Usb_wLength =2;
//  }

    //while(_GetUSB_CTRL0()&EP0_CTRL_TRANEN);
    if(!(_GetUSB_CTRL0()&EP0_CTRL_TRANEN)) {
        UserToPMABufferCopy(DataBuffer, ENDP0, Length);
        _SetUSB_CTRL0(EP0_CTRL_TRANEN | Length) ;


        pEPinfo->Usb_wLength -= Length;
        pEPinfo->Usb_wOffset += Length;
    }

Expect_Status_Out:
    pInformation->ControlState = ControlState;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Proceed the processing of setup request without data stage.
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void NoData_Setup0(void)
{
    RESULT Result = USB_UNSUPPORT;
    u32 RequestNo = pInformation->USBbRequest;
    u32 ControlState;

    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)) {
        // Device Request//
        // SET_CONFIGURATION//
        if (RequestNo == SET_CONFIGURATION) {
            Result = Standard_SetConfiguration();
        }

        //SET ADDRESS//
        else if (RequestNo == SET_ADDRESS) {
            if ((pInformation->USBwValue0 > 127) || (pInformation->USBwValue1 != 0)
                    || (pInformation->USBwIndex != 0)
                    || (pInformation->Current_Configuration != 0))
                // Device Address should be 127 or less//
            {
                ControlState = STALLED;

                goto exit_NoData_Setup0;
            }
            else {
                Result = USB_SUCCESS;
            }
        }
        //SET FEATURE for Device//
        else if (RequestNo == SET_FEATURE) {
            if ((pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP)
                    && (pInformation->USBwIndex == 0)
                    && (ValBit(pInformation->Current_Feature, 5))) {
                Result = Standard_SetDeviceFeature();
            }
            else {
                Result = USB_UNSUPPORT;
            }
        }
        //Clear FEATURE for Device //
        else if (RequestNo == CLEAR_FEATURE) {
            if (pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP
                    && pInformation->USBwIndex == 0
                    && ValBit(pInformation->Current_Feature, 5)) {
                Result = Standard_ClearFeature();
            }
            else {
                Result = USB_UNSUPPORT;
            }
        }

    }

    // Interface Request//
    else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) {
        //SET INTERFACE//
        if (RequestNo == SET_INTERFACE) {
            Result = Standard_SetInterface();
        }
    }

    // EndPoint Request//
    else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT)) {
        //CLEAR FEATURE for EndPoint//
        if (RequestNo == CLEAR_FEATURE) {
            Result = Standard_ClearFeature();
        }
        // SET FEATURE for EndPoint//
        else if (RequestNo == SET_FEATURE) {
            Result = Standard_SetEndPointFeature();
        }
    }
    else {
        Result = USB_SUCCESS;
    }


    if (Result != USB_SUCCESS) {
        Result = (*pProperty->Class_NoData_Setup)(RequestNo);
        if (Result == USB_NOT_READY) {
            ControlState = PAUSE;
            goto exit_NoData_Setup0;
        }
    }

    if (Result != USB_SUCCESS) {
        goto exit_NoData_Setup0;
    }

    ControlState = WAIT_STATUS_IN;// After no data stage SETUP //

    USB_StatusIn();

exit_NoData_Setup0:
    pInformation->ControlState = ControlState;
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Proceed the processing of setup request with data stage..
/// @param  None.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void Data_Setup0(void)
{
    u8* (*CopyRoutine)(u16);
    RESULT Result;
    u32 Request_No = pInformation->USBbRequest;

    u32 Related_Endpoint, Reserved;
    u32 wOffset = 0, Status;

    CopyRoutine = NULL;

    if (Request_No == GET_DESCRIPTOR) {
        if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)) {
            u8 wValue1 = pInformation->USBwValue1;
            if (wValue1 == DEVICE_DESCRIPTOR) {
                CopyRoutine = pProperty->GetDeviceDescriptor;
            }
            else if (wValue1 == CONFIG_DESCRIPTOR) {
                CopyRoutine = pProperty->GetConfigDescriptor;
            }
            else if (wValue1 == STRING_DESCRIPTOR) {
                CopyRoutine = pProperty->GetStringDescriptor;
            }  // End of GET_DESCRIPTOR //
        }
    }

    //GET STATUS//
    else if ((Request_No == GET_STATUS) && (pInformation->USBwValue == 0)
             && (pInformation->USBwLength == 0x0002)
             && (pInformation->USBwIndex1 == 0)) {
        // GET STATUS for Device//
        if ((Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
                && (pInformation->USBwIndex == 0)) {
            CopyRoutine = Standard_GetStatus;
        }

        // GET STATUS for Interface//
        else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT)) {
            if (((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS)
                    && (pInformation->Current_Configuration != 0)) {
                CopyRoutine = Standard_GetStatus;
            }
        }

        // GET STATUS for EndPoint//
        else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT)) {
            Related_Endpoint = (pInformation->USBwIndex0 & 0x0f);
            Reserved = pInformation->USBwIndex0 & 0x70;

            Status = (_GetUSB_HALT() >> Related_Endpoint) & 0x01;

            if ((Related_Endpoint < Device_Table.Total_Endpoint) && (Reserved == 0)
                    && (Status != 0)) {
                CopyRoutine = Standard_GetStatus;
            }
        }

    }

    //GET CONFIGURATION//
    else if (Request_No == GET_CONFIGURATION) {
        if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)) {
            CopyRoutine = Standard_GetConfiguration;
        }
    }
    //GET INTERFACE//
    else if (Request_No == GET_INTERFACE) {
        if ((Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
                && (pInformation->Current_Configuration != 0) && (pInformation->USBwValue == 0)
                && (pInformation->USBwIndex1 == 0) && (pInformation->USBwLength == 0x0001)
                && ((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS)) {
            CopyRoutine = Standard_GetInterface;
        }

    }

    if (CopyRoutine) {
        pInformation->Ctrl_Info.Usb_wOffset = wOffset;
        pInformation->Ctrl_Info.CopyData = CopyRoutine;
        // sb in the original the cast to word was directly //
        // now the cast is made step by step //
        (*CopyRoutine)(0);
        Result = USB_SUCCESS;
    }
    else {
        Result = (*pProperty->Class_Data_Setup)(pInformation->USBbRequest);

        if (Result == USB_NOT_READY) {
            pInformation->ControlState = PAUSE;
            return;
        }
    }

    if (pInformation->Ctrl_Info.Usb_wLength == 0xFFFF) {
        // Data is not ready, wait it //
        pInformation->ControlState = PAUSE;
        return;
    }
    if ((Result == USB_UNSUPPORT) || (pInformation->Ctrl_Info.Usb_wLength == 0)) {
        // Unsupported request //
        pInformation->ControlState = STALLED;
        return;
    }


    if (ValBit(pInformation->USBbmRequestType, 7)) {
        // Device ==> Host //
        __IO u32 wLength = pInformation->USBwLength;

        // Restrict the data length to be the one host asks //
        if (pInformation->Ctrl_Info.Usb_wLength > wLength) {
            pInformation->Ctrl_Info.Usb_wLength = wLength;
        }

        else if (pInformation->Ctrl_Info.Usb_wLength < pInformation->USBwLength) {
            if (pInformation->Ctrl_Info.Usb_wLength < pProperty->MaxPacketSize) {
                Data_Mul_MaxPacketSize = false;
            }
            else if ((pInformation->Ctrl_Info.Usb_wLength % pProperty->MaxPacketSize) == 0) {
                Data_Mul_MaxPacketSize = true;
            }
        }

        pInformation->Ctrl_Info.PacketSize = pProperty->MaxPacketSize;
        DataStageIn();
    }
    else {
        pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLength;//
        pInformation->ControlState = OUT_DATA;

    }

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Get the device request data and dispatch to individual process.
/// @param  None.
/// @retval Post0_Process.
////////////////////////////////////////////////////////////////////////////////
u8 Setup0_Process(void)
{

    if (pInformation->ControlState != PAUSE) {
        pInformation->USBbmRequestType = USB->SETUP0;  // bmRequestType
        pInformation->USBbRequest = USB->SETUP1;       // bRequest //
        pInformation->USBwValue = (USB->SETUP3) | (USB->SETUP2 << 8); // wValue
        pInformation->USBwIndex  = (USB->SETUP5) | (USB->SETUP4 << 8); // wIndex
        pInformation->USBwLength = (USB->SETUP6) | (USB->SETUP7 << 8); // wLength
    }

    pInformation->ControlState = SETTING_UP;

    if (pInformation->USBwLength == 0) {
        // Setup with no data stage //
        NoData_Setup0();
    }
    else {
        // Setup with data stage //
        Data_Setup0();
    }
    return Post0_Process();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Process the IN token on all default endpoint.
/// @param  None.
/// @retval Post0_Process.
////////////////////////////////////////////////////////////////////////////////
u8 In0_Process(void)
{
    u32 ControlState = pInformation->ControlState;

    if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA)) {
        DataStageIn();
        // ControlState may be changed outside the function //
        ControlState = pInformation->ControlState;
    }

    else if (ControlState == WAIT_STATUS_IN) {
        if ((pInformation->USBbRequest == SET_ADDRESS) &&
                (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))) {
            pUser_Standard_Requests->User_SetDeviceAddress();
        }

        (*pProperty->Process_Status_IN)();
    }

    else {
        ControlState = STALLED;

    }

    pInformation->ControlState = ControlState;

    return Post0_Process();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Process the OUT token on all default endpoint.
/// @param  None.
/// @retval Post0_Process.
////////////////////////////////////////////////////////////////////////////////
u8 Out0_Process(void)
{
    u32 ControlState = pInformation->ControlState;

    if ((ControlState == OUT_DATA) || (ControlState == LAST_OUT_DATA)) {
        DataStageOut();
        ControlState = pInformation->ControlState; // may be changed outside the function //
    }

    else if (ControlState == WAIT_STATUS_OUT) {
        (*pProperty->Process_Status_OUT)();
        ControlState = STALLED;
    }

    else if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA)) {
        // host aborts the transfer before finish //
        ControlState = STALLED;

    }

    // Unexpect state, STALL the endpoint //
    else {
        ControlState = STALLED;
    }

    pInformation->ControlState = ControlState;

    return Post0_Process();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Stall the Endpoint 0 in case of error.
/// @param  None.
/// @retval - 0 if the control State is in PAUSE
///         - 1 if not.
////////////////////////////////////////////////////////////////////////////////
u8 Post0_Process(void)
{

    if (pInformation->ControlState == STALLED) {
        USB->EP_HALT |= 0X01;
    }
    return (pInformation->ControlState == PAUSE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the device and all the used Endpoints addresses.
/// @param  - Val: device adress.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void SetDeviceAddress(u8 Val)
{
    _SetUSB_ADDR(Val);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief  No operation function.
/// @param  None.
/// @retval NOne.
////////////////////////////////////////////////////////////////////////////////
void NOP_Process(void)
{
}

/// @}

/// @}

/// @}
