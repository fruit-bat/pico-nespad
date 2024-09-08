#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/clocks.h"
#include "nespad.h"
#include "pico/stdio_uart.h"


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
    printf("nespad_state = %32.32lb\n", nespad_state());
  }
}
