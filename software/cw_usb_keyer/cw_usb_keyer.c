#include "src/system.h"                   // system functions
#include "src/usb_keyboard.h"             // USB HID keyboard functions
#include <stdbool.h>                      // boolean functions

// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}


void delay(void) {
  for(volatile unsigned int i=0; i<2000; i++);
}


void main(void) {                         // Setup
  CLK_config();                           // configure system clock
  KBD_init();                             // init USB HID keyboard

  // Configure P1.5 and P1.6 as inputs with internal pull-ups
  P1_DIR_PU &= ~(1 << 5);    // clear output bit → input
  P1_DIR_PU |= (1 << 5);     // enable internal pull-up (1 = pull-up enabled)

  P1_DIR_PU &= ~(1 << 6);    // clear output bit → input
  P1_DIR_PU |= (1 << 6);     // enable internal pull-up (1 = pull-up enabled)

  // set initial values for key state buffers
  bool prev_state_a = true;
  bool prev_state_b = true;

  // default characters
  char left_key_char = 'a';
  char right_key_char = 'b';

  // read both input pins once
  bool state_a = (P1 & (1 << 5)) != 0;
  bool state_b = (P1 & (1 << 6)) != 0;

  // if both paddles are pressed while USB device is being plugged in, '[' and ']' characters will be used
  if (!state_a && !state_b) {
    left_key_char = '[';
    right_key_char = ']';
  }

  while (1) {
    // read both input states
    bool state_a = (P1 & (1 << 5)) != 0;
    bool state_b = (P1 & (1 << 6)) != 0;

    // if current state does not match state buffer, update state buffer and send press\release command based on current state
    if (state_a != prev_state_a) {
      prev_state_a = state_a;
      if (state_a) {
        KBD_release(left_key_char);
      } else {
        KBD_press(left_key_char);
      }
    }

    if (state_b != prev_state_b) {
      prev_state_b = state_b;
      if (state_b) {
        KBD_release(right_key_char);
      } else {
        KBD_press(right_key_char);
      }
    }
    delay();
  }
}
