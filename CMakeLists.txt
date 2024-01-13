cmake_minimum_required(VERSION 3.00)

project(liblcd VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

# source files
set(SOURCES
    src/gpio.c
    src/lcd.c
)

# include dir
set(INCLUDE_DIRS
    lib
)

# create static lib
add_library(lcd STATIC ${SOURCES})

# include directories for the library
target_include_directories(lcd PUBLIC ${INCLUDE_DIRS})

# version of the library
set_target_properties(lcd PROPERTIES VERSION ${PROJECT_VERSION})

# install the library
include(GNUInstallDirs)
install(TARGETS lcd
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/liblcd
)

# install the headers
install(FILES lib/liblcd.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/liblcd)

