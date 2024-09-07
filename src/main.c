#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/clocks.h"
#include "nespad.h"
#include "pico/stdio_uart.h"

static PIO _pio;
static uint _sm;
static uint32_t _nespad_state = 0;

static void __not_in_flash_func(nespad_isr)() {
  while (!pio_sm_is_rx_fifo_empty(_pio, _sm)) {
    _nespad_state = ~_pio->rxf[_sm];
  }  
}

// It may be more sensible to run much slower than the original clock of 83.3kHz (12e-6s)
// 17kHz (59e-6s) would give 1 reading per millisecond
#ifndef NESPAD_CLOCK_PERIOD_S
#define NESPAD_CLOCK_PERIOD_S 59e-6
#endif 

static void nespad_program_init(PIO pio, uint sm, uint data_pin_base, uint clock_pin_base) {
  _pio = pio;
  _sm = sm;
  printf("Adding program\n");

  uint offset = pio_add_program(pio, &nespad_program);

  printf("Setting up GPIO\n");

  pio_gpio_init(pio, data_pin_base + 0);
  pio_gpio_init(pio, data_pin_base + 1);
  pio_gpio_init(pio, clock_pin_base + 0);
  pio_gpio_init(pio, clock_pin_base + 1);
  gpio_set_pulls(data_pin_base, true, false);
  gpio_set_pulls(data_pin_base+1, true, false);

  pio_sm_config c = nespad_program_get_default_config(offset);

  sm_config_set_in_pins(&c, data_pin_base);
  sm_config_set_sideset_pins(&c, clock_pin_base);
  //sm_config_set_sideset_pin_base(&c, clock_pin_base);

  sm_config_set_in_shift(&c, true, true, 32);

  float div = (float)clock_get_hz(clk_sys) * (NESPAD_CLOCK_PERIOD_S) / 16;
  sm_config_set_clkdiv(&c, div);

  printf("pio_sm_init\n");
  pio_sm_init(pio, sm, offset, &c);

  const uint pin_mask = (3u << clock_pin_base) | (3u << data_pin_base);
  const uint pin_dirs = (3u << clock_pin_base);
  pio_sm_set_pindirs_with_mask(pio, sm, pin_dirs, pin_mask);
  pio_sm_set_pins(pio, sm, 0); // clear pins

  // Set the state machine running
  pio_sm_set_enabled(pio, sm, true);

  printf("Adding ISR handler\n");

  // irq_set_exclusive_handler(PIO0_IRQ_0, nespad_isr);
  irq_add_shared_handler(PIO0_IRQ_0, nespad_isr, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
  irq_set_enabled(PIO0_IRQ_0, true);
  pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty + sm, true);    
}

int main() {
  stdio_uart_init();
  printf("Startup\n");

  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);

  printf("Claimed sm %d\n", sm);

  const uint data_pin_base = 12;
  const uint clock_pin_base = 14;
  nespad_program_init(pio, sm, data_pin_base, clock_pin_base);

  printf("Waiting...\n");

  while(true) {
    sleep_ms(40);
    printf("nespad_state = %32.32lb\n", _nespad_state);
  }
}
