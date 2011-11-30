
aux_source_directory(. DIR_RENDER_SRCS)
add_library(render ${DIR_RENDER_SRCS})

include_directories(        
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/render
    ${PROJECT_SOURCE_DIR}/win32
    )


