
add_executable(nespad_serial_test
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_compile_definitions(nespad_serial_test PRIVATE
  NESPAD_DATA_PIN_BASE=20
  NESPAD_CLK_PIN_BASE=26
  NESPAD_PIO=pio1
  NESPAD_IRQ=PIO1_IRQ_0
  NESPAD_SM=3
  DEBUG_NESPAD
)

target_link_libraries(nespad_serial_test
  pico_stdlib
  pico_util
  hardware_pio
  pico_nespad
)

# create map/bin/hex file etc.
pico_add_extra_outputs(nespad_serial_test)
