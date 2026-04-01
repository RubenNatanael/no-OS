cmake_minimum_required(VERSION 3.14)
project(scheduler_test C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -g -O0)

# Dependencies
include(FetchContent)
FetchContent_Declare(nanopb
    GIT_REPOSITORY https://github.com/nanopb/nanopb.git
    GIT_TAG 0.4.9
)
FetchContent_MakeAvailable(nanopb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${nanopb_SOURCE_DIR}/extra)
find_package(Nanopb REQUIRED)

# CAPI
set(CAPI_DIR "/home/ruben/ADI/core-sw-common-hal" CACHE PATH "Path to core-sw-common-hal")
set(NO_OS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# Proto generation
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/capi.proto)

# Main executable
add_executable(main
    main.c
    Scheduler.c
    encoders.c
    ${PROTO_SRCS}
    ${CAPI_DIR}/src/capi_i2c.c
    ${NO_OS_ROOT}/drivers/platform/generic/capi/capi_scheduler_local.c
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CAPI_DIR}/api
)

target_link_libraries(main PRIVATE nanopb)
