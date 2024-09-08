add_library(pico_nespad INTERFACE)

pico_generate_pio_header(pico_nespad ${CMAKE_CURRENT_LIST_DIR}/src/nespad.pio)

target_sources(pico_nespad INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/nespad.c
)

target_include_directories(pico_nespad INTERFACE
    src
)
