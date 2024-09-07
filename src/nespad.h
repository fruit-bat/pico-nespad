#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#include "hardware/clocks.h"
#endif

// ------ //
// nespad //
// ------ //

#define nespad_wrap_target 0
#define nespad_wrap 5

static const uint16_t nespad_program_instructions[] = {
            //     .wrap_target
    0xb742, //  0: nop                    side 2 [7] 
    0xb742, //  1: nop                    side 2 [7] 
    0xe52f, //  2: set    x, 15           side 0 [5] 
    0x4102, //  3: in     pins, 2         side 0 [1] 
    0xaf42, //  4: nop                    side 1 [7] 
    0x0543, //  5: jmp    x--, 3          side 0 [5] 
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program nespad_program = {
    .instructions = nespad_program_instructions,
    .length = 6,
    .origin = -1,
};

static inline pio_sm_config nespad_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + nespad_wrap_target, offset + nespad_wrap);
    sm_config_set_sideset(&c, 2, false, false);
    return c;
}
#endif
