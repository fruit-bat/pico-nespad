#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/clocks.h"
#include "nespad.h"
#include "pico/stdio_uart.h"

/**
 * Read joypads every 40ms and send output to the serial port
 */
int main() {
  stdio_uart_init();
  printf("Startup\n");

  nespad_default_init();

  printf("Waiting...\n");

  while(true) {
    sleep_ms(40);

    const uint32_t pad_state = nespad_state();
    const uint32_t kempston_state = nespad_to_kempston(pad_state, 0);
    const uint32_t sinclair_left_state = nespad_to_sinclair_left(pad_state, 0);
    const uint32_t sinclair_right_state = nespad_to_sinclair_right(pad_state, 1);
    printf("NES %32.32lb, Kempston %8.8lb, Sincliar Left %8.8lb, Sincliar Right %8.8lb\n", 
      pad_state, 
      kempston_state,
      sinclair_left_state,
      sinclair_right_state);
  }
}
