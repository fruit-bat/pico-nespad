# pico-nespad
Pico PI NES joypad reader

![image](docs/nes_joypad.jpg)

Reads from dual NES joypads on a Pico PI.

## Example use
Basic flow:
```
  #include "nespad.h"

  ...

  nespad_default_init();
  uint32 joystate = nespad_state();
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

## NES joypad pins

![image](docs/nes_joypad_pins.png)

## Clocking scheme

![image](docs/nes_joypad_clocks.png)

## References
https://gamefaqs.gamespot.com/snes/916396-super-nintendo/faqs/5395<br/>
https://tresi.github.io/nes/<br/>
https://www.bertiusgames.com/projects/gamepad.html<br/>
https://github.com/xrip/pico-nes/tree/main/drivers/nespad<br/>
https://wokwi.com/projects/407394757834126337<br/>

