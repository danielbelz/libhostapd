cmake_minimum_required (VERSION 3.12)

project (libhostapd VERSION 0.0.1 DESCRIPTION "hostapd client library")

set (CMAKE_SYSTEM_NAME Linux)

if (ARCH_arm)
    set (CMAKE_SYSTEM_PROCESSOR arm)
else ()
    set (CMAKE_SYSTEM_PROCESSOR x86_64)
endif()

set (CMAKE_C_FLAGS_RELEASE -O0)

if (ARCH_arm)
    if(ARCH_ARM_MUSL) # if MUSL is ON(true), specify a different cross compiler directory
        set (CMAKE_SYSROOT  ${CROSS_SDK_PATH}/arm-buildroot-linux-musleabihf/sysroot)
    else()
        set (CMAKE_SYSROOT  ${CROSS_SDK_PATH}/arm-buildroot-linux-gnueabihf/sysroot)
    endif()

    set (CMAKE_CFG_INTDIR )
    set (COMPILER_OPTS  ${COMPILER_OPTS} )
else()
    set (CMAKE_SYSROOT  /)
    set (COMPILER_OPTS -Wno-format-truncation -Wformat=0 -Wno-error=format-security -Wno-error=strict-overflow -DARCH_x86)
endif()

add_subdirectory(source)
add_subdirectory(test)
