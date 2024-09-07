cmake_minimum_required(VERSION 3.12)

# Pico SDC on Raspberry Pi 4
cmake_policy(VERSION 3.12)

# Pico SDC Toolchai on Windows 10
# cmake_policy(SET CMP0115 OLD)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

# We also need PICO EXTRAS
include(pico_extras_import.cmake)

project(picodvi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_compile_options(-Wall)



add_executable(pico_nespad_test
	src/main.c
)

pico_generate_pio_header(pico_nespad_test 
  ${CMAKE_CURRENT_LIST_DIR}/src/nespad.pio
)

target_compile_definitions(pico_nespad_test PRIVATE

)

target_link_libraries(pico_nespad_test
  pico_stdlib
  pico_util
  hardware_pio
)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_nespad_test)
