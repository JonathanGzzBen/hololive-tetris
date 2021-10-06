cmake_minimum_required(VERSION 3.1)

project(hololive-tetris)

if (WIN32)
    set(SFML_DOWNLOAD_URL "https://www.sfml-dev.org/files/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit.zip")
    set(SFML_DOWNLOAD_PATH "${CMAKE_CURRENT_BINARY_DIR}/sfml-download/SFML-2.5.1.zip")
    set(SFML_EXTRACTED "${CMAKE_CURRENT_SOURCE_DIR}")

    if (NOT EXISTS ${SFML_DOWNLOAD_PATH})
        file(DOWNLOAD "${SFML_DOWNLOAD_URL}" "${SFML_DOWNLOAD_PATH}")
    endif()

    file(ARCHIVE_EXTRACT INPUT "${SFML_DOWNLOAD_PATH}" DESTINATION "${SFML_EXTRACTED}")
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
set(link_dst "${CMAKE_CURRENT_BINARY_DIR}/images")

set(fonts_src "${CMAKE_CURRENT_SOURCE_DIR}/fonts")
set(fonts_dst "${CMAKE_CURRENT_BINARY_DIR}/fonts")

add_custom_command(
    TARGET hololive-tetris
    POST_BUILD COMMAND ${CMAKE_COMMAND}
        -E create_symlink ${link_src} ${link_dst}
    DEPENDS ${link_dst}
    COMMENT "Symbolic link ${link_src} => ${link_dst}"
)

add_custom_command(
    TARGET hololive-tetris
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E create_symlink ${fonts_src} ${fonts_dst}
    DEPENDS ${link_dst}
    COMMENT "Symbolic link ${fonts_src} => ${fonts_dst}"
)

