// ===================================================================================
// Project:   Rubber Ducky for CH551, CH552 and CH554
// Version:   v1.1
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// Identifies itself as a USB HID keyboard and types a message when the touchkey is
// pressed. It can be used to control the PC via keyboard shortcuts.
//
// References:
// -----------
// - Blinkinlabs: https://github.com/Blinkinlabs/ch554_sdcc
// - Deqing Sun: https://github.com/DeqingSun/ch55xduino
// - Ralph Doncaster: https://github.com/nerdralph/ch554_sdcc
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Chip:  CH551, CH552 or CH554
// - Clock: 16 MHz internal
// - Adjust the firmware parameters in src/config.h if necessary.
// - Make sure SDCC toolchain and Python3 with PyUSB is installed.
// - Press BOOT button on the board and keep it pressed while connecting it via USB
//   with your PC.
// - Run 'make flash' immediatly afterwards.
// - To compile the firmware using the Arduino IDE, follow the instructions in the 
//   .ino file.
//
// Operating Instructions:
// -----------------------
// - Connect the board via USB to your PC. It should be detected as a HID keyboard.
// - Open a text editor und press the touchkey on the board.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include "src/system.h"                   // system functions
#include "src/usb_keyboard.h"             // USB HID keyboard functions

// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

void delay(void) {
  for(volatile unsigned int i=0; i<3000; i++);
}

// ===================================================================================
// Main Function
// ===================================================================================
void main(void) {
  // Setup
  CLK_config();                           // configure system clock
  KBD_init();                             // init USB HID keyboard

  // Configure P1.5 as input with pull-up
  P1_MOD_OC |= (1 << 5);     // open-drain mode (optional, depends on board)
  P1_DIR_PU &= ~(1 << 5);    // clear output bit → input
  P1_DIR_PU |= (1 << 5);     // enable internal pull-up (1 = pull-up enabled)

  P1_MOD_OC |= (1 << 6);     // open-drain mode (optional, depends on board)
  P1_DIR_PU &= ~(1 << 6);    // clear output bit → input
  P1_DIR_PU |= (1 << 6);     // enable internal pull-up (1 = pull-up enabled)

  // Loop
  while(1) {
    if(!(P1 & (1 << 5))) {
      KBD_press('a');
    } else {
      KBD_release('a');
    }

    if (!(P1 & (1 << 6))) {
      KBD_press('b');
    } else {
      KBD_release('b');
    }
  }
  delay();
}
