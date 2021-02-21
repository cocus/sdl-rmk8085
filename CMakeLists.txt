cmake_minimum_required(VERSION 3.16)
project(main VERSION 0.1.0)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

add_executable(
    main
    main.c
    SDL_terminal.c
    rmk8085/8080emu.c
    rmk8085/pe_base_pin.c
    rmk8085/pe_bq3285.c
    rmk8085/pe_i8155.c
    rmk8085/pe_i8251.c
    rmk8085/pe_i8279.c
    rmk8085/pe_rom.c
    rmk8085/pe_sram.c
    rmk8085/pe_interaction.c
    rmk8085/console_impl.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_link_libraries(main PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_ttf)