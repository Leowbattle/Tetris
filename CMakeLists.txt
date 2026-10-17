cmake_minimum_required(VERSION 3.20)

project(Tetris
    VERSION 1.0.0
    DESCRIPTION "Tetris"
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Default to a Release build so a plain `cmake -B build` gives an optimised
# binary.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_executable(tetris Tetris/main.c)

if(NOT MSVC)
    target_compile_options(tetris PRIVATE -Wall -Wextra)
endif()

# Loaded by the game relative to the working directory.
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Tetris/resources)

if(EMSCRIPTEN)
    # ------------------------------------------------------------------
    # Web (Emscripten) build
    # ------------------------------------------------------------------

    # Emscripten ships its own SDL2 / SDL2_ttf ports.
    set(SDL_PORT_FLAGS
        -sUSE_SDL=2
        -sUSE_SDL_TTF=2)

    target_compile_options(tetris PRIVATE ${SDL_PORT_FLAGS})
    target_link_options(tetris PRIVATE
        ${SDL_PORT_FLAGS}
        -sALLOW_MEMORY_GROWTH=1
        # main() returns immediately after registering the animation-frame
        # loop, so the runtime must stay alive.
        -sEXIT_RUNTIME=0
        # Preload the assets at /resources so the game's relative
        # "resources/..." paths resolve from the virtual root.
        "SHELL:--preload-file ${RESOURCE_DIR}@/resources")

    # Emit index.html + index.js + index.wasm + index.data.
    set_target_properties(tetris PROPERTIES
        OUTPUT_NAME "index"
        SUFFIX ".html")

else()
    # ------------------------------------------------------------------
    # Desktop (native) build
    # ------------------------------------------------------------------

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

    target_include_directories(tetris PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS})
    target_compile_options(tetris PRIVATE
        ${SDL2_CFLAGS_OTHER}
        ${SDL2_TTF_CFLAGS_OTHER})
    target_link_directories(tetris PRIVATE
        ${SDL2_LIBRARY_DIRS}
        ${SDL2_TTF_LIBRARY_DIRS})
    target_link_libraries(tetris PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})

    # Make the build tree runnable as well (`cd build && ./tetris`).
    add_custom_command(TARGET tetris POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${RESOURCE_DIR} $<TARGET_FILE_DIR:tetris>/resources
        COMMENT "Copying resources next to the executable")

    # Install the binary next to its resources so the tarball is runnable as-is.
    install(TARGETS tetris RUNTIME DESTINATION .)
    install(DIRECTORY ${RESOURCE_DIR} DESTINATION .)
endif()
