
aux_source_directory(. DIR_WIN32_SRCS)

include_directories(        
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/render
    ${PROJECT_SOURCE_DIR}/win32
    )

add_library(win32 STATIC ${DIR_WIN32_SRCS})

target_link_libraries(win32
    kernel32
    gdi32
    user32
    )

