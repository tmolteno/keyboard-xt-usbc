// This state machine is liberated from https://github.com/mc-cat-tty/IBM-Model-F-XT-Keyboard-ESP32
// See also https://seasip.info/VintagePC/ibm_1501105.html

#include <stdbool.h>
#include <ch32x035_usbfs_device.h>
#include "debug.h"
#include "usbd_composite_km.h"

#include "HID_defs.h"
#include "gpio.h"
#include "HAL.h"

// XT Protocol State Machine States
#define XT_STATE_START_BITS_START   0x00
#define XT_STATE_START_BITS_END     0x01
#define XT_STATE_PAYLOAD_RECEIVING  0x02

// XT Protocol State Variables
static uint8_t xt_current_byte;
static uint8_t xt_last_byte;
static int xt_received_bits = 0;
static uint8_t xt_state = XT_STATE_START_BITS_START;

// XT Keymap to HID values
#define XT_MOD_FLAG (1 << 8)
static const unsigned int xt_to_hid_translation_table[128] = {
  0,  // Not Used
  HID_KEY_ESC,
  HID_KEY_1,
  HID_KEY_2,
  HID_KEY_3,
  HID_KEY_4,
  HID_KEY_5,
  HID_KEY_6,
  HID_KEY_7,
  HID_KEY_8,
  HID_KEY_9,
  HID_KEY_0,
  HID_KEY_MINUS,
  HID_KEY_EQUAL,
  HID_KEY_BACKSPACE,
  
  HID_KEY_TAB,
  HID_KEY_Q,
  HID_KEY_W,
  HID_KEY_E,
  HID_KEY_R,
  HID_KEY_T,
  HID_KEY_Y,
  HID_KEY_U,
  HID_KEY_I,
  HID_KEY_O,
  HID_KEY_P,
  HID_KEY_LEFTBRACE,
  HID_KEY_RIGHTBRACE,
  HID_KEY_ENTER,
  
  XT_MOD_FLAG | HID_KEY_MOD_LCTRL,
  HID_KEY_A,
  HID_KEY_S,
  HID_KEY_D,
  HID_KEY_F,
  HID_KEY_G,
  HID_KEY_H,
  HID_KEY_J,
  HID_KEY_K,
  HID_KEY_L,
  HID_KEY_SEMICOLON,
  HID_KEY_APOSTROPHE,
  HID_KEY_GRAVE,
  
  XT_MOD_FLAG | HID_KEY_MOD_LSHIFT,
  HID_KEY_BACKSLASH,
  HID_KEY_Z,
  HID_KEY_X,
  HID_KEY_C,
  HID_KEY_V,
  HID_KEY_B,
  HID_KEY_N,
  HID_KEY_M,
  HID_KEY_COMMA,
  HID_KEY_DOT,
  HID_KEY_SLASH,
  XT_MOD_FLAG | HID_KEY_MOD_RSHIFT,
  HID_KEY_SYSRQ,
  
  XT_MOD_FLAG | HID_KEY_MOD_LALT,
  HID_KEY_SPACE,
  HID_KEY_CAPSLOCK,

  HID_KEY_F1,
  HID_KEY_F2,
  HID_KEY_F3,
  HID_KEY_F4,
  HID_KEY_F5,
  HID_KEY_F6,
  HID_KEY_F7,
  HID_KEY_F8,
  HID_KEY_F9,
  HID_KEY_F10,
  
  HID_KEY_NUMLOCK,
  HID_KEY_SCROLLLOCK,
  HID_KEY_KP7,
  HID_KEY_KP8,
  HID_KEY_KP9,
  HID_KEY_KPMINUS,
  HID_KEY_KP4,
  HID_KEY_KP5,
  HID_KEY_KP6,
  HID_KEY_KPPLUS,
  HID_KEY_KP1,
  HID_KEY_KP2,
  HID_KEY_KP3,
  HID_KEY_KP0,
  HID_KEY_KPDOT
};

// ===================================================================================
// XT Protocol State Machine Handler Functions
// ===================================================================================

// Initialize the XT interface state machine
static void xt_interface_init(void)
{
    xt_current_byte = 0x00;
    xt_last_byte = 0x00;
    xt_received_bits = 0;
    xt_state = XT_STATE_START_BITS_START;
}

// Reset the state machine to initial state
static void xt_interface_reset(void)
{
    xt_received_bits = 0;
    xt_current_byte = 0x00;
    xt_state = XT_STATE_START_BITS_START;
}

// Handle start bits detection state
static void xt_handle_start_bits_start(void)
{
    if (!PIN_read(data_pin))
        xt_state = XT_STATE_START_BITS_END;
    else
        xt_state = XT_STATE_START_BITS_START;
}

// Handle end of start bits state
static void xt_handle_start_bits_end(void)
{
    if (PIN_read(data_pin))
        xt_state = XT_STATE_PAYLOAD_RECEIVING;
    else
        xt_state = XT_STATE_START_BITS_END;
}

// Translate XT scan code to HID key code
static unsigned int xt_translate_to_hid(uint8_t xt_code)
{
    uint8_t key_index = xt_code & 0x7f;
    if (key_index < sizeof(xt_to_hid_translation_table) / sizeof(xt_to_hid_translation_table[0])) {
        return xt_to_hid_translation_table[key_index];
    }
    return 0; // Invalid key
}

// Dispatch key event to USB HID system
static void xt_dispatch_key_event(uint8_t xt_code)
{
    int is_key_release = xt_code & 0x80; // MSB indicates release
    unsigned int hid_key = xt_translate_to_hid(xt_code);
    
    if (hid_key == 0) {
        return; // Invalid key, ignore
    }
    
    if (is_key_release) {
        // Key release
        if (hid_key >= XT_MOD_FLAG) {
            KeyModRelease(hid_key & 0xff);
        } else {
            KeyRelease(hid_key);
        }
    } else {
        // Key press
        if (hid_key >= XT_MOD_FLAG) {
            KeyModPress(hid_key & 0xff);
        } else {
            KeyPress(hid_key);
        }
    }
}

// Control data pin during USB transmission
static void xt_control_data_pin(bool enable_keyboard)
{
    if (enable_keyboard) {
        PIN_input_PU(data_pin); // Re-activate keyboard sending data
    } else {
        PIN_low(data_pin);      // Prevent keyboard from issuing new commands
        PIN_output(data_pin);
    }
}

// Handle payload receiving state
static void xt_handle_payload_receiving(void)
{
    if (xt_received_bits < 7) {
        // Still receiving bits
        xt_current_byte |= (PIN_read(data_pin) << xt_received_bits);
        xt_received_bits++;
    } else {
        // Complete byte received
        xt_current_byte |= (PIN_read(data_pin) << xt_received_bits);
        
        // Process the received byte if it's valid and different from last
        if (xt_current_byte != xt_last_byte && (xt_current_byte & 0x7f) <= 83) {
            // Prevent keyboard interference during USB transmission
            xt_control_data_pin(false);
            
            // Send key event to USB HID system if enumerated
            if (USBFS_DevEnumStatus) {
                xt_dispatch_key_event(xt_current_byte);
            }
            
            xt_last_byte = xt_current_byte;
            
            // Re-enable keyboard data transmission
            xt_control_data_pin(true);
        }
        
        // Reset for next byte
        xt_interface_reset();
    }
}

// ===================================================================================
// Public Interface Functions
// ===================================================================================

// Initialize XT keyboard interface
void xt_interface_initialize(void)
{
    xt_interface_init();
}

// Decode and act on XT clock signal falling edge
void clk_down(void) 
{
    switch (xt_state) {
        case XT_STATE_START_BITS_START:
            xt_handle_start_bits_start();
            break;
            
        case XT_STATE_START_BITS_END:
            xt_handle_start_bits_end();
            break;
            
        case XT_STATE_PAYLOAD_RECEIVING:
            xt_handle_payload_receiving();
            break;
            
        default:
            // Invalid state, reset to known state
            xt_interface_reset();
            break;
    }
}

