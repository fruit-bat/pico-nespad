
add_executable(nespad_serial_test
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
)

target_compile_definitions(nespad_serial_test PRIVATE

)

target_link_libraries(nespad_serial_test
  pico_stdlib
  pico_util
  hardware_pio
  pico_nespad
)

# create map/bin/hex file etc.
pico_add_extra_outputs(nespad_serial_test)
