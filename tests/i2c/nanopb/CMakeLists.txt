cmake_minimum_required(VERSION 3.14)
project(capi_i2c_nanopb_test C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Os -g -ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

# Dependencies
include(FetchContent)
FetchContent_Declare(nanopb
    GIT_REPOSITORY https://github.com/nanopb/nanopb.git
    GIT_TAG 0.4.9
)
FetchContent_MakeAvailable(nanopb)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${nanopb_SOURCE_DIR}/extra)
find_package(Nanopb REQUIRED)

# Paths
set(CAPI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../../core-sw-common-hal")
set(NO_OS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")

# Proto generation
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/capi.proto)

# RPC generation
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/capi_rpc.c ${CMAKE_CURRENT_BINARY_DIR}/capi_rpc.h
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/utils/nanopb_rpc_generator.py
        ${CMAKE_CURRENT_SOURCE_DIR}/capi.proto --output ${CMAKE_CURRENT_BINARY_DIR}/capi_rpc
    DEPENDS capi.proto utils/nanopb_rpc_generator.py
)

# Common
set(COMMON_SRCS
    transport/transport.c
    ${PROTO_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/capi_rpc.c
    ${CAPI_DIR}/src/capi_i2c.c
)

set(COMMON_INCS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/transport
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CAPI_DIR}/api
    ${NO_OS_ROOT}/include/capi
)

# Client
add_executable(client
    main_client.c
    ${NO_OS_ROOT}/drivers/platform/nanopb/capi_i2c_nanopbClient_driver.c
    ${COMMON_SRCS}
)
target_include_directories(client PRIVATE ${COMMON_INCS})
target_link_libraries(client PRIVATE nanopb)

# Server
set(SERVER_PLATFORM "linux" CACHE STRING "Server platform (linux or dummy)")

if(SERVER_PLATFORM STREQUAL "linux")
    set(SERVER_PLATFORM_SRC ${NO_OS_ROOT}/drivers/platform/linux/capi/capi_i2c_linux_driver.c)
else()
    set(SERVER_PLATFORM_SRC ${NO_OS_ROOT}/drivers/platform/nanopb/capi_i2c_dummy_driver.c)
endif()

add_executable(server
    server.c
    ${SERVER_PLATFORM_SRC}
    ${COMMON_SRCS}
)
target_include_directories(server PRIVATE ${COMMON_INCS})
target_link_libraries(server PRIVATE nanopb)
