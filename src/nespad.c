#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/clocks.h"
#include "nespad.pio.h"
#include "nespad.h"

//#define DEBUG_NESPAD
#ifdef DEBUG_NESPAD
#define DBG_NESPAD(...) printf(__VA_ARGS__)
#else
#define DBG_NESPAD(...)
#endif
#ifndef NESPAD_IRQ
#define NESPAD_IRQ PIO0_IRQ_0
#endif
#ifndef NESPAD_PIO
#define NESPAD_PIO pio0
#endif
#ifndef NESPAD_CLOCK_PERIOD_S
// It may be more sensible to run much slower than the original clock of 83.3kHz (12e-6s)
// 17kHz (59e-6s) gives 1 reading per millisecond
#define NESPAD_CLOCK_PERIOD_S 59e-6
#endif 
#ifndef NESPAD_CLK_PIN_BASE
#define NESPAD_CLK_PIN_BASE 14
#endif
#ifndef NESPAD_DATA_PIN_BASE
#define NESPAD_DATA_PIN_BASE 12
#endif

static PIO _pio;
static uint _sm;
static uint32_t _nespad_state = 0;

static void __not_in_flash_func(nespad_isr)() {
  while (!pio_sm_is_rx_fifo_empty(_pio, _sm)) {
    _nespad_state = ~_pio->rxf[_sm];
  }  
}

uint32_t __not_in_flash_func(nespad_state)() {
    return _nespad_state;
}

void nespad_default_init() {
  DBG_NESPAD("nespad: default initialization\n");

#ifndef NESPAD_SM
  uint sm = pio_claim_unused_sm(NESPAD_PIO, true);
#else
  uint sm = NESPAD_SM;
#endif

  DBG_NESPAD("nespad: Claimed sm %d\n", sm);

  nespad_program_init(
    NESPAD_PIO, 
    NESPAD_IRQ, 
    sm, 
    NESPAD_DATA_PIN_BASE, 
    NESPAD_CLK_PIN_BASE,
    NESPAD_CLOCK_PERIOD_S
  );
}

void nespad_program_init(PIO pio, uint pio_irq, uint sm, uint data_pin_base, uint clock_pin_base, float clock_period_s) {
  _pio = pio;
  _sm = sm;
  DBG_NESPAD("nespad: Adding program\n");

  uint offset = pio_add_program(pio, &nespad_program);

  DBG_NESPAD("nespad: Setting up GPIO\n");

  pio_gpio_init(pio, data_pin_base + 0);
  pio_gpio_init(pio, data_pin_base + 1);
  pio_gpio_init(pio, clock_pin_base + 0);
  pio_gpio_init(pio, clock_pin_base + 1);
  gpio_set_pulls(data_pin_base, true, false);
  gpio_set_pulls(data_pin_base+1, true, false);

  pio_sm_config c = nespad_program_get_default_config(offset);

  sm_config_set_in_pins(&c, data_pin_base);
  sm_config_set_sideset_pins(&c, clock_pin_base);
  //sm_config_set_sideset_pin_base(&c, clock_pin_base); // TODO use this one

  sm_config_set_in_shift(&c, true, true, 32);

  float div = (float)clock_get_hz(clk_sys) * (clock_period_s) / 16;
  sm_config_set_clkdiv(&c, div);

  DBG_NESPAD("nespad: pio_sm_init\n");
  pio_sm_init(pio, sm, offset, &c);

  const uint pin_mask = (3u << clock_pin_base) | (3u << data_pin_base);
  const uint pin_dirs = (3u << clock_pin_base);
  pio_sm_set_pindirs_with_mask(pio, sm, pin_dirs, pin_mask);
  pio_sm_set_pins(pio, sm, 0); // clear pins

  // Set the state machine running
  pio_sm_set_enabled(pio, sm, true);

  DBG_NESPAD("nespad: Adding ISR handler\n");

  // irq_set_exclusive_handler(pio_irq, nespad_isr);
  irq_add_shared_handler(pio_irq, nespad_isr, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
  irq_set_enabled(pio_irq, true);
  pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty + sm, true);    
}
