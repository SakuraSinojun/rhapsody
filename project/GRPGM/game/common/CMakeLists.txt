
aux_source_directory(. DIR_COMMON_SRCS)
add_library(common ${DIR_COMMON_SRCS})

include_directories(        
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/render
    ${PROJECT_SOURCE_DIR}/win32
    )


