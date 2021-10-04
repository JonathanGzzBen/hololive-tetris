cmake_minimum_required(VERSION 3.1)

project(hololive-tetris)

if (WIN32)
    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML-2.5.1/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
endif()


find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCES
        main.cpp
        images/
    )

add_executable(hololive-tetris ${SOURCES})

target_link_libraries(hololive-tetris sfml-graphics sfml-window sfml-system)

# Make symbolink link to run the binary directly from build directory
set(link_src "${CMAKE_CURRENT_SOURCE_DIR}/images")
if (UNIX AND NOT APPLE)
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/images")
else()
    set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/images")
endif()

add_custom_command(
    TARGET hololive-tetris
    POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E create_symlink ${link_src} ${link_dst}
    DEPENDS ${link_dst}
    COMMENT "Symbolic link ${link_src} => ${link_dst}"
)

