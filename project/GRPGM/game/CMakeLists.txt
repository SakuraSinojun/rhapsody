
PROJECT (grpgm)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

MESSAGE(STATUS "Project Directory: ${PROJECT_SOURCE_DIR}")

set(CMAKE_BUILD_TYPE        DEBUG)
set(TARGET_NAME             game CACHE STRING "exe name")
#set(PLATFORM                "mingw" CACHE STRING "mingw vc")
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/target CACHE STRING "OUTPUT PATH")
set(LIBRARY_OUTPUT_PATH     ${grpgm_BINARY_DIR}/lib)

#list(APPEND CMAKE_MODULE_PATH
#    ${grpgm_SOURCE_DIR}/cmake
#    )
#if(PLATFORM STREQUAL    "mingw")
#    MESSAGE(STATUS "MINGW!")
#    include( MinGW )
#else(PLATFORM STREQUAL    "mingw")
#    MESSAGE(STATUS "VC!")
#    include( VC )
#endif(PLATFORM STREQUAL    "mingw")


#add_subdirectory(common)
#add_subdirectory(win32)
#add_subdirectory(render)

aux_source_directory( . DIR_MAIN_SRCS)
aux_source_directory(common DIR_COMMON_SRCS)
aux_source_directory(render DIR_RENDER_SRCS)
aux_source_directory(win32 DIR_WIN32_SRCS)

add_executable(${TARGET_NAME}  
	${DIR_MAIN_SRCS}
	${DIR_RENDER_SRCS}
	${DIR_COMMON_SRCS}
	${DIR_WIN32_SRCS}
	)

target_link_libraries(${TARGET_NAME}
    kernel32
    gdi32
    user32
    )

include_directories(        
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/render
    ${PROJECT_SOURCE_DIR}/win32
    )


