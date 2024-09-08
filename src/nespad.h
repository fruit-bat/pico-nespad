#pragma once

#include "hardware/pio.h"
#include "hardware/gpio.h"

/**
 * Read dual NES joypads on a Pico PI
 * 
 * The driver uses a PIO program to constantly read from both joypads.
 * Data from the PIO program is read using an ISR and stored to a static variable.
 * 
 * The original clock used by the joypads was 83.3kHz (12e-6s) but I think it is 
 * more sensible to run a little slower and is defaulted to 17kHz (59e-6s), which
 * gives 1 reading per millisecond.
 * 
 * General compile time symbols:
 * 
 * Symbol                   Default     Description
 * -----------------------------------------------------------------------------------
 * DEBUG_NESPAD                         Define this symbol for some debug to stdout
 */

/**
 * Initialize using defaults from compile time symbols:
 * 
 * Symbol                   Default     Description
 * -----------------------------------------------------------------------------------
 * NESPAD_CLOCK_PERIOD_S    59e-6       NES joypad data clock period in seconds
 * NESPAD_PIO               pio0        Use PIO 0
 * NESPAD_SM                            PIO statemachine; undefined to pick a free one
 * NESPAD_IRQ               PIO0_IRQ_0  Use IRQ 0 for pio0
 * NESPAD_CLK_PIN_BASE      12          GPIO start for CLK and ENABLE
 * NESPAD_DATA_PIN_BASE     14          GPIO start for DATA1 and DATA2
 * 
 */
void nespad_default_init();

/**
 * Initialize specifying resourses as paramters
 */
void nespad_program_init(PIO pio, uint pio_irq, uint sm, uint data_pin_base, uint clock_pin_base, float clock_period_s);

/**
 * Fetch the NES joypad state for pad 1 and pad 2, bitwise interleaved.
 */
uint32_t nespad_state();

/**
 * Some masks to help access the joypad state for pad 1 and pad 2.
 */
#define NESPAD_M1(X) (1 << ((X) << 1)) 
#define NESPAD_M2(X) (NESPAD_M1(X) << 1)

#define NESPAD_BP_B 0
#define NESPAD_BP_Y 1
#define NESPAD_BP_SELECT 2
#define NESPAD_BP_START 3
#define NESPAD_BP_UP 4
#define NESPAD_BP_DOWN 5
#define NESPAD_BP_LEFT 6
#define NESPAD_BP_RIGHT 7
#define NESPAD_BP_A 8
#define NESPAD_BP_X 9
#define NESPAD_BP_LS 10
#define NESPAD_BP_RS 11

/**
 * Masks for joypad 1
 */
#define NESPAD_M1_B NESPAD_M1(NESPAD_BP_B)
#define NESPAD_M1_Y NESPAD_M1(NESPAD_BP_Y)
#define NESPAD_M1_SELECT NESPAD_M1(NESPAD_BP_SELECT)
#define NESPAD_M1_START NESPAD_M1(NESPAD_BP_START)
#define NESPAD_M1_UP NESPAD_M1(NESPAD_BP_UP)
#define NESPAD_M1_DOWN NESPAD_M1(NESPAD_BP_DOWN)
#define NESPAD_M1_LEFT NESPAD_M1(NESPAD_BP_LEFT)
#define NESPAD_M1_RIGHT NESPAD_M1(NESPAD_BP_RIGHT)
#define NESPAD_M1_A NESPAD_M1(NESPAD_BP_A)
#define NESPAD_M1_X NESPAD_M1(NESPAD_BP_X)
#define NESPAD_M1_LS NESPAD_M1(NESPAD_BP_LS)
#define NESPAD_M1_RS NESPAD_M1(NESPAD_BP_RS)

/**
 * Masks for joypad 2
 */
#define NESPAD_M2_B NESPAD_M2(NESPAD_BP_B)
#define NESPAD_M2_Y NESPAD_M2(NESPAD_BP_Y)
#define NESPAD_M2_SELECT NESPAD_M2(NESPAD_BP_SELECT)
#define NESPAD_M2_START NESPAD_M2(NESPAD_BP_START)
#define NESPAD_M2_UP NESPAD_M2(NESPAD_BP_UP)
#define NESPAD_M2_DOWN NESPAD_M2(NESPAD_BP_DOWN)
#define NESPAD_M2_LEFT NESPAD_M2(NESPAD_BP_LEFT)
#define NESPAD_M2_RIGHT NESPAD_M2(NESPAD_BP_RIGHT)
#define NESPAD_M2_A NESPAD_M2(NESPAD_BP_A)
#define NESPAD_M2_X NESPAD_M2(NESPAD_BP_X)
#define NESPAD_M2_LS NESPAD_M2(NESPAD_BP_LS)
#define NESPAD_M2_RS NESPAD_M2(NESPAD_BP_RS)