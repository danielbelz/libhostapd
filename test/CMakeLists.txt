cmake_minimum_required (VERSION 3.12)

# Hostapd library test application
set(HAPDCLI hapd_test_cli)
add_executable (${HAPDCLI}
        ./hapd_cli.c
)
target_compile_options (${HAPDCLI} PRIVATE -g3 -Wall -Werror )
target_include_directories (${HAPDCLI} PRIVATE ../source/common ${CMAKE_SYSROOT}/usr/include ${H_FILES})
target_link_libraries (${HAPDCLI} PRIVATE c hostapd)


