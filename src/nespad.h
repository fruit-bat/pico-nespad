#pragma once

#include "hardware/pio.h"
#include "hardware/gpio.h"

void nespad_program_init(PIO pio, uint sm, uint data_pin_base, uint clock_pin_base);

uint32_t nespad_state();

