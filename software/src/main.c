/*
 * USB-HID adapter for IBM XT PC Keyboards.
 * Adapted from the WCH's "Composite Keyboard and Mouse Example" for the CH32X035
 */

#include <ch32x035_usbfs_device.h>
#include "debug.h"
#include "usbd_composite_km.h"

#include "HID_defs.h"
#include "gpio.h"
#include "HAL.h"
#include "xt_interface.h"

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200); //TODO: This is currently necessary or the uc will crash

    // Init Keyboard pins
    PORT_enable(data_pin);
    PORT_enable(clk_pin);
    PORT_enable(LED_pin);
    
    PIN_input_PU(data_pin);
    PIN_input_PU(clk_pin);
    PIN_output(LED_pin);
    PIN_low(LED_pin);  
    
    PIN_INT_set(clk_pin, PIN_EVT_FALLING);
    PIN_INT_enable();

    /* Usb Init */
    USBFS_RCC_Init( );
    USBFS_Device_Init( ENABLE , PWR_VDD_SupplyVoltage());
    USB_Sleep_Wakeup_CFG( );
    
    KeyInit();
    
    // Initialize XT keyboard interface
    xt_interface_initialize();
    
    while(1)
    {
        KB_LED_Handle();
    }
}


// Interrupt service: call to xt interface if the clock pin has caused the interrupt.
PIN_INT_ISR {
    if (PIN_INTFLAG_read(clk_pin))
        {
        PIN_INTFLAG_clear(clk_pin);
        clk_down();
        }
}
