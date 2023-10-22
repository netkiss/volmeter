cmake_minimum_required(VERSION 3.22)

project(Voltmeter LANGUAGES C CXX
    VERSION 1.0.0
)

# Режим кросс-компиляции
set(WIN32_CROSS_COMPILE OFF CACHE BOOL
    "Включение режима кросс-компиляции под Windows"
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(DEBUG "DEBUG")
else()
    unset(DEBUG)
endif()

if(WIN32 OR WIN32_CROSS_COMPILE)
    add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}"
        ${DEBUG}
        DONT_USE_SDK_VER)
else()
    add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}"
        ${DEBUG})
endif(WIN32 OR WIN32_CROSS_COMPILE)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Os -flto -g3")
set(CMAKE_C_FLAGS "-Wall -Os -flto -g3")

if(WIN32 OR WIN32_CROSS_COMPILE)
    # Only Winwods need
    add_library(modbus_master SHARED IMPORTED)
    set_property(TARGET modbus_master PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/libmodbus.dll")
    set_property(TARGET modbus_master PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/libmodbus.lib")
endif(WIN32 OR WIN32_CROSS_COMPILE)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

if(WIN32 OR WIN32_CROSS_COMPILE)
    target_link_libraries(${PROJECT_NAME} modbus_master) # WINDOWS

else()
    message("Atention: should be installed libmodbus-dev (>=3.1.8)")
    target_link_libraries(${PROJECT_NAME} modbus) # LINUX
endif(WIN32 OR WIN32_CROSS_COMPILE)
