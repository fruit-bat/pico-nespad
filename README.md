# pico-nespad
Pico PI NES joypad reader

![image](docs/nes_joypad.jpg)

Reads from dual NES joypads on a Pico PI.

The library should work for both NES (8-bit) and SNES (12-bit) controllers,
with button A and B reporting in the LSB respectively.

![image](docs/nespad_8_12.png)

## Example use
Basic flow:
```
  #include "nespad.h"

  ...

  nespad_default_init();

  ...

  uint32 joystate = nespad_state();
```

## Helper functions
There are some functions to help convert the returned pad state to something useful.

General helper functions:
```
/**
 * Get the bit position of a button given the pad-state and button index
 */
uint32_t nespad_bitpos(
    const uint32_t nespad_pad, // The joypad index (0 or 1)
    const uint32_t nespad_bi   // One of NESPAD_BI_XXX
);

/**
 * Get the bit-mask for a button given the pad-state and button index
 */
uint32_t nespad_bitmask(
    const uint32_t nespad_pad, // The joypad index (0 or 1)
    const uint32_t nespad_bi   // One of NESPAD_BI_XXX
);

/**
 * Get a single button and return its state in a specific bit position
 */
uint32_t nespad_bit_shifted(
    const uint32_t nespad_state,
    const uint32_t nespad_pad, // The joypad index (0 or 1)
    const uint32_t nespad_bi,  // One of NESPAD_BI_XXX
    const uint32_t target_bp   // target bit position
);
```

Conversions for specific joysticks on the ZX Spectrum:
```
/**
 * Convert the joypad state to Kempston joystick state
 */
uint32_t nespad_to_kempston(
    const uint32_t nespad_state,
    const uint32_t nespad_pad // The joypad index (0 or 1)   
);

/**
 * Convert the joypad state to Left Sinclair joystick state
 */
uint32_t nespad_to_sinclair_left(
    const uint32_t nespad_state,
    const uint32_t nespad_pad // The joypad index (0 or 1)   
);

/**
 * Convert the joypad state to Right Sinclair joystick state
 */
uint32_t nespad_to_sinclair_right(
    const uint32_t nespad_state,
    const uint32_t nespad_pad // The joypad index (0 or 1)   
);

```

Build the sample app:
```
cd pico-nespad
cd pico-nespad/apps/
mkdir build
cd build
cmake -DPICO_SDK_PATH=~/pico/pico-sdk ..
make -j4
cp ./nespad_serial_test/nespad_serial_test.uf2 /media/neo/RPI-RP2/
```

Sample app output:
```
NES 00000000000000010000000000000100, Kempston 00110000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000000010000000000000100, Kempston 00110000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000001010001000100000100, Kempston 01111010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000001010001000100000100, Kempston 01111010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000001000001000100000100, Kempston 01011010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000001000001000100000100, Kempston 01011010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000000000001000100000000, Kempston 00001010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000000000000000000000000, Kempston 00000000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000001010000000000000100, Kempston 01110000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000001010000000000000100, Kempston 01110000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000001010000000000000100, Kempston 01110000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000001010000000000000100, Kempston 01110000, Sincliar Left 11111111, Sincliar Right 11111111
NES 00000000000001010001000100000100, Kempston 01111010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000000010001000100000100, Kempston 00111010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000000010001000100000100, Kempston 00111010, Sincliar Left 11110110, Sincliar Right 11111111
NES 00000000000000000001000100000000, Kempston 00001010, Sincliar Left 11110110, Sincliar Right 11111111
```

Compile time symbols:
```
 * Symbol                   Default     Description
 * -----------------------------------------------------------------------------------
 * DEBUG_NESPAD                         Define this symbol for some debug to stdout
 * NESPAD_CLOCK_PERIOD_S    59e-6       NES joypad data clock period in seconds
 * NESPAD_PIO               pio0        Use PIO 0
 * NESPAD_SM                            PIO statemachine; undefined to pick a free one
 * NESPAD_IRQ               PIO0_IRQ_0  Use IRQ 0 for pio0
 * NESPAD_CLK_PIN_BASE      12          GPIO start for CLK and ENABLE
 * NESPAD_DATA_PIN_BASE     14          GPIO start for DATA1 and DATA2
```
## NES joypad pins

![image](docs/nes_joypad_pins.png)

## Clocking scheme

![image](docs/nes_joypad_clocks.png)

## Build for RP2350
```
mkdir build-2350
cd build-2350
cmake -DPICO_SDK_PATH=~/pico/pico-sdk -DPICO_MCU=rp2350 ..
make -j4
```



## References
https://gamefaqs.gamespot.com/snes/916396-super-nintendo/faqs/5395<br/>
https://tresi.github.io/nes/<br/>
https://www.bertiusgames.com/projects/gamepad.html<br/>
https://github.com/xrip/pico-nes/tree/main/drivers/nespad<br/>
https://wokwi.com/projects/407394757834126337<br/>

