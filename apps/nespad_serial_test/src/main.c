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
    printf("nespad_state = %32.32lb\n", nespad_state());
  }
}
