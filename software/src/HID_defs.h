#pragma once


/*
 * See 
 * https://wiki.osdev.org/USB_Human_Interface_Devices
 */

/* Keyboard LED status bits */
#define HID_LED_NumLock    (1 << 0)
#define HID_LED_CapsLock   (1 << 1)
#define HID_LED_ScrollLock (1 << 2)
#define HID_LED_Compose    (1 << 3)
#define HID_LED_Kana       (1 << 4)

/*******************************
 * Key press commands. Packet is between 3 and 8 bytes
 * First byte is modifier keys
 * Second byte is reserved (zero)
 * the rest of the bytes are the keys held down, or zeros for no key
 */
#define HID_KEY_MOD_LCTRL  0x01
#define HID_KEY_MOD_LSHIFT 0x02
#define HID_KEY_MOD_LALT   0x04
#define HID_KEY_MOD_LMETA  0x08
#define HID_KEY_MOD_RCTRL  0x10
#define HID_KEY_MOD_RSHIFT 0x20
#define HID_KEY_MOD_RALT   0x40
#define HID_KEY_MOD_RMETA  0x80




#define HID_KEY_NONE 0x00 // No key pressed

#define HID_KEY_NONE 0x00 // No key pressed
#define HID_KEY_ERR_OVF 0x01 //  Keyboard Error Roll Over - used for all slots if too many keys are pressed ("Phantom key")
// 0x02 //  Keyboard POST Fail
// 0x03 //  Keyboard Error Undefined
#define HID_KEY_A 0x04 // Keyboard a and A
#define HID_KEY_B 0x05 // Keyboard b and B
#define HID_KEY_C 0x06 // Keyboard c and C
#define HID_KEY_D 0x07 // Keyboard d and D
#define HID_KEY_E 0x08 // Keyboard e and E
#define HID_KEY_F 0x09 // Keyboard f and F
#define HID_KEY_G 0x0a // Keyboard g and G
#define HID_KEY_H 0x0b // Keyboard h and H
#define HID_KEY_I 0x0c // Keyboard i and I
#define HID_KEY_J 0x0d // Keyboard j and J
#define HID_KEY_K 0x0e // Keyboard k and K
#define HID_KEY_L 0x0f // Keyboard l and L
#define HID_KEY_M 0x10 // Keyboard m and M
#define HID_KEY_N 0x11 // Keyboard n and N
#define HID_KEY_O 0x12 // Keyboard o and O
#define HID_KEY_P 0x13 // Keyboard p and P
#define HID_KEY_Q 0x14 // Keyboard q and Q
#define HID_KEY_R 0x15 // Keyboard r and R
#define HID_KEY_S 0x16 // Keyboard s and S
#define HID_KEY_T 0x17 // Keyboard t and T
#define HID_KEY_U 0x18 // Keyboard u and U
#define HID_KEY_V 0x19 // Keyboard v and V
#define HID_KEY_W 0x1a // Keyboard w and W
#define HID_KEY_X 0x1b // Keyboard x and X
#define HID_KEY_Y 0x1c // Keyboard y and Y
#define HID_KEY_Z 0x1d // Keyboard z and Z

#define HID_KEY_1 0x1e // Keyboard 1 and !
#define HID_KEY_2 0x1f // Keyboard 2 and @
#define HID_KEY_3 0x20 // Keyboard 3 and #
#define HID_KEY_4 0x21 // Keyboard 4 and $
#define HID_KEY_5 0x22 // Keyboard 5 and %
#define HID_KEY_6 0x23 // Keyboard 6 and ^
#define HID_KEY_7 0x24 // Keyboard 7 and &
#define HID_KEY_8 0x25 // Keyboard 8 and *
#define HID_KEY_9 0x26 // Keyboard 9 and (
#define HID_KEY_0 0x27 // Keyboard 0 and )

#define HID_KEY_ENTER 0x28 // Keyboard Return (ENTER)
#define HID_KEY_ESC 0x29 // Keyboard ESCAPE
#define HID_KEY_BACKSPACE 0x2a // Keyboard DELETE (Backspace)
#define HID_KEY_TAB 0x2b // Keyboard Tab
#define HID_KEY_SPACE 0x2c // Keyboard Spacebar
#define HID_KEY_MINUS 0x2d // Keyboard - and _
#define HID_KEY_EQUAL 0x2e // Keyboard = and +
#define HID_KEY_LEFTBRACE 0x2f // Keyboard [ and {
#define HID_KEY_RIGHTBRACE 0x30 // Keyboard ] and }
#define HID_KEY_BACKSLASH 0x31 // Keyboard \ and |
#define HID_KEY_HASHTILDE 0x32 // Keyboard Non-US # and ~
#define HID_KEY_SEMICOLON 0x33 // Keyboard ; and :
#define HID_KEY_APOSTROPHE 0x34 // Keyboard ' and "
#define HID_KEY_GRAVE 0x35 // Keyboard ` and ~
#define HID_KEY_COMMA 0x36 // Keyboard , and <
#define HID_KEY_DOT 0x37 // Keyboard . and >
#define HID_KEY_SLASH 0x38 // Keyboard / and ?
#define HID_KEY_CAPSLOCK 0x39 // Keyboard Caps Lock

#define HID_KEY_F1 0x3a // Keyboard F1
#define HID_KEY_F2 0x3b // Keyboard F2
#define HID_KEY_F3 0x3c // Keyboard F3
#define HID_KEY_F4 0x3d // Keyboard F4
#define HID_KEY_F5 0x3e // Keyboard F5
#define HID_KEY_F6 0x3f // Keyboard F6
#define HID_KEY_F7 0x40 // Keyboard F7
#define HID_KEY_F8 0x41 // Keyboard F8
#define HID_KEY_F9 0x42 // Keyboard F9
#define HID_KEY_F10 0x43 // Keyboard F10
#define HID_KEY_F11 0x44 // Keyboard F11
#define HID_KEY_F12 0x45 // Keyboard F12

#define HID_KEY_SYSRQ 0x46 // Keyboard Print Screen
#define HID_KEY_SCROLLLOCK 0x47 // Keyboard Scroll Lock
#define HID_KEY_PAUSE 0x48 // Keyboard Pause
#define HID_KEY_INSERT 0x49 // Keyboard Insert
#define HID_KEY_HOME 0x4a // Keyboard Home
#define HID_KEY_PAGEUP 0x4b // Keyboard Page Up
#define HID_KEY_DELETE 0x4c // Keyboard Delete Forward
#define HID_KEY_END 0x4d // Keyboard End
#define HID_KEY_PAGEDOWN 0x4e // Keyboard Page Down
#define HID_KEY_RIGHT 0x4f // Keyboard Right Arrow
#define HID_KEY_LEFT 0x50 // Keyboard Left Arrow
#define HID_KEY_DOWN 0x51 // Keyboard Down Arrow
#define HID_KEY_UP 0x52 // Keyboard Up Arrow

#define HID_KEY_NUMLOCK 0x53 // Keyboard Num Lock and Clear
#define HID_KEY_KPSLASH 0x54 // Keypad /
#define HID_KEY_KPASTERISK 0x55 // Keypad *
#define HID_KEY_KPMINUS 0x56 // Keypad -
#define HID_KEY_KPPLUS 0x57 // Keypad +
#define HID_KEY_KPENTER 0x58 // Keypad ENTER
#define HID_KEY_KP1 0x59 // Keypad 1 and End
#define HID_KEY_KP2 0x5a // Keypad 2 and Down Arrow
#define HID_KEY_KP3 0x5b // Keypad 3 and PageDn
#define HID_KEY_KP4 0x5c // Keypad 4 and Left Arrow
#define HID_KEY_KP5 0x5d // Keypad 5
#define HID_KEY_KP6 0x5e // Keypad 6 and Right Arrow
#define HID_KEY_KP7 0x5f // Keypad 7 and Home
#define HID_KEY_KP8 0x60 // Keypad 8 and Up Arrow
#define HID_KEY_KP9 0x61 // Keypad 9 and Page Up
#define HID_KEY_KP0 0x62 // Keypad 0 and Insert
#define HID_KEY_KPDOT 0x63 // Keypad . and Delete

#define HID_KEY_102ND 0x64 // Keyboard Non-US \ and |
#define HID_KEY_COMPOSE 0x65 // Keyboard Application
#define HID_KEY_POWER 0x66 // Keyboard Power
#define HID_KEY_KPEQUAL 0x67 // Keypad =
