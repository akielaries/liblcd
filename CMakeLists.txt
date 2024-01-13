project(liblcd LANGUAGES C)
set(CMAKE_C_STANDARD 99)

set (SOURCE_FILES
    src/lcd.c
    src/gpio.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

add_library(liblcd SHARED ${SOURCE_FILES})

set_target_properties(liblcd PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)

install(TARGETS liblcd LIBRARY DESTINATION lib)

install(FILES
    lib/gpio.h
    lib/liblcd.h
    DESTINATION include/liblcd
)

add_library(liblcd::liblcd ALIAS liblcd)

