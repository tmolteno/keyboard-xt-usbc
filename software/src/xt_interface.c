// This state machine is liberated from https://github.com/mc-cat-tty/IBM-Model-F-XT-Keyboard-ESP32
// See also https://seasip.info/VintagePC/ibm_1501105.html


#include <ch32x035_usbfs_device.h>
#include "debug.h"
#include "usbd_composite_km.h"

#include "HID_defs.h"
#include "gpio.h"
#include "HAL.h"

// Decoder State
#define START_BITS_START 0x00
#define START_BITS_END 0x01
#define PAYLOAD_RECEIVING 0x02
uint8_t val;
uint8_t lastVal;
int received_bits = 0;
uint8_t state = START_BITS_START;

// XT Keymap to HID values
#define MOD_FLAG (1 << 8)
unsigned int translationTable[128] = {
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
  
  MOD_FLAG | HID_KEY_MOD_LCTRL,
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
  
  MOD_FLAG | HID_KEY_MOD_LSHIFT,
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
  MOD_FLAG | HID_KEY_MOD_RSHIFT,
  HID_KEY_SYSRQ,
  
  MOD_FLAG | HID_KEY_MOD_LALT,
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

// Decode and act on XT clock signal falling edge
void clk_down() {
  switch (state) {
    case START_BITS_START:
      if (!PIN_read(data_pin))
        state = START_BITS_END;
      else
        state = START_BITS_START;
      break;
    case START_BITS_END:
      if (PIN_read(data_pin))
        state = PAYLOAD_RECEIVING;
      else
        state = START_BITS_END;
      break;
    case PAYLOAD_RECEIVING:
      if (received_bits < 7) {  // Receiving
        val |= (PIN_read(data_pin) << received_bits);
        received_bits++;
      }
      else {
        // Out Key
        val |= (PIN_read(data_pin) << received_bits);
        
        if (val != lastVal && (val & 0x7f) <= 83) {
          // Prevent keyboard from issuing new command during write out to USB
          PIN_low(data_pin);
          PIN_output(data_pin);  
          
          int msb = val & 0x80;  // Only the byte's MSB is on

          // If USB is enumerated, then send it to the computer
          if (USBFS_DevEnumStatus) {
            unsigned int key = translationTable[val & 0x7f];
            if (msb) {  // msb == 1 --> release
              if (key >= MOD_FLAG)  // If it is a modifier key
                KeyModRelease(key & 0xff);
              else
                KeyRelease(key);
                
            } else {  // msb == 0 --> press
              if (key >= MOD_FLAG)  // If it is a modifier key
                KeyModPress(key & 0xff);
              else
                KeyPress(key);
            }
          }

          lastVal = val;
          PIN_input_PU(data_pin);  // Re-activate Keyboard sending data
        }
        
        received_bits = 0;
        val = 0x00;
        state = START_BITS_START;
      }
      break;
  }
}

